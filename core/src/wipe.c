#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/wipe.h"

/**
 * Get the size of a file (in bytes)
 */
long get_file_size(FILE *fp) {
    if (!fp) return -1;
    if (fseek(fp, 0, SEEK_END) != 0) return -1;
    long size = ftell(fp);
    rewind(fp);
    return size;
}

/**
 * Perform secure wiping of a file using the selected method.
 * Logs the operation with before/after SHA256 hashes.
 */
int wipe_file(const char *path, int passes, wipe_method_t method) {
    if (!path) {
        fprintf(stderr, "❌ Invalid file path.\n");
        return -1;
    }

    unsigned char before_hash[32], after_hash[32];

    // Compute initial hash
    compute_sha256(path, before_hash);

    FILE *fp = fopen(path, "r+b");
    if (!fp) {
        perror("❌ Error opening file");
        return -1;
    }

    long size = get_file_size(fp);
    if (size <= 0) {
        fprintf(stderr, "⚠️ Invalid or empty file.\n");
        fclose(fp);
        return -1;
    }

    printf("🧾 Target: %s (%.2f MB)\n", path, size / (1024.0 * 1024.0));

    int result = 0;
    switch (method) {
        case WIPE_ZERO:
            overwrite_zero(fp, size, passes);
            break;
        case WIPE_RANDOM:
            overwrite_random(fp, size, passes);
            break;
        case WIPE_DOD3:
            overwrite_dod3(fp, size);
            break;
        default:
            fprintf(stderr, "❌ Unknown wipe method.\n");
            result = -1;
    }

    fclose(fp);

    // Compute final hash
    compute_sha256(path, after_hash);


    // Log operation result
    log_wipe_event(path, size, method, before_hash, after_hash, (result == 0));

    if (result == 0)
        printf("✅ Wipe complete and logged.\n");
    else
        printf("⚠️ Wipe failed.\n");

    return result;
}