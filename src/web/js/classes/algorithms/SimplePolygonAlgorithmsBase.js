import Algorithm from './Algorithm.js';
import Segment from '../Segment.js';
import GOGroup from '../GOGroup.js';
import Point from '../Point.js';

export default class SimplePolygonAlgorithmsBase extends Algorithm {
  constructor(context, name, id) {
    super(context, name, id);

    super._enableAlgorithmInputDraw();
    this._resultRemover = null;
  }

  _redrawAlgorithmInput() {
    const segments = [];

    let prev = null, first = null;
    this.context.pointsList.forEach(point => {
      if(!prev) {
        first = prev = point;
      } else {
        segments.push(new Segment(prev, point));
        prev = point;
      }
    });

    if(prev && first) segments.push(new Segment(prev, first));

    return segments.length > 0
      ? this.context.stage.normal_back.addObject(new GOGroup(segments))
      : undefined;
  }

  isValidResult(result) {return result === null || Array.isArray(result);}
  generateResultMessage(result) {
    return result === null
      ? `No hubo resultados. Verifique que los datos de entrada sean correctos y el polígono sea simple.`
      : undefined;
  }
  showResultUI(result) {
    if(!this.isValidResult(result)) return;
    if(result === null) return;

    const objs = [];

    let i = 0;
    while(i < result.length) {
      objs.push(
        new Segment(result[i++], result[i++])
      );
    }

    this._resultRemover = super.context.stage.normal_back.addObject(
      new GOGroup(objs),
      {
        Segment:{
          strokeColor:'#A0FFB6',
          strokeWidth: 2
        }
      }
    );
  }
  hideResultUI() {
    if(this._resultRemover) {
      this._resultRemover();
      this._resultRemover = null;
    }
  }

}