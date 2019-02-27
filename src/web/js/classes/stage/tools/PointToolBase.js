import ToolBase from './ToolBase.js';
import getActiveContext from '../../Context.js';

export default class PointToolBase extends ToolBase {
  constructor(stage, options) {
    super(stage);

    this._gridOverListener = null;
    this._pointRemover = null;
    this._resetCursor = false;
    this._showPtrPosition = false;
    this._options = options;
  }

  get options() {return this._options;}

  _showPointerPosition(value) { this._showPtrPosition = value !== false; }

  _isInvalidPosition(position) { throw new Error('Abstract method not implemented!'); }

  _doClickAction(position) { throw new Error('Abstract method not implemented!'); }

  _setDefaultCursor() {
    if(typeof this._options.cursor === 'string') {
      this.stage.interactive.setCursor(this._options.cursor);
    }
  }

  _setInvalidCursor() {
    if(typeof this._options.cursor_invalid === 'string') {
      this.stage.interactive.setCursor(this._options.cursor_invalid);
    }
  }

  executeOnPoint(point) {
    if(!this.canExecuteOnPoint) return;

    if(!getActiveContext().pointsList.has(point)) {
      throw new Error('The given position is not an active point.');
    }

    this._doClickAction(point);
  }

  activate() {
    if(this.isActive) return;

    this._setInvalidCursor();
    this._resetCursor = true;

    this._gridOverListener = eventData => {
      if(this._resetCursor) {
        this._setDefaultCursor();
        this._resetCursor = false;
      }

      this._gridOutListener();

      if(this._isInvalidPosition(eventData.position)) {
        this._setInvalidCursor();
        this._resetCursor = true;
      }else if(this._showPtrPosition) {
        this._pointRemover = this.stage.interactive.addObject(eventData.position);
      }
    };
    this._gridOutListener = ()=>{
      if(this._pointRemover) {
        const remover = this._pointRemover;
        this._pointRemover = null;
        remover();
      }
    };
    this._gridClickListener = eventData => {
      this._gridOutListener();
      this._doClickAction(eventData.gridPosition);
    };

    this.stage.interactive.gridOverEvent.subscribe(this._gridOverListener);
    this.stage.interactive.gridOutEvent.subscribe(this._gridOutListener);
    this.stage.interactive.clickEvent.subscribe(this._gridClickListener);

    super.activate();
  }

  deactivate() {
    if(!this.isActive) return;

    this._gridOutListener();

    this.stage.interactive.gridOverEvent.unsubscribe(this._gridOverListener);
    this.stage.interactive.gridOutEvent.unsubscribe(this._gridOutListener);
    this.stage.interactive.clickEvent.unsubscribe(this._gridClickListener);

    super.deactivate();
  }
}
