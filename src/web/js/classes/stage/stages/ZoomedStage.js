import StageBase from './StageBase.js';

export default class ZoomedStage extends StageBase {
  constructor(project, stage) {
    super(project, stage);

    super._enableZoom();

    stage.scrollEvent.subscribe(offset => {
      const view = project.view;
      const viewCenter = view.projectToView(view.center);
      view.center = view.viewToProject(viewCenter.subtract(offset));
    });
  }
}
