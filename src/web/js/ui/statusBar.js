import {logger, FLAGS} from '../logger.js'

const STATUS_BAR_SELECTOR = ".statusBar";
const STATUS_SELECTOR = ".statusBar > span";
const LOG_SELECTOR = ".statusBar > div";

const MAX_LOG_ELEMS = 150;

const container = document.querySelector(STATUS_BAR_SELECTOR);
const status_container = document.querySelector(STATUS_SELECTOR);
const log_container = document.querySelector(LOG_SELECTOR);
const log = [];

container.addEventListener('mouseenter', e => scrollDown(e.target));

logger.addListener(FLAGS.ALL, (message, flags) => {
  let msg = "";

  if((flags & FLAGS.CRITICAL) !== 0) msg = "ERROR CRÍTICO: ";
  else if((flags & FLAGS.ERROR) !== 0) msg = "ERROR: ";
  else if((flags & FLAGS.INFO_ALL) !== 0) msg = "INFORMACIÓN: ";

  const pre = msg;

  msg += message;

  setMessage(pre, message);

  if((flags & (FLAGS.CRITICAL | FLAGS.ERROR)) !== 0) {
    console.error(msg);
    alert(msg);
  } else {
    console.log(msg);
  }
});

function setMessage(pre, content) {
  let elem;

  const scrolledDown = isScrolledDown(container);

  if(log.length < MAX_LOG_ELEMS) {
    elem = document.createElement('div');
  } else {
    elem = log.shift(log);
    log_container.removeChild(elem);
  }

  elem.innerText = pre + content;

  status_container.innerText = content;

  log_container.appendChild(elem);
  log.push(elem);

  if(scrolledDown) scrollDown(container);
}

function isScrolledDown(e) {
  return e.scrollHeight-e.clientHeight <= e.scrollTop + 20;
}

function scrollDown(e) {
  e.scrollTop = e.scrollHeight - e.clientHeight;
}
