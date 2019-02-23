import { Context } from '../classes/Context.js';
import Point from '../classes/Point.js';

const AXES_CANVAS_ID = "canvas_axes";
const ZOOMED_CANVAS_ID = "canvas_zoomed";
const NORMAL_BACK_CANVAS_ID = "canvas_normal_back";
const NORMAL_FRONT_CANVAS_ID = "canvas_normal_front";
const INTERACTIVE_CANVAS_ID = "canvas_interactive";

new Context({
  stage: {
    views: {
      axes: document.getElementById(AXES_CANVAS_ID),
      zoomed: document.getElementById(ZOOMED_CANVAS_ID),
      normal_back: document.getElementById(NORMAL_BACK_CANVAS_ID),
      normal_front: document.getElementById(NORMAL_FRONT_CANVAS_ID),
      interactive: document.getElementById(INTERACTIVE_CANVAS_ID)
    },
    use_active_paper_scope: true
  }
});
