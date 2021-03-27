#include <stdio.h>
#include <stdlib.h>

#include "directory_processing.h"
#include "files_processing.h"

#define PATH "../data/"

int main() {
    files_t *files = read_files(PATH);
    if (files == NULL) {
        return -1;
    }

    set_t *set = create_set_from_files(files);
    if (set == NULL) {
        delete_files_set(files);
        return -1;
    }

    bag_of_words_t *bag = create_bag(files, set);
    if (bag == NULL) {
        delete_files_set(files);
        delete_set(set);
        return -1;
    }


    for (size_t i = 0; i < bag->rows; ++i) {
        for (int j = 0; j < bag->cols; ++j) {
            printf("%5ld ", bag->matrix[i][j]);
        }
        printf("\n");
    }

    delete_bag(bag);
    delete_files_set(files);
    delete_set(set);

//    bag_of_words_t
    return 0;
}