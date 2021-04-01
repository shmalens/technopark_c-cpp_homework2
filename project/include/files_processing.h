#ifndef PROCESSING_H
#define PROCESSING_H

#include "directory_processing.h"

#define NOT_FOUND -1
#define MAXSIZE 500
#define MAX_TABLE_SIZE 65536
#define RAND_SIZE 256
#define INIT_VECTOR_SIZE 1

enum set_adding_events {
    ERROR_NODE_CREATING = -3,
    ALREADY_EXISTS = 1,
    INSERTED = 2
};

typedef struct {
    size_t *indexes;
    char **words;

    size_t amount;
    size_t capacity;
} vector_t;

typedef struct {
    vector_t *hash_table[MAX_TABLE_SIZE];
    size_t total_size;
    unsigned char rand[RAND_SIZE];
} hash_table_t;

typedef struct {
    double **matrix;

    size_t cols;
    size_t rows;
} bag_of_words_t;

bag_of_words_t *create_bag(const files_t *files, hash_table_t *set);
int delete_bag(bag_of_words_t *bag);

hash_table_t *create_hash_from_files(files_t *files);
int delete_hash(hash_table_t *hash);

#endif //PROCESSING_H
