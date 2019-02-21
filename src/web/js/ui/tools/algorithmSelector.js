import { activeContextChangedEvent } from '../../classes/Context.js';
import log from '../../logger.js';

const ALGORITHM_SELECT_QUERY = "#algorithmSelect";
const EXECUTE_ALGORITHM_BTN_QUERY = "#executeAlgorBtn";

const select = document.querySelector(ALGORITHM_SELECT_QUERY);
const execute_btn = document.querySelector(EXECUTE_ALGORITHM_BTN_QUERY);

while(select.firstChild) select.removeChild(select.firstChild);

let lastContext = null;

activeContextChangedEvent.subscribe(context => {
  if(lastContext) lastContext.algorithms.enabledAlgorithmsChangedEvent.unsubscribe(onAlgorChange);

  context.algorithms.algorithms.forEach(algorithm => {
    const opt = document.createElement('option');
    opt.value = algorithm.id;
    opt.innerText = algorithm.name;
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

  lastContext.algorithms.get(select.value).enable();
  lastContext.algorithms.enabledAlgorithms.forEach(algorithm => {
    if(algorithm.id !== select.value) algorithm.disable();
  })
});

execute_btn.addEventListener('click', ()=>{
  alert(select.value);
});