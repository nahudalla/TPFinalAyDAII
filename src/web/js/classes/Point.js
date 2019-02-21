import GeometricObject from './GeometricObject.js';

export default class Point extends GeometricObject {
  constructor(x, y) {
    super();

    if(typeof x !== 'number' || typeof y !== 'number')
      throw new TypeError("Invalid Point coordinates: "+x+", "+y);

    this._x = x === -0 ? 0 : x;
    this._y = y === -0 ? 0 : y;
  }

  get x() {
    return this._x;
  }

  get y() {
    return this._y;
  }

  get hashString() {
    return `${this._x}_${this._y}_Point`;
  }

  toPaperObject(stage, scale, applyStyles) {
    return applyStyles(this, ()=> {
      return new stage.Circle({
        center: (new stage.Point(this._x, -this._y)).multiply(stage.settings.grid_size)
          .multiply(scale),
        radius: stage.settings.point_radius
      });
    });
  }

  selectStyle(styles) {
    return styles['Point'];
  }

  equals(point) {
    if(!(point instanceof Point)) return false;

    return point._x === this._x && point._y === this._y;
  }
}
