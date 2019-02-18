export default class GeometricObject {
  get hashString() {throw new Error("Abstract method not implemented.");}

  toPaperObject(stage, scale) {throw new Error("Abstract method not implemented.");}

  selectStyle(styles) {throw new Error("Abstract method not implemented.");}
}
