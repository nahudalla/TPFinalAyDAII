import getActiveContext from '../../Context.js';

const CURSOR = 'pointer';
const CURSOR_INVALID = 'not-allowed';

import PointToolBase from './PointToolBase.js';

export default class RemovePointTool extends PointToolBase {
  constructor(stage) {
    super(stage, {cursor: CURSOR, cursor_invalid: CURSOR_INVALID});

    super._enableGridPositionToolTip(position => !this._isInvalidPosition(position));
  }

  _isInvalidPosition(position) {
    return !getActiveContext().pointsList.has(position);
  }

  _doClickAction(position) {
    if(this._isInvalidPosition(position)) return;

    getActiveContext().pointsList.remove(position);
  }

  get canExecuteOnPoint() { return true; }
}
