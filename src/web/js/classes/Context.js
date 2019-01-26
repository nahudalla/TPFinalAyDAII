import Observable from './Observable.js';
import Stage from './Stage.js';
import PointsList from './PointsList.js';
import Algorithms from './Algorithms.js';

let activeContext = null;

export default function getActiveContext() {
  return activeContext;
};
export const activeContextChangedEvent = new Observable();

function setActiveContext(context) {
  const prev = activeContext;
  activeContext = context;
  if(prev) prev.deactivateEvent.emit(prev);
  activeContextChangedEvent.emit(context, prev);
}

export class Context {
  constructor(settings) {
    this._activateEvent = new Observable();
    this._deactivateEvent = new Observable();

    this._stage = new Stage(settings.stage, this);
    this._pointsList = new PointsList(settings.pointsList);
    this._algorithms = new Algorithms(this);

    this._pointsList.addEvent.subscribe(point => {
      this._stage.zoomed.addPoint(point);
    });

    this._pointsList.removeEvent.subscribe(point => {
      this._stage.zoomed.removePoint(point);
    });

    this.activate();
  }

  get stage() { return this._stage; }
  get pointsList() { return this._pointsList; }
  get algorithms() { return this._algorithms; }

  get activateEvent() { return this._activateEvent; }
  get deactivateEvent() { return this._deactivateEvent; }

  activate() {
    setActiveContext(this);
    this._activateEvent.emit(this);
  }
}
