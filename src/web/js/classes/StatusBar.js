const STATUS_BAR_SELECTOR = ".statusBar";
const STATUS_SELECTOR = ".statusBar > span";
const LOG_SELECTOR = ".statusBar > div";

const MAX_LOG_ELEMS = 150;

class StatusBar {
  constructor() {
    this._container = document.querySelector(STATUS_BAR_SELECTOR);
    this._status_container = document.querySelector(STATUS_SELECTOR);
    this._log_container = document.querySelector(LOG_SELECTOR);

    this._container.addEventListener('mouseenter', e => scrollDown(e.target));

    this._log = [];
  }

  setMessage(content) {
    let elem;

    const scrolledDown = isScrolledDown(this._container);

    if(this._log.length < MAX_LOG_ELEMS) {
      elem = document.createElement('div');
    } else {
      elem = this._log.shift(this._log);
      this._log_container.removeChild(elem);
    }

    elem.innerText = content;

    this._status_container.innerText = content;

    this._log_container.appendChild(elem);
    this._log.push(elem);

    if(scrolledDown)
      scrollDown(this._container);
  }
}

const instance = new StatusBar();

export default instance;

function isScrolledDown(e) {
  return e.scrollHeight-e.clientHeight <= e.scrollTop + 20;
}

function scrollDown(e) {
  e.scrollTop = e.scrollHeight - e.clientHeight;
}
