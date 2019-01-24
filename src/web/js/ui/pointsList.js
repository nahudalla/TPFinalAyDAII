import { activeContextChangedEvent } from '../classes/Context.js';

const container = document.getElementById('pointsList');

activeContextChangedEvent.subscribe(context => {
  while(container.firstChild) container.removeChild(container.firstChild);

  context.pointsList.addEvent.subscribe(addPoint);
  context.pointsList.removeEvent.subscribe(removePoint);
  context.deactivateEvent.subscribe(onDeactivate);

  context.pointsList.forEach(addPoint);
});

function onDeactivate(context) {
  context.pointsList.addEvent.unsubscribe(addPoint);
  context.pointsList.removeEvent.unsubscribe(removePoint);
  context.deactivateEvent.unsubscribe(onDeactivate);
}

function addPoint(point) {
  const elem = generateElement(point);
  container.appendChild(elem);
}

function removePoint(point) {
  const elem = container.querySelector(`[data-x='${point.x}'][data-y='${point.y}']`);
  container.removeChild(elem);
}

function generateElement(point) {
  const li = document.createElement('li');

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
