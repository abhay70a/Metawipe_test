const { app, BrowserWindow } = require("electron");
const path = require("path");
const net = require("net");

function createWindow() {
  const win = new BrowserWindow({
    width: 1200,
    height: 800,
    webPreferences: {
      preload: path.join(__dirname, "preload.js"),
      nodeIntegration: false,
      contextIsolation: true,
    },
  });

  const port = 8080; // ✅ the port your Vite app uses
  const devURL = `http://localhost:${port}`;
  const fallbackFile = path.join(__dirname, "../dist/index.html");

  // 🔍 Wait for Vite to actually start before loading
  const tryConnect = setInterval(() => {
    const socket = new net.Socket();
    socket
      .connect(port, "127.0.0.1", () => {
        clearInterval(tryConnect);
        socket.end();
        console.log(`✅ Connected to Vite at ${devURL}`);
        win.loadURL(devURL);
      })
      .on("error", () => {
        console.log(`⏳ Waiting for Vite to start on port ${port}...`);
      });
  }, 1000);

  // If load fails (like in production mode)
  win.webContents.on("did-fail-load", () => {
    console.log("⚠️ Falling back to local build (dist folder)");
    win.loadFile(fallbackFile);
  });

  win.webContents.on("did-finish-load", () => {
    console.log("🚀 MetaWipe frontend loaded successfully!");
  });
}

app.whenReady().then(createWindow);

app.on("window-all-closed", () => {
  if (process.platform !== "darwin") app.quit();
});
