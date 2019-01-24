const ZOOM_IN_CURSOR = 'zoom-in';
const ZOOM_OUT_CURSOR = 'zoom-out';

import ToolBase from './ToolBase.js';

class ZoomTool extends ToolBase {
  constructor(stage, cursor) {
    super(stage);

    this._cursor = cursor;

    this._onClickHandler = (eventData) => {
      this._applyZoom(eventData.position);
    };
  }

  _applyZoom(viewPosition) {
    throw new Error('This method should be overridden!');
  }

  activate() {
    if(this.isActive) return;
    this.stage.interactive.clickEvent.subscribe(this._onClickHandler);
    this.stage.interactive.setCursor(this._cursor);
    super.activate();
  }

  deactivate() {
    if(!this.isActive) return;
    this.stage.interactive.clickEvent.unsubscribe(this._onClickHandler);
    super.deactivate();
  }
}

export class ZoomInTool extends ZoomTool {
  constructor(stage) {
    super(stage, ZOOM_IN_CURSOR);
  }

  _applyZoom(viewPosition) { this.stage.zoomIn(viewPosition); }
}

export class ZoomOutTool extends ZoomTool {
  constructor(stage) {
    super(stage, ZOOM_OUT_CURSOR);
  }

  _applyZoom(viewPosition) { this.stage.zoomOut(viewPosition); }
}
