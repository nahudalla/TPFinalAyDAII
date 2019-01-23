import StageBase from './StageBase.js';
import Observable from '../../Observable.js';

export default class InteractiveStage extends StageBase {
  constructor(project, stage) {
    super(project, stage);

    super._centerOriginInView();
    super._enableZoom();
    super._enableScroll();

    this._clickEvent = new Observable();
    this._dragEvent = new Observable();

    this._setupZoom();
    this._setupClickEvent();
    this._setupDragEvent();
  }

  get clickEvent() { return this._clickEvent; }
  get dragEvent() { return this._dragEvent; }

  _setupZoom() {
    const elem = this.project.view.element;
    elem.addEventListener('mousewheel', e => {
      e.preventDefault();
      e.stopPropagation();

      const viewPosition = new this.stage.Point(calculateMousePos(elem, e));

      if(e.deltaY < 0) this.stage.zoomIn(viewPosition);
      else this.stage.zoomOut(viewPosition);
    });
  }

  _setupClickEvent() {
    const element = this.project.view.element;
    element.addEventListener('click', event => {
      this._clickEvent.emit({
        position: new this.stage.Point(calculateMousePos(element, event)),
        event
      });
    });
  }

  _setupDragEvent() {
    const element = this.project.view.element;

    let lastPosition, isDrag;

    const onMouseMoveBound = onMouseMove.bind(this);
    const onMouseUpBound = onMouseUp.bind(this);

    element.addEventListener('mousedown', e => {
      lastPosition = new this.stage.Point(calculateMousePos(element, e));
      isDrag = false;
      window.addEventListener('mousemove', onMouseMoveBound);
      window.addEventListener('mouseup', onMouseUpBound);
    });

    function onMouseMove(event) {
      event.preventDefault();
      event.stopPropagation();
      isDrag = true;

      const position = new this.stage.Point(calculateMousePos(element, event));
      const previous_position = lastPosition;
      lastPosition = position;

      this._dragEvent.emit({
        offset: position.subtract(previous_position),
        position, previous_position, event
      });
    }

    function onMouseUp(e) {
      if(isDrag) {
        e.preventDefault();
        e.stopPropagation();
      }
      window.removeEventListener('mousemove', onMouseMoveBound);
      window.removeEventListener('mouseup', onMouseUpBound);
    }
  }
}

function calculateMousePos(elem, e) {
  const rect = elem.getBoundingClientRect();
  const x = e.clientX - rect.left; //x position within the element.
  const y = e.clientY - rect.top;  //y position within the element.
  return {x, y};
}
