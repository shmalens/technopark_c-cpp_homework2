#ifndef DIRECTORY_PROCESSING_H
#define DIRECTORY_PROCESSING_H

#include <dirent.h>

#include "misc.h"

#define DIRECTORY_SIZE 500

typedef struct {
    char **file_names;
    size_t amount;
    size_t capacity;
} files_t;

enum error_adding_file {
    ERROR_EMPTY_FILE_SET = 0,
    ERROR_EMPTY_FILE_NAME_PTR,
    ERROR_REALLOC_FILES_SET
};

files_t *create_files_set();
int delete_files_set(files_t *src);
int add_files_in_set(files_t *src, const char *new_file);

files_t *read_files(const char *path_to_directory);

#endif //DIRECTORY_PROCESSING_H
