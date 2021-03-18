#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "words_storage.h"

pair_t *create_pair(const char *word) {
    if (word == NULL) {
        return NULL;
    }

    size_t tmp_len = strlen(word) + 1;
    char *word_buff = (char *) malloc(sizeof(char) * tmp_len);
    if (word_buff == NULL) {
        return NULL;
    }
    strncpy(word_buff, word, tmp_len);

    pair_t *new_pair = (pair_t *) malloc(sizeof(pair_t));
    if (new_pair == NULL) {
        free(word_buff);
        return NULL;
    }

    new_pair->word = word_buff;
    new_pair->word_len = tmp_len;
    new_pair->amount = 1;
    return new_pair;
}

int delete_pair(pair_t *pair) {
    if (pair == NULL) {
        return -1;
    }

    free(pair->word);
    free(pair);
    return 0;
}

words_tree_t *create_tree_node(const char *init_word) {
    words_tree_t *new_tree = (words_tree_t *) malloc(sizeof(words_tree_t));
    if (new_tree == NULL) {
        return NULL;
    }

    new_tree->right = NULL;
    new_tree->left = NULL;
    new_tree->pair = create_pair(init_word);
    if (new_tree->pair == NULL) {
        free(new_tree);
        return NULL;
    }

    return new_tree;
}

void delete_tree(words_tree_t *root) {
    if (root != NULL) {
        delete_tree(root->left);
        delete_tree(root->right);
        delete_pair(root->pair);
        free(root);
    }
}

int add_word(words_tree_t *root, const char *word) {
    if (root == NULL) {
        return EMPTY_ROOT;
    }

    if (word == NULL) {
        return EMPTY_WORD;
    }

    words_tree_t *cursor = root;
    while (1) {
        int comparator = strcmp(cursor->pair->word, word);
        if (comparator == 0) {
            cursor->pair->amount += 1;
            return 0;
        }

        if (comparator == 1) {
            if (cursor->left == NULL) {
                cursor->left = create_tree_node(word);
                if (cursor->left == NULL) {
                    return ERROR_CREATING_NEW_NODE;
                }
                cursor = cursor->left;
                break;
            }
            cursor = cursor->left;
        } else {
            if (cursor->right == NULL) {
                cursor->right = create_tree_node(word);
                if (cursor->right == NULL) {
                    return ERROR_CREATING_NEW_NODE;
                }
                cursor = cursor->right;
                break;
            }
            cursor = cursor->right;
        }
    }

    cursor->right = NULL;
    cursor->left = NULL;
    return 0;
}

void preorder_show(words_tree_t *root) {
    if (root != NULL) {
        preorder_show(root->left);
        printf("word %s met %lu times\n", root->pair->word, root->pair->amount);
        preorder_show(root->right);
    }
}

words_storage_t *create_storage(const char *init_word) {
    words_storage_t *new_storage = (words_storage_t *) malloc(sizeof(words_storage_t));
    if (new_storage == NULL) {
        return NULL;
    }

    new_storage->root = create_tree_node(init_word);
    if (new_storage->root == NULL) {
        free(new_storage);
        return NULL;
    }

    new_storage->words_amount = 1;
    return new_storage;
}

int delete_storage(words_storage_t *storage) {
    if (storage == NULL) {
        return -1;
    }

    delete_tree(storage->root);
    free(storage);
    return 0;
}

int insert_in_storage(words_storage_t *storage, const char *word) {
    if (storage == NULL) {
        return EMPTY_STORAGE;
    }

    if (word == NULL) {
        return EMPTY_WORD_STORAGE_INSERT;
    }

    storage->words_amount += 1;
    return add_word(storage->root, word);
}

int show_storage(words_storage_t *storage) {
    if (storage == NULL) {
        return -1;
    }

    printf("Total word count: %lu\n", storage->words_amount);
    preorder_show(storage->root);
    return 0;
}

