import Point from "./Point.js";
import Observable from "./Observable.js";

export default class PointsList {
  constructor() {
    this._orderedPoints = [];
    this._points = new Map();

    this._addEvent = new Observable();
    this._removeEvent = new Observable();
    this._reorderEvent = new Observable();
  }

  get addEvent() {
    return this._addEvent;
  }

  get removeEvent() {
    return this._removeEvent;
  }

  get reorderEvent() {
    return this._reorderEvent;
  }

  forEach(fn) {
    this._orderedPoints.forEach(value => fn(value));
  }

  get length() {
    return this._points.size;
  }

  reorder(point, beforePoint) {
    const key = `${point.x}_${point.y}`;

    const oldIndex = this._points.get(key);

    if(oldIndex !== 0 && !oldIndex) return;

    if(!beforePoint) {
      this._orderedPoints.splice(oldIndex, 1);
      this._orderedPoints.push(point);
      for(let i = oldIndex; i < this._orderedPoints.length; ++i) {
        this._points.set(`${this._orderedPoints[i].x}_${this._orderedPoints[i].y}`, i);
      }
    } else {
      const beforeKey = `${beforePoint.x}_${beforePoint.y}`;
      const newIndex = this._points.get(beforeKey);

      if(newIndex !== 0 && !newIndex) return;

      this._points.set(key, newIndex);
      this._orderedPoints.splice(oldIndex, 1);
      this._orderedPoints.splice(newIndex+(oldIndex < newIndex ? -1 : 0), 0, point);
      for(let i = Math.min(oldIndex, newIndex); i < this._orderedPoints.length; ++i) {
        this._points.set(`${this._orderedPoints[i].x}_${this._orderedPoints[i].y}`, i);
      }
    }

    this._reorderEvent.emit(point, beforePoint);
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

    this._points.set(key, this._orderedPoints.length);
    this._orderedPoints.push(point);

    this.addEvent.emit(point);
  }

  remove(point) {
    if(!(point instanceof Point)) {
      throw new TypeError("Not an instance of Point");
    }

    const key = `${point.x}_${point.y}`;

    const index = this._points.get(key);

    if(!this._points.delete(key)) return;

    this._orderedPoints.splice(index, 1);

    this.removeEvent.emit(point);
  }

  has(point) {
    return this._points.has(`${point.x}_${point.y}`);
  }

  clear() {
    const points = this._orderedPoints;
    this._points = new Map();
    this._orderedPoints = [];

    points.forEach(point => {
      this.removeEvent.emit(point);
    });
  }
}
