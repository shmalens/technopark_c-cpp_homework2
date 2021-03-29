#ifndef CONTROL_UNITS_H
#define CONTROL_UNITS_H

#include "files_processing.h"
#include "directory_processing.h"

#define TOP 5

enum init_block_errors {
    EMPTY_PATH = 1,
    ERR_CREATING_FILES_SET,
    ERR_CREATING_SET,
    ERR_CREATING_BAG,
    ERR_GETTING_TOP
};

enum data_processing_errors {
    EMPTY_FILES = 1,
    EMPTY_SET,
    EMPTY_BAG,
    ERR_TF_IDF,
};

int init_block(const char *dir_path, files_t **files, hash_table_t **set, bag_of_words_t **bag);
int data_processing(files_t *files, hash_table_t *set, bag_of_words_t *bag);
void free_resources(files_t *files, hash_table_t *set, bag_of_words_t *bag);

#endif //CONTROL_UNITS_H
