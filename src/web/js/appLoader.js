(()=>{
  const third_party_libraries = ['download.js', 'paper-core.js', 'Sortable.js'];

  const message = document.getElementById('loadingMessage');

  window.Module = {
    onRuntimeInitialized: async function () {
      loadAppStylesheet()
        .then(loadThirdPartyLibraries)
        .then(loadMainModule)
        .then(loadFinished)
        .catch((...params) => {
          alert("Error al cargar la aplicación. Para más información ver la consola de desarrollador.");
          console.error(...params);
        });
    }
  };

  window.addEventListener('load', function () {
    message.innerText = "Cargando módulo nativo de algoritmos...";

    const script = document.createElement('script');
    script.type = "text/javascript";
    script.src = `wasm/WASMModule.js`;

    document.body.appendChild(script);
  });

  function loadAppStylesheet() {
    message.innerText = "Cargando interfaz gráfica...";

    return new Promise((resolve, reject) => {
      let stylesheet = document.createElement('link');
      stylesheet.rel = "stylesheet";
      stylesheet.type = "text/css";
      stylesheet.href = "css/app.css";

      stylesheet.onload = resolve;
      stylesheet.onerror = reject;

      document.head.appendChild(stylesheet);
    });
  }

  function loadThirdPartyLibraries() {
    let loaded = -1;
    function libraryLoaded() {
      message.innerText = `Cargando librerías externas (${++loaded}/${third_party_libraries.length})...`;
    }
    libraryLoaded();

    const promises = [];

    third_party_libraries.forEach(library => {
      promises.push(new Promise((resolve, reject) => {
        const script = document.createElement('script');
        script.type = "text/javascript";
        script.src = `third_party/${library}`;

        script.onload = (...params)=>{
          libraryLoaded();
          resolve(...params);
        };
        script.onerror = reject;

        document.body.appendChild(script);
      }));
    });

    return Promise.all(promises);
  }

  function loadMainModule() {
    message.innerText = "Preparando funcionalidades...";
    return new Promise((resolve, reject) => {
      let script = document.createElement('script');
      script.type = "module";
      script.src = "js/main.js";

      script.onload = resolve;
      script.onerror = reject;

      document.body.appendChild(script);
    });
  }

  function loadFinished() {
    message.innerText = "Listo";
    const loadingMessage = document.querySelector(".loadingMessage");
    document.body.removeChild(loadingMessage);
  }
})();
