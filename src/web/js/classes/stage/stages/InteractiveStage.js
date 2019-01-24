import Observable from '../../Observable.js';
import Point from '../../Point.js';
import StageWithPointsBase from './StageWithPointsBase.js';

export default class InteractiveStage extends StageWithPointsBase {
  constructor(project, stage) {
    super(project, stage);

    super._centerOriginInView();
    super._enableResetCenterOnResize();
    super._enableZoom();
    super._enableScroll();

    this._clickEvent = new Observable();
    this._downEvent = new Observable();
    this._dragEvent = new Observable();
    this._upEvent = new Observable();
    this._gridPositionEvent = new Observable();
    this._gridOverEvent = new Observable();
    this._gridOutEvent = new Observable();

    this._lastGridPositionPoint = null;

    this._setupZoom();
    this._setupClickEvent();
    this._setupDragEvent();
    this._setupGridEvents();
  }

  _getPointStyle() { return this.stage.settings.interactive_point_style; }

  get clickEvent() { return this._clickEvent; }
  get dragEvent() { return this._dragEvent; }
  get downEvent() { return this._downEvent; }
  get upEvent() { return this._upEvent; }
  get gridPositionEvent() { return this._gridPositionEvent; }
  get gridOverEvent() { return this._gridOverEvent; }
  get gridOutEvent() { return this._gridOutEvent; }

  setCursor(cursor) {
    this.view.element.style.cursor = cursor;
  }

  resetCursor() {
    this.setCursor(this.stage.settings.interactive_default_cursor);
  }

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
      const position = new this.stage.Point(calculateMousePos(element, event));

      this._clickEvent.emit({
        position, gridPosition: this._calculateGridPosition(position),
        event
      });
    });
  }

  _setupDragEvent() {
    const element = this.project.view.element;
    const _this = this;

    let lastPosition, isDrag;

    const onMouseMoveBound = onMouseMove.bind(this);
    const onMouseUpBound = onMouseUp.bind(this);

    element.addEventListener('mousedown', e => {
      lastPosition = new this.stage.Point(calculateMousePos(element, e));
      this._downEvent.emit({position:lastPosition, event:e});

      if(e.defaultPrevented) return;

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

      _this._dragEvent.emit({
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
      _this._upEvent.emit({
        position: calculateMousePos(element, e),
        isDrag, event: e
      });
    }
  }

  _setupGridEvents() {
    const element = this.project.view.element;
    element.addEventListener('mouseout', event => {
      this._gridOutEvent.emit(event);
    });
    element.addEventListener('mousemove', event => {
      const positionCount = this._gridPositionEvent.listenerCount;
      const overCount = this._gridOverEvent.listenerCount;

      if(positionCount === 0 && overCount === 0) return;

      const position = this._calculateGridPosition(calculateMousePos(element, event));

      if(overCount !== 0) this._gridOverEvent.emit({position, event});

      if( positionCount === 0
          || (this._lastGridPositionPoint && this._lastGridPositionPoint.equals(position))
      ) { return; }

      this._lastGridPositionPoint = position;

      this._gridPositionEvent.emit({position, event});
    });
  }

  _calculateGridPosition(viewPosition) {
    const pos = this.view.viewToProject(viewPosition)
      .divide(this.stage.settings.grid_size);
    return new Point(Math.round(pos.x), Math.round(-pos.y));
  }
}

function calculateMousePos(elem, e) {
  const rect = elem.getBoundingClientRect();
  const x = e.clientX - rect.left; //x position within the element.
  const y = e.clientY - rect.top;  //y position within the element.
  return {x, y};
}
