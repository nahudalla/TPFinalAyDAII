import getActiveContext, { activeContextChangedEvent } from '../classes/Context.js';

const CLEAR_ALL_BTN_QUERY = "#clearAllBtn";
const LOAD_FILE_BTN_QUERY = "#loadFileBtn";
const SAVE_FILE_BTN_QUERY = "#saveFileBtn";
const PAN_TOOL_BTN_QUERY = "#panToolBtn";
const ZOOM_IN_TOOL_BTN_QUERY = "#zoomInToolBtn";
const ZOOM_OUT_TOOL_BTN_QUERY = "#zoomOutToolBtn";
const ADD_POINT_TOOL_BTN_QUERY = "#addPointBtn";
const REMOVE_POINT_TOOL_BTN_QUERY = "#removePointBtn";

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