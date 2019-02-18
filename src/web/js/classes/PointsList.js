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

  buildInt32CoordinatesArray() {
    return new Int32Array(this._orderedPoints);
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
    for(let i = 0; i < this._orderedPoints.length; i += 2) {
      fn(new Point(this._orderedPoints[i], this._orderedPoints[i+1]))
    }
  }

  get length() {
    return this._points.size;
  }

  reorder(point, beforePoint) {
    const key = point.hashString;

    const oldIndex = this._points.get(key);

    if(oldIndex !== 0 && !oldIndex) return;

    if(!beforePoint) {
      this._orderedPoints.splice(oldIndex, 2);
      this._orderedPoints.push(point.x, point.y);
      for(let i = oldIndex; i < this._orderedPoints.length; i += 2) {
        this._points.set((new Point(this._orderedPoints[i], this._orderedPoints[i+1])).hashString, i);
      }
    } else {
      const beforeKey = beforePoint.hashString;
      const newIndex = this._points.get(beforeKey);

      if(newIndex !== 0 && !newIndex) return;

      this._points.set(key, newIndex);
      this._orderedPoints.splice(oldIndex, 2);
      this._orderedPoints.splice(newIndex+(oldIndex < newIndex ? -2 : 0), 0, point.x, point.y);
      for(let i = Math.min(oldIndex, newIndex); i < this._orderedPoints.length; i += 2) {
        this._points.set((new Point(this._orderedPoints[i], this._orderedPoints[i+1])).hashString, i);
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

    const key = point.hashString;

    if(this._points.has(key)) return;

    this._points.set(key, this._orderedPoints.length);
    this._orderedPoints.push(point.x, point.y);

    this.addEvent.emit(point);
  }

  remove(point) {
    if(!(point instanceof Point)) {
      throw new TypeError("Not an instance of Point");
    }

    const key = point.hashString;

    const index = this._points.get(key);

    if(!this._points.delete(key)) return;

    this._orderedPoints.splice(index, 2);

    this.removeEvent.emit(point);
  }

  has(point) {
    return this._points.has(point.hashString);
  }

  clear() {
    const points = this._orderedPoints;
    this._points = new Map();
    this._orderedPoints = [];

    for(let i = 0; i < points.length; i += 2) {
      this.removeEvent.emit(new Point(points[i], points[i+1]))
    }
  }
}
