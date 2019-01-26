import Observable from './Observable.js';

(()=>{
  const url = new URL(window.location.href);
  threads.config.set({basepath:{web:url.origin+url.pathname}});
})();

const pool = new threads.Pool().run(worker, ['wasm/WASMModule.js']);

export default class Algorithms {
  constructor(context) {
    this._context = context;

    this._grahamScan = new Algorithm(context, 'Graham Scan', 'graham_scan');
    this._jarvisMarch = new Algorithm(context, 'Jarvis March', 'jarvis_march');
    this._closestPairOfPoints = new Algorithm(context, 'Puntos más cercanos', 'closest_points');
    this._anySegmentsIntersect = new Algorithm(context, 'Algún segmento interseca', 'any_segments_intersect');
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

class Algorithm {
  constructor(context, name, id) {
    this._context = context;
    this._name = name;
    this._id = id;

    this._enabled = false;
  }

  get name() { return this._name; }
  get id() { return this._id; }

  get enabled() { return this._enabled; }

  enable() { this._enabled = true; }
  disable() { this._enabled = false; }

  run() { return new AlgorithmRunner([this], this._context.pointsList.buildInt32CoordinatesArray()); }
}

export class AlgorithmRunner {
  constructor(algorithms, coordinatesArray) {
    if(!Array.isArray(algorithms)) throw new TypeError('AlgorithmRunner: algorithms must be an array.');
    if(!(coordinatesArray instanceof Int32Array)) throw new TypeError('AlgorithmRunner: coordinatesArray must be of type Int32Array');

    this._algorithms = new Map();
    algorithms.forEach(algorithm => this._algorithms.set(algorithm.id, algorithm));

    this._progressEvent = new Observable();
    this._doneEvent = new Observable();
    this._errorEvent = new Observable();

    this._result = null;
    this._error = null;

    const removeAllListeners = () => {
      this._progressEvent.removeAll();
      this._doneEvent.removeAll();
      this._errorEvent.removeAll();
    };

    pool
      .send({
        algorithms: algorithms.map(algorithm=>algorithm.id),
        location: window.location.href,
        coordinatesArray: {
          length: coordinatesArray.length,
          BYTES_PER_ELEMENT: coordinatesArray.BYTES_PER_ELEMENT,
          buffer: coordinatesArray.buffer
        }
      }, [coordinatesArray.buffer])
      .on('progress', (message_id, algor_id, ...args) => {
        this._progressEvent.emit(message_id, this._algorithms.get(algor_id), ...args);
      })
      .once('error', (err, ...args) => {
        this._error = err;
        this._errorEvent.emit(err, ...args);
        removeAllListeners();
      })
      .once('done', (response, ...args) => {
        const res = this._result = new Map();
        this._algorithms.forEach((algorithm, key) => {
          res.set(key, {algorithm, result: response[key]});
        });
        this._doneEvent.emit(res, ...args);
        removeAllListeners();
      });
  }

  get PROGRESS_EVENT_IDS() {return worker();}

  get algorithms() { return this._algorithms.values(); }

  get progressEvent() { return this._progressEvent; }
  get doneEvent() { return this._doneEvent; }
  get errorEvent() { return this._errorEvent; }

  get result() { return this._result; }
  get error() { return this._error; }
}

// IMPORTANT: This function won't have access to its surrounding context when run!!
function worker(input, done, progress) {
  const PROGRESS_IDS = {
    JOB_STARTED: 'job_started',
    MODULE_LOADING: 'module_loading',
    MODULE_READY: 'module_ready'
  };

  if(typeof importScripts !== 'function' && typeof AlgorithmRunner === 'function') {
    return PROGRESS_IDS;
  }

  progress(PROGRESS_IDS.JOB_STARTED);

  const onModuleReady = () => {
    progress(PROGRESS_IDS.MODULE_READY);

    const response = {};

    const heapBytes = arrayToHeap(input.coordinatesArray, this.wasm_module);
    input.algorithms.forEach(algorithm => {
      const res = this.wasm_module[`run_${algorithm}`](
        heapBytes.byteOffset,
        input.coordinatesArray.length,
        function(str){progress(str);}
      );
      if(res.ptr) {
        this.wasm_module._free(res.ptr);
      }
      response[algorithm] = res;
    });
    this.wasm_module._free(heapBytes.byteOffset);

    done(response);
  };

  if(this.wasm_module) onModuleReady();
  else {
    progress(PROGRESS_IDS.MODULE_LOADING);

    const base = new URL('./wasm/', input.location);

    this.wasm_module = WASMModule({
      locateFile: (path) => { return (new URL(path, base)).href; },
      noExitRuntime: true,
      onRuntimeInitialized: onModuleReady
    });
  }

  function arrayToHeap(typedArray, module){
    var numBytes = typedArray.length * typedArray.BYTES_PER_ELEMENT;
    var ptr = module._malloc(numBytes);
    var heapBytes = new Uint8Array(module.HEAPU8.buffer, ptr, numBytes);
    heapBytes.set(new Uint8Array(typedArray.buffer));
    return heapBytes;
  }
}
