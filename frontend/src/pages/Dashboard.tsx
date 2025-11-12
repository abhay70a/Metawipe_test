import { useState, useRef } from "react";
import { Button } from "@/components/ui/button";
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from "@/components/ui/card";
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from "@/components/ui/select";
import { Progress } from "@/components/ui/progress";
import { toast } from "sonner";
import { Upload, Trash2, AlertCircle } from "lucide-react";
import Layout from "@/components/Layout";

const Dashboard = () => {
  const [selectedFile, setSelectedFile] = useState<string>("");
  const [algorithm, setAlgorithm] = useState<string>("1");
  const [consoleOutput, setConsoleOutput] = useState<string[]>([
    "MetaWipe v1.0 - Ready",
    "Waiting for file selection...",
  ]);
  const [isWiping, setIsWiping] = useState(false);
  const [progress, setProgress] = useState(0);
  const fileInputRef = useRef<HTMLInputElement>(null);

  const handleFileSelect = (e: React.ChangeEvent<HTMLInputElement>) => {
    const file = e.target.files?.[0];
    if (file) {
      setSelectedFile(file.name);
      addConsoleOutput(`File selected: ${file.name}`);
      toast.success(`File selected: ${file.name}`);
    }
  };

  const addConsoleOutput = (message: string) => {
    const timestamp = new Date().toLocaleTimeString();
    setConsoleOutput((prev) => [...prev, `[${timestamp}] ${message}`]);
  };

  const getAlgorithmName = (algo: string) => {
    const algorithms: Record<string, string> = {
      "1": "Zero Fill",
      "2": "Random Fill",
      "3": "DoD 3-Pass",
    };
    return algorithms[algo];
  };

  const simulateWipe = () => {
    if (!selectedFile) {
      toast.error("Please select a file first");
      return;
    }

    setIsWiping(true);
    setProgress(0);
    const algoName = getAlgorithmName(algorithm);
    
    addConsoleOutput(`Starting wipe operation...`);
    addConsoleOutput(`Algorithm: ${algoName}`);
    addConsoleOutput(`Target: ${selectedFile}`);

    // Simulate wiping progress
    let currentProgress = 0;
    const interval = setInterval(() => {
      currentProgress += 10;
      setProgress(currentProgress);

      if (currentProgress === 30) {
        addConsoleOutput(`Initializing ${algoName}...`);
      } else if (currentProgress === 60) {
        addConsoleOutput(`Overwriting data blocks...`);
      } else if (currentProgress === 90) {
        addConsoleOutput(`Verifying deletion...`);
      } else if (currentProgress >= 100) {
        clearInterval(interval);
        setIsWiping(false);
        addConsoleOutput(`✓ File wiped successfully!`);
        addConsoleOutput(`Metadata cleared and logged.`);
        toast.success("File wiped successfully!");
        setSelectedFile("");
        setProgress(0);
      }
    }, 500);
  };

  return (
    <Layout>
      <div className="p-8 space-y-6">
        <div>
          <h1 className="text-3xl font-bold text-foreground mb-2">
            MetaWipe Dashboard
          </h1>
          <p className="text-muted-foreground">
            Select files and choose a wiping algorithm to securely delete data
          </p>
        </div>

        <div className="grid gap-6 md:grid-cols-2">
          {/* File Selection Card */}
          <Card className="border-border">
            <CardHeader>
              <CardTitle className="flex items-center gap-2">
                <Upload className="w-5 h-5 text-primary" />
                File Selection
              </CardTitle>
              <CardDescription>Choose a file or folder to wipe</CardDescription>
            </CardHeader>
            <CardContent className="space-y-4">
              <input
                ref={fileInputRef}
                type="file"
                onChange={handleFileSelect}
                className="hidden"
                id="fileInput"
              />
              <Button
                variant="secondary"
                className="w-full"
                onClick={() => fileInputRef.current?.click()}
              >
                <Upload className="w-4 h-4 mr-2" />
                Select File
              </Button>
              
              {selectedFile && (
                <div className="p-3 rounded-lg bg-secondary border border-border">
                  <p className="text-sm font-medium text-foreground truncate">
                    {selectedFile}
                  </p>
                </div>
              )}
            </CardContent>
          </Card>

          {/* Algorithm Selection Card */}
          <Card className="border-border">
            <CardHeader>
              <CardTitle className="flex items-center gap-2">
                <Trash2 className="w-5 h-5 text-primary" />
                Wiping Algorithm
              </CardTitle>
              <CardDescription>Select the security level</CardDescription>
            </CardHeader>
            <CardContent className="space-y-4">
              <Select value={algorithm} onValueChange={setAlgorithm}>
                <SelectTrigger id="algoSelect" className="bg-secondary border-border">
                  <SelectValue />
                </SelectTrigger>
                <SelectContent>
                  <SelectItem value="1">Zero Fill (1-Pass)</SelectItem>
                  <SelectItem value="2">Random Fill (1-Pass)</SelectItem>
                  <SelectItem value="3">DoD 3-Pass (Military Grade)</SelectItem>
                </SelectContent>
              </Select>

              <Button
                id="wipeBtn"
                onClick={simulateWipe}
                disabled={!selectedFile || isWiping}
                className="w-full"
              >
                {isWiping ? "Wiping..." : "Wipe File"}
              </Button>

              {isWiping && (
                <div className="space-y-2">
                  <Progress value={progress} className="w-full" />
                  <p className="text-xs text-center text-muted-foreground">
                    {progress}% Complete
                  </p>
                </div>
              )}
            </CardContent>
          </Card>
        </div>

        {/* Console Output */}
        <Card className="border-border">
          <CardHeader>
            <CardTitle className="flex items-center gap-2">
              <AlertCircle className="w-5 h-5 text-primary" />
              Console Output
            </CardTitle>
            <CardDescription>Real-time operation logs</CardDescription>
          </CardHeader>
          <CardContent>
            <div
              id="output"
              className="bg-console rounded-lg p-4 h-64 overflow-y-auto font-mono text-sm border border-border"
            >
              {consoleOutput.map((line, index) => (
                <div
                  key={index}
                  className={`py-1 ${
                    line.includes("✓")
                      ? "text-success"
                      : line.includes("Error")
                      ? "text-destructive"
                      : "text-foreground"
                  }`}
                >
                  {line}
                </div>
              ))}
            </div>
          </CardContent>
        </Card>
      </div>
    </Layout>
  );
};

export default Dashboard;
