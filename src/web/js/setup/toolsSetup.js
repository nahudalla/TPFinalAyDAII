const CLEAR_ALL_BTN_QUERY = "#clearAllBtn";
const LOAD_FILE_BTN_QUERY = "#loadFileBtn";
const SAVE_FILE_BTN_QUERY = "#saveFileBtn";
const PAN_TOOL_BTN_QUERY = "#panToolBtn";

import pointsList from '../classes/PointsList.js';
import log from '../classes/Logger.js';

/* CLEAR ALL BUTTON */
(()=>{
  const elem = document.querySelector(CLEAR_ALL_BTN_QUERY);
  elem.addEventListener('click', ()=>{
    if(pointsList.length === 0) return;

    if(confirm('¿Desea eliminar todos los puntos del espacio de trabajo?')) {
      pointsList.clear();
      log(log.FLAGS.INFO_1, "Se eliminaron todos los puntos.");
    }
  });
})();

import loadFile from '../fileLoad.js';

/* LOAD FILE BUTTON */
(()=>{
  const elem = document.querySelector(LOAD_FILE_BTN_QUERY);
  elem.addEventListener('click', loadFile);
})();

import saveFile from '../fileSave.js'

/* SAVE FILE BUTTON */
(()=>{
  const elem = document.querySelector(SAVE_FILE_BTN_QUERY);
  elem.addEventListener("click", saveFile);
})();

import {TOOLS} from '../classes/Stage.js';

(()=>{
  const TOOL_SELECTED_CLASSNAME = ' selected';

  setupTool(TOOLS.PAN, PAN_TOOL_BTN_QUERY);

  function setupTool(tool, query) {
    const elem = document.querySelector(query);

    if(tool.isActive()) {
      elem.className += TOOL_SELECTED_CLASSNAME;
    }

    tool.on({
      activate(){
        if(!elem.className.includes(TOOL_SELECTED_CLASSNAME)) {
          elem.className += TOOL_SELECTED_CLASSNAME;
        }
      },
      deactivate() {
        elem.className.replace(TOOL_SELECTED_CLASSNAME, '');
      }
    });

    elem.addEventListener('click', ()=>{
      tool.activate();
    });
  }
})();
