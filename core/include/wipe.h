#ifndef WIPE_H
#define WIPE_H

#include <stdio.h>

#define BUFFER_SIZE 4096

typedef enum {
    WIPE_ZERO,
    WIPE_RANDOM,
    WIPE_DOD3
} wipe_method_t;

// Core wipe
int wipe_file(const char *path, int passes, wipe_method_t method);

// Utilities
long get_file_size(FILE *fp);
void overwrite_zero(FILE *fp, long size, int passes);
void overwrite_random(FILE *fp, long size, int passes);
void overwrite_dod3(FILE *fp, long size);

// New additions for Phase 7
void compute_sha256(const char *path, unsigned char *out_hash);
void log_wipe_event(const char *path, long size, wipe_method_t method,
                    const unsigned char *before, const unsigned char *after, int success);

#endif
