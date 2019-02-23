import getActiveContext from '../../Context.js';

const CURSOR = 'crosshair';
const CURSOR_INVALID = 'not-allowed';

import PointToolBase from './PointToolBase.js';

export default class AddPointTool extends PointToolBase {
  constructor(stage) {
    super(stage, {cursor: CURSOR, cursor_invalid: CURSOR_INVALID});

    super._enableGridPositionToolTip();
    super._showPointerPosition();
  }

  _isInvalidPosition(position) {
    return getActiveContext().pointsList.has(position);
  }

  _doClickAction(position) {
    if(this._isInvalidPosition(position)) return;

    getActiveContext().pointsList.add(position);
  }
}
