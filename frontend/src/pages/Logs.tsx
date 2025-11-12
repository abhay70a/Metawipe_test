import Layout from "@/components/Layout";
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from "@/components/ui/card";
import { Table, TableBody, TableCell, TableHead, TableHeader, TableRow } from "@/components/ui/table";
import { Badge } from "@/components/ui/badge";
import { FileText, CheckCircle2 } from "lucide-react";

// Dummy data - will be replaced with MySQL data later
const dummyLogs = [
  {
    id: 1,
    timestamp: "2025-01-15 14:32:10",
    filename: "confidential_report.pdf",
    algorithm: "DoD 3-Pass",
    status: "Success",
    user: "admin",
  },
  {
    id: 2,
    timestamp: "2025-01-15 13:15:42",
    filename: "old_database_backup.sql",
    algorithm: "Random Fill",
    status: "Success",
    user: "admin",
  },
  {
    id: 3,
    timestamp: "2025-01-14 16:22:18",
    filename: "sensitive_data.xlsx",
    algorithm: "Zero Fill",
    status: "Success",
    user: "admin",
  },
  {
    id: 4,
    timestamp: "2025-01-14 11:08:55",
    filename: "personal_photos.zip",
    algorithm: "DoD 3-Pass",
    status: "Success",
    user: "admin",
  },
  {
    id: 5,
    timestamp: "2025-01-13 09:45:30",
    filename: "project_drafts.docx",
    algorithm: "Random Fill",
    status: "Success",
    user: "admin",
  },
];

const Logs = () => {
  return (
    <Layout>
      <div className="p-8 space-y-6">
        <div>
          <h1 className="text-3xl font-bold text-foreground mb-2">
            Operation Logs
          </h1>
          <p className="text-muted-foreground">
            View history of all file wiping operations
          </p>
        </div>

        <Card className="border-border">
          <CardHeader>
            <CardTitle className="flex items-center gap-2">
              <FileText className="w-5 h-5 text-primary" />
              Deletion History
            </CardTitle>
            <CardDescription>
              Complete audit trail of secure file deletions
            </CardDescription>
          </CardHeader>
          <CardContent>
            <div className="rounded-lg border border-border overflow-hidden">
              <Table>
                <TableHeader>
                  <TableRow className="bg-muted/50">
                    <TableHead className="text-foreground font-semibold">ID</TableHead>
                    <TableHead className="text-foreground font-semibold">Timestamp</TableHead>
                    <TableHead className="text-foreground font-semibold">Filename</TableHead>
                    <TableHead className="text-foreground font-semibold">Algorithm</TableHead>
                    <TableHead className="text-foreground font-semibold">User</TableHead>
                    <TableHead className="text-foreground font-semibold">Status</TableHead>
                  </TableRow>
                </TableHeader>
                <TableBody>
                  {dummyLogs.map((log) => (
                    <TableRow key={log.id} className="hover:bg-muted/30">
                      <TableCell className="font-mono text-muted-foreground">
                        #{log.id}
                      </TableCell>
                      <TableCell className="font-mono text-sm">
                        {log.timestamp}
                      </TableCell>
                      <TableCell className="font-medium">
                        {log.filename}
                      </TableCell>
                      <TableCell>
                        <Badge variant="secondary" className="font-mono text-xs">
                          {log.algorithm}
                        </Badge>
                      </TableCell>
                      <TableCell className="text-muted-foreground">
                        {log.user}
                      </TableCell>
                      <TableCell>
                        <div className="flex items-center gap-2">
                          <CheckCircle2 className="w-4 h-4 text-success" />
                          <span className="text-success font-medium">
                            {log.status}
                          </span>
                        </div>
                      </TableCell>
                    </TableRow>
                  ))}
                </TableBody>
              </Table>
            </div>
          </CardContent>
        </Card>
      </div>
    </Layout>
  );
};

export default Logs;
