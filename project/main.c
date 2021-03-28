#include <stdio.h>
#include <stdlib.h>

#include "directory_processing.h"
#include "files_processing.h"
#include <calculations.h>
#include <string.h>
#include "processing.h"

#define PATH "../data/"

int main() {
    files_t *files;

    /*files_t *files = read_files(PATH);
    if (files == NULL) {
        return -1;
    }

    set_t *set = create_set_from_files(files);
    if (set == NULL) {
        delete_files_set(files);
        return -1;
    }

    print_traverse(set->root);

    bag_of_words_t *bag = create_bag(files, set);
    if (bag == NULL) {
        delete_files_set(files);
        delete_set(set);
        return -1;
    }


    for (size_t i = 0; i < bag->rows; ++i) {
        for (int j = 0; j < bag->cols; ++j) {
            printf("%5.3f ", bag->matrix[i][j]);
        }
        printf("\n");
    }

    printf("\n----------------------\n\n");

    tf_idf_transform(bag);

    for (size_t i = 0; i < bag->rows; ++i) {
        for (int j = 0; j < bag->cols; ++j) {
            printf("%.3f ", bag->matrix[i][j]);
        }
        printf("\n");
    }

    size_t **top_tabe = (size_t **) malloc(sizeof(size_t *) * bag->rows);
    if (top_tabe == NULL) {
        delete_bag(bag);
        delete_files_set(files);
        delete_set(set);
        return -1;
    }

    for (size_t i = 0; i < bag->rows; ++i) {
        top_tabe[i] = (size_t *) malloc(sizeof(size_t) * 3);
        if (top_tabe[i] == NULL) {
            for (size_t j = 0; j < i; ++j) {
                free(top_tabe[j]);
            }
            free(top_tabe);
            delete_bag(bag);
            delete_files_set(files);
            delete_set(set);
            return -1;
        }
        memset((void *)top_tabe[i], 0, sizeof(size_t) * 3);
    }

    get_top(bag, top_tabe, 3);

    for (size_t i = 0; i < bag->rows; ++i) {
        free(top_tabe[i]);
    }
    free(top_tabe);
    delete_bag(bag);
    delete_files_set(files);
    delete_set(set);

    return 0;*/
}

//int main(void)
//{
//    char src[] = "******************************";
//    char dest[] = "abcdefghijklmnopqrstuvwxyz0123456789";
//    printf("Строка назначения перед копированием: %s\n",dest);
//    memmove(&(dest[1]),dest,26);
//    dest[0] = '1';
//    printf("Строка назначения после копирования: %s\n",dest);
//    return 0;
//}