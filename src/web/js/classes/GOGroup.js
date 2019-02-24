import GeometricObject from './GeometricObject.js';

export default class GOGroup extends GeometricObject {
  constructor(elements, name) {
    super(name);

    if(!Array.isArray(elements) || !elements.length) {
      throw new TypeError('Parameter "elements" must be an Array of GeometricObject.');
    }

    this.__hashString = "";

    elements.forEach(elem => {
      if(!(elem instanceof GeometricObject)) {
        throw new TypeError('Parameter "elements" must be an Array of GeometricObject.');
      }
      this.__hashString += elem.hashString + '_';
    });

    this.__hashString += 'GOGroup';

    this._elems = elements;
  }

  get _hashString() {
    return this.__hashString;
  }

  toPaperObject(stage, scale, applyStyles) {
    const group = new stage.Group();

    this._elems.forEach(elem => {
      group.addChild(elem.toPaperObject(stage, scale, applyStyles));
    });

    return group;
  }

  selectStyle(styles) {
    super.selectStyle(styles);
  }
}
