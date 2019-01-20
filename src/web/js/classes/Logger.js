export const FLAGS = {
  CRITICAL  : 1 << 0,
  ERROR     : 1 << 1,
  INFO_1    : 1 << 2,
  INFO_2    : 1 << 3,
  INFO_3    : 1 << 4,
  INFO_4    : 1 << 5,
  INFO_5    : 1 << 6,
};

FLAGS.INFO      = FLAGS.INFO_1;
FLAGS.INFO_ALL  = FLAGS.INFO_1 | FLAGS.INFO_2 | FLAGS.INFO_3 | FLAGS.INFO_4 | FLAGS.INFO_5;
FLAGS.ALL       = FLAGS.INFO_ALL | FLAGS.CRITICAL | FLAGS.ERROR;

class Logger {
  constructor() {
    this._listeners = [];
  }

  addListener(flags, listener) {
    this._listeners.push({flags, listener});
  }

  log(flags, message) {
    this._listeners.forEach(list => {
      if((list.flags & flags) !== 0) {
        list.listener(message, flags);
      }
    });
  }
}

export const logger = new Logger();

function log(...data) {
  logger.log(...data);
}

log.FLAGS = FLAGS;

export default log;
