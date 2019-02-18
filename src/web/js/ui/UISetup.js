import { Context } from '../classes/Context.js';
import Point from '../classes/Point.js';
import Arrow from '../classes/Arrow.js';

const AXES_CANVAS_ID = "canvas_axes";
const ZOOMED_CANVAS_ID = "canvas_zoomed";
const NORMAL_BACK_CANVAS_ID = "canvas_normal_back";
const NORMAL_FRONT_CANVAS_ID = "canvas_normal_front";
const INTERACTIVE_CANVAS_ID = "canvas_interactive";

const context = new Context({
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

context.pointsList.add(new Point(0,0));
context.pointsList.add(new Point(3,3));
context.pointsList.add(new Point(3,-3));
context.pointsList.add(new Point(-3,3));
context.pointsList.add(new Point(-3,-3));
context.pointsList.add(new Point(-1,-1));

context.stage.normal_back.addObject(new Arrow(
  new Point(-2, -2),
  new Point(3, 3)
));

context.stage.normal_front.addObject(new Arrow(
  new Point(2, 2),
  new Point(-3, -3)
));

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
