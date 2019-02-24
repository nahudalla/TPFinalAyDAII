export default class GeometricObject {
  constructor(name) {
    this._name = name ? (name + '_') : undefined;
  }

  get hashString() {
    return (this._name || '') + this._hashString;
  }
  get _hashString() {throw new Error("Abstract method not implemented.");}

  toPaperObject(stage, scale, applyStyles) {throw new Error("Abstract method not implemented.");}

  selectStyle(styles) {throw new Error("Abstract method not implemented.");}
}
