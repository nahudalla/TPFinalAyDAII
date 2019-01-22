import StageBase from './StageBase.js';

const paper = window.paper;

export default class AxesStage extends StageBase {
  constructor(project, stage) {
    super(project, stage);

    stage.zoomEvent.subscribe(()=>this.draw());
    stage.scrollEvent.subscribe(()=>this.draw());

    project.view.autoUpdate = false;

    project.currentStyle = stage.settings.axes_style;

    this.draw();
  }

  draw() {
    const Line = this.stage.Line;
    const Point = this.stage.Point;

    const layer = this.project.activeLayer;

    const axesBounds = this.view.bounds;
    const interactiveView = this.stage.interactive.view;
    const bounds = interactiveView.bounds;

    const padding = this.stage.settings.axes_padding;

    const interactivePadding = interactiveView.viewToProject(new Point(padding, 0)).subtract(interactiveView.viewToProject(new Point(0,0))).x;

    let x, y;

    if(bounds.top < -interactivePadding && bounds.bottom > interactivePadding) {
      y = this.view.viewToProject(
        interactiveView.projectToView(
          new Point(bounds.left, 0)
        )
      ).y;
    } else if(bounds.top > -interactivePadding) y = axesBounds.top + padding;
    else y = axesBounds.bottom - padding;

    if(bounds.left < -interactivePadding && bounds.right > interactivePadding) {
      x = this.view.viewToProject(
        interactiveView.projectToView(
          new Point(0, bounds.top)
        )
      ).x;
    } else if(bounds.left > -interactivePadding) x = axesBounds.left + padding;
    else x = axesBounds.right - padding;

    layer.removeChildren();

    layer.addChildren([
      new Line(new Point(axesBounds.left, y), new Point(axesBounds.right, y)),
      new Line(new Point(x, axesBounds.top), new Point(x, axesBounds.bottom))
    ]);

    this.view.update();
  }
}
