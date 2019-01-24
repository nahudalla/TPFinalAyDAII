import StageBase from './StageBase.js';
import Point from '../../Point.js';

export default class StageWithPointsBase extends StageBase {
  constructor(project, stage) {
    super(project, stage);

    this._layer = project.activeLayer;
    this._points = new Map();
  }

  _getPointStyle() {throw new Error("Abstract method not implemented!");}

  addPoint(point) {
    if(!(point instanceof Point)) throw new Error('Invalid Point type!');

    this.project.activate();

    this._pushStyle(this._getPointStyle());

    const p = new this.stage.Circle({
      center: (new this.stage.Point(point.x, -point.y)).multiply(this.stage.settings.grid_size),
      radius: this.stage.settings.point_radius
    });

    this._points.set(`${point.x}_${point.y}`, p);

    this._layer.addChild(p);

    this._popStyle();

    return ()=>{this.removePoint(point);}
  }

  removePoint(point) {
    if(!(point instanceof Point)) throw new Error('Invalid Point type!');

    this.project.activate();

    const key = `${point.x}_${point.y}`;

    const item = this._points.get(key);
    if(item) {
      item.remove();
      this._points.delete(key);
    }
  }
}
