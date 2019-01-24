import StageWithPointsBase from './StageWithPointsBase.js';

export default class ZoomedStage extends StageWithPointsBase {
  constructor(project, stage) {
    super(project, stage);

    super._centerOriginInView();
    super._enableResetCenterOnResize();
    super._enableZoom();
    super._enableScroll();
  }

  _getPointStyle() { return this.stage.settings.point_style; }
}
