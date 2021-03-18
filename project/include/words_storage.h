#ifndef WORDS_STORAGE_H
#define WORDS_STORAGE_H

typedef struct {
    size_t amount;
    size_t word_len;
    char *word;
} pair_t;

typedef struct node {
    pair_t *pair;
    struct node *left;
    struct node *right;
} words_tree_t;

typedef struct storage {
    size_t words_amount;
    words_tree_t *root;
} words_storage_t;

enum errors_add_tree {
    EMPTY_ROOT = 1,
    EMPTY_WORD,
    ERROR_CREATING_NEW_NODE
};

enum errors_add_storage {
    EMPTY_STORAGE = 1,
    EMPTY_WORD_STORAGE_INSERT
};

pair_t *create_pair(const char *word);
int delete_pair(pair_t *pair);

words_tree_t *create_tree_node(const char *init_word);
void delete_tree(words_tree_t *root);
int add_word(words_tree_t *root, const char *word);

void preorder_show(words_tree_t *root);

words_storage_t *create_storage(const char *init_word);
int delete_storage(words_storage_t *storage);
int insert_in_storage(words_storage_t *storage, const char *word);

int show_storage(words_storage_t *storage);

#endif //WORDS_STORAGE_H
