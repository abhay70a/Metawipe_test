#include <stdio.h>
#include <mysql/mysql.h>
#include "../core/include/db.h"

#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "your_password"
#define DB_NAME "project_db"

MYSQL *db_connect() {
    MYSQL *conn = mysql_init(NULL);
    if (!conn) {
        fprintf(stderr, "mysql_init() failed\n");
        return NULL;
    }
    if (!mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, NULL, 0)) {
        fprintf(stderr, "MySQL connection error: %s\n", mysql_error(conn));
        mysql_close(conn);
        return NULL;
    }
    return conn;
}

void db_close(MYSQL *conn) {
    if (conn)
        mysql_close(conn);
}

void db_log_wipe(MYSQL *conn, const char *file, double size_mb,
                 const char *method, const char *status) {
    if (!conn) return;

    char query[1024];
    snprintf(query, sizeof(query),
             "INSERT INTO logs (user_id, action) "
             "VALUES (1, 'Wiped file %s (%.2f MB) using %s - %s');",
             file, size_mb, method, status);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "MySQL query failed: %s\n", mysql_error(conn));
    }
}
