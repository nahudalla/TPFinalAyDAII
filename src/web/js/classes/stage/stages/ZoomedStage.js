import StageBase from './StageBase.js';
import Point from '../../Point.js';

export default class ZoomedStage extends StageBase {
  constructor(project, stage) {
    super(project, stage);

    super._centerOriginInView();
    super._enableZoom();
    super._enableScroll();

    this._layer = project.activeLayer;
  }

  addPoint(point) {
    if(!(point instanceof Point)) throw new Error('Invalid Point type!');

    this._pushStyle(this.stage.settings.point_style);

    this._layer.addChild(new this.stage.Circle({
      center: (new this.stage.Point(point.x, -point.y)).multiply(this.stage.settings.grid_size),
      radius: this.stage.settings.point_radius
    }));

    this._popStyle();
  }
}
