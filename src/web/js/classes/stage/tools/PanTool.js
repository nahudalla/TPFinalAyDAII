import getActiveContext from '../../Context.js';

const CURSOR_DEFAULT = '-webkit-grab';
const CURSOR_DRAGGING = '-webkit-grabbing';

import ToolBase from './ToolBase.js';

export default class PanTool extends ToolBase {
  constructor(stage) {
    super(stage);

    super._enableGridPositionToolTip(
      point => getActiveContext().pointsList.has(point)
    );

    this._onDrag = onDrag.bind(this);
    this._onDown = onDown.bind(this);
    this._onUp = onUp.bind(this);
  }

  activate() {
    if(this.isActive) return;
    this.stage.interactive.dragEvent.subscribe(this._onDrag);
    this.stage.interactive.downEvent.subscribe(this._onDown);
    this.stage.interactive.upEvent.subscribe(this._onUp);
    this.stage.interactive.setCursor(CURSOR_DEFAULT);
    super.activate();
  }

  deactivate() {
    if(!this.isActive) return;
    this.stage.interactive.dragEvent.unsubscribe(this._onDrag);
    this.stage.interactive.downEvent.unsubscribe(this._onDown);
    this.stage.interactive.upEvent.unsubscribe(this._onUp);
    super.deactivate();
  }
}

function onDrag(eventData) {
  this.stage.scroll(eventData.offset);
}

function onDown(eventData) {
  this.stage.interactive.setCursor(CURSOR_DRAGGING);
}

function onUp(eventData) {
  this.stage.interactive.setCursor(CURSOR_DEFAULT);
}
