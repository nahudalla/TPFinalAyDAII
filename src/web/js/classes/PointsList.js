import Point from "./Point.js";
import Observable from "./Observable.js";

export default class PointsList {
  constructor() {
    this._points = new Map();

    this._addEvent = new Observable();
    this._removeEvent = new Observable();
  }

  get addEvent() {
    return this._addEvent;
  }

  get removeEvent() {
    return this._removeEvent;
  }

  forEach(fn) {
    this._points.forEach(value => fn(value));
  }

  get length() {
    return this._points.size;
  }

  add(point) {
    if(Array.isArray(point)) {
      point.forEach(p => this.add(p));
      return;
    }

    if(!(point instanceof Point)) {
      throw new TypeError("Not an instance of Point");
    }

    const key = `${point.x}_${point.y}`;

    if(this._points.has(key)) return;

    this._points.set(key, point);

    this.addEvent.emit(point);
  }

  remove(point) {
    if(!(point instanceof Point)) {
      throw new TypeError("Not an instance of Point");
    }

    const key = `${point.x}_${point.y}`;

    if(!this._points.delete(key)) return;

    this.removeEvent.emit(point);
  }

  has(point) {
    return this._points.has(`${point.x}_${point.y}`);
  }

  clear() {
    const points = this._points;
    this._points = new Map();

    points.forEach(point => {
      this.removeEvent.emit(point);
    });
  }
}
