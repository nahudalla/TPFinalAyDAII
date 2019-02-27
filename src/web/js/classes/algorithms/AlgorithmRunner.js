import Observable from '../Observable.js';
import Point from '../Point.js';

(()=>{
  const url = new URL(window.location.href);
  threads.config.set({basepath:{web:url.origin+url.pathname}});
})();

const pool = new threads.Pool().run(worker, ['wasm/WASMModule.js']);

export default class AlgorithmRunner {
  constructor(algorithms, coordinatesArray) {
    if(!Array.isArray(algorithms)) throw new TypeError('AlgorithmRunner: algorithms must be an array.');
    if(!(coordinatesArray instanceof Int32Array)) throw new TypeError('AlgorithmRunner: coordinatesArray must be of type Int32Array');

    if(coordinatesArray.length === 0) throw new Error('Empty coordinates array!');

    this._algorithms = new Map();
    algorithms.forEach(algorithm => {
      this._algorithms.set(algorithm.id, algorithm);
    });

    this._progressEvent = new Observable();
    this._doneEvent = new Observable();
    this._errorEvent = new Observable();
    this._abortEvent = new Observable();

    this._result = null;
    this._error = null;

    const removeAllListeners = () => {
      this._progressEvent.removeAll();
      this._doneEvent.removeAll();
      this._errorEvent.removeAll();
      this._abortEvent.removeAll();
    };

    this._initializing = true;
    this._startTime = performance.now();
    this._endTime = null;

    const onError = (err) => {
      this._initializing = false;
      this._error = err;
      this._job = null;
      this._endTime = performance.now();
      this._errorEvent.emit(err);
      removeAllListeners();
    };

    this._job = pool
      .send({
        algorithms: algorithms.map(algorithm=>algorithm.id),
        location: window.location.href,
        coordinatesArray: {
          length: coordinatesArray.length,
          buffer: coordinatesArray.buffer
        }
      }, [coordinatesArray.buffer])
      .on('progress', (data) => {
        this._initializing = false;
        if(data.algorithm) data.algorithm = this._algorithms.get(data.algorithm);
        this._progressEvent.emit(data);
      })
      .once('error', onError)
      .once('done', (response, ...args) => {
        if(response.error) {
          onError(response.error);
          return;
        }

        this._initializing = false;
        const res = this._result = new Map();
        this._algorithms.forEach((algorithm, key) => {
          let result;
          if(response[key] && response[key].buffer instanceof ArrayBuffer) {
            const i32Array = new Int32Array(response[key].buffer);
            result = [];
            for(let i = 0; i < i32Array.length-1; i += 2) {
              result.push(new Point(i32Array[i], i32Array[i+1]));
            }

            delete response[key].buffer;

            Object.assign(result, response[key])
          } else result = response[key];
          res.set(key, result);
        });
        this._job = null;
        this._endTime = performance.now();
        this._doneEvent.emit(res, ...args);
        removeAllListeners();
      })
      .once('abort', ()=>{
        this._initializing = false;
        this._job = null;
        this._endTime = performance.now();
        this._abortEvent.emit();
        removeAllListeners();
      });
  }

  get PROGRESS_EVENT_IDS() {return worker();}

  abort() { if(this._job) this._job.abort(); }

  get algorithms() { return Array.from(this._algorithms.values()); }

  get progressEvent() { return this._progressEvent; }
  get doneEvent() { return this._doneEvent; }
  get errorEvent() { return this._errorEvent; }
  get abortEvent() { return this._abortEvent; }

  get executionTime() { return (this._endTime || performance.now()) - this._startTime; }

  get isInitializing() { return this._initializing; }
  get isExecuting() { return !!this._job; }
  get isAborted() {return !this._job && !this._result && !this._error;}
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

  self.NativeError = function(message) {
      this.message = message;
  };

  progress({progressID: PROGRESS_IDS.JOB_STARTED});

  const onModuleReady = () => {
    progress({progressID: PROGRESS_IDS.MODULE_READY});

    const response = {};
    const transferableObjects = [];

    let err;

    try {
      progress({progressID: PROGRESS_IDS.PREPARING_DATA});
      const heapPtr = arrayToHeap(input.coordinatesArray, this.wasm_module);

      this.wasm_module.set_raw_input(heapPtr, input.coordinatesArray.length);

      delete input.coordinatesArray;

      if (input.algorithms.includes('graham_scan')
        || input.algorithms.includes('jarvis_march')
      ) {
        const res = this.wasm_module.prepare_list_of_points_input();

        if (res instanceof Error || res instanceof self.NativeError) err = res;
      }

      if(input.algorithms.includes('closest_points')
         || input.algorithms.includes('art_gallery_problem_on_simple_polygon')
         || input.algorithms.includes('partition_simple_polygon_to_monotone')
         || input.algorithms.includes('triangulate_simple_polygon')
      ) {
        const res = this.wasm_module.prepare_array_of_points_input();

        if (res instanceof Error || res instanceof self.NativeError) err = res;
      }

      if(input.algorithms.includes('any_segments_intersect')) {
        const res = this.wasm_module.prepare_list_of_segments_input();

        if (res instanceof Error || res instanceof self.NativeError) err = res;
      }

      if(!err) {
        input.algorithms.forEach(algorithm => {
          progress({ progressID: PROGRESS_IDS.EXECUTING_ALGORITHM, algorithm });

          let res = this.wasm_module[`run_${algorithm}`]();

          progress({progressID: PROGRESS_IDS.COLLECTING_RESULTS, algorithm });

          response[algorithm] = parseOutput(res, transferableObjects, this.wasm_module);
        });
      }
    }catch (e) {
      err = e;
    } finally {
      try {
        this.wasm_module._free(heapPtr);
      } catch (e) {}
    }

    if(err) {
      console.error(err);
      response.error = err;
    }

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
    if(output instanceof Error || output instanceof self.NativeError) throw output;

    if(typeof output !== 'object' || output === null) return output;

    if(output.data && output.length) {
      const copy = Int32Array.from(
        new Int32Array(module.HEAP8.buffer, output.data, output.length)
      );

      module._free(output.data);

      transferableObjects.push(copy.buffer);

      delete output.data;
      delete output.length;

      return {
        ...output,
        buffer: copy.buffer
      };
    }
  }
}
