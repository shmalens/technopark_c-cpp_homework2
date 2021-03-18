#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "files.h"

files_t *create_files_collection() {
    files_t *new_collection = (files_t *) malloc(sizeof(files_t));
    if (new_collection == NULL) {
        return NULL;
    }

    new_collection->amount = 0;
    new_collection->capacity = 1;
    new_collection->file_nodes = (file_node_t *) malloc(sizeof(file_node_t) * new_collection->capacity);
    if (new_collection->file_nodes == NULL) {
        free(new_collection);
        return NULL;
    }
    return new_collection;
}

int delete_files_collection(files_t *deleting_collection) {
    if (deleting_collection == NULL) {
        return -1;
    }

    if (deleting_collection->file_nodes != NULL) {
        for (int i = 0; i < deleting_collection->amount; ++i) {
            free(deleting_collection->file_nodes[i].file_name);
        }
        free(deleting_collection->file_nodes);
    }
    free(deleting_collection);

    return 0;
}

static int resize_files_buffer(files_t *files) {
    if (files == NULL) {
        return -1;
    }

    file_node_t *tmp = (file_node_t *) realloc(files->file_nodes, sizeof(file_node_t) * files->capacity * 2);
    if (tmp == NULL) {
        return -1;
    }

    files->capacity *= 2;
    files->file_nodes = tmp;

    return 0;
}

int add_file(files_t *files, char *file_name) {
    if (files == NULL) {
        return EMPTY_FILES_PTR;
    }

    if (file_name == NULL) {
        return EMPTY_FILE_NAME_PTR;
    }

    size_t name_len = strlen(file_name) + 1;
    if (name_len == 1) {
        return WRONG_NAME_LEN;
    }

    char *tmp_name = (char *) malloc(sizeof(char) * name_len);
    if (tmp_name == NULL) {
        return ERROR_ALLOCATING_NAME_MEM;
    }

    if (files->amount == files->capacity) {
        if(resize_files_buffer(files) != 0) {
            free(tmp_name);
            return RESIZING_BUFFER_ERROR;
        }
    }

    strncpy(tmp_name, file_name, name_len);
    files->file_nodes[files->amount].file_name = tmp_name;
    files->file_nodes[files->amount].len = name_len;
    ++files->amount;
    return 0;
}

int show_files_names(files_t * files) {
    if (files == NULL) {
        return -1;
    }

    for (int i = 0; i < files->amount; ++i) {
        printf("File name: %s\n", files->file_nodes[i].file_name);
    }

    return 0;
}

