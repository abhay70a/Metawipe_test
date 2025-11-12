#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <openssl/sha.h>
#include <mysql/mysql.h>
#include "../include/wipe.h"
#include "../include/db.h"  // include MySQL DB functions

#define BUFFER_SIZE 4096

/* Convert hash bytes to hex string */
static void hash_to_string(const unsigned char *hash, char *output) {
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        sprintf(output + (i * 2), "%02x", hash[i]);
    output[SHA256_DIGEST_LENGTH * 2] = '\0';
}

/* Compute SHA256 hash of a file */
void compute_sha256(const char *path, unsigned char *out_hash) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        perror("open");
        memset(out_hash, 0, SHA256_DIGEST_LENGTH);
        return;
    }

    SHA256_CTX ctx;
    SHA256_Init(&ctx);

    unsigned char buf[BUFFER_SIZE];
    size_t bytes;
    while ((bytes = fread(buf, 1, BUFFER_SIZE, file)) != 0)
        SHA256_Update(&ctx, buf, bytes);

    SHA256_Final(out_hash, &ctx);
    fclose(file);
}

/* Get current timestamp string */
static void get_timestamp(char *buf, size_t len) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buf, len, "%Y-%m-%d %H:%M:%S", tm_info);
}

/* Log wipe event to MySQL database */
static void log_to_db(const char *path, long size, wipe_method_t method, int success) {
    MYSQL *conn = db_connect();
    if (!conn) {
        fprintf(stderr, "DB connection failed.\n");
        return;
    }

    char method_str[16];
    switch (method) {
        case WIPE_ZERO: strcpy(method_str, "zero"); break;
        case WIPE_RANDOM: strcpy(method_str, "random"); break;
        case WIPE_DOD3: strcpy(method_str, "dod3"); break;
        default: strcpy(method_str, "unknown");
    }

    char query[1024];
    snprintf(query, sizeof(query),
        "INSERT INTO logs (user_id, action, log_time) "
        "VALUES (1, 'Wiped file %s using %s method (%.2f MB) [%s]', NOW());",
        path, method_str, size / (1024.0 * 1024.0), success ? "success" : "failed");

    if (mysql_query(conn, query)) {
        fprintf(stderr, "MySQL error: %s\n", mysql_error(conn));
    }

    db_close(conn);
}

/* Log wipe event to text, JSON, and DB */
void log_wipe_event(const char *path, long size, wipe_method_t method,
                    const unsigned char *before, const unsigned char *after, int success) {

    FILE *logf = fopen("logs/wipe_log.txt", "a");
    FILE *jsonf = fopen("logs/wipe_result.json", "a");
    if (!logf || !jsonf) {
        perror("log");
        return;
    }

    char ts[64], method_str[32], before_str[65], after_str[65];
    get_timestamp(ts, sizeof(ts));

    switch (method) {
        case WIPE_ZERO: strcpy(method_str, "zero"); break;
        case WIPE_RANDOM: strcpy(method_str, "random"); break;
        case WIPE_DOD3: strcpy(method_str, "dod3"); break;
        default: strcpy(method_str, "unknown");
    }

    hash_to_string(before, before_str);
    hash_to_string(after, after_str);

    /* Write to text log */
    fprintf(logf,
        "[%s] File: %s | Size: %.2f MB | Method: %s | Status: %s\n"
        "Before Hash: %s\nAfter  Hash: %s\n\n",
        ts, path, size / (1024.0 * 1024.0), method_str,
        success ? "SUCCESS" : "FAILED", before_str, after_str);

    /* Write to JSON log */
    fprintf(jsonf,
        "{\n"
        "  \"timestamp\": \"%s\",\n"
        "  \"file\": \"%s\",\n"
        "  \"size_MB\": %.2f,\n"
        "  \"method\": \"%s\",\n"
        "  \"status\": \"%s\",\n"
        "  \"before_hash\": \"%s\",\n"
        "  \"after_hash\": \"%s\"\n"
        "}\n",
        ts, path, size / (1024.0 * 1024.0), method_str,
        success ? "success" : "failed", before_str, after_str);

    fclose(logf);
    fclose(jsonf);

    /* Write to MySQL DB */
    log_to_db(path, size, method, success);
}
