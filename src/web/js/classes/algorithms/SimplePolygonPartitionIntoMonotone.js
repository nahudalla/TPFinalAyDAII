import SimplePolygonAlgorithmsBase from './SimplePolygonAlgorithmsBase.js';

export default class SimplePolygonPartitionIntoMonotone extends SimplePolygonAlgorithmsBase {
  constructor(context) {
    super(context,
      'Partición de polígono simple en partes monótonas',
      'partition_simple_polygon_to_monotone'
    );
  }

  generateResultMessage(result) {
    return super.generateResultMessage(result)
      || `Hay ${(result.length/2)+1} polígono${result.length === 0 ? '' : 's'}.`;
  }
}
