#ifndef HOMEWORK2_MISC_H
#define HOMEWORK2_MISC_H

#include <stdio.h>

#define TOKENIZER_PATTERN "%500[^ \n    .,!?()\":;]"

enum error_string_processing {
    ERROR_EMPTY_SRC_PTR = 1,
    ERROR_EMPTY_DEST_PTR,
    ERROR_BAD_MEM_ALLOCATION
};

int custom_strcpy(char **dest, const char *src);
int custom_strcat(char **dest, const char *src1, const char *src2);
int naive_tokenizer(FILE *fd, char *str);

#endif //HOMEWORK2_MISC_H
