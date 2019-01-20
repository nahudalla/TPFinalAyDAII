import log from './classes/Logger.js'
import pointsList from './classes/PointsList.js'

export default function saveFile() {
  log(log.FLAGS.INFO_1, "Preparando archivo para guardado...");

  let contents = "";

  pointsList.forEach(point=>{
    contents += `${point.x} ${point.y}\n`;
  });

  window.download(contents, "Puntos.txt", "text/plain");
}
