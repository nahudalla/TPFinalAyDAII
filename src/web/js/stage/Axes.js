const paper = window.paper;

export default function(stage) {
  drawXAxis(stage);
  drawYAxis(stage);
}

function drawXAxis(stage) {
  const b = stage.bounds;

  if(b.contains(new paper.Point(b.left, 0))) {
    console.log("X in view");
  } else if(b.top > 0) {
    console.log("X at top");
  } else {
    console.log("X at bottom");
  }

}

function drawYAxis(stage) {
  const b = stage.bounds;

  if(b.contains(new paper.Point(0, b.top))) {
    console.log("Y in view");
  } else if(b.left > 0) {
    console.log("Y at left");
  } else {
    console.log("Y at right");
  }
}
