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

int get_top(const bag_of_words_t *bag, size_t **top_table, unsigned int positions) {
    if (bag == NULL) {
        return EMPTY_PTR;
    }

    if (top_table == NULL) {
        return EMPTY_PTR;
    }

    if (positions == 0 || positions > bag->cols) {
        return WRONG_VALUE;
    }

    for (size_t i = 0; i < bag->rows; ++i) {
        for (size_t j = 0; j < bag->cols; ++j) {
            for (unsigned int k = 0; k < positions; ++k) {
                if (bag->matrix[i][j] > bag->matrix[i][top_table[i][k]]) {
                    memmove(top_table[i], top_table[i] + 1, sizeof(size_t) * (positions - 1));
                    top_table[i][positions - 1] = j;
                    printf("row %lu %f >= %f on %lu %lu\n", i,
                           bag->matrix[i][j],
                           bag->matrix[i][top_table[i][k]],
                           j,
                           top_table[i][k]);
                    break;
                }
            }
        }
    }

    return 0;
}