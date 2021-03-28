#ifndef PROCESSING_H
#define PROCESSING_H

#include "directory_processing.h"

#define NOT_FOUND -1
#define MAXSIZE 500

enum set_adding_events {
    EMPTY_DEST = -1,
    EMPTY_WORD = -2,
    ERROR_NODE_CREATING = -3,
    ALREADY_EXISTS = 1,
    INSERTED = 2
};

typedef struct node {
    char *word;
    size_t index;
    struct node *left;
    struct node *right;
} node_t;

typedef struct {
    node_t *root;
    size_t total_size;
} set_t;

typedef struct {
    double **matrix;

    size_t cols;
    size_t rows;
} bag_of_words_t;

bag_of_words_t *create_bag(const files_t *files, const set_t *set);
int delete_bag(bag_of_words_t *bag);

set_t *create_set_from_files(const files_t *files);
int delete_set(set_t *set);

int print_traverse(node_t *root);

#endif //PROCESSING_H
