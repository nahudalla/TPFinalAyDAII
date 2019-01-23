import statusBar from '../statusBar.js'
import {logger, FLAGS} from '../logger.js'

logger.addListener(FLAGS.ALL, (message, flags) => {
  let msg = "";

  if((flags & FLAGS.CRITICAL) !== 0) msg = "ERROR CRÍTICO: ";
  else if((flags & FLAGS.ERROR) !== 0) msg = "ERROR: ";
  else if((flags & FLAGS.INFO_ALL) !== 0) msg = "INFORMACIÓN: ";

  const pre = msg;

  msg += message;

  statusBar.setMessage(pre, message);

  if((flags & (FLAGS.CRITICAL | FLAGS.ERROR)) !== 0) {
    console.error(msg);
    alert(msg);
  } else {
    console.log(msg);
  }
});
