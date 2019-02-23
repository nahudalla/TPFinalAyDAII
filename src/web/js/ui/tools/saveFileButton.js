import saveFile from '../../filesSupport/fileSave.js';

const SAVE_FILE_BTN_QUERY = "#saveFileBtn";

const elem = document.querySelector(SAVE_FILE_BTN_QUERY);
elem.addEventListener("click", saveFile);