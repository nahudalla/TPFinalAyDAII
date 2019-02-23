import Algorithm from './Algorithm.js';
import GOGroup from '../GOGroup.js';

const POINT_1_STYLE = {
  fillColor:'#A0FFB6',
  strokeColor:'#000000',
  strokeWidth: 0.5
};

const POINT_2_STYLE = {};
Object.assign(POINT_2_STYLE, POINT_1_STYLE);
POINT_2_STYLE.fillColor = '#16ebff';

export default class ClosestPairOfPoints extends Algorithm {
  constructor(context) {
    super(context, 'Puntos más cercanos', 'closest_points');

    this._group = null;
  }

  isValidResult(result) {return Array.isArray(result) && result.length === 2;}

  generateResultMessage(result) {
    if(!this.isValidResult(result)) throw new Error('Result is not valid.');

    const span = document.createElement('span');

    span.innerHTML = `Puntos más cercanos: `
      + `<span style="background-color: ${POINT_1_STYLE.fillColor}; font-weight: bolder; text-shadow: 0 0 1px rgba(0,0,0,.5); border-radius: 5px; padding: 2px;">
            (${result[0].x},${result[0].y})
         </span>, 
         <span style="background-color: ${POINT_2_STYLE.fillColor}; font-weight: bolder; text-shadow: 0 0 1px rgba(0,0,0,.5); border-radius: 5px; padding: 2px;">
            (${result[1].x},${result[1].y})
         </span>`;

    return span;
  }

  showResultUI(result) {
    if(!this.isValidResult(result)) throw new Error('Result is not valid.');

    this.hideResultUI();

    this._group = new GOGroup(result);

    const styles = {};
    styles[result[0].hashString] = POINT_1_STYLE;
    styles[result[1].hashString] = POINT_2_STYLE;

    super.context.stage.interactive.addObject(this._group, styles);
  }

  hideResultUI() {
    if(this._group) {
      super.context.stage.interactive.removeObject(this._group);
      this._group = null;
    }
  }
}
