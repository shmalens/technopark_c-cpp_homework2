#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>

#include "directory_processing.h"

files_t *create_files_set(size_t init_capacity) {
    files_t *new_files_set = (files_t *) malloc(sizeof(files_t));
    if (new_files_set == NULL) {
        return NULL;
    }

    new_files_set->amount = 0;
    new_files_set->capacity = init_capacity;
    new_files_set->file_names = (char **) malloc(sizeof(char *) * new_files_set->capacity);
    if (new_files_set->file_names == NULL) {
        free(new_files_set);
        return NULL;
    }

    return new_files_set;
}

int delete_files_set(files_t *src) {
    if (src == NULL) {
        return -1;
    }

    for (int i = 0; i < src->amount; ++i) {
        free(src->file_names[i]);
    }
    free(src->file_names);
    free(src);
    return 0;
}

static int resize_files_set(files_t *set) {
    if (set == NULL) {
        return -1;
    }

    char **tmp = (char **) realloc(set->file_names, sizeof(char *) * set->capacity * 2);
    if (tmp == NULL) {
        return -1;
    }
    set->file_names = tmp;
    set->capacity *= 2;
    return 0;
}

int add_files_in_set(files_t *src, const char *new_file) {
    if (src == NULL) {
        return ERROR_EMPTY_FILE_SET;
    }

    if (new_file == NULL) {
        return ERROR_EMPTY_FILE_NAME_PTR;
    }

    char *tmp;
    int err = custom_strcpy(&tmp, new_file);
    if (err != 0) {
        return err;
    }

    if (src->amount == src->capacity) {
        if (resize_files_set(src) != 0) {
            return ERROR_REALLOC_FILES_SET;
        }
    }

    src->file_names[src->amount] = tmp;
    ++src->amount;

    return 0;
}

files_t *read_files(char *path_to_directory) {
    DIR *directory = opendir(path_to_directory);
    if (directory == NULL) {
        return NULL;
    }

    files_t *new_files = create_files_set(DIRECTORY_SIZE);

    struct dirent *directory_element;
    while ((directory_element = readdir(directory)) != NULL) {
        if (directory_element->d_type == DT_REG) {
            char *tmp;
            custom_strcat(&tmp, path_to_directory, directory_element->d_name);

            if(add_files_in_set(new_files, tmp) != 0) {
                delete_files_set(new_files);
                return NULL;
            }

            free(tmp);
        }
    }

    closedir(directory);
    return new_files;
}
