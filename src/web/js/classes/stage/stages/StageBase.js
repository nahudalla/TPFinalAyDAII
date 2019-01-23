
export default class StageBase {
  constructor(project, stage) {
    this._project = project;
    this._stage = stage;

    this._styleStack = [];
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

  _enableZoom(pre, post) {
    const view = this.project.view;
    this.stage.zoomEvent.subscribe((level, viewPosition) => {
      if(typeof pre === 'function') pre();
      view.scale(level/view.zoom, view.viewToProject(viewPosition));
      if(typeof post === 'function') post();
    });
  }

  _enableScroll(pre, post) {
    this.stage.scrollEvent.subscribe(offset => {
      if(typeof pre === 'function') pre();
      const view = this.view;
      const viewCenter = view.projectToView(view.center);
      view.center = view.viewToProject(viewCenter.subtract(offset));
      if(typeof post === 'function') post();
    });
  }

  get view() { return this._project.view; }
  get project() { return this._project; }
  get stage() { return this._stage; }
}
