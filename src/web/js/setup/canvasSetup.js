const CANVAS_ID = "canvas";

import log from '../classes/Logger.js';

if(!("paper" in window) || !('setup' in window.paper)) {
  log(log.FLAGS.CRITICAL, "Could not initialize canvas: Paper.js not loaded!");
}

const paper = window.paper;

paper.setup(CANVAS_ID);

import {initializeStage} from '../classes/Stage.js';

initializeStage();
