import GeometricObject from './GeometricObject.js';

export default class Arrow extends GeometricObject {
  constructor(from, to, name) {
    super(name, 'Arrow');

    this._from = from;
    this._to = to;
  }

  get from() {
    return this._from;
  }

  get to() {
    return this._to;
  }

  get _hashString() {
    return `${this._from.hashString}_${this._to.hashString}_Arrow`;
  }

  toPaperObject(stage, scale, applyStyles) {
    const from = (new stage.Point(this._from.x, -this._from.y)).multiply(stage.settings.grid_size).multiply(scale);
    const to = (new stage.Point(this._to.x, -this._to.y)).multiply(stage.settings.grid_size).multiply(scale);

    return applyStyles(this, ()=>{
      const group = new stage.Group();

      group.addChild(new stage.Line(from, to));

      const arrCoords = calculateArrow(this._from, this._to, scale);

      group.addChild(new stage.Line(
        (new stage.Point(arrCoords.x1, -arrCoords.y1)).multiply(stage.settings.grid_size).multiply(scale),
        to
      ));

      group.addChild(new stage.Line(
        (new stage.Point(arrCoords.x2, -arrCoords.y2)).multiply(stage.settings.grid_size).multiply(scale),
        to
      ));

      return group;
    });
  }
}

function calculateArrow(tail, tip, scale) {
  // TODO: use settings
  const arrowLength = (scale < 1 ? scale : 1) / scale;
  const angle = 35;

  const dx = tip.x - tail.x;
  const dy = tip.y - tail.y;

  const theta = Math.atan2(dy, dx);

  const rad = angle * (Math.PI / 180);
  const x1 = tip.x - arrowLength * Math.cos(theta + rad);
  const y1 = tip.y - arrowLength * Math.sin(theta + rad);

  const rad2 = (-angle) * (Math.PI / 180);
  const x2 = tip.x - arrowLength * Math.cos(theta + rad2);
  const y2 = tip.y - arrowLength * Math.sin(theta + rad2);

  return {x1, y1, x2, y2};
}
