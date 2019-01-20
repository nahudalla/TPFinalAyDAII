export class ParserError extends Error {
  constructor() {
    super("Incorrect file format");
  }
}

import Point from './Point.js'

export default function parse(str) {
  const lines = str.split('\n');

  const res = [];

  lines.forEach(line => {
    if(line.length === 0) return;

    const terms = line.split(' ');
    if(terms.length !== 2) throw new ParserError();

    const x = parseInt(terms[0]);
    const y = parseInt(terms[1]);

    if(Number.isNaN(x) || Number.isNaN(y)) throw new ParserError();

    res.push(new Point(x, y));
  });

  return res;
}
