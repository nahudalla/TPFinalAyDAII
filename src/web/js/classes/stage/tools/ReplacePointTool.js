import getActiveContext from '../../Context.js';

const CURSOR_ADD = 'crosshair';
const CURSOR_SELECT = 'pointer';
const CURSOR_INVALID = 'not-allowed';

import PointToolBase from './PointToolBase.js';
import Point from '../../Point.js';

export default class ReplacePointTool extends PointToolBase {
  constructor(stage) {
    super(stage, {cursor: CURSOR_SELECT, cursor_invalid: CURSOR_INVALID});

    super._enableGridPositionToolTip();

    this._selectedPoint = null;
    this._visualElementRemover = null;
  }

  get canExecuteOnPoint() { return !this._selectedPoint; }

  _isInvalidPosition(position) {
    return this._selectedPoint
      ? (getActiveContext().pointsList.has(position) && !this._selectedPoint.equals(position))
      : !getActiveContext().pointsList.has(position);
  }

  _doClickAction(position) {
    if(this._isInvalidPosition(position)) return;

    if(this._visualElementRemover) {
      this._visualElementRemover();
      this._visualElementRemover = null;
    }

    if(this._selectedPoint) {
      this.options.cursor = CURSOR_SELECT;
      super._showPointerPosition(false);
      getActiveContext().pointsList.replace(position, this._selectedPoint);
      this._selectedPoint = null;
    } else {
      this.options.cursor = CURSOR_ADD;
      this._visualElementRemover = this.stage.interactive.addObject(
        new Point(position.x, position.y, 'visualElementForReplace'),
        {Point:{fillColor:'#25CE10',strokeWidth:1, strokeColor:'#000000'}}
      );
      super._showPointerPosition();
      this._selectedPoint = position;
    }
  }

  deactivate() {
    if(this._visualElementRemover) {
      this._visualElementRemover();
      this._visualElementRemover = null;
    }
    return super.deactivate();
  }
}
