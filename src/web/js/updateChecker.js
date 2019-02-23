const UPDATE_MSG_CONTAINER_ID = "updateAvailableMessage";
const UPDATE_LINK_TXT_ID = "newUpdateDownloadLink";
const CHANGELOG_BTN_ID = "updateChangelogBtn";

const container = document.getElementById(UPDATE_MSG_CONTAINER_ID);
const link_txt = document.getElementById(UPDATE_LINK_TXT_ID);
const changelog_btn = document.getElementById(CHANGELOG_BTN_ID);

const RELEASE_REQUEST_URL = 'https://tpaydaii.nahueldallacamina.com.ar/release.json';
const RELEASE_CHECK_INTERVAL = 30 * 60 * 1000; // 30 minutes

const APP_VERSION = (new URL(window.location.href)).searchParams.get('version');

async function fetchReleaseData() {
  try {
    const response = await fetch(RELEASE_REQUEST_URL, {
      cache: "no-cache",
      headers: {"Accept": "application/json"},
      method: 'GET'
    });
    if(!response.ok) {
      console.error(
        `Failed to fetch app update information from online source. Status code: ${response.status}`,
        response.statusText
      );
      return;
    }
    return await response.json();
  } catch (e) {
    console.error('Failed to fetch app update information from online source.', e);
  }
}

let changelog = null;
changelog_btn.addEventListener('click', (e)=>{
  if(!changelog) return;

  e.preventDefault();
  e.stopPropagation();

  alert(changelog);
});

let download_url = null;
container.addEventListener('click', ()=>{
  if(!download_url) return;

  window.open(download_url);
});

let checkInterval = RELEASE_CHECK_INTERVAL;
async function checkForUpdates() {
  const release_data = await fetchReleaseData();

  checkInterval *= 2;

  let updateAvailable = false;

  if(typeof release_data === 'object') {
    if(!semverLite.validate(release_data.version)) {
      console.error('Invalid update version information from server: ', release_data.version);
    } else if(semverLite.lt(APP_VERSION, release_data.version)){
      if(!release_data.url || typeof release_data.url !== 'string') {
        console.error('Invalid update download url from server: ', release_data.url);
      } else {
        changelog = release_data.changelog;
        if(typeof changelog === 'string' && changelog.length > 0) container.classList.add('changelogAvailable');
        else container.classList.remove('changelogAvailable');

        download_url = release_data.url;
        link_txt.dataset.link = download_url;

        updateAvailable = true;
      }
    }
  }

  if(updateAvailable) container.classList.add('shown');
  else container.classList.remove('shown');

  checkInterval = checkInterval || RELEASE_CHECK_INTERVAL;
  setTimeout(checkForUpdates, checkInterval);
}

if(APP_VERSION) {
  console.info('App version: '+APP_VERSION);
  checkForUpdates();
  setTimeout(checkForUpdates, RELEASE_CHECK_INTERVAL);
} else {
  console.error('Could not detect app version! Update checker disabled.');
}
