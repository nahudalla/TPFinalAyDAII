import getActiveContext from '../../classes/Context.js';
import log from '../../logger.js';

const CLEAR_ALL_BTN_QUERY = "#clearAllBtn";

const elem = document.querySelector(CLEAR_ALL_BTN_QUERY);
elem.addEventListener('click', ()=>{
  const pointsList = getActiveContext().pointsList;

  if(pointsList.length === 0) return;

  if(confirm('¿Desea eliminar todos los puntos del espacio de trabajo?')) {
    pointsList.clear();
    log(log.FLAGS.INFO_1, "Se eliminaron todos los puntos.");
  }
});