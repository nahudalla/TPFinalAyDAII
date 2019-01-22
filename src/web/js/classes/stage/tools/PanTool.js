import ToolBase from './ToolBase.js';

export default class PanTool extends ToolBase {
  constructor(stage) {
    super();
    this._stage = stage;

    this._onDrag = onDrag.bind(this);
  }

  activate() {
    if(this.isActive) return;
    this._stage.interactive.dragEvent.subscribe(this._onDrag);
    super.activate();
  }

  deactivate() {
    if(!this.isActive) return;
    this._stage.interactive.dragEvent.unsubscribe(this._onDrag);
    super.deactivate();
  }
}

function onDrag(eventData) {
  this._stage.scroll(eventData.offset);
}
