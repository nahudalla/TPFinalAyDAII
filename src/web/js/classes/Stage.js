import log from '../logger.js';

if(!("paper" in window) || !('setup' in window.paper)) {
  log(log.FLAGS.CRITICAL, "Could not initialize canvas: Paper.js not loaded!");
}

import ZoomHandler from './stage/ZoomHandler.js';
import AxesStage from './stage/stages/AxesStage.js';
import ZoomedStage from './stage/stages/ZoomedStage.js';
import NormalStage from './stage/stages/NormalStage.js';
import InteractiveStage from './stage/stages/InteractiveStage.js';
import Observable from './Observable.js';
import StageTools from './stage/StageTools.js';

const DEFAULT_STAGE_SETTINGS = {
  use_active_paper_scope: false,
  grid_size: 30,
  point_radius: 7,
  point_style: {
    fillColor: '#00F'
  },
  zoom_amount: 0.2,
  zoom_timeout: 350,
  zoom_min: 0.35,
  zoom_max: 15,
  axes_lines_style: {
    strokeWidth: 1,
    strokeScaling: false,
    strokeColor: '#000'
  },
  axes_markers_style: {
    strokeWidth: 1,
    strokeScaling: false,
    strokeColor: '#000'
  },
  axes_text_style: {
    justification: 'center',
    fillColor: '#000'
  },
  axes_padding: 5,
  axes_marker_length: 10,
  axes_markers_min_distance: 23,
  axes_text_to_line_offset: 15,
  axes_text_preferred_side: 'lower' // lower or upper
};

let activeStage = null;

export const activeStageChangedEvent = new Observable();

function setActiveStage(stage) {
  const prevStage = activeStage;
  activeStage = stage;
  if(prevStage) prevStage.deactivate();
  activeStageChangedEvent.emit(activeStage, prevStage);
}

export function getActiveStage() {
  return activeStage;
}

function generateSettings(settings) {
  return copyObject(settings, copyObject(DEFAULT_STAGE_SETTINGS));
}

export default class Stage {
  constructor(views, settings) {
    if(!(views.axes instanceof HTMLCanvasElement)
       || !(views.zoomed instanceof HTMLCanvasElement)
       || !(views.normal instanceof HTMLCanvasElement)
       || !(views.interactive instanceof HTMLCanvasElement)
    ) {
      throw new Error('Invalid views in instantiation of Stage.');
    }

    this._settings = generateSettings(settings);

    if(this._settings.use_active_paper_scope === true) {
      this._paperScope = paper;
    } else {
      this._paperScope = new paper.PaperScope();
    }

    const scope = this._paperScope;

    this._zoomEvent = new Observable();
    this._scrollEvent = new Observable();
    this._activateEvent = new Observable();
    this._deactivateEvent = new Observable();

    this._paperScope.activate();
    scope.settings.insertItems = false;

    this._zoomLevel = 1;
    this._zoomHandler = new ZoomHandler(this);

    this._interactive = new InteractiveStage(new scope.Project(views.interactive), this);
    this._normal = new NormalStage(new scope.Project(views.normal), this);
    this._zoomed = new ZoomedStage(new scope.Project(views.zoomed), this);
    this._axes = new AxesStage(new scope.Project(views.axes), this);

    this._tools = new StageTools(this);

    this.activate();
  }

  get axes() { return this._axes; }
  get zoomed() { return this._zoomed; }
  get normal() { return this._normal; }
  get interactive() { return this._interactive; }

  get settings() { return this._settings; }

  get activateEvent() { return this._activateEvent; }
  get deactivateEvent() { return this._deactivateEvent; }
  activate() {
    this._paperScope.activate();
    setActiveStage(this);
    this._activateEvent.emit();
  }
  deactivate() {
    this._deactivateEvent.emit();
  }

  get zoom() { return this._zoomLevel; }
  set zoom(level) {this.setZoom(level);}
  setZoom(level, viewPosition) {
    if(level < this._settings.zoom_min) level = this._settings.zoom_min;
    else if(level > this._settings.zoom_max) level = this._settings.zoom_max;

    if(this._zoomLevel === level) return;

    this._zoomLevel = level;

    this._zoomEvent.emit(level, viewPosition);
  }
  zoomIn(viewPosition) { this._zoomHandler.zoomIn(viewPosition); }
  zoomOut(viewPosition) { this._zoomHandler.zoomOut(viewPosition); }
  get zoomEvent() { return this._zoomEvent; }

  scroll(viewOffset) { this._scrollEvent.emit(viewOffset); }
  get scrollEvent() { return this._scrollEvent; }

  get tools() { return this._tools; }

  get scope() { return this._paperScope; }
  get Line() { return this._paperScope.Path.Line; }
  get Tool() { return this._paperScope.Tool; }
  get Point() { return this._paperScope.Point; }
  get Circle() { return this._paperScope.Path.Circle; }
  get PointText() { return this._paperScope.PointText; }
}

function copyObject(obj, target) {
  if(typeof obj === 'undefined') {
    return target;
  }

  if(obj === null || typeof obj !== 'object') return obj;

  target = (target !== null && typeof target === 'object') ? target : {};

  Object.keys(obj).forEach(key => {
    target[key] = copyObject(obj[key], target[key]);
  });

  return target;
}
