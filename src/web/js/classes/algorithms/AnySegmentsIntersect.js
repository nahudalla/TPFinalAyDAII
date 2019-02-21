import Algorithm from './Algorithm.js';
import GOGroup from '../GOGroup.js';
import Segment from '../Segment.js';

export default class AnySegmentsIntersect extends Algorithm {
  constructor(context) {
    super(context, 'Algún segmento interseca', 'any_segments_intersect');

    this._context = context;

    super._enableAlgorithmInputDraw();
  }

  _redrawAlgorithmInput() {
    const segments = [];

    let prevPoint = null;
    this._context.pointsList.forEach(point => {
      if(prevPoint) {
        segments.push(new Segment(prevPoint, point));
        prevPoint = null;
      } else prevPoint = point;
    });

    if(segments.length !== 0) {
      return this._context.stage.normal_back.addObject(new GOGroup(segments));
    }
  }
}
