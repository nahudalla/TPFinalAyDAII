import StageWithGeometricObjectsBase from './StageWithGeometricObjectsBase.js';

export default class NormalStage extends StageWithGeometricObjectsBase {
  constructor(project, stage, objectStyles) {
    super(project, stage, objectStyles);
    super._centerOriginInView();
    super._setupZoomStorage();
    super._enableCoordinatesScaling();
    super._enableZoom(
      () => {
        super._restoreZoom();
        this.autoUpdate = false;
      },
      () => {
        super._redrawObjects();
        super.update();
        this.autoUpdate = true;
        super._saveZoom();
      }
    );
    super._enableScroll(
      ()=>{super._restoreZoom();},
      ()=>{super._saveZoom();}
    );
    super._enableResetCenterOnResize();
  }
}
