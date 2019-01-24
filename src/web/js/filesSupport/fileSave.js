import log from '../logger.js'
import getActiveContext from '../classes/Context.js';

export default function saveFile() {
  log(log.FLAGS.INFO_1, "Preparando archivo para guardado...");

  let contents = "";

  getActiveContext().pointsList.forEach(point=>{
    contents += `${point.x} ${point.y}\n`;
  });

  window.download(contents, "Puntos.txt", "text/plain");
}
