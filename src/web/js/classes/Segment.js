import Point from './Point.js';
import GeometricObject from './GeometricObject.js';

export default class Segment extends GeometricObject {
  constructor(from, to) {
    super();

    if(!(from instanceof Point && to instanceof Point)) {
      throw new TypeError("Invalid types for segment start and/or end point(s).");
    }

    this._from = from;
    this._to = to;
  }

  get from() { return this._from; }

  get to() { return this._to; }

  get hashString() {
    return `${this._from.hashString}_${this._to.hashString}_Segment`;
  }

  selectStyle(styles) {
    return styles['Segment'];
  }

  toPaperObject(stage, scale, applyStyles) {
    return applyStyles(this, ()=> {
      return new stage.Line(
        (new stage.Point(this._from.x, -this._from.y)).multiply(stage.settings.grid_size)
          .multiply(scale),
        (new stage.Point(this._to.x, -this._to.y)).multiply(stage.settings.grid_size)
          .multiply(scale)
      );
    });
  }

  equals(other) {
    if(!(other instanceof Segment)) return false;

    return this._from.equals(other._from) && this._to.equals(other._to);
  }
}
