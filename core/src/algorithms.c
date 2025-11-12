#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>     /* for fsync */
#include <sys/types.h>  /* for off_t */
#include <sys/stat.h>
#include <fcntl.h>

#include "../include/wipe.h"

/* Internal helper: write a buffer repeatedly to the file and handle partial writes.
   Using fwrite is okay for files; we will fflush and fsync to ensure data is pushed to device. */
static int write_buffer_repeat(FILE *fp, unsigned char *buf, size_t buf_len, long remaining) {
    /* write in chunks until remaining bytes covered */
    while (remaining > 0) {
        size_t to_write = (remaining < (long)buf_len) ? (size_t)remaining : buf_len;
        size_t written = fwrite(buf, 1, to_write, fp);
        if (written != to_write) {
            perror("fwrite");
            return -1;
        }
        remaining -= written;
    }
    return 0;
}

/* Ensure data reaches the physical device: fflush + fsync */
static int flush_and_sync(FILE *fp) {
    if (fflush(fp) != 0) {
        perror("fflush");
        return -1;
    }
    int fd = fileno(fp);
    if (fd == -1) {
        perror("fileno");
        return -1;
    }
    if (fsync(fd) != 0) {
        perror("fsync");
        return -1;
    }
    return 0;
}

/* Show progress to console */
static void show_progress(long done, long total) {
    double pct = (total == 0) ? 100.0 : ((double)done / (double)total) * 100.0;
    if (pct > 100.0) pct = 100.0;
    printf("\rProgress: %.2f%%", pct);
    fflush(stdout);
}

/* Overwrite with zeros for 'passes' passes */
void overwrite_zero(FILE *fp, long size, int passes) {
    unsigned char *buf = malloc(BUFFER_SIZE);
    if (!buf) { perror("malloc"); return; }
    memset(buf, 0x00, BUFFER_SIZE);

    for (int p = 1; p <= passes; ++p) {
        rewind(fp);                         /* go to start */
        long remaining = size;
        long done = 0;
        printf("\n🔄 ZERO-Fill pass %d/%d\n", p, passes);

        while (remaining > 0) {
            size_t chunk = (remaining < BUFFER_SIZE) ? remaining : BUFFER_SIZE;
            if (fwrite(buf, 1, chunk, fp) != chunk) { perror("fwrite"); break; }
            remaining -= chunk;
            done += chunk;
            show_progress(done, size);
        }

        if (flush_and_sync(fp) != 0) { printf("\n⚠️  flush failed\n"); }
        printf("\n✅ ZERO-Fill pass %d done\n", p);
    }

    free(buf);
}

/* Overwrite with random bytes for 'passes' passes */
void overwrite_random(FILE *fp, long size, int passes) {
    unsigned char *buf = malloc(BUFFER_SIZE);
    if (!buf) { perror("malloc"); return; }

    /* seed once */
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    srand((unsigned int)(ts.tv_nsec ^ ts.tv_sec));

    for (int p = 1; p <= passes; ++p) {
        rewind(fp);
        long remaining = size;
        long done = 0;
        printf("\n🔄 RANDOM-Fill pass %d/%d\n", p, passes);

        while (remaining > 0) {
            size_t chunk = (remaining < BUFFER_SIZE) ? remaining : BUFFER_SIZE;
            /* fill buffer with rand() */
            for (size_t i = 0; i < chunk; ++i) {
                buf[i] = (unsigned char)(rand() & 0xFF);
            }
            if (fwrite(buf, 1, chunk, fp) != chunk) { perror("fwrite"); break; }
            remaining -= chunk;
            done += chunk;
            show_progress(done, size);
        }

        if (flush_and_sync(fp) != 0) { printf("\n⚠️  flush failed\n"); }
        printf("\n✅ RANDOM-Fill pass %d done\n", p);
    }

    free(buf);
}

/* DoD 3-pass algorithm:
   1st pass: 0xFF
   2nd pass: 0x00
   3rd pass: random data
   Note: passes parameter not used — DoD is fixed 3 passes per spec here. */
void overwrite_dod3(FILE *fp, long size) {
    unsigned char *buf = malloc(BUFFER_SIZE);
    if (!buf) { perror("malloc"); return; }

    /* PASS 1: 0xFF */
    memset(buf, 0xFF, BUFFER_SIZE);
    rewind(fp);
    long remaining = size;
    long done = 0;
    printf("\n🔄 DoD Pass 1/3 (0xFF)\n");
    while (remaining > 0) {
        size_t chunk = (remaining < BUFFER_SIZE) ? remaining : BUFFER_SIZE;
        if (fwrite(buf, 1, chunk, fp) != chunk) { perror("fwrite"); break; }
        remaining -= chunk;
        done += chunk;
        show_progress(done, size);
    }
    flush_and_sync(fp);
    printf("\n✅ DoD Pass 1 done\n");

    /* PASS 2: 0x00 */
    memset(buf, 0x00, BUFFER_SIZE);
    rewind(fp);
    remaining = size;
    done = 0;
    printf("\n🔄 DoD Pass 2/3 (0x00)\n");
    while (remaining > 0) {
        size_t chunk = (remaining < BUFFER_SIZE) ? remaining : BUFFER_SIZE;
        if (fwrite(buf, 1, chunk, fp) != chunk) { perror("fwrite"); break; }
        remaining -= chunk;
        done += chunk;
        show_progress(done, size);
    }
    flush_and_sync(fp);
    printf("\n✅ DoD Pass 2 done\n");

    /* PASS 3: random */
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    srand((unsigned int)(ts.tv_nsec ^ ts.tv_sec));
    rewind(fp);
    remaining = size;
    done = 0;
    printf("\n🔄 DoD Pass 3/3 (random)\n");
    while (remaining > 0) {
        size_t chunk = (remaining < BUFFER_SIZE) ? remaining : BUFFER_SIZE;
        for (size_t i = 0; i < chunk; ++i) {
            buf[i] = (unsigned char)(rand() & 0xFF);
        }
        if (fwrite(buf, 1, chunk, fp) != chunk) { perror("fwrite"); break; }
        remaining -= chunk;
        done += chunk;
        show_progress(done, size);
    }
    flush_and_sync(fp);
    printf("\n✅ DoD Pass 3 done\n");

    free(buf);
}
