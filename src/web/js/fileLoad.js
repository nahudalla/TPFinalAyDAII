const fileInput = document.createElement('input');
fileInput.type = 'file';
fileInput.multiple = false;
fileInput.accept = '.txt';

let currentPromise = null;

export default function loadFile() {
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

  handleFile(fileInput.files[0])
    .then(prom.resolve)
    .catch(prom.reject);
});

import log from './classes/Logger.js'
import {default as parse, ParserError} from './classes/InputFileParser.js';
import pointsList from './classes/PointsList.js'

export function handleFile(file) {
  return new Promise((resolve, reject) => {
    log(log.FLAGS.INFO_3, "Leyendo contenidos del archivo: "+file.name);
    const reader = new FileReader();
    reader.onload = ()=>{
      try {
        const points = parse(reader.result);

        if(points.length === 0) {
          log(log.FLAGS.INFO_1, "El archivo no contiene puntos")
        } else {
          if(pointsList.length === 0
              || confirm("¿Seguro desea cargar el archivo? Se borrarán todos los puntos actuales del espacio de trabajo."))
          {
            pointsList.clear();
            pointsList.add(points);
          }
        }
        resolve();
      } catch (e) {
        if(e instanceof ParserError) {
          log(log.FLAGS.ERROR, "No se pudo cargar el archivo. Formato inválido.");
        }
        reject(e);
      }
    };
    reader.onerror = ()=>{
      log(log.FLAGS.ERROR, "No se pudo leer el archivo: "+file.name);
      reject();
    };
    reader.readAsText(file);
  });
}
