const AXES_CANVAS_ID = "canvas_axes";
const ZOOMED_CANVAS_ID = "canvas_zoomed";
const NORMAL_CANVAS_ID = "canvas_normal";
const INTERACTIVE_CANVAS_ID = "canvas_interactive";

import Stage from '../classes/Stage.js';

const stage = new Stage({
  axes: document.getElementById(AXES_CANVAS_ID),
  zoomed: document.getElementById(ZOOMED_CANVAS_ID),
  normal: document.getElementById(NORMAL_CANVAS_ID),
  interactive: document.getElementById(INTERACTIVE_CANVAS_ID)
}, {
  use_active_paper_scope: true
});
