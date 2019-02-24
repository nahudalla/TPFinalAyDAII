import Observable from './Observable.js';
import AlgorithmRunner from './algorithms/AlgorithmRunner.js';
import GrahamScan from './algorithms/GrahamScan.js';
import JarvisMarch from './algorithms/JarvisMarch.js';
import ClosestPairOfPoints from './algorithms/ClosestPairOfPoints.js';
import AnySegmentsIntersect from './algorithms/AnySegmentsIntersect.js';

export default class Algorithms {
  constructor(context) {
    this._context = context;

    const algorithms = {
      grahamScan : new GrahamScan(context),
      jarvisMarch : new JarvisMarch(context),
      closestPairOfPoints : new ClosestPairOfPoints(context),
      anySegmentsIntersect : new AnySegmentsIntersect(context)
    };

    this._enabledAlgorithmsChangedEvent = new Observable();
    this._algorithmsById = new Map();

    Object.keys(algorithms).forEach(algorithm => {
      const algor = algorithms[algorithm];
      this[`_${algorithm}`] = algor;
      algor.enabledStatusChangedEvent.subscribe(enabled=>{
        this._enabledAlgorithmsChangedEvent.emit(algor, enabled);
      });
      this._algorithmsById.set(algor.id, algor);
      Object.defineProperty(this, algorithm, {get: () => algor});
    });
  }
  
  get enabledAlgorithmsChangedEvent() {
    return this._enabledAlgorithmsChangedEvent;
  }

  get listAll() {
    return Array.from(this._algorithmsById.values());
  }

  get(algorId) {
    return this._algorithmsById.get(algorId);
  }

  get enabledAlgorithms() {
    return this.listAll.filter(algor => algor.enabled);
  }

  disableAll() {
    this.listAll.forEach(algorithm => algorithm.disable());
  }

  parallelRun() {
    const runners = [];
    this.listAll.forEach(algor => {
      if(algor.enabled) {
        runners.push(algor.run());
      }
    });
    return runners;
  }

  run() {
    return new AlgorithmRunner(this.enabledAlgorithms, this._context.pointsList.buildInt32CoordinatesArray());
  }
}
