import setupPanTool from '../stage/tools/Pan.js';
import drawAxes from '../stage/Axes.js';

const paper = window.paper;

const STAGE_OPTIONS = {
  grid_size: 10,
  zoom_amount: 0.2,
  zoom_timeout: 350,
  zoom_min: 0.5,
  zoom_max: 15
};

export const TOOLS = {
  PAN: setupPanTool
};

class Stage {
  constructor() {
    this._lastZoomAmount = 0;
    this._zoomTimeout = null;

    this._axesLayer = new paper.Layer();
    this._pointsLayer = new paper.Layer();
    this._algorExecutionLayer = new paper.Layer();
    this._interactiveLayer = new paper.Layer();

    const onResize = ()=>{
      this._pause();
      this._redrawAxes();
      this._resume();
    };

    onResize();

    paper.view.on('resize', onResize);
  }

  get layer() {
    return paper.project.activeLayer;
  }

  _pause() {
    paper.view.pause();
    paper.view.autoUpdate = false;
  }

  _resume() {
    paper.view.requestUpdate();
    paper.view.autoUpdate = true;
    paper.view.play();
  }

  _redrawAxes() {
    this._axesLayer.activate();

    const tmp = paper.project.currentStyle;
    paper.project.currentStyle = {
      strokeWidth: 1,
      strokeScaling: false
    };

    drawAxes(this);

    paper.project.currentStyle = tmp;

    this._interactiveLayer.activate();
  }

  _handleZoom(amount, center) {
    clearTimeout(this._zoomTimeout);

    if((amount < 0 && this._lastZoomAmount > 0)
       || (amount > 0 && this._lastZoomAmount < 0)
    ) {
      this._lastZoomAmount = 0;
    }

    this._lastZoomAmount += amount;

    let new_zoom = paper.view.zoom + this._lastZoomAmount;

    if(new_zoom < STAGE_OPTIONS.zoom_min) new_zoom = STAGE_OPTIONS.zoom_min;
    else if(new_zoom > STAGE_OPTIONS.zoom_max) new_zoom = STAGE_OPTIONS.zoom_max;

    this._pause();

    paper.view.scale(new_zoom/paper.view.zoom, center);
    this._redrawAxes();

    this._resume();

    this._zoomTimeout = setTimeout(()=>{
      this._lastZoomAmount = 0;
      this._zoomTimeout = null;
    }, STAGE_OPTIONS.zoom_timeout);
  }

  zoomIn(center) {
    this._handleZoom(STAGE_OPTIONS.zoom_amount, center);
  }

  zoomOut(center) {
    this._handleZoom(-STAGE_OPTIONS.zoom_amount, center);
  }

  scrollView(offset) {
    this._pause();
    paper.view.translate(offset.divide(paper.view.zoom));
    this._redrawAxes();
    this._resume();
  }

  get bounds() {
    return paper.view.bounds;
  }

  projectToView(point) {
    return paper.view.projectToView(point);
  }

  static get TOOLS(){return TOOLS;}
  get tools(){return TOOLS;}
}

let stage = null;

export default stage;

export function initializeStage() {
  paper.view.center = new paper.Point(0,0);

  stage = new Stage();

  Object.keys(TOOLS).forEach(tool => {
    if(typeof TOOLS[tool] === 'function') {
      TOOLS[tool] = TOOLS[tool](stage);
    }
  });

  TOOLS.PAN.activate();

  const point = new paper.Path.Circle(new paper.Point(0,0), 5);
  point.fillColor = "#F00";
}
