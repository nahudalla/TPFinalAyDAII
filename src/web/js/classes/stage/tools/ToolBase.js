import Observable from '../../Observable.js';

export default class ToolBase {
  constructor(stage) {
    this._activateEvent = new Observable();
    this._deactivateEvent = new Observable();

    this._stage = stage;

    this._isActive = false;

    this._gridPositionEnabled = false;
    this._gridPositionFilter = null;
    this._gridPositionToolTip = null;
  }

  get stage() { return this._stage; }

  get canExecuteOnPoint() { return false; }
  executeOnPoint() {
    if(this.canExecuteOnPoint) throw new Error('Abstract method not implemented.');
  }

  _enableGridPositionToolTip(filter) {
    if(this._gridPositionEnabled) return;

    this._gridPositionEnabled = true;
    this._gridPositionFilter = (typeof filter === 'function') ? filter : null;

    if(this._isActive) this._setupGridPositionToolTip();
  }

  _setupGridPositionToolTip() {
    let lastPosition, lastPositionFiltered;

    this._gridOutEventListener = ()=>{
      if(this._gridPositionToolTip) {
        document.body.removeChild(this._gridPositionToolTip);
        this._gridPositionToolTip = null;
      }
    };
    this._gridOverEventListener = eventData => {
      const prevPos = lastPosition;
      lastPosition = eventData.position;

      if( (eventData.position.equals(prevPos) && lastPositionFiltered)
          || (!eventData.position.equals(prevPos)
              && this._gridPositionFilter
              && (lastPositionFiltered = !this._gridPositionFilter(eventData.position))
             )
      ) {
        this._gridOutEventListener();
        return;
      }

      if(!this._gridPositionToolTip) {
        const el = this._gridPositionToolTip = document.createElement('div');
        el.className = 'mouseToolTip';
        document.body.appendChild(el);
      }

      if(!lastPosition.equals(prevPos)) {
        this._gridPositionToolTip.innerText = `(${lastPosition.x}, ${lastPosition.y})`;
      }
      this._gridPositionToolTip.style.top = `${eventData.event.pageY+20}px`;
      this._gridPositionToolTip.style.left = `${eventData.event.pageX}px`;
    };

    this._stage.interactive.gridOverEvent.subscribe(this._gridOverEventListener);
    this._stage.interactive.gridOutEvent.subscribe(this._gridOutEventListener);
  }

  _removeGridPositionToolTip(){
    if(!this._gridPositionEnabled) return;

    if(this._gridOutEventListener) this._gridOutEventListener();

    this._stage.interactive.gridOverEvent.unsubscribe(this._gridOverEventListener);
    this._stage.interactive.gridOutEvent.unsubscribe(this._gridOutEventListener);
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

    if(this._gridPositionEnabled) this._setupGridPositionToolTip();
  }
  deactivate() {
    if(!this._isActive) return;
    this._isActive = false;
    this._deactivateEvent.emit();

    this._removeGridPositionToolTip();
  }
}
