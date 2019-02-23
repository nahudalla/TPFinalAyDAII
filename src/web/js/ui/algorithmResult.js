import AlgorithmRunner from '../classes/algorithms/AlgorithmRunner.js';
import {
  freezeAppState,
  unfreezeAppState,
  userAuthorizedModificationEvent
} from '../appStateFreeze.js';
import log from '../logger.js';

const CONTAINER_ID = "algorithmResultToolsContainer";
const CLOSE_BTN_ID = "hideResultToolsBtn";
const RESULT_TXT_ID = "algorithmResultText";
const MAKE_RESULT_INTERACTIVE_BTN_ID = "makeResultInteractiveBtn";
const CANCEL_EXECUTION_BTN_ID = "cancelAlgorithmExecutionBtn";
const EXECUTING_MSG_CONTAINER_ID = "executingAlgorithmMsgContainer";
const EXECUTING_MSG_ID = "executingAlgorithmMsg";
const CONTROLS_CONTAINER_ID = "algorithmInteractiveResultControls";
const GO_TO_START_BTN_ID = "goToStartResultBtn";
const GO_TO_PREV_BTN_ID = "goToPrevResultBtn";
const PLAYBACK_SPEED_SELECTOR_ID = "resultPlaybackSpeed";
const PLAY_BTN_ID = "playStepsResultBtn";
const PAUSE_BTN_ID = "pauseStepsResultBtn";
const GO_TO_NEXT_BTN_ID = "goToNextResultBtn";
const GO_TO_END_BTN_ID = "goToEndResultBtn";
const STEP_SLIDER_ID = "resultPlaybackStep";

const FREEZE_REASON = "Se está mostrando en pantalla el resultado de la ejecución de un algoritmo.";
const INITIALIZING_MSG = "Inicializando...";
const EXECUTING_MSG = "Ejecutando algoritmo...";

const SHOW_ABORT_BTN_TIMEOUT = 7000; // ms

const container = document.getElementById(CONTAINER_ID);
const close_btn = document.getElementById(CLOSE_BTN_ID);
const result_txt = document.getElementById(RESULT_TXT_ID);
const make_interactive_btn = document.getElementById(MAKE_RESULT_INTERACTIVE_BTN_ID);
const cancel_execution_btn = document.getElementById(CANCEL_EXECUTION_BTN_ID);
const executing_msg_container = document.getElementById(EXECUTING_MSG_CONTAINER_ID);
const executing_msg = document.getElementById(EXECUTING_MSG_ID);
const controls_container = document.getElementById(CONTROLS_CONTAINER_ID);

const algorithmResultUI = {
  showResult, hideResult
};

export default algorithmResultUI;

let current_runner = null;
let abortBtnTimeout = null;

function showResult(runner) {
  if(!(runner instanceof AlgorithmRunner)) {
    throw new TypeError("Invalid algorithm runner. Must be an instance of AlgorithmRunner.");
  }

  freezeAppState(FREEZE_REASON);

  if(current_runner) removeListeners();
  current_runner = runner;

  reset();

  if(container.classList.contains('hide-animation')) {
    container.classList.replace('hide-animation','hidden-state');
    void container.offsetWidth; // Trigger reflow to reset CSS animations
    container.classList.replace('hidden-state', 'show-animation');
  } else {
    container.classList.add('show-animation');
  }

  function showLoadedFromCache() {
    log(
      log.FLAGS.INFO,
      `Resultado de ejecución del algoritmo "${runner.algorithms[0].name}" cargado desde la caché`
    );
  }

  if(runner.result) {
    showLoadedFromCache();
    showResultInUI();
  } else if(runner.error) {
    showLoadedFromCache();
    showErrorInUI();
  } else {
    executing_msg_container.classList.remove('hidden');
    if(runner.isExecuting && !runner.isInitializing) {
      updateProgress(EXECUTING_MSG);
      abortBtnTimeout = setTimeout(showAbortBtn, SHOW_ABORT_BTN_TIMEOUT);
    } else updateProgress(INITIALIZING_MSG);
    runner.progressEvent.subscribe(onProgress);
    runner.doneEvent.subscribe(onDone);
    runner.errorEvent.subscribe(onError);
  }

  runner.abortEvent.subscribe(onAbort);
}

function showAbortBtn() {
  cancel_execution_btn.classList.remove('hidden');
}

function onAbort() {
  log(log.FLAGS.INFO, `Ejecución del algoritmo "${current_runner.algorithms[0].name}" cancelada`);
  hideResult();
}

userAuthorizedModificationEvent.subscribe(()=>{
  if(current_runner.isExecuting) current_runner.abort();
  else hideResult();
});

function hideResult() {
  if(!current_runner) return;

  if(current_runner.isExecuting) {
    if(confirm("¿Confirma cerrar el resultado?\nEl algoritmo aún se encuentra en ejecución y será cancelado."))
    { current_runner.abort(); }
    return;
  }

  clearTimeout(abortBtnTimeout);
  abortBtnTimeout = null;

  const algor = current_runner.algorithms[0];
  algor.hideResultUI();

  removeListeners();
  current_runner = null;
  unfreezeAppState();
  container.classList.replace('show-animation', 'shown-state');
  void container.offsetWidth; // Trigger reflow to reset CSS animations
  container.classList.replace('shown-state', 'hide-animation');
}

