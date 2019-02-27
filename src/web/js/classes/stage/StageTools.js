import PanTool from './tools/PanTool.js';
import { ZoomInTool, ZoomOutTool } from './tools/ZoomTool.js';
import AddPointTool from './tools/AddPointTool.js';
import RemovePointTool from './tools/RemovePointTool.js';
import ReplacePointTool from './tools/ReplacePointTool.js';

export default class StageTools {
  constructor(stage, context) {
    this._panTool = setupTool.call(this, PanTool, stage);
    this._zoomInTool = setupTool.call(this, ZoomInTool, stage);
    this._zoomOutTool = setupTool.call(this, ZoomOutTool, stage);
    this._addPointTool = setupTool.call(this, AddPointTool, stage);
    this._removePointTool = setupTool.call(this, RemovePointTool, stage);
    this._replacePointTool = setupTool.call(this, ReplacePointTool, stage);

    this._stage = stage;

    this._activeTool = null;

    this._panTool.activate();

    const previousTools = [];
    const lastKeyCodesPressed = [];
    const keydownListener = evt => {
      let newTool;

      if(evt.ctrlKey && evt.shiftKey) {
        newTool = this._replacePointTool;
      } else if(evt.ctrlKey) {
        newTool = this._panTool;
      } else if(evt.shiftKey) {
        newTool = this._addPointTool;
      } else if(evt.altKey) {
        newTool = this._removePointTool;
      } else return;

      if(newTool.isActive) return;

      previousTools.push(this._activeTool);
      lastKeyCodesPressed.push(evt.code);

      newTool.activate();
    };
    const keyupListener = evt => {
      if(!evt.ctrlKey && !evt.shiftKey && !evt.altKey) {
        let last = null;
        while(previousTools.length) {
          last = previousTools.pop();
        }
        if(last) last.activate();
        while(lastKeyCodesPressed.length) lastKeyCodesPressed.pop();
      }

      if(lastKeyCodesPressed.length > 0 && evt.code === lastKeyCodesPressed[lastKeyCodesPressed.length-1]) {
        lastKeyCodesPressed.pop();
        previousTools.pop().activate();
      }
    };

    context.activateEvent.subscribe(()=>{
      window.addEventListener('keydown', keydownListener);
      window.addEventListener('keyup', keyupListener);
    });
    context.deactivateEvent.subscribe(()=>{
      window.removeEventListener('keydown', keydownListener);
      window.removeEventListener('keyup', keyupListener);
    });
  }

  get panTool() { return this._panTool; }
  get zoomInTool() { return this._zoomInTool; }
  get zoomOutTool() { return this._zoomOutTool; }
  get addPointTool() { return this._addPointTool; }
  get removePointTool() { return this._removePointTool; }
  get replacePointTool() { return this._replacePointTool; }
  get activeTool() { return this._activeTool; }
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
