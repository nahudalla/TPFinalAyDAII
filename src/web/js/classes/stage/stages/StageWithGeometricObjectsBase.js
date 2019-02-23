import StageBase from './StageBase.js';
import GeometricObject from '../../GeometricObject.js';

export default class StageWithGeometricObjectsBase extends StageBase {
  constructor(project, stage, objectStyles) {
    super(project, stage);

    this._layer = project.activeLayer;
    this._objects = new Map();

    if(typeof objectStyles !== 'object') objectStyles = {};
    this._styles = objectStyles;

    this._scaleCoords = false;
  }

  _enableCoordinatesScaling() {
    this._scaleCoords = true;
  }

  _redrawObjects() {
    this.project.activate();

    this._layer.removeChildren();

    for(let objInfo of this._objects.values()) {
      objInfo.pObj = this._addObj(objInfo.obj, objInfo.style);
    }
  }

  _addObj(obj, style) {
    const o = obj.toPaperObject(this.stage, this._scaleCoords ? this.stage.zoom : 1, (obj, callback) => {
      const selectedStyle = obj.selectStyle(style) || obj.selectStyle(this._styles);

      if(selectedStyle) this._pushStyle(selectedStyle);

      const ret = callback();

      if(selectedStyle) this._popStyle();

      return ret;
    });

    this._layer.addChild(o);

    return o;
  }

  addObject(obj, style) {
    if(!(obj instanceof GeometricObject)) throw new Error('Invalid Geometric Object.');
    if(typeof style !== 'object') style = {};

    this.project.activate();

    const o = this._addObj(obj, style);
    this._objects.set(obj.hashString, {obj, pObj: o, style});

    return ()=>{this.removeObject(obj);}
  }

  removeObject(obj) {
    if(!(obj instanceof GeometricObject)) throw new Error('Invalid Geometric Object.');

    this.project.activate();

    const item = this._objects.get(obj.hashString);
    if(item) {
      item.pObj.remove();
      this._objects.delete(obj.hashString);
    }
  }
}
