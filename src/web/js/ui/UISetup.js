import { Context } from '../classes/Context.js';
import Point from '../classes/Point.js';

const AXES_CANVAS_ID = "canvas_axes";
const ZOOMED_CANVAS_ID = "canvas_zoomed";
const NORMAL_CANVAS_ID = "canvas_normal";
const INTERACTIVE_CANVAS_ID = "canvas_interactive";

const context = new Context({
  stage: {
    views: {
      axes: document.getElementById(AXES_CANVAS_ID),
      zoomed: document.getElementById(ZOOMED_CANVAS_ID),
      normal: document.getElementById(NORMAL_CANVAS_ID),
      interactive: document.getElementById(INTERACTIVE_CANVAS_ID)
    },
    use_active_paper_scope: true
  }
});

context.pointsList.add(new Point(0,0));
context.pointsList.add(new Point(3,3));
context.pointsList.add(new Point(3,-3));
context.pointsList.add(new Point(-3,3));
context.pointsList.add(new Point(-3,-3));
context.pointsList.add(new Point(-1,-1));

context.algorithms.grahamScan.enable();
context.algorithms.jarvisMarch.enable();
context.algorithms.closestPairOfPoints.enable();

const setEvents = runner => {
  runner.progressEvent.subscribe((...args) => console.log(...args));
  runner.doneEvent.subscribe((...args) => console.log(...args));
  runner.errorEvent.subscribe((...args) => console.log(...args));
};

const runner = context.algorithms.run();
setEvents(runner);
