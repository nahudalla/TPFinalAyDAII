import Point from "./Point.js";
import Observable from "./Observable.js";
import canModifyAppState from '../appStateFreeze.js';

export default class PointsList {
  constructor(settings) {
    this._orderedPoints = [];
    this._points = new Map();

    this._addEvent = new Observable();
    this._removeEvent = new Observable();
    this._reorderEvent = new Observable();
    this._replaceEvent = new Observable();

    this._changedEvent = new Observable();

    this._maxHistory = (settings && settings.maxHistory) || 77;
    this._undoRedoInProgress = false;
    this._undoHist = [];
    this._redoHist = [];

    this._changedEvent.subscribe((...data) => {
      Sentry.configureScope((scope) => {
        scope.setExtra("points", this._orderedPoints);
        scope.setExtra('undo', this._undoHist);
        scope.setExtra('redo', this._redoHist);
      });
      if(this._undoRedoInProgress) return;
      this._redoHist = [];
      this._undoHist.push(data);
      if(this._undoHist.length > this._maxHistory) this._undoHist.splice(0, 1);
    });

    this._addEvent.subscribe(this._changedEvent.emit.bind(this._changedEvent, 'add'));
    this._removeEvent.subscribe(this._changedEvent.emit.bind(this._changedEvent, 'remove'));
    this._reorderEvent.subscribe(this._changedEvent.emit.bind(this._changedEvent, 'reorder'));
    this._replaceEvent.subscribe(this._changedEvent.emit.bind(this._changedEvent, 'replace'));
  }

  get _canModify() {
    return canModifyAppState('modificar los puntos de entrada');
  }

  get undoAvailable() {return this._undoHist.length !== 0;}
  get redoAvailable() {return this._redoHist.length !== 0;}

  undo() {
    if(!this._canModify) return false;

    if(this._undoRedoInProgress || !this.undoAvailable) return false;

    this._undoRedoInProgress = true;

    const action = this._undoHist.pop();
    const [type, point, oldPoint, wasBeforePoint] = action;

    let res;
    if(type === 'add') res = this.remove(point);
    else if(type === 'remove') res = this.add(point);
    else if(type === 'reorder') res = this.reorder(point, wasBeforePoint);
    else if(type === 'replace') res = this.replace(oldPoint, point);

    if(res) this._redoHist.push(action);
    else this._undoHist.pop(action);

    this._undoRedoInProgress = false;

    return res;
  }

  redo() {
    if(!this._canModify) return false;

    if(this._undoRedoInProgress || !this.redoAvailable) return false;

    this._undoRedoInProgress = true;

    const action = this._redoHist.pop();
    const [type, point, beforePoint] = action;

    let res;
    if(type === 'add') res = this.add(point);
    else if(type === 'remove') res = this.remove(point);
    else if(type === 'reorder') res = this.reorder(point, beforePoint);
    else if(type === 'replace') res = this.replace(point, beforePoint);

    if(res) this._undoHist.push(action);
    else this._redoHist.push(action);

    this._undoRedoInProgress = false;

    return res;
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

  get replaceEvent() {
    return this._replaceEvent;
  }

  get changedEvent() {
    return this._changedEvent;
  }

  forEach(fn) {
    for(let i = 0; i < this._orderedPoints.length; i += 2) {
      fn(new Point(this._orderedPoints[i], this._orderedPoints[i+1]))
    }
  }

  get length() {
    return this._points.size;
  }

  replace(newPoint, oldPoint) {
    if(newPoint.equals(oldPoint) || !this._canModify) return false;

    const index = this._points.get(oldPoint.hashString);
    if((index !== 0 && !index) || this._points.has(newPoint.hashString)) {
      return false;
    }

    this._orderedPoints[index] = newPoint.x;
    this._orderedPoints[index + 1] = newPoint.y;
    this._points.delete(oldPoint.hashString);
    this._points.set(newPoint.hashString, index);

    this._replaceEvent.emit(newPoint, oldPoint);
  }

  reorder(point, beforePoint) {
    if(!this._canModify) return false;

    const key = point.hashString;

    const oldIndex = this._points.get(key);

    if(oldIndex !== 0 && !oldIndex) return false;

    let wasBeforePoint = null;
    if(oldIndex < this._orderedPoints.length-2) {
      wasBeforePoint = new Point(
        this._orderedPoints[oldIndex+2],
        this._orderedPoints[oldIndex+3]
      );
    }

    if(!beforePoint) {
      this._orderedPoints.splice(oldIndex, 2);
      this._orderedPoints.push(point.x, point.y);
      for(let i = oldIndex; i < this._orderedPoints.length; i += 2) {
        this._points.set((new Point(this._orderedPoints[i], this._orderedPoints[i+1])).hashString, i);
      }
    } else {
      const beforeKey = beforePoint.hashString;
      const newIndex = this._points.get(beforeKey);

      if(newIndex !== 0 && !newIndex) return false;

      this._points.set(key, newIndex);
      this._orderedPoints.splice(oldIndex, 2);
      this._orderedPoints.splice(newIndex+(oldIndex < newIndex ? -2 : 0), 0, point.x, point.y);
      for(let i = Math.min(oldIndex, newIndex); i < this._orderedPoints.length; i += 2) {
        this._points.set((new Point(this._orderedPoints[i], this._orderedPoints[i+1])).hashString, i);
      }
    }

    this._reorderEvent.emit(point, beforePoint, wasBeforePoint);

    return true;
  }

  add(point) {
    if(!this._canModify) return false;

    if(Array.isArray(point)) {
      let res = true;
      point.forEach(p => {res = res && this.add(p);});
      return res;
    }

    if(!(point instanceof Point)) {
      throw new TypeError("Not an instance of Point");
    }

    const key = point.hashString;

    if(this._points.has(key)) return false;

    this._points.set(key, this._orderedPoints.length);
    this._orderedPoints.push(point.x, point.y);

    this.addEvent.emit(point);

    return true;
  }

  remove(point) {
    if(!this._canModify) return false;

    if(!(point instanceof Point)) {
      throw new TypeError("Not an instance of Point");
    }

    if(this._points.has(point.hashString)) {
      this.reorderEvent.paused = true;
      this.reorder(point);
      this.reorderEvent.paused = false;

      this._points.delete(point.hashString);
      this._orderedPoints.splice(this._orderedPoints.length-2, 2);

      this.removeEvent.emit(point);

      return true;
    }

    return false;
  }

  has(point) {
    return this._points.has(point.hashString);
  }

  clear() {
    if(!this._canModify) return false;
    
    const points = this._orderedPoints;
    this._points = new Map();
    this._orderedPoints = [];

    this._undoRedoInProgress = true;
    this._redoHist = [];
    this._undoHist = [];

    for(let i = 0; i < points.length; i += 2) {
      this.removeEvent.emit(new Point(points[i], points[i+1]))
    }
    this._undoRedoInProgress = false;

    return true;
  }
}
