import Observable from './Observable.js';
import Point from './Point.js';

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
    algorithms.forEach(algorithm => {
      this._algorithms.set(algorithm.id, algorithm);
    });

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
          buffer: coordinatesArray.buffer
        }
      }, [coordinatesArray.buffer])
      .on('progress', (data) => {
        if(data.algorithm) data.algorithm = this._algorithms.get(data.algorithm);
        this._progressEvent.emit(data);
      })
      .once('error', (err) => {
        this._error = err;
        this._errorEvent.emit(err);
        removeAllListeners();
      })
      .once('done', (response, ...args) => {
        const res = this._result = new Map();
        this._algorithms.forEach((algorithm, key) => {
          let result;
          if(response[key] && response[key].buffer instanceof ArrayBuffer) {
            const i32Array = new Int32Array(response[key].buffer);
            result = [];
            for(let i = 0; i < i32Array.length-1; i += 2) {
              result.push(new Point(i32Array[i], i32Array[i+1]));
            }
          } else result = response[key];
          res.set(key, {algorithm, result});
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
    MODULE_READY: 'module_ready',
    PREPARING_DATA: 'preparing_data',
    EXECUTING_ALGORITHM: 'executing_algorithm',
    COLLECTING_RESULTS: 'collecting_results'
  };

  if(typeof importScripts !== 'function' && typeof AlgorithmRunner === 'function') {
    return PROGRESS_IDS;
  }

  progress({progressID: PROGRESS_IDS.JOB_STARTED});

  const onModuleReady = () => {
    progress({progressID: PROGRESS_IDS.MODULE_READY});

    const response = {};
    const transferableObjects = [];

    progress({progressID: PROGRESS_IDS.PREPARING_DATA});
    const heapPtr = arrayToHeap(input.coordinatesArray, this.wasm_module);

    let err;

    try {
      this.wasm_module.set_raw_input(heapPtr, input.coordinatesArray.length);

      delete input.coordinatesArray;

      if (input.algorithms.includes('graham_scan')
        || input.algorithms.includes('jarvis_march')
      ) {
        const res = this.wasm_module.prepare_list_of_points_input();

        if (res instanceof Error) err = res;
      }

      if(input.algorithms.includes('closest_points')) {
        const res = this.wasm_module.prepare_array_of_points_input();

        if (res instanceof Error) err = res;
      }

      if(!err) {
        input.algorithms.forEach(algorithm => {
          progress({ progressID: PROGRESS_IDS.EXECUTING_ALGORITHM, algorithm });

          const res = this.wasm_module[`run_${algorithm}`]();

          progress({progressID: PROGRESS_IDS.COLLECTING_RESULTS, algorithm });

          response[algorithm] = parseOutput(res, transferableObjects, this.wasm_module);
        });
      }
    }catch (e) {
      err = e;
    } finally {
      this.wasm_module._free(heapPtr);
    }

    if(err) throw err;

    if(transferableObjects.length > 0) {
      done.transfer(response, transferableObjects);
    } else {
      done(response);
    }
  };

  if(this.wasm_module) onModuleReady();
  else {
    progress({progressID: PROGRESS_IDS.MODULE_LOADING});

    const base = new URL('./wasm/', input.location);

    this.wasm_module = WASMModule({
      locateFile: (path) => { return (new URL(path, base)).href; },
      noExitRuntime: true,
      onRuntimeInitialized: ()=>{
        this.wasm_module.set_constants({PROGRESS_IDS});
        onModuleReady();
      }
    });
  }

  function arrayToHeap(typedArray, module) {
    const numBytes = typedArray.length * Int32Array.BYTES_PER_ELEMENT;
    const ptr = module._malloc(numBytes);
    const heapBytes = new Uint8Array(module.HEAPU8.buffer, ptr, numBytes);
    heapBytes.set(new Uint8Array(typedArray.buffer, typedArray.byteOffset, numBytes));

    return heapBytes.byteOffset;
  }

  function parseOutput(output, transferableObjects, module) {
    if(output instanceof Error) throw output;

    if(typeof output !== 'object') return output;

    if(output.data && output.length) {
      const copy = Int32Array.from(
        new Int32Array(module.HEAP8.buffer, output.data, output.length)
      );

      module._free(output.data);

      transferableObjects.push(copy.buffer);

      return {
        buffer: copy.buffer
      };
    }
  }
}
