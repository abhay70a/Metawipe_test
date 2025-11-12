#include <stdio.h>
#include "../include/db.h"

int main() {
    MYSQL *conn = db_connect();
    if (conn) {
        printf("✅ MySQL connection successful!\n");
        db_close(conn);
    } else {
        printf("❌ Failed to connect to MySQL.\n");
    }
    return 0;
}
