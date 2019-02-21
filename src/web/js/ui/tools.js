import getActiveContext, { activeContextChangedEvent } from '../classes/Context.js';

const CLEAR_ALL_BTN_QUERY = "#clearAllBtn";
const LOAD_FILE_BTN_QUERY = "#loadFileBtn";
const SAVE_FILE_BTN_QUERY = "#saveFileBtn";
const PAN_TOOL_BTN_QUERY = "#panToolBtn";
const ZOOM_IN_TOOL_BTN_QUERY = "#zoomInToolBtn";
const ZOOM_OUT_TOOL_BTN_QUERY = "#zoomOutToolBtn";
const ADD_POINT_TOOL_BTN_QUERY = "#addPointBtn";
const REMOVE_POINT_TOOL_BTN_QUERY = "#removePointBtn";

const ALGORITHM_SELECT_QUERY = "#algorithmSelect";
const EXECUTE_ALGORITHM_BTN_QUERY = "#executeAlgorBtn";

const UNDO_BTN_QUERY = "#undoBtn";
const REDO_BTN_QUERY = "#redoBtn";

import log from '../logger.js';

/* CLEAR ALL BUTTON */
(()=>{
  const elem = document.querySelector(CLEAR_ALL_BTN_QUERY);
  elem.addEventListener('click', ()=>{
    const pointsList = getActiveContext().pointsList;

    if(pointsList.length === 0) return;

    if(confirm('¿Desea eliminar todos los puntos del espacio de trabajo?')) {
      pointsList.clear();
      log(log.FLAGS.INFO_1, "Se eliminaron todos los puntos.");
    }
  });
})();

import triggerFileSelectionDialog from '../filesSupport/fileLoad.js';

/* LOAD FILE BUTTON */
(()=>{
  const elem = document.querySelector(LOAD_FILE_BTN_QUERY);
  elem.addEventListener('click', triggerFileSelectionDialog);
})();

import saveFile from '../filesSupport/fileSave.js'

/* SAVE FILE BUTTON */
(()=>{
  const elem = document.querySelector(SAVE_FILE_BTN_QUERY);
  elem.addEventListener("click", saveFile);
})();

/* ALGORITHM SELECTOR */
(()=>{
  const select = document.querySelector(ALGORITHM_SELECT_QUERY);
  const execute_btn = document.querySelector(EXECUTE_ALGORITHM_BTN_QUERY);

  while(select.firstChild) select.removeChild(select.firstChild);

  let lastContext = null;

  activeContextChangedEvent.subscribe(context => {
    if(lastContext) lastContext.algorithms.enabledAlgorithmsChangedEvent.unsubscribe(onAlgorChange);

    context.algorithms.algorithms.forEach(algorithm => {
      const opt = document.createElement('option');
      opt.value = algorithm.id;
      opt.innerText = algorithm.name;
      select.appendChild(opt);
    });

    lastContext = context;
    context.algorithms.enabledAlgorithmsChangedEvent.subscribe(onAlgorChange);
  });

  function onAlgorChange(algor, enabled) {
    if(!enabled) return;

    if(select.value !== algor.id) select.value = algor.id;

    log(log.FLAGS.INFO_2, `Usando algoritmo: ${algor.name}`);
  }

  select.addEventListener('change', ()=>{
    if(!lastContext) return;

    lastContext.algorithms.get(select.value).enable();
    lastContext.algorithms.enabledAlgorithms.forEach(algorithm => {
      if(algorithm.id !== select.value) algorithm.disable();
    })
  });

  execute_btn.addEventListener('click', ()=>{
    alert(select.value);
  });
})();

/* CANVAS TOOLS */
(()=>{
  const TOOL_SELECTED_CLASSNAME = 'selected';

  activeContextChangedEvent.subscribe(context => {
    const stage = context.stage;
    setupTool(context, stage.tools.panTool, PAN_TOOL_BTN_QUERY);
    setupTool(context, stage.tools.zoomInTool, ZOOM_IN_TOOL_BTN_QUERY);
    setupTool(context, stage.tools.zoomOutTool, ZOOM_OUT_TOOL_BTN_QUERY);
    setupTool(context, stage.tools.addPointTool, ADD_POINT_TOOL_BTN_QUERY);
    setupTool(context, stage.tools.removePointTool, REMOVE_POINT_TOOL_BTN_QUERY);
  });

  function setupTool(context, tool, query) {
    const elem = document.querySelector(query);

    if(tool.isActive) setToolAsActive(elem);
    else setToolAsNotActive(elem);

    tool.activateEvent.subscribe(onActivateTool);
    tool.deactivateEvent.subscribe(onDeactivateTool);
    elem.addEventListener('click', activate);
    context.deactivateEvent.subscribe(onContextDeactivate);

    function activate() {
      tool.activate();
    }

    function onActivateTool() {
      setToolAsActive(elem);
    }

    function onDeactivateTool() {
      setToolAsNotActive(elem);
    }

    function onContextDeactivate(){
      tool.activateEvent.unsubscribe(onActivateTool);
      tool.deactivateEvent.unsubscribe(onDeactivateTool);
      elem.removeEventListener('click', activate);
      context.deactivateEvent.unsubscribe(onContextDeactivate);
    }
  }

  function setToolAsActive(element) {
    element.classList.add(TOOL_SELECTED_CLASSNAME);
  }

  function setToolAsNotActive(element) {
    element.classList.remove(TOOL_SELECTED_CLASSNAME);
  }
})();

/* UNDO / REDO */

(()=>{
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
})();
