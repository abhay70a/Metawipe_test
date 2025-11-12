#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/wipe.h"

static void print_usage(const char *prog) {
    printf("Usage: %s <file_or_device> <passes> <method>\n", prog);
    printf("  methods: zero | random | dod3\n");
    printf("Example: %s tests/test_files/test.img 2 random\n", prog);
}

wipe_method_t parse_method(const char *s) {
    if (strcmp(s, "zero") == 0) return WIPE_ZERO;
    if (strcmp(s, "random") == 0) return WIPE_RANDOM;
    if (strcmp(s, "dod3") == 0) return WIPE_DOD3;
    return -1;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        print_usage(argv[0]);
        return 1;
    }

    const char *path = argv[1];
    int passes = atoi(argv[2]);
    if (passes <= 0) passes = 1;

    wipe_method_t method = parse_method(argv[3]);
    if (method == (wipe_method_t)-1) {
        printf("Invalid method\n");
        print_usage(argv[0]);
        return 1;
    }

    int rc = wipe_file(path, passes, method);
    return rc == 0 ? 0 : 1;
}
