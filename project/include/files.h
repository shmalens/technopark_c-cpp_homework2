#ifndef FILES_H
#define FILES_H

#include <dirent.h>

enum add_file_errors {
    EMPTY_FILES_PTR = 1,
    EMPTY_FILE_NAME_PTR,
    RESIZING_BUFFER_ERROR,
    WRONG_NAME_LEN,
    ERROR_ALLOCATING_NAME_MEM
};

typedef struct {
    char *file_name;
    size_t len;
} file_node_t;

typedef struct {
    file_node_t *file_nodes;
    size_t amount;
    size_t capacity;
} files_t;

files_t *create_files_collection();
int delete_files_collection(files_t *deleting_collection);

int add_file(files_t *files, char *file_name);
int show_files_names(files_t * files);

#endif //FILES_H
