import Observable from '../../Observable.js';

export default class ToolBase {
  constructor() {
    this._activateEvent = new Observable();
    this._deactivateEvent = new Observable();

    this._isActive = false;
  }

  get isActive() {
    return this._isActive;
  }

  get activateEvent() { return this._activateEvent; }
  get deactivateEvent() { return this._deactivateEvent; }

  activate() {
    if(this._isActive) return;
    this._isActive = true;
    this._activateEvent.emit();
  }
  deactivate() {
    if(!this._isActive) return;
    this._isActive = false;
    this._deactivateEvent.emit();
  }
}
