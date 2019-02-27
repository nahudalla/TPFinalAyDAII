import SimplePolygonAlgorithmsBase from './SimplePolygonAlgorithmsBase.js';
import GOGroup from '../GOGroup.js';
import Point from '../Point.js';

const colorAStyle = {
  fillColor:'#d0689e',
  strokeColor:'#000000',
  strokeWidth: 0.5
};
const colorBStyle = {...colorAStyle, fillColor: '#16ebff'};
const colorCStyle = {...colorAStyle, fillColor: '#ffd11b'};

export default class ArtGalleryProblemOverSimplePolygon extends SimplePolygonAlgorithmsBase {
  constructor(context) {
    super(context,
      'Problema de la galería de arte sobre polígono simple',
      'art_gallery_problem_on_simple_polygon',
      true
    );

    this._removeResultUI = null;
  }

  isValidResult(result) {
    console.log(result);
    return super.isValidResult(result)
      && (result === null
          || (typeof result.colorASize === 'number'
              && typeof result.colorBSize === 'number'
              && typeof result.colorCSize === 'number'
              && result.length === result.colorASize + result.colorBSize + result.colorCSize
              )
          );
  }

  showResultUI(result) {
    if(!this.isValidResult(result)) return;

    const objects = [];

    let i = 0;
    for(let j = 0; j < result.colorASize; ++j, ++i) {
      objects.push(new Point(result[i].x, result[i].y, 'A'));
    }
    for(let j = 0; j < result.colorBSize; ++j, ++i) {
      objects.push(new Point(result[i].x, result[i].y, 'B'));
    }
    for(let j = 0; j < result.colorCSize; ++j, ++i) {
      objects.push(new Point(result[i].x, result[i].y, 'C'));
    }

    this._removeResultUI = this.context.stage.interactive.addObject(new GOGroup(objects), {
      Point_A: colorAStyle,
      Point_B: colorBStyle,
      Point_C: colorCStyle
    });
  }

  hideResultUI() {
    if(this._removeResultUI) {
      this._removeResultUI();
      this._removeResultUI = null;
    }
  }

  generateResultMessage(result) {
    if(!this.isValidResult(result)) return;

    const superMessage = super.generateResultMessage(result);

    if(superMessage) return superMessage;

    let color, size;

    if(result.colorASize < result.colorBSize) {
      if(result.colorASize < result.colorCSize) {
        color = colorAStyle.fillColor;
        size = result.colorASize;
      } else {
        color = colorCStyle.fillColor;
        size = result.colorCSize;
      }
    } else {
      if (result.colorBSize < result.colorCSize) {
        color = colorBStyle.fillColor;
        size = result.colorBSize;
      } else {
        color = colorCStyle.fillColor;
        size = result.colorCSize;
      }
    }

    const span = document.createElement('span');

    span.innerHTML = `El color óptimo tiene ${size} cámara${size !== 1?'s':''} y es: 
        <div style="
            display: inline-block;
            width: 10px;
            height: 10px;
            margin-left: 5px;
            border-radius: 50%;
            border: #000 1px solid;
            background-color: ${color};
        "></div>`;

    return span;
  }
}
