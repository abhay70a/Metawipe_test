const { contextBridge, ipcRenderer } = require("electron");

contextBridge.exposeInMainWorld("MetaWipeAPI", {
  wipeFile: (filePath, method) =>
    ipcRenderer.invoke("wipe-file", { filePath, method })
});
