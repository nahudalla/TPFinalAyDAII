import PanTool from './tools/PanTool.js';
import { ZoomInTool, ZoomOutTool } from './tools/ZoomTool.js';

export default class StageTools {
  constructor(stage) {
    this._panTool = setupTool.call(this, PanTool, stage);
    this._zoomInTool = setupTool.call(this, ZoomInTool, stage);
    this._zoomOutTool = setupTool.call(this, ZoomOutTool, stage);

    this._activeTool = null;

    this._panTool.activate();
  }

  get panTool() { return this._panTool; }
  get zoomInTool() { return this._zoomInTool; }
  get zoomOutTool() { return this._zoomOutTool; }
}

function setupTool(Tool, stage) {
  const tool = new Tool(stage);
  tool.activateEvent.subscribe(onActivate.bind(this, tool));
  tool.deactivateEvent.subscribe(onDeactivate.bind(this, tool));
  return tool;
}

function onActivate(tool) {
  if(this._activeTool !== tool) {
    const prev = this._activeTool;
    this._activeTool = tool;
    if(prev !== null) prev.deactivate();
  }
}

function onDeactivate(tool) {
  if(this._activeTool === tool) {
    this._activeTool = null;
  }
}
