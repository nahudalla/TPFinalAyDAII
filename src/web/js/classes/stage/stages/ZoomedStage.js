import StageWithGeometricObjectsBase from './StageWithGeometricObjectsBase.js';

export default class ZoomedStage extends StageWithGeometricObjectsBase {
  constructor(project, stage) {
    super(project, stage, {
      'Point': stage.settings.point_style
    });

    super._centerOriginInView();
    super._enableResetCenterOnResize();
    super._enableZoom();
    super._enableScroll();
  }
}
