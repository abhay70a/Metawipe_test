#include <stdio.h>
#include "sqlite3.h"

int main() {
    sqlite3 *db;
    int rc;

    // Open (or create) the database
    rc = sqlite3_open("metawipe.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    } else {
        printf("Opened database successfully\n");
    }

    // Example: execute a simple SQL statement
    char *sql = "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT);";
    char *errMsg = 0;

    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("Table created successfully\n");
    }

    sqlite3_close(db);
    return 0;
}  