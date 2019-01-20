const overlay = document.createElement('div');
overlay.className = "drop-file-overlay";
overlay.innerHTML = "<div>Suelte el archivo aquí para cargarlo</div>";
document.body.appendChild(overlay);

document.addEventListener("dragenter", function( e ) {
  if(!e.dataTransfer.types.includes('Files')) return;

  e.preventDefault();
  e.stopPropagation();

  dragStart();
}, false);

document.addEventListener("dragover", function( e ) {
  if(!e.dataTransfer.types.includes('Files')) return;

  e.preventDefault();
  e.stopPropagation();
}, false);

window.addEventListener("dragleave", function( e ) {
  if(!e.dataTransfer.types.includes('Files')) return;

  e.preventDefault();
  e.stopPropagation();

  dragEnd();
}, false);

document.body.addEventListener("drop", function( e ) {
  if(e.dataTransfer.files.length === 0) return;

  e.preventDefault();
  e.stopPropagation();

  dragEnd();

  handleFile(e.dataTransfer.files[0]);
}, false);

function dragStart() {
  overlay.className = "drop-file-overlay show";
}

function dragEnd() {
  overlay.className = "drop-file-overlay";
}

import {handleFile as loadFile} from './fileLoad.js';
import log from './classes/Logger.js'

async function handleFile(file) {
  if(!file.name.endsWith('.txt')) {
    log(log.FLAGS.ERROR, 'Archivo no soportado. La extensión debe ser ".txt".');
    return;
  }

  await loadFile(file);
}
