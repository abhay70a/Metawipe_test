const { app, BrowserWindow, ipcMain } = require("electron");
const path = require("path");
const { execFile, spawn } = require("child_process");
const net = require("net");

function createWindow() {
  const win = new BrowserWindow({
    width: 1200,
    height: 800,
    webPreferences: {
      preload: path.join(__dirname, "preload.js"),
      contextIsolation: true,
      nodeIntegration: false,
    },
  });

  // Connect to Vite (development)
  const port = 8080;
  const devURL = `http://localhost:${port}`;
  const prodFile = path.join(__dirname, "../frontend/dist/index.html");

  const tryConnect = setInterval(() => {
    const socket = new net.Socket();
    socket
      .connect(port, "127.0.0.1", () => {
        clearInterval(tryConnect);
        socket.end();
        console.log(`✅ Connected to ${devURL}`);
        win.loadURL(devURL);
      })
      .on("error", () => {
        console.log(`⏳ Waiting for Vite on port ${port}...`);
      });
  }, 1000);

  win.webContents.on("did-fail-load", () => {
    console.log("⚠️ Fallback: loading local build");
    win.loadFile(prodFile);
  });
}

// ✅ Run C Wipe Engine
ipcMain.handle("run-wipe", async (event, { filePath, method }) => {
  return new Promise((resolve, reject) => {
    const exePath = path.join(__dirname, "../core/build/wipe_engine.exe");
    console.log("🧹 Running:", exePath, filePath, method);

    execFile(exePath, [filePath, method], (error, stdout, stderr) => {
      if (error) {
        console.error("❌ Error:", stderr);
        reject(stderr);
      } else {
        console.log("✅ Output:", stdout);
        resolve(stdout);
      }
    });
  });
});

// ✅ Run Python Login Script
ipcMain.handle("login-user", async (event, { username, password }) => {
  return new Promise((resolve, reject) => {
    const pyPath = path.join(__dirname, "../python/login.py");
    console.log("🔐 Executing:", pyPath);

    const py = spawn("python", [pyPath, username, password]);

    let output = "";
    py.stdout.on("data", (data) => (output += data.toString()));
    py.stderr.on("data", (err) => console.error("⚠️ Python Error:", err.toString()));
    py.on("close", () => resolve(output.trim()));
  });
});

app.whenReady().then(createWindow);
app.on("window-all-closed", () => {
  if (process.platform !== "darwin") app.quit();
});
