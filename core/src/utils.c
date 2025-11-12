// utils.c
#include <stdio.h>
#include "utils.h"

long get_file_size(FILE *fp) {
    long current = ftell(fp);
    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);
    fseek(fp, current, SEEK_SET);
    return size;
}
