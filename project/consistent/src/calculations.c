#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

#include "calculations.h"
#include "files_processing.h"

int tf_idf_transform(bag_of_words_t *bag) {
    if (bag == NULL) {
        return EMPTY_PTR;
    }

    // сумма всех элементов каждой строки матрицы
    // нужно для tf
    double *rows_total = (double *) malloc(sizeof(double) * bag->rows);
    if (rows_total == NULL) {
        return ERROR_MEM_ALLOC;
    }

    // сумма всех элементов по столбцам матрицы
    // нужно для idf
    double *cols_total = (double *) malloc(sizeof(double) * bag->cols);
    if (cols_total == NULL) {
        free(rows_total);
        return ERROR_MEM_ALLOC;
    }

    memset((void *) rows_total, 0, sizeof(double) * bag->rows);
    memset((void *) cols_total, 0, sizeof(double) * bag->cols);

    for (size_t i = 0; i < bag->rows; ++i) {
        for (size_t j = 0; j < bag->cols; ++j) {
            rows_total[i] += bag->matrix[i][j];
            if (bag->matrix[i][j] != 0) {
                cols_total[j] += 1;
            }
        }
    }

    for (size_t i = 0; i < bag->rows; ++i) {
        for (size_t j = 0; j < bag->cols; ++j) {
            double tf = bag->matrix[i][j] / rows_total[i];
            double idf = log((double) bag->rows / cols_total[j]);
            bag->matrix[i][j] = tf * idf;
        }
    }

    free(cols_total);
    free(rows_total);
    return 0;
}

static int comparator(const void *l, const void *r) {
    index_val_t *left = (index_val_t *)l;
    index_val_t *right = (index_val_t *)r;
    double res = left->val - right->val;
    if (res < 0) {
        return 1;
    }

    if (res > 0) {
        return -1;
    }

    return 0;
}

index_val_t **get_top(const bag_of_words_t *bag) {
    if (bag == NULL) {
        return NULL;
    }

    index_val_t **top_table = (index_val_t **) malloc(sizeof(index_val_t *) * bag->rows);
    if (top_table == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < bag->rows; ++i) {
        top_table[i] = (index_val_t *) malloc(sizeof(index_val_t) * bag->cols);
        if (top_table[i] == NULL) {
            for (size_t j = 0; j < i; ++j) {
                free(top_table[j]);
            }
            free(top_table);
            return NULL;
        }
    }

    for (size_t i = 0; i < bag->rows; ++i) {
        for (size_t j = 0; j < bag->cols; ++j) {
            top_table[i][j].val = bag->matrix[i][j];
            top_table[i][j].index = j;
        }
    }

    for (size_t i = 0; i < bag->rows; ++i) {
        qsort(top_table[i], bag->cols, sizeof(index_val_t), comparator);
    }

    return top_table;
}

int delete_top(const bag_of_words_t *bag, index_val_t **top_table) {
    if (bag == NULL) {
        return -1;
    }

    if (top_table == NULL) {
        return -1;
    }

    for (size_t i = 0; i < bag->rows; ++i) {
        free(top_table[i]);
    }
    free(top_table);
}

int show_top(const hash_table_t *set, const files_t *files, index_val_t **table, size_t rows, size_t positions) {
    if (set == NULL) {
        return -1;
    }

    if (table == NULL) {
        return -1;
    }

    char **words = (char **) malloc(sizeof(char *) * set->total_size);
    if (words == NULL) {
        return -1;
    }

    for (int i = 0; i < MAX_TABLE_SIZE; ++i) {
        if (set->hash_table[i] != NULL) {
            for (size_t j = 0; j < set->hash_table[i]->amount; ++j) {
                size_t index = set->hash_table[i]->indexes[j];
                custom_strcpy(&(words[index]), set->hash_table[i]->words[j]);
            }
        }
    }

    for (size_t i = 0; i < rows; ++i) {
        printf("file %s top:\n", files->file_names[i]);
        for (int j = 0; j < positions; ++j) {
            printf("%d: %s value: %.8f\n", j + 1, words[table[i][j].index], table[i][j].val);
        }
        printf("\n");
    }

    for (int i = 0; i < set->total_size; ++i) {
        free(words[i]);
    }
    free(words);

    return 0;
}