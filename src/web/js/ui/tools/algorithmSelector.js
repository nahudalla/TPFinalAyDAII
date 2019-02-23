import { activeContextChangedEvent } from '../../classes/Context.js';
import log from '../../logger.js';
import getActiveContext from '../../classes/Context.js';
import algorithmResultUI from '../algorithmResult.js';
import canModifyAppState from '../../appStateFreeze.js';

const ALGORITHM_SELECT_QUERY = "#algorithmSelect";
const EXECUTE_ALGORITHM_BTN_QUERY = "#executeAlgorBtn";

const select = document.querySelector(ALGORITHM_SELECT_QUERY);
const execute_btn = document.querySelector(EXECUTE_ALGORITHM_BTN_QUERY);

let lastContext = null;

activeContextChangedEvent.subscribe(context => {
  if(lastContext) lastContext.algorithms.enabledAlgorithmsChangedEvent.unsubscribe(onAlgorChange);

  while(select.firstChild) select.removeChild(select.firstChild);

  let prevSelected = null;
  context.algorithms.listAll.forEach(algorithm => {
    const opt = document.createElement('option');
    opt.value = algorithm.id;
    opt.innerText = algorithm.name;

    if(algorithm.enabled) {
      if(prevSelected) prevSelected.removeAttribute('selected');
      opt.selected = "selected";
    }

    select.appendChild(opt);
  });

  lastContext = context;
  context.algorithms.enabledAlgorithmsChangedEvent.subscribe(onAlgorChange);
});

function onAlgorChange(algor, enabled) {
  if(!enabled) return;


  if(select.value !== algor.id) select.value = algor.id;

  log(log.FLAGS.INFO_2, `Usando algoritmo: ${algor.name}`);
}

select.addEventListener('change', ()=>{
  if(!lastContext) return;

  if(canModifyAppState('cambiar el algoritmo activo')) {
    lastContext.algorithms.disableAll();

    lastContext.algorithms.get(select.value)
      .enable();
  } else {
    select.value = lastContext.algorithms.enabledAlgorithms[0].id;
  }
});

execute_btn.addEventListener('click', ()=>{
  const ctx = getActiveContext();

  if(ctx.pointsList.length === 0) {
    alert('No hay puntos de entrada.');
    return;
  }

  const runner = ctx.algorithms.enabledAlgorithms[0].run();

  algorithmResultUI.showResult(runner);
});
