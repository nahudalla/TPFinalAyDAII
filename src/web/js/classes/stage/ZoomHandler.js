export default class ZoomHandler {
  constructor(stage) {
    this._stage = stage;

    this._lastAmount = 0;
    this._timeout = null;
  }

  _handleZoom(amount, viewPosition) {
    const settings = this._stage.settings;

    clearTimeout(this._timeout);

    if((amount < 0 && this._lastAmount > 0)
      || (amount > 0 && this._lastAmount < 0)
    ) {
      this._lastAmount = 0;
    }

    this._lastAmount += amount;

    this._stage.setZoom(this._stage.zoom + this._lastAmount, viewPosition);

    this._timeout = setTimeout(()=>{
      this._lastAmount = 0;
      this._timeout = null;
    }, settings.zoom_timeout);
  }

  zoomIn(viewPosition) {
    this._handleZoom(this._stage.settings.zoom_amount, viewPosition);
  }

  zoomOut(viewPosition) {
    this._handleZoom(-this._stage.settings.zoom_amount, viewPosition);
  }
}
