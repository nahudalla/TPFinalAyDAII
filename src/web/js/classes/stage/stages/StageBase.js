
export default class StageBase {
  constructor(project, stage) {
    this._project = project;
    this._stage = stage;

    this._styleStack = [];

    this._center = null;
  }

  _enableResetCenterOnResize(pre, post) {
    this._center = this.view.center;
    this._project.view.on('resize', ()=>{
      if(typeof pre === 'function') pre();
      this.view.center = this._center;
      if(typeof post === 'function') post();
    });
  }

  _pushStyle(style) {
    const backup = {};

    Object.keys(style).forEach(key => {
      backup[key] = this._project.currentStyle[key];
      this._project.currentStyle[key] = style[key];
    });

    this._styleStack.push(backup);
  }

  _popStyle() {
    const backup = this._styleStack.pop();

    Object.keys(backup).forEach(key => {
      this._project.currentStyle[key] = backup[key];
    });

    return backup;
  }

  _centerOriginInView() {
    this._project.view.center = new this._stage.Point(0,0);
  }

  _setupZoomStorage() {
    this._zoom = 1;
    this._zoom_center = this.view.center;
  }

  _saveZoom() {
    this._zoom = this.view.zoom;
    this._zoom_center = this.view.center;
    this.view.zoom = 1;
    this.view.center = this._zoom_center.multiply(this._zoom);
  }

  _restoreZoom() {
    this.view.zoom = this._zoom;
    this.view.center = this._zoom_center;
  }

  _enableZoom(pre, post) {
    const view = this.project.view;
    this.stage.zoomEvent.subscribe((level, viewPosition) => {
      if(typeof pre === 'function') pre();
      view.scale(level/view.zoom, view.viewToProject(viewPosition));
      if(this._center) this._center = view.center.clone();
      if(typeof post === 'function') post();
    });
  }

  _enableScroll(pre, post) {
    this.stage.scrollEvent.subscribe(offset => {
      if(typeof pre === 'function') pre();
      const view = this.view;
      const viewCenter = view.projectToView(view.center);
      view.center = view.viewToProject(viewCenter.subtract(offset));
      if(this._center) this._center = view.center.clone();
      if(typeof post === 'function') post();
    });
  }

  get view() { return this._project.view; }
  get project() { return this._project; }
  get stage() { return this._stage; }

  set autoUpdate(value) {
    if(typeof value !== 'boolean') throw new TypeError("autoUpdate must be a boolean.");

    this.view.autoUpdate = value;
  }

  get autoUpdate() { return this.view.autoUpdate; }

  update() { return this.view.update(); }
}
