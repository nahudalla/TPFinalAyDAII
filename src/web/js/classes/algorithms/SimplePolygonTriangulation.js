import SimplePolygonAlgorithmsBase from './SimplePolygonAlgorithmsBase.js';

export default class SimplePolygonTriangulation extends SimplePolygonAlgorithmsBase {
  constructor(context) {
    super(context, 'Triangulación de polígono simple', 'triangulate_simple_polygon');
  }

  generateResultMessage(result) {
    return super.generateResultMessage(result)
      || `Hay ${(result.length/2)+1} triángulo${result.length === 0 ? '' : 's'}.`;
  }
}
