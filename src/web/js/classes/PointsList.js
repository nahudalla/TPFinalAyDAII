const POINTS_LIST_SELECTOR = "#pointsList";

import Point from "./Point.js";
import Observable from "./Observable.js";

class PointsList extends Observable {
  static get POINT_ADDED_EVENT() {return 0;};
  static get POINT_REMOVED_EVENT() {return 1;};

  constructor(container) {
    super();

    this._container = container;
    this._points = [];
  }

  forEach(callback) {
    this._points.forEach((value, index)=>{
      callback(value.point, index)
    });
  }

  set container(container) {
    this._container = container;
  }

  get container() {
    if(this._container === null) {
      throw new Error("PointsList has not been setup!");
    }

    return this._container;
  }

  get points() {
    return Array.from(this._points);
  }

  get length() {
    return this._points.length;
  }

  add(point) {
    if(Array.isArray(point)) {
      point.forEach(p => this.add(p));
      return;
    }

    if(!(point instanceof Point))
      throw new TypeError("Not an instance of Point");

    const elem = generateElement(point);

    this._points.push({point, elem});
    this.container.appendChild(elem);

    this.emit(PointsList.POINT_ADDED_EVENT, point);
  }

  remove(point) {
    if(!(point instanceof Point))
      throw new TypeError("Not an instance of Point");

    const i = this._points.findIndex(entry => entry.point.equals(point));

    const elem = this._points.splice(i, 1)[0].elem;

    this.container.removeChild(elem);

    this.emit(PointsList.POINT_REMOVED_EVENT, point);
  }

  clear() {
    this._points.forEach(entry => {
      this.container.removeChild(entry.elem);
      this.emit(PointsList.POINT_REMOVED_EVENT, entry.point);
    });

    this._points = [];
  }
}

function generateElement(point) {
  const li = document.createElement('li');
  const icon = document.createElement('span');
  icon.className = 'material-icons';
  icon.innerText = 'reorder';
  li.appendChild(icon);
  const text = document.createElement('span');
  text.innerText = `(${point.x}, ${point.y})`;
  li.appendChild(text);
  li.dataset.x = point.x;
  li.dataset.y = point.y;
  return li;
}

const instance = new PointsList(document.querySelector(POINTS_LIST_SELECTOR));

export default instance;
