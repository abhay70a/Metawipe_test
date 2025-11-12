export {};

declare global {
  interface Window {
    MetaWipeAPI: {
      loginUser: (username: string, password: string) => Promise<string>;
      wipeFile: (filePath: string, method: string) => Promise<string>;
    };
  }

  // ✅ Electron adds a native path property to File objects
  interface File {
    path: string;
  }
}
