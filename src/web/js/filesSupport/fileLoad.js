import log from '../logger.js'
import {default as parse, ParserError} from './fileParser.js';
import getActiveContext from '../classes/Context.js';

const fileInput = document.createElement('input');
fileInput.type = 'file';
fileInput.multiple = false;
fileInput.accept = '.txt';

let currentPromise = null;

export default function triggerFileSelectionDialog() {
  if(currentPromise != null) currentPromise.reject();
  return new Promise((resolve, reject) => {
    currentPromise = {resolve, reject};
    fileInput.click();
  });
}

fileInput.addEventListener('change', ()=>{
  const prom = currentPromise;
  currentPromise = null;

  if(fileInput.files.length === 0) {
    prom.resolve();
    return;
  }

  loadFile(fileInput.files[0])
    .then(prom.resolve)
    .catch(prom.reject);
});

export function loadFile(file) {
  const pointsList = getActiveContext().pointsList;
  return new Promise((resolve, reject) => {
    const reader = new FileReader();
    reader.onload = ()=>{
      try {
        const points = parse(reader.result);

        if(points.length === 0) {
          log(log.FLAGS.INFO_1, "El archivo no contiene puntos")
        } else {
          if(pointsList.length === 0
              || confirm("¿Confirma la carga del archivo? Se borrarán todos los puntos actuales del espacio de trabajo."))
          {
            pointsList.clear();
            pointsList.add(points);
            log(log.FLAGS.INFO_1, 'Archivo cargado: '+file.name);
          } else {
            log(log.FLAGS.INFO_1, 'Carga cancelada.');
          }
        }
        resolve();
      } catch (e) {
        if(e instanceof ParserError) {
          log(log.FLAGS.ERROR, "Error al leer el contenido del archivo: formato inválido.");
        }
        reject(e);
      }
    };
    reader.onerror = (e)=>{
      log(log.FLAGS.ERROR, "No se pudo leer el archivo: "+file.name);
      reject(e);
    };
    log(log.FLAGS.INFO_3, "Leyendo contenidos del archivo: "+file.name);
    reader.readAsText(file);
  });
}
