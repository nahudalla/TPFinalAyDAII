import Observable from './Observable.js';
import AlgorithmRunner from './algorithms/AlgorithmRunner.js';
import GrahamScan from './algorithms/GrahamScan.js';
import JarvisMarch from './algorithms/JarvisMarch.js';
import ClosestPairOfPoints from './algorithms/ClosestPairOfPoints.js';
import AnySegmentsIntersect from './algorithms/AnySegmentsIntersect.js';

export default class Algorithms {
  constructor(context) {
    this._context = context;

    this._grahamScan = new GrahamScan(context);
    this._jarvisMarch = new JarvisMarch(context);
    this._closestPairOfPoints = new ClosestPairOfPoints(context);
    this._anySegmentsIntersect = new AnySegmentsIntersect(context);

    this._enabledAlgorithmsChangedEvent = new Observable();
    const statusChanged = (algor, enabled)=>{
      this._enabledAlgorithmsChangedEvent.emit(algor, enabled);
    };
    this._grahamScan.enabledStatusChangedEvent.subscribe(statusChanged.bind(this, this._grahamScan));
    this._jarvisMarch.enabledStatusChangedEvent.subscribe(statusChanged.bind(this, this._jarvisMarch));
    this._closestPairOfPoints.enabledStatusChangedEvent.subscribe(statusChanged.bind(this, this._closestPairOfPoints));
    this._anySegmentsIntersect.enabledStatusChangedEvent.subscribe(statusChanged.bind(this, this._anySegmentsIntersect));

    this._algorithmsById = new Map();

    this._algorithmsById.set(this._grahamScan.id, this._grahamScan);
    this._algorithmsById.set(this._jarvisMarch.id, this._jarvisMarch);
    this._algorithmsById.set(this._closestPairOfPoints.id, this._closestPairOfPoints);
    this._algorithmsById.set(this._anySegmentsIntersect.id, this._anySegmentsIntersect);
  }
  
  get enabledAlgorithmsChangedEvent() {
    return this._enabledAlgorithmsChangedEvent;
  }

  get grahamScan() { return this._grahamScan; }
  get jarvisMarch() { return this._jarvisMarch; }
  get closestPairOfPoints() { return this._closestPairOfPoints; }
  get anySegmentsIntersect() { return this._anySegmentsIntersect; }

  get algorithms() {
    return [
      this._grahamScan,
      this._jarvisMarch,
      this._closestPairOfPoints,
      this._anySegmentsIntersect
    ];
  }

  get(algorId) {
    return this._algorithmsById.get(algorId);
  }

  get enabledAlgorithms() {
    const enabled = [];

    this.algorithms.forEach(algorithm => {
      if(algorithm.enabled) enabled.push(algorithm);
    });

    return enabled;
  }

  disableAll() {
    this.algorithms.forEach(algorithm => algorithm.disable());
  }

  parallelRun() {
    const runners = [];
    this.algorithms.forEach(algor => {
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
