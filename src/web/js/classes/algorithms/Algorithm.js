import Observable from '../Observable.js';
import AlgorithmRunner from './AlgorithmRunner.js';

export default class Algorithm {
  constructor(context, name, id) {
    this._context = context;
    this._name = name;
    this._id = id;

    this._enabled = false;

    this._changedEvt = new Observable();
    this._inputDrawRemover = null;
    this._inputDrawEnabled = false;
  }

  get enabledStatusChangedEvent() { return this._changedEvt; }

  get name() { return this._name; }
  get id() { return this._id; }

  get enabled() { return this._enabled; }

  enable() {
    const fireEvent = !this._enabled;
    this._enabled = true;
    if(this._inputDrawEnabled) this._inputDrawRemover = this._redrawAlgorithmInput();
    if(fireEvent) this._changedEvt.emit(this._enabled);
  }
  disable() {
    const fireEvent = this._enabled;
    this._enabled = false;
    if(this._inputDrawRemover) {
      this._inputDrawRemover();
      this._inputDrawRemover = null;
    }
    if(fireEvent) this._changedEvt.emit(this._enabled);
  }

  _enableAlgorithmInputDraw() {
    this._inputDrawEnabled = true;
    this._context.pointsList.changedEvent.subscribe(()=>{
      if(!this._enabled) return;
      if(this._inputDrawRemover) this._inputDrawRemover();
      this._inputDrawRemover = this._redrawAlgorithmInput();
    });
    if(this._enabled) this._inputDrawRemover = this._redrawAlgorithmInput();
  }
  _redrawAlgorithmInput() {throw new Error('Abstract method not implemented.');}

  // TODO: cache result until pointsList change
  run() { return new AlgorithmRunner([this], this._context.pointsList.buildInt32CoordinatesArray()); }
}
