import statusBar from './classes/StatusBar.js'
import {logger, FLAGS} from './classes/Logger.js'

logger.addListener(FLAGS.ALL, (message, flags) => {
  let msg = "";

  if((flags & FLAGS.CRITICAL) !== 0) msg = "ERROR CRÍTICO: ";
  else if((flags & FLAGS.ERROR) !== 0) msg = "ERROR: ";
  else if((flags & FLAGS.INFO_ALL) !== 0) msg = "INFORMACIÓN: ";

  msg += message;

  statusBar.setMessage(msg);

  if((flags & (FLAGS.CRITICAL | FLAGS.ERROR)) !== 0) {
    alert(msg);
  }
});

import './toolsSetup.js';
import './fileDropSetup.js';
import './canvasSetup.js';
