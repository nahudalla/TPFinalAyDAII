// This file generates three distributable .zip files from the packaged versions of the app,
// a release.json file describing the current release, a download page for users to select
// a specific platform distributable and a configuration file to upload everything to Firebase

const zipper = require('zip-a-folder');
const FS = require('fs');
const rimraf = require('rimraf');
const {valid, lt} = require('semver');
const fetch = require('node-fetch');

const OUTPUT_DIRECTORY = "./dist";
const CONTENT_DIRECTORY = `${OUTPUT_DIRECTORY}/public`;
const PACKAGES_DIRECTORY = "./packaged";
const BASE_URL = "https://tpaydaii.nahueldallacamina.com.ar";
const FIREBASE_CONFIG_FILENAME = "firebase.json";
const RELEASE_JSON_FILENAME = "release.json";
const CHANGELOG_FILE = "./CHANGELOG";
const DOWNLOAD_PAGE_FILENAME = "download.html";
const PACKAGE_JSON_FILE = "./package.json";

const PLATFORMS = [
  {
    platform: 'darwin',
    displayName: 'Mac OS',
    distributableName: 'macOS'
  },
  {
    platform: 'linux',
    displayName: 'Linux',
    distributableName: 'linux'
  },
  {
    platform: 'win32',
    displayName: 'Windows',
    distributableName: 'win32'
  }
];

const FIREBASE_CONFIG = {
  "hosting": {
    "public": "public",
    "ignore": [
      "firebase.json",
      "**/.*",
      "**/node_modules/**"
    ],
    "headers": [
      {
        "source": "*",
        "headers": [
          {
            "key": "Access-Control-Allow-Origin",
            "value": "*"
          },
          {
            "key": "Cache-Control",
            "value": "no-cache, no-store, must-revalidate"
          }
        ]
      }
    ]
  }
};

void async function main() {
  console.log('Cleaning up old distributable files...');
  rimraf.sync(OUTPUT_DIRECTORY);

  ensureDirectoryExists(OUTPUT_DIRECTORY);
  ensureDirectoryExists(CONTENT_DIRECTORY);

  console.log('Reading version information from "package.json"...');

  const package_json = JSON.parse(FS.readFileSync(PACKAGE_JSON_FILE, "utf8"));
  const version = package_json.version;

  if(!version) {
    console.error('No version field in package.json!');
    return;
  }

  if(!valid(version)) {
    console.error('Invalid version in package.json!');
    return;
  }

  console.log(`Version is: ${version}`);

  console.log('Obtaining latest published release version from server...')

  const response = await fetch(`${BASE_URL}/${RELEASE_JSON_FILENAME}`, {
    cache: "no-cache",
    headers: {"Accept": "application/json"},
    method: 'GET'
  });

  if(!response.ok) {
    console.error(
      `Failed to fetch app information from online source. Status code: ${response.status}`,
      response.statusText
    );
    return;
  }

  const server_json = await response.json();

  if(!server_json || !server_json.version || !valid(server_json.version)) {
    console.error('Invalid version information from server.');
    return;
  }

  if(!lt(server_json.version, version)) {
    console.error(`Version from "package.json" (${version}) is not greater than latest `
                + `published version from server (${server_json.version}). Please update version `
                + `information in "package.json".`);
    return;
  }

  console.log('Reading changelog...');

  const changelog = FS.readFileSync(CHANGELOG_FILE, "utf8");

  if(!changelog) {
    console.error('Changelog empty!');
    return;
  }

  if(!changelog.includes(version)) {
    console.error('Version number not found in changelog!');
    return;
  }

  console.log('Generating distributables...');

  await generateDistributables(version);

  console.log(`Generating "${RELEASE_JSON_FILENAME}"...`);

  FS.writeFileSync(
    `${CONTENT_DIRECTORY}/${RELEASE_JSON_FILENAME}`,
    JSON.stringify(generateReleaseJSON(version, changelog))
  );

  console.log('Generating download page...');

  FS.writeFileSync(
    `${CONTENT_DIRECTORY}/${DOWNLOAD_PAGE_FILENAME}`,
    generateDownloadPage(version)
  );

  console.log('Writing Firebase configuration...');

  FS.writeFileSync(
    `${OUTPUT_DIRECTORY}/${FIREBASE_CONFIG_FILENAME}`,
    JSON.stringify(FIREBASE_CONFIG)
  );

  console.log('Done');
}();

function generatePackagePath(platform) {
  return `${PACKAGES_DIRECTORY}/tpfinalaydaii-${platform.platform}-x64/`;
}

function generateDistributableFilename(platform, version) {
  return `TPFinalAyDAII_v${version}_${platform.distributableName}_x64.zip`;
}

function generateDownloadPage(version) {
  let platforms = '';
  PLATFORMS.forEach(platform => {
    platforms += `<li>
<b>${platform.displayName} (x64):</b>
<a href="${BASE_URL}/${generateDistributableFilename(platform,version)}">${generateDistributableFilename(platform,version)}</a>
</li>`;
  });

  return `
<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8" />
<title>TPFinalAyDAII - Descargar</title>
</head>
<body>
<h1>TPFinalAyDAII - v${version}</h1>
<p>Este trabajo fue hecho para final de las materias Análisis y Diseño de Algoritmos I y II, en la
Universidad Nacional del Centro de la Provincia de Buenos Aires, Argentina. Desarrollado por Nahuel
Dallacamina Ortea entre Noviembre de 2018 y Febrero de 2019.</p>
<p>El objetivo fue realizar una herramienta que permita trabajar con algoritmos geométricos básicos
y permita realizar una ejecución paso a paso de los mismos, con fines didácticos.</p>
<p>Algunas funcionalidades propias de la interfaz gráfica aún no se encuentran finalizadas. La 
herramienta disponible para descarga en esta página verificará periódicamente si hay nuevas versiones
disponibles y mostrará un mensaje para que el usuario pueda ingresar a esta página nuevamente y 
descargue la última versión disponible.</p>
<p>Es posible acceder al código de fuente de esta herramienta en GitHub: <a href="https://github.com/nahudalla/TPFinalAyDAII">https://github.com/nahudalla/TPFinalAyDAII</a></p>
<p>A continuación, descargue el archivo comprimido que corresponda para su plataforma. Una vez
descargado, descomprímalo y ejecute el archivo "tpfinalaydaii", con la extensión que corresponda
para su plataforma.</p>
<p><b>La herramienta sólo se distribuye para la arquitectura x64.</b></p>
<ul>${platforms}</ul>
</body>
</html>
`;
}

async function generateDistributables(version) {
  for(let platform of PLATFORMS) {
    const path = generatePackagePath(platform);
    if(!FS.existsSync(path)) {
      console.error(`Skipping platform ${platform.displayName}, package not found.`);
      return;
    }

    await zipper.zip(path, `${CONTENT_DIRECTORY}/${generateDistributableFilename(platform, version)}`);
  }
}

function ensureDirectoryExists(directory) {
  if (!FS.existsSync(directory)){
    FS.mkdirSync(directory);
  }
}

function generateReleaseJSON(version, changelog) {
  return {
    version, changelog, url: `${BASE_URL}/${DOWNLOAD_PAGE_FILENAME}`
  };
}
