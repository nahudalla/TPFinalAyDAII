import Algorithm from './Algorithm.js';
import GOGroup from '../GOGroup.js';
import Segment from '../Segment.js';

export default class AnySegmentsIntersect extends Algorithm {
  constructor(context) {
    super(context, 'Algún segmento interseca', 'any_segments_intersect');

    this._context = context;

    super._enableAlgorithmInputDraw();
  }


  isValidResult(result) {return typeof result === 'boolean';}
  generateResultMessage(result) {
    if(!this.isValidResult(result)) throw new Error('Result is not valid.');
    const span = document.createElement('span');
    span.innerHTML = `<span style="font-weight:bolder">${result?'H':'No h'}ay</span> ${
      result ? 'al menos una' : 'ninguna'} intersección entre segmentos.`;
    return span;
  }

  showResultUI(result) {}
  hideResultUI() {}

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
