import StageBase from './StageBase.js';
import Observable from '../../Observable.js';

// FIXME: Wrong mouse position when dragging outside of interactive layer

export default class InteractiveStage extends StageBase {
  constructor(project, stage) {
    super(project, stage);

    super._enableZoom();

    this._clickEvent = new Observable();
    this._dragEvent = new Observable();

    this._setupScroll();
    this._setupZoom();
    this._setupClickEvent();
    this._setupDragEvent();

    project.activeLayer.addChild(new stage.Circle({
      center: project.view.center,
      radius: 5,
      fillColor: '#F00'
    }));
  }

  get clickEvent() { return this._clickEvent; }
  get dragEvent() { return this._dragEvent; }

  _setupScroll() {
    this.stage.scrollEvent.subscribe(offset => {
      const view = this.view;
      const viewCenter = view.projectToView(view.center);
      view.center = view.viewToProject(viewCenter.subtract(offset));
    });
  }

  _setupZoom() {
    this.project.view.element.addEventListener('mousewheel', e => {
      e.preventDefault();
      e.stopPropagation();

      const viewPosition = new this.stage.Point(calculateMousePos(e));

      if(e.deltaY < 0) this.stage.zoomIn(viewPosition);
      else this.stage.zoomOut(viewPosition);
    });
  }

  _setupClickEvent() {
    const element = this.project.view.element;
    element.addEventListener('click', event => {
      this._clickEvent.emit({
        position: new this.stage.Point(calculateMousePos(event)),
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
      lastPosition = new this.stage.Point(calculateMousePos(e));
      isDrag = false;
      window.addEventListener('mousemove', onMouseMoveBound);
      window.addEventListener('mouseup', onMouseUpBound);
    });

    function onMouseMove(event) {
      event.preventDefault();
      event.stopPropagation();
      isDrag = true;

      const position = new this.stage.Point(calculateMousePos(event));
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

function calculateMousePos(e) {
  const rect = e.target.getBoundingClientRect();
  const x = e.clientX - rect.left; //x position within the element.
  const y = e.clientY - rect.top;  //y position within the element.
  return {x, y};
}
