import { activeContextChangedEvent } from '../classes/Context.js';
import getActiveContext from '../classes/Context.js';
import Point from '../classes/Point.js';

const container = document.getElementById('pointsList');
const header = document.getElementById('pointsHeader');

activeContextChangedEvent.subscribe(context => {
  while(container.firstChild) container.removeChild(container.firstChild);

  context.pointsList.addEvent.subscribe(addPoint);
  context.pointsList.removeEvent.subscribe(removePoint);
  context.pointsList.reorderEvent.subscribe(onReorder);
  context.deactivateEvent.subscribe(onDeactivate);

  context.pointsList.forEach(addPoint);
});

let userReorderPoint = null, userReorderBeforePoint = null;

const sortable = new Sortable(container, {
  group: "pointsList",  // or { name: "...", pull: [true, false, 'clone', array], put: [true, false, array] }
  sort: true,  // sorting inside list
  delay: 0, // time in milliseconds to define when the sorting should start
  touchStartThreshold: 0, // px, how many pixels the point should move before cancelling a delayed drag event
  disabled: false, // Disables the sortable if set to true
  animation: 150,  // ms, animation speed moving items when sorting, `0` — without animation
  easing: "cubic-bezier(1, 0, 0, 1)", // Easing for animation. Defaults to null. See https://easings.net/ for examples.
  handle: ".material-icons",  // Drag handle selector within list items
  preventOnFilter: true, // Call `event.preventDefault()` when triggered `filter`
  draggable: "li",  // Specifies which items inside the element should be draggable
  ghostClass: "ghost",  // Class name for the drop placeholder
  chosenClass: "chosen",  // Class name for the chosen item
  dragClass: "drag",  // Class name for the dragging item

  swapThreshold: 1, // Threshold of the swap zone
  invertSwap: false, // Will always use inverted swap zone if set to true
  invertedSwapThreshold: 1, // Threshold of the inverted swap zone (will be set to swapThreshold value by default)
  direction: 'vertical', // Direction of Sortable (will be detected automatically if not given)

  forceFallback: true,  // ignore the HTML5 DnD behaviour and force the fallback to kick in

  fallbackOnBody: false,  // Appends the cloned DOM Element into the Document's Body
  fallbackTolerance: 0, // Specify in pixels how far the mouse should move before it's considered as a drag.

  scroll: true, // or HTMLElement
  scrollSensitivity: 30, // px, how near the mouse must be to an edge to start scrolling.
  scrollSpeed: 10, // px
  bubbleScroll: true, // apply autoscroll to all parent elements, allowing for easier movement

  dragoverBubble: false,
  removeCloneOnHide: true, // Remove the clone element when it is not showing, rather than just hiding it
  emptyInsertThreshold: 5, // px, distance mouse must be from empty sortable to insert drag element into it

  // Element dragging started
  onStart: function () {
    container.classList.add('reordering');
  },

  // Element dragging ended
  onEnd: function (/**Event*/evt) {
    container.classList.remove('reordering');
    if(evt.newIndex === evt.oldIndex) return;

    const elem = container.childNodes[evt.newIndex];
    const nextElem = container.childNodes[evt.newIndex+1];

    userReorderPoint = new Point(parseInt(elem.dataset.x), parseInt(elem.dataset.y));
    userReorderBeforePoint =
      nextElem
      ? new Point(parseInt(nextElem.dataset.x), parseInt(nextElem.dataset.y))
      : undefined;

    if(!getActiveContext().pointsList.reorder(
      userReorderPoint, userReorderBeforePoint
    )) {
      const before = container.childNodes[evt.oldIndex];
      container.removeChild(elem);
      container.insertBefore(elem, before);
    }
  }
});

function onReorder(point, beforePoint) {
  if(!point || !(point instanceof Point) || (beforePoint && !(beforePoint instanceof Point))) return;

  if(point.equals(userReorderPoint)
     && ( (!beforePoint && !userReorderBeforePoint)
          || (beforePoint && beforePoint.equals(userReorderBeforePoint))
        )
  ) {
    userReorderPoint = null;
    userReorderBeforePoint = null;

    return;
  }

  const pElem = container.querySelector(`[data-x='${point.x}'][data-y='${point.y}']`);
  const beforePElem = beforePoint && container.querySelector(`[data-x='${beforePoint.x}'][data-y='${beforePoint.y}']`);

  if(!pElem || (beforePoint && !beforePElem)) return;

  container.removeChild(pElem);

  if(beforePElem) {
    container.insertBefore(pElem, beforePElem);
  } else {
    container.addChild(pElem);
  }
}

function onDeactivate(context) {
  context.pointsList.addEvent.unsubscribe(addPoint);
  context.pointsList.removeEvent.unsubscribe(removePoint);
  context.deactivateEvent.unsubscribe(onDeactivate);
  while(container.firstChild) container.removeChild(container.firstChild);
}

function updateHeaderTitle() {
  const cant = getActiveContext().pointsList.length;
  header.title = `${cant} punto${cant !== 1 ? 's' : ''}`;
}

function addPoint(point) {
  updateHeaderTitle();
  const elem = generateElement(point);
  container.appendChild(elem);
}

function removePoint(point) {
  updateHeaderTitle();
  const elem = container.querySelector(`[data-x='${point.x}'][data-y='${point.y}']`);
  getActiveContext().stage.interactive.removeObject(point);
  container.removeChild(elem);
}

function generateElement(point) {
  const li = document.createElement('li');

  li.addEventListener('mouseover', ()=>{
    const stage = getActiveContext().stage;

    const interactiveStage = stage.interactive;

    interactiveStage.addObject(point, {Point:stage.settings.point_selected_style});

    const tool = getActiveContext().stage.tools.activeTool;

    if(!tool.canExecuteOnPoint) return;

    li.style.cursor = "pointer";
  });

  li.addEventListener('mouseout', ()=>{
    getActiveContext().stage.interactive.removeObject(point);
    li.style.cursor = "default";
  });

  li.addEventListener('click', ()=>{
    const tool = getActiveContext().stage.tools.activeTool;

    if(!tool.canExecuteOnPoint) return;

    tool.executeOnPoint(point);
  });

  const icon = document.createElement('span');
  icon.className = 'material-icons';
  icon.innerText = 'reorder';
  li.appendChild(icon);

  const text = document.createElement('span');
  text.innerText = `(${point.x}, ${point.y})`;
  li.appendChild(text);

  li.dataset.x = point.x;
  li.dataset.y = point.y;

  return li;
}
