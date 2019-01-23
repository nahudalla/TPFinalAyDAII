import StageBase from './StageBase.js';

export default class AxesStage extends StageBase {
  constructor(project, stage) {
    super(project, stage);

    super._centerOriginInView();

    this._zoom = 1;
    this._center = this.view.center;

    super._enableZoom(
      () => {this._restoreZoom();},
      () => {
        this._saveZoom();
        this.draw();
      }
    );
    super._enableScroll(
      ()=>{
        this._restoreZoom();
      },
      ()=>{
        this._saveZoom();
        this.draw();
      }
    );

    this._layer = project.activeLayer;

    project.view.on('resize', ()=>this.draw());

    project.view.autoUpdate = false;

    this.draw();
  }

  _saveZoom() {
    this._zoom = this.view.zoom;
    this._center = this.view.center;
    this.view.zoom = 1;
    this.view.center = this._center.multiply(this._zoom);
  }

  _restoreZoom() {
    this.view.zoom = this._zoom;
    this.view.center = this._center;
  }

  draw() {
    this._restoreZoom();

    const horizontalAxisInfo = this._calculateAxisInfo();
    const verticalAxisInfo = this._calculateAxisInfo(true);

    this._layer.removeChildren();

    super._pushStyle(this.stage.settings.axes_lines_style);
    this._drawAxesLines(horizontalAxisInfo, verticalAxisInfo);
    super._popStyle();

    super._pushStyle(this.stage.settings.axes_markers_style);
    this._drawMarkers(horizontalAxisInfo, verticalAxisInfo);
    super._popStyle();

    super._pushStyle(this.stage.settings.axes_text_style);
    this._drawNumbers(horizontalAxisInfo, verticalAxisInfo);
    super._popStyle();

    this._saveZoom();

    this.view.update();
  }

  _drawAxesLines(horizontalAxisInfo, verticalAxisInfo) {
    const Line = this.stage.Line;
    const Point = this.stage.Point;

    const layer = this._layer;

    layer.addChildren([
      new Line(
        new Point(horizontalAxisInfo.lowerBound, horizontalAxisInfo.position),
        new Point(horizontalAxisInfo.upperBound, horizontalAxisInfo.position)
      ),
      new Line(
        new Point(verticalAxisInfo.position, verticalAxisInfo.lowerBound),
        new Point(verticalAxisInfo.position, verticalAxisInfo.upperBound)
      )
    ]);
  }

  _drawMarkers(horizontalAxisInfo, verticalAxisInfo) {
    this._drawAxisMarkers(horizontalAxisInfo);
    this._drawAxisMarkers(verticalAxisInfo, true);
  }

  _drawAxisMarkers(axisInfo, vertical) {
    vertical = vertical === true;

    const stage = this.stage;
    const layer = this._layer;

    const grid_size = stage.settings.grid_size;
    const zoomedMarkerDistance = Math.round(grid_size*stage.zoom);

    let firstNumber;
    if(axisInfo.originLessThanLowerBound) {
      firstNumber = Math.ceil(axisInfo.lowerBound/zoomedMarkerDistance);
    } else {
      firstNumber = Math.floor(axisInfo.lowerBound/zoomedMarkerDistance);
    }

    axisInfo.zoomedMarkerDistance = zoomedMarkerDistance;
    axisInfo.firstNumber = firstNumber;

    const Line = stage.Line;
    const Point = stage.Point;

    const half_marker_length = stage.settings.axes_marker_length/2;
    const markerStart = axisInfo.position - half_marker_length;
    const markerEnd = axisInfo.position + half_marker_length;

    const skipOdd = zoomedMarkerDistance < stage.settings.axes_markers_min_distance;

    let num = firstNumber;
    let pos;
    do {
      const n = num++;
      pos = n*zoomedMarkerDistance;

      if(n === 0 || (skipOdd && n%2 !== 0)) continue;

      layer.addChild(
        vertical
          ? new Line(new Point(markerStart, pos), new Point(markerEnd, pos))
          : new Line(new Point(pos, markerStart), new Point(pos, markerEnd))
      );
    } while (pos < axisInfo.upperBound);
  }

  _drawNumbers(horizontalAxisInfo, verticalAxisInfo) {
    this._drawAxisNumbers(horizontalAxisInfo);
    this._drawAxisNumbers(verticalAxisInfo, true);
  }

  // TODO: fix numbers justification in vertical axis

  _drawAxisNumbers(axisInfo, vertical) {
    vertical = vertical === true;

    const stage = this.stage;

    const Text = stage.PointText;
    const Point = stage.Point;

    const side = (
      axisInfo.atLowerBound
        ? (vertical ? 'upper' : 'lower')
        : (
          axisInfo.atUpperBound
            ? (vertical ? 'lower' : 'upper')
            : stage.settings.axes_text_preferred_side
        )
    );

    const marginMultiplier = (vertical && side === 'upper') || (!vertical && side === 'lower') ? 1 : -1;
    const positionAcrossLine = axisInfo.position + (stage.settings.axes_text_to_line_offset * marginMultiplier);

    const skipOdd = axisInfo.zoomedMarkerDistance < stage.settings.axes_markers_min_distance;

    let number = axisInfo.firstNumber;
    let pos;
    do {
      const num = number++;
      pos = num*axisInfo.zoomedMarkerDistance;

      if(num === 0 || (skipOdd && num%2 !== 0)) continue;

      const text = new Text({
        point: this.view.center,
        content: num
      });

      text.position = (
        vertical
          ? new Point(positionAcrossLine, pos)
          : new Point(pos, positionAcrossLine)
      );

      this._layer.addChild(text);
    } while (pos < axisInfo.upperBound);
  }

  _calculateAxisInfo(vertical) {
    const stage = this.stage;
    const view = this.view;

    vertical = vertical === true;

    const otherLowerBoundZoomed = vertical ? view.bounds.left : view.bounds.top;
    const otherUpperBoundZoomed = vertical ? view.bounds.right : view.bounds.bottom;

    const lowerBoundZoomed = vertical ? view.bounds.top : view.bounds.left;
    const upperBoundZoomed = vertical ? view.bounds.bottom : view.bounds.right;

    this._saveZoom();
    const otherLowerBound = vertical ? view.bounds.left : view.bounds.top;
    const otherUpperBound = vertical ? view.bounds.right : view.bounds.bottom;
    const lowerBound = vertical ? view.bounds.top : view.bounds.left;
    const upperBound = vertical ? view.bounds.bottom : view.bounds.right;
    this._restoreZoom();

    const axisInfo = {
      lowerBound, upperBound,
      lowerBoundZoomed, upperBoundZoomed
    };

    axisInfo.originLessThanLowerBound = otherLowerBoundZoomed > 0;

    const padding = stage.settings.axes_padding;
    const zoomedPadding = padding/stage.zoom;

    if(otherLowerBoundZoomed <= -zoomedPadding && otherUpperBoundZoomed >= zoomedPadding) {
      axisInfo.inView = true;

      if(vertical) axisInfo.position = 0;
      else axisInfo.position =0;

    } else if(otherLowerBoundZoomed > -zoomedPadding) {
      axisInfo.atLowerBound = true;
      axisInfo.position = otherLowerBound + padding;
    } else {
      axisInfo.atUpperBound = true;
      axisInfo.position = otherUpperBound - padding;
    }

    return axisInfo;
  }
}
