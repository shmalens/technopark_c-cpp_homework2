#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include "files_processing.h"

enum calculations_errors {
    EMPTY_PTR = 1,
    ERROR_MEM_ALLOC,
    WRONG_VALUE
};

typedef struct {
    double val;
    size_t index;
} index_val_t;

int tf_idf_transform(bag_of_words_t *bag);

index_val_t **get_top(const bag_of_words_t *bag);
int delete_top(const bag_of_words_t *bag, index_val_t **top_table);
int show_top(const set_t *set, const files_t *files, index_val_t **table, size_t rows, size_t positions);

#endif //CALCULATIONS_H
