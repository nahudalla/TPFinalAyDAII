import Algorithm from './Algorithm.js';
import GOGroup from '../GOGroup.js';
import Segment from '../Segment.js';

const STYLES = {
  Segment: {
    strokeColor: '#95E9FF',
    strokeWidth: 3
  },
  Point:{
    fillColor:'#A0FFB6',
    strokeColor:'#000000',
    strokeWidth: 0.5
  }
};

export default class GrahamJarvisBase extends Algorithm {
  constructor(context, name, id) {
    super(context, name, id);
    this._backGroup = null;
    this._frontGroup = null;
  }

  isValidResult(result) {return Array.isArray(result) && result.length >= 3;}

  generateResultMessage(result) {
    if(!this.isValidResult(result)) throw new Error('Result is not valid.');
    return `Polígono convexo de ${result.length} vértice${result.length > 1 ? 's' : ''}.`;
  }
  showResultUI(result) {
    if(!this.isValidResult(result)) throw new Error('Result is not valid.');

    const backItems = [];

    let prevPoint = null, firstPoint = null;
    result.forEach(point => {
      if(prevPoint) {
        backItems.push(new Segment(
          prevPoint, point
        ))
      } else firstPoint = point;
      prevPoint = point;
    });

    backItems.push(new Segment(prevPoint, firstPoint));

    this.hideResultUI();

    this._backGroup = new GOGroup(backItems);
    super.context.stage.normal_back.addObject(this._backGroup, STYLES);

    this._frontGroup = new GOGroup(result);
    super.context.stage.interactive.addObject(this._frontGroup, STYLES);
  }

  hideResultUI() {
    if(this._backGroup) super.context.stage.normal_back.removeObject(this._backGroup);
    if(this._frontGroup) super.context.stage.interactive.removeObject(this._frontGroup);
  }
}
