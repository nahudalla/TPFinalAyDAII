export default class GeometricObject {
  constructor(name, className) {
    this._name = name;
    this._className = className;
  }

  get name() {
    return this._name;
  }

  get hashString() {
    return (this._name ? this._name + '_' : '') + this._hashString;
  }
  get _hashString() {throw new Error("Abstract method not implemented.");}

  toPaperObject(stage, scale, applyStyles) {throw new Error("Abstract method not implemented.");}

  selectStyle(styles) {
    return styles[this.hashString]
      || styles[this._name]
      || styles[`${this._className}_${this._name}`]
      || styles[this._className];
  }
}
