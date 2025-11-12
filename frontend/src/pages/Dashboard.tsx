import { useState } from "react";
import { Button } from "@/components/ui/button";
import { Input } from "@/components/ui/input";
import { Select, SelectItem, SelectTrigger, SelectValue, SelectContent } from "@/components/ui/select";
import { toast } from "sonner";

const Dashboard = () => {
  const [filePath, setFilePath] = useState("");
  const [method, setMethod] = useState("1");
  const [loading, setLoading] = useState(false);

  const handleWipe = async () => {
    if (!filePath) {
      toast.error("Please select a file first!");
      return;
    }

    setLoading(true);
    try {
      const result = await window.MetaWipeAPI.wipeFile(filePath, method);
      console.log("Wipe Output:", result);

      if (result.toLowerCase().includes("success")) {
        toast.success("File wiped successfully!");
      } else {
        toast.info(result);
      }
    } catch (err) {
      console.error("Wipe Error:", err);
      toast.error("Failed to wipe file!");
    }
    setLoading(false);
  };

  return (
    <div className="p-8">
      <h1 className="text-2xl font-bold mb-6">🧹 Secure File Wipe</h1>

      <div className="space-y-4 max-w-lg">
        <Input
          type="file"
          onChange={(e) => setFilePath(e.target.files?.[0]?.path || "")}
        />

        <Select value={method} onValueChange={setMethod}>
          <SelectTrigger>
            <SelectValue placeholder="Select wiping method" />
          </SelectTrigger>
          <SelectContent>
            <SelectItem value="1">Zero Fill (Fast)</SelectItem>
            <SelectItem value="2">Random Fill</SelectItem>
            <SelectItem value="3">DoD 3-Pass</SelectItem>
          </SelectContent>
        </Select>

        <Button onClick={handleWipe} disabled={loading} className="w-full">
          {loading ? "Wiping..." : "Wipe File"}
        </Button>
      </div>
    </div>
  );
};

export default Dashboard;
