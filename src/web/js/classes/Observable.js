export default class Observable {
  constructor() {
    this._listeners = [];
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
    this._listeners.forEach(listener => listener(...data));
  }
}