function showResultInUI() {
  const algor = current_runner.algorithms[0];
  const result = current_runner.result.get(current_runner.algorithms[0].id);

  if(typeof result === 'undefined') {
    inputDataNotValid();
    reset();
    showErrorInUI();
    return;
  } else if(!algor.isValidResult(result)) {
    const err = new Error(`Invalid result for algorithm "${algor.name}".`);
    err.result = result;
    onError(err);
    return;
  }

  const msg = algor.generateResultMessage(result);

  if(msg instanceof HTMLElement) result_txt.appendChild(msg);
  else result_txt.innerText = msg;

  algor.showResultUI(result);
}

function showErrorInUI() {
  result_txt.innerText = "Error en la ejecución del algoritmo";
}

function onDone() {
  log(log.FLAGS.INFO, `Ejecución del algoritmo "${current_runner.algorithms[0].name}" finalizada `
    +`en ${current_runner.executionTime} milisegundos`);
  reset();
  showResultInUI();
}

function inputDataNotValid(err) {
  log(log.FLAGS.ERROR, `Puntos de entrada no válidos para ejecutar el algoritmo "${current_runner.algorithms[0].name}"`, err);
}

function onError(err) {
  if(err.message && /\binvalid\b/gi.test(err.message)
     && /\binput\b/.test(err.message)
     && /\bdata\b/.test(err.message))
  {
    inputDataNotValid(err);
  } else {
    Sentry.withScope(scope => {
      scope.setTag("type", "algorithm-execution-failed");
      scope.setLevel('fatal');
      Sentry.captureException(err);
    });
    log(log.FLAGS.ERROR, `Error ejecutando "${current_runner.algorithms[0].name}"`, err);
  }

  reset();
  showErrorInUI();
}

function onProgress(data) {
  const IDS = current_runner.PROGRESS_EVENT_IDS;

  const algorName = data.algorithm ? ` "${data.algorithm.name}"` : '';

  switch (data.progressID) {
    case IDS.JOB_STARTED:
      updateProgress(INITIALIZING_MSG, `Trabajo de ejecución de algoritmo${algorName} iniciado`, log.FLAGS.INFO_4);
      break;
    case IDS.MODULE_LOADING:
      updateProgress(INITIALIZING_MSG, "Cargando módulo nativo de algoritmos...", log.FLAGS.INFO_4);
      break;
    case IDS.MODULE_READY:
      updateProgress(INITIALIZING_MSG, "Módulo nativo de algoritmos listo", log.FLAGS.INFO_4);
      break;
    case IDS.PREPARING_DATA:
      updateProgress("Preparando datos...", "Transfiriendo datos a módulo nativo de algoritmos...", log.FLAGS.INFO_4);
      break;
    case IDS.EXECUTING_ALGORITHM:
      updateProgress("Ejecutando algoritmo...", `Ejecutando algoritmo${algorName}...`);
      if(!abortBtnTimeout) abortBtnTimeout = setTimeout(showAbortBtn, SHOW_ABORT_BTN_TIMEOUT);
      break;
    case IDS.COLLECTING_RESULTS:
      updateProgress(
        "Obteniendo resultado...",
        `Obteniendo resultados de ejecución del algoritmo${algorName}...`,
        log.FLAGS.INFO_4
      );
      break;
    default:
      updateProgress(
        "Código desconocido",
        `Código de progreso desconocido: ${data.progressID}`,
        log.FLAGS.INFO_4
      );
  }
}

function updateProgress(uiMessage, loggerMessage, loggerLevel) {
  executing_msg.innerText = uiMessage;

  if(loggerMessage) {
    log(loggerLevel || log.FLAGS.INFO, loggerMessage);
  }
}

function removeListeners() {
  current_runner.progressEvent.unsubscribe(onProgress);
  current_runner.doneEvent.unsubscribe(onDone);
  current_runner.errorEvent.unsubscribe(onError);
  current_runner.abortEvent.unsubscribe(onAbort);
}

function reset() {
  result_txt.innerText = "";
  while(result_txt.firstChild) result_txt.removeChild(result_txt.firstChild);
  make_interactive_btn.classList.add('hidden');
  cancel_execution_btn.classList.add('hidden');
  executing_msg_container.classList.add('hidden');
  executing_msg.innerText = "";
  controls_container.classList.add('hidden');
  clearTimeout(abortBtnTimeout);
  abortBtnTimeout = null;
}

close_btn.addEventListener('click', hideResult);

cancel_execution_btn.addEventListener('click', ()=>{
  if(current_runner && current_runner.isExecuting
     && confirm("¿Confirma cancelar la ejecución?\nLa ejecución del algoritmo será cancelada.")
  ) { current_runner.abort(); }
});

window.addEventListener('beforeunload', (event)=>{
  if(current_runner && current_runner.isExecuting) {
    event.preventDefault();
    return event.returnValue = '¡ATENCIÓN! ¿Seguro desea salir?\nLa ejecución de un algoritmo está en progreso.';
  }
});
