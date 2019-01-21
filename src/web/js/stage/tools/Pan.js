const paper = window.paper;

let stage = null;
const PAN_TOOL = new paper.Tool();

let pan_last_view_position = null;

PAN_TOOL.on({
  mousedown(e) {
    pan_last_view_position = stage.projectToView(e.point);
  },
  mousedrag(event) {
    const curr_pos = stage.projectToView(event.point);
    stage.scrollView(curr_pos.subtract(pan_last_view_position));
    pan_last_view_position = curr_pos;
  },
  activate() {
    paper.view.element.addEventListener('mousewheel', onMouseWheel);
  },
  deactivate() {
    paper.view.element.removeEventListener('mousewheel', onMouseWheel);
  }
});

function onMouseWheel(evt) {
  evt.preventDefault();
  const delta = evt.deltaY;

  const rect = evt.target.getBoundingClientRect();
  const x = evt.clientX - rect.left; //x position within the element.
  const y = evt.clientY - rect.top;  //y position within the element.

  const projectPosition = paper.view.viewToProject(new paper.Point(x, y));

  if(delta < 0) stage.zoomIn(projectPosition);
  else stage.zoomOut(projectPosition);
}

export default function setup(s) {
  stage = s;
  return PAN_TOOL;
}
