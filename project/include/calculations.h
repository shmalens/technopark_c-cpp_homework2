#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include "files_processing.h"

enum calculations_errors {
    EMPTY_PTR = 1,
    ERROR_MEM_ALLOC,
    WRONG_VALUE
};

int tf_idf_transform(bag_of_words_t *bag);
int get_top(const bag_of_words_t *bag, size_t **top_table, unsigned int positions);

#endif //CALCULATIONS_H
