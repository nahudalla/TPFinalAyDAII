export default class Point {
  constructor(x, y) {
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

  equals(point) {
    if(!(point instanceof Point)) return false;

    return point._x === this._x && point._y === this._y;
  }
}
