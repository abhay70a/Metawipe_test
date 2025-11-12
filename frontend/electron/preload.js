const { contextBridge, ipcRenderer } = require("electron");

contextBridge.exposeInMainWorld("MetaWipeAPI", {
  // 🔐 Login (Python)
  loginUser: (username, password) =>
    ipcRenderer.invoke("login-user", { username, password }),

  // 🧹 File Wipe (C)
  wipeFile: (filePath, method) =>
    ipcRenderer.invoke("run-wipe", { filePath, method }),
});
