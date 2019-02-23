import Observable from './classes/Observable.js';

let frozenReason = null;

export const userAuthorizedModificationEvent = new Observable();

export function freezeAppState(reason) {
  if(typeof reason !== 'string') {
    throw new TypeError("Invalid freeze reason. Parameter 'reason' must be a string.");
  }

  frozenReason = reason;
}
export function unfreezeAppState() {
  frozenReason = null;
}

export default function canModifyAppState(action) {
  if(typeof action !== 'string') {
    throw new TypeError("Invalid action. Parameter 'action' must be a string.");
  }

  if(!frozenReason) return true;
  else if(confirm(`¿Confirma ${action}?\n${frozenReason}`)) {
    userAuthorizedModificationEvent.emit();
    return true;
  } else return false;
}
