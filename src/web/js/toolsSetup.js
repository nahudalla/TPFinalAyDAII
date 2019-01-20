const CLEAR_ALL_BTN_QUERY = "#clearAllBtn";
const LOAD_FILE_BTN_QUERY = "#loadFileBtn";
const SAVE_FILE_BTN_QUERY = "#saveFileBtn";

setupClearAllBtn();
setupLoadFileBtn();
setupSaveFileBtn();

import pointsList from './classes/PointsList.js';
import log from './classes/Logger.js';

function setupClearAllBtn() {
  const elem = document.querySelector(CLEAR_ALL_BTN_QUERY);
  elem.addEventListener('click', ()=>{
    if(pointsList.length === 0) return;

    if(confirm('¿Desea eliminar todos los puntos del espacio de trabajo?')) {
      pointsList.clear();
      log(log.FLAGS.INFO_1, "Se eliminaron todos los puntos.");
    }
  });
}

import loadFile from './fileLoad.js';

function setupLoadFileBtn() {
  const elem = document.querySelector(LOAD_FILE_BTN_QUERY);
  elem.addEventListener('click', loadFile);
}

import saveFile from './fileSave.js'

function setupSaveFileBtn() {
  const elem = document.querySelector(SAVE_FILE_BTN_QUERY);
  elem.addEventListener("click", saveFile);
}
