const { app, BrowserWindow, session, shell } = require('electron');
const FS = require('fs');
const finalhandler = require('finalhandler');
const http = require('http');
const serveStatic = require('serve-static');

const URL = require('url').URL;

app.on('web-contents-created', (event, contents) => {
  contents.on('will-navigate', (event, navigationUrl) => {
    const parsedUrl = new URL(navigationUrl);

    if (parsedUrl.origin !== 'http://localhost') {
      event.preventDefault()
    }
  });

  contents.on('new-window', (event, navigationUrl) => {
    // We'll ask the operating system
    // to open this event's url in the default browser.
    event.preventDefault();

    if(navigationUrl.startsWith('https://')) {
      shell.openExternal(navigationUrl);
    }
  });
});

// Load package.json
let app_version = null;
try {
  const package_json = JSON.parse(FS.readFileSync(__dirname+'/package.json'));
  if(package_json && package_json.version) {
    app_version = package_json.version;
  }
}catch(e){
  console.error(e);
}

if(app_version) console.info('App version: '+app_version);

// Serve up . folder
var serve = serveStatic(__dirname, {setHeaders:(res, path)=>{
  if(path.endsWith('.wasm')) {
    res.setHeader('Content-Type', 'application/wasm');
  }
}});

// Create server
const server = http.createServer( (req, res) => {
  serve(req, res, finalhandler(req, res));
});

// Keep a global reference of the window object, if you don't, the window will
// be closed automatically when the JavaScript object is garbage collected.
let mainWindow, errWindow;

const appReady = () => {
  session
    .fromPartition('some-partition')
    .setPermissionRequestHandler((webContents, permission, callback) => {
      // Denies the permissions request
      return callback(false)
    });

  server.on('error',()=>{
    if(errWindow) return;

    errWindow = new BrowserWindow({
      width: 800,
      height: 600,
      nodeIntegration: false
    });

    errWindow.loadURL(`file://${__dirname}/appError.html`);

    errWindow.on('closed', ()=>{
      errWindow.close();
    });
  });

  server.listen(()=>{
    // Create the browser window.
    mainWindow = new BrowserWindow({
      width: 800,
      height: 600,
      center: true,
      autoHideMenuBar: true,
      webPreferences: {
        nodeIntegration: false,
        contextIsolation: true,
        devTools: false,
        webaudio: false,
        plugins: false
      }
    });

    const addr = server.address();

    session.defaultSession.webRequest.onHeadersReceived((details, callback) => {
      callback({
        responseHeaders: {
          ...details.responseHeaders,
          'Content-Security-Policy': [
            `script-src 'unsafe-inline' http://localhost:${addr.port}`,
            `style-src 'unsafe-inline' http://localhost:${addr.port}`,
            `worker-src blob:http://localhost:${addr.port}`,
            `default-src 'none'`].join(';')
        }
      })
    });

    // and load the index.html of the app.
    mainWindow.loadURL(`http://localhost:${addr.port}/` + (app_version?`?version=${app_version}`:''));

    // Open the DevTools.
    //mainWindow.webContents.openDevTools();

    // Emitted when the window is closed.
      mainWindow.on('closed', () => {
        // Dereference the window object, usually you would store windows
        // in an array if your app supports multi windows, this is the time
        // when you should delete the corresponding element.
        mainWindow = null;
        server.close();
      });
  });
};

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.on('ready', appReady);

// Quit when all windows are closed.
app.on('window-all-closed', () => {
  // On OS X it is common for applications and their menu bar
  // to stay active until the user quits explicitly with Cmd + Q
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

app.on('activate', () => {
  // On OS X it's common to re-create a window in the app when the
  // dock icon is clicked and there are no other windows open.
  if (mainWindow === null) {
    appReady();
  }
});

// In this file you can include the rest of your app's specific main process
// code. You can also put them in separate files and import them here.
