
export default class StageBase {
  constructor(project, stage) {
    project.view.center = new stage.Point(0,0);

    this._project = project;
    this._stage = stage;
  }

  _enableZoom() {
    const view = this.project.view;
    this.stage.zoomEvent.subscribe((level, viewPosition) => {
      view.scale(level/view.zoom, view.viewToProject(viewPosition));
    });
  }

  get view() { return this._project.view; }
  get project() { return this._project; }
  get stage() { return this._stage; }
}
