export default class Observable {
  constructor() {
    this._listeners = [];
    this._paused = false;
  }

  get paused() {
    return this._paused;
  }

  set paused(value) {
    if(typeof value !== 'boolean') throw new TypeError('Property "paused" must be a boolean.');
    this._paused = value;
  }

  get listenerCount() { return this._listeners.length; }

  subscribe(fn) {
    if(typeof fn !== 'function')
      throw new TypeError("Parameter is not a function");

    this._listeners.push(fn);
  }

  unsubscribe(fn) {
    if(typeof fn !== 'function')
      throw new TypeError("Parameter is not a function");

    this._listeners = this._listeners.filter(elem => elem !== fn);
  }

  removeAll() { this._listeners = []; }

  emit(...data) {
    if(this._paused) return;
    this._listeners.forEach(listener => listener(...data));
  }
}
