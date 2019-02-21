import { activeContextChangedEvent } from '../../classes/Context.js';
import getActiveContext from '../../classes/Context.js';

const UNDO_BTN_QUERY = "#undoBtn";
const REDO_BTN_QUERY = "#redoBtn";

const undoBtn = document.querySelector(UNDO_BTN_QUERY);
const redoBtn = document.querySelector(REDO_BTN_QUERY);

let prevCtx = null, prevChangeListener = null;
activeContextChangedEvent.subscribe(ctx => {
  if(prevCtx && prevChangeListener) {
    prevCtx.pointsList.changedEvent.unsubscribe(prevChangeListener);
    prevChangeListener = null;
  }

  prevCtx = ctx;
  prevChangeListener = enable_disable_buttons.bind(this, ctx);

  ctx.pointsList.changedEvent.subscribe(prevChangeListener);

  enable_disable_buttons(ctx);
});

undoBtn.addEventListener('click', ()=> {
  getActiveContext().pointsList.undo();
});

redoBtn.addEventListener('click', ()=> {
  getActiveContext().pointsList.redo();
});

window.addEventListener('keyup', evt => {
  if(!evt.ctrlKey || evt.code !== 'KeyZ') return;

  if(evt.shiftKey) getActiveContext().pointsList.redo();
  else getActiveContext().pointsList.undo();
});

function enable_disable_buttons(ctx) {
  if(ctx.pointsList.undoAvailable) {
    undoBtn.classList.remove('disabled');
  } else {
    undoBtn.classList.add('disabled');
  }
  if(ctx.pointsList.redoAvailable) {
    redoBtn.classList.remove('disabled');
  } else {
    redoBtn.classList.add('disabled');
  }
}