import triggerFileSelectionDialog from '../../filesSupport/fileLoad.js';

const LOAD_FILE_BTN_QUERY = "#loadFileBtn";

const elem = document.querySelector(LOAD_FILE_BTN_QUERY);
elem.addEventListener('click', triggerFileSelectionDialog);