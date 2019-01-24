import PanTool from './tools/PanTool.js';
import { ZoomInTool, ZoomOutTool } from './tools/ZoomTool.js';
import AddPointTool from './tools/AddPointTool.js';
import RemovePointTool from './tools/RemovePointTool.js';

export default class StageTools {
  constructor(stage, context) {
    this._panTool = setupTool.call(this, PanTool, stage);
    this._zoomInTool = setupTool.call(this, ZoomInTool, stage);
    this._zoomOutTool = setupTool.call(this, ZoomOutTool, stage);
    this._addPointTool = setupTool.call(this, AddPointTool, stage);
    this._removePointTool = setupTool.call(this, RemovePointTool, stage);

    this._stage = stage;

    this._activeTool = null;

    this._panTool.activate();

    this._previousTool = null;
    this._lastKeyCodePressed = null;
    this._keydownListener = evt => {
      let newTool;

      if(evt.ctrlKey) {
        newTool = this._panTool;
      } else if(evt.shiftKey) {
        newTool = this._addPointTool;
      } else if(evt.altKey) {
        newTool = this._removePointTool;
      } else return;

      if(newTool.isActive) return;

      if(!this._previousTool) {
        this._previousTool = this._activeTool;
      }

      this._lastKeyCodePressed = evt.code;

      newTool.activate();
    };
    this._keyupListener = evt => {
      if(evt.code === this._lastKeyCodePressed) {
        this._previousTool.activate();
        this._previousTool = null;
        this._lastKeyCodePressed = null;
      }
    };

    context.activateEvent.subscribe(()=>{
      window.addEventListener('keydown', this._keydownListener);
      window.addEventListener('keyup', this._keyupListener);
    });
    context.deactivateEvent.subscribe(()=>{
      window.removeEventListener('keydown', this._keydownListener);
      window.removeEventListener('keyup', this._keyupListener);
    });
  }

  get panTool() { return this._panTool; }
  get zoomInTool() { return this._zoomInTool; }
  get zoomOutTool() { return this._zoomOutTool; }
  get addPointTool() { return this._addPointTool; }
  get removePointTool() { return this._removePointTool; }
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
    this._stage.interactive.resetCursor();
  }
}
