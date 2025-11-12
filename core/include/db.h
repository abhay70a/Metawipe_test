#ifndef DB_H
#define DB_H

#include <mysql/mysql.h>

/* Connect to MySQL database */
MYSQL *db_connect();

/* Close MySQL connection */
void db_close(MYSQL *conn);

/* Example: log wipe event into database */
void db_log_wipe(MYSQL *conn, const char *file, double size_mb,
                 const char *method, const char *status);

#endif
