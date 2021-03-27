#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "misc.h"

int custom_strcpy(char **dest, const char *src) {
    if (src == NULL) {
        return ERROR_EMPTY_SRC_PTR;
    }

    if (dest == NULL) {
        return ERROR_EMPTY_DEST_PTR;
    }

    size_t src_size = strlen(src) + 1;
    *dest = (char *) malloc(sizeof(char) * src_size);
    if (*dest == NULL) {
        return ERROR_BAD_MEM_ALLOCATION;
    }

    strncpy(*dest, src, src_size);
    return 0;
}

int custom_strcat(char **dest, const char *src1, const char *src2) {
    if (src1 == NULL) {
        return ERROR_EMPTY_SRC_PTR;
    }

    if (src2 == NULL) {
        return ERROR_EMPTY_SRC_PTR;
    }

    if (dest == NULL) {
        return ERROR_EMPTY_DEST_PTR;
    }

    size_t src1_size = strlen(src1) + 1;
    size_t src2_size = strlen(src2) + 1;
    *dest = (char *) malloc(sizeof(char) * (src1_size + src2_size));
    if (*dest == NULL) {
        return ERROR_BAD_MEM_ALLOCATION;
    }
    memset((void *)*dest, 0, src1_size + src2_size);
    *dest = strcat(*dest, src1);
    *dest = strcat(*dest, src2);

    return 0;
}

int naive_tokenizer(FILE *fd, char *str) {
    if (fd == NULL) {
        return 1;
    }

    int readed = fscanf(fd, TOKENIZER_PATTERN, str);
    if (readed == 0) {
        fgetc(fd);
        return 1;
    }

    if (readed == EOF) {
        return EOF;
    }

    return 0;
}
