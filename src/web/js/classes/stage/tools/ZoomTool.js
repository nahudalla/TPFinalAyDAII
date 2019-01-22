import ToolBase from './ToolBase.js';

class ZoomTool extends ToolBase {
  constructor(stage) {
    super();

    this._onClickHandler = (eventData) => {
      this._applyZoom(eventData.position);
    };
    this._stage = stage;
  }

  _applyZoom(viewPosition) {
    throw new Error('This method should be overridden!');
  }

  activate() {
    if(this.isActive) return;
    this._stage.interactive.clickEvent.subscribe(this._onClickHandler);
    super.activate();
  }

  deactivate() {
    if(!this.isActive) return;
    this._stage.interactive.clickEvent.unsubscribe(this._onClickHandler);
    super.deactivate();
  }
}

export class ZoomInTool extends ZoomTool {
  constructor(stage) {
    super(stage);
  }

  _applyZoom(viewPosition) { this._stage.zoomIn(viewPosition); }
}

export class ZoomOutTool extends ZoomTool {
  constructor(stage) {
    super(stage);
  }

  _applyZoom(viewPosition) { this._stage.zoomOut(viewPosition); }
}
