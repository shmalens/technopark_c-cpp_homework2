#ifndef PROCESSING_H
#define PROCESSING_H

#include "directory_processing.h"

#define NOT_FOUND -1
#define MAXSIZE 500
#define MAX_TABLE_SIZE 65536
#define RAND_SIZE 256
#define INIT_VECTOR_SIZE 500
#define FIFO_NAME "/tmp/tf_idf"


enum set_adding_events {
    EMPTY_DEST = -1,
    EMPTY_WORD = -2,
    ERROR_NODE_CREATING = -3,
    ALREADY_EXISTS = 1,
    INSERTED = 2
};

/*typedef struct node {
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
    char **expanded_set;
    size_t amount;
} expanded_set_t;

 */

typedef struct {
    size_t *indexes;
    char **words;
    int amount;
    int capacity;
} vector_t;

typedef struct {
    vector_t *hash_table[MAX_TABLE_SIZE];
    unsigned char rand[RAND_SIZE];
    size_t total_size;
} hash_table_t;

typedef struct {
    double **matrix;

    size_t cols;
    size_t rows;
} bag_of_words_t;

bag_of_words_t *create_bag(const files_t *files, hash_table_t *set);
int delete_bag(bag_of_words_t *bag);

hash_table_t *create_hash_from_files(files_t *files);
int insert_in_hash(hash_table_t *hash, char *word);
int delete_hash(hash_table_t *hash);
long get_index(hash_table_t *hash, char *word);

#endif //PROCESSING_H
