#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "files_processing.h"
#include "misc.h"

static node_t *create_tree_node(const char *word, const size_t index) {
    if (word == NULL) {
        return NULL;
    }

    node_t *new_node = (node_t *) malloc(sizeof(node_t));
    if (new_node == NULL) {
        return NULL;
    }

    if (custom_strcpy(&(new_node->word), word) != 0) {
        free(new_node);
        return NULL;
    }

    new_node->right = NULL;
    new_node->left = NULL;
    new_node->index = index;

    return new_node;
}

static int delete_tree(node_t *node) {
    if (node != NULL) {
        delete_tree(node->left);
        delete_tree(node->right);
        free(node->word);
        free(node);
    }
    return 0;
}

static set_t *create_set() {
    set_t *new_set = (set_t *) malloc(sizeof(set_t));
    if (new_set == NULL) {
        return NULL;
    }

    new_set->total_size = 0;
    new_set->root = NULL;
    return new_set;
}

static int set_include(set_t *dest, const char *word) {
    if (dest == NULL) {
        return EMPTY_DEST;
    }

    if (word == NULL) {
        return EMPTY_WORD;
    }

    if (dest->root == NULL) {
        dest->root = create_tree_node(word, dest->total_size);
        if (dest->root == NULL) {
            return ERROR_NODE_CREATING;
        }
        dest->total_size += 1;
        return INSERTED;
    }

    node_t *cursor = dest->root;
    node_t **tmp_ptr;
    while (cursor != NULL) {
        int cmp = strcmp(cursor->word, word);
        if (cmp == 0) {
            return ALREADY_EXISTS;
        } else if (cmp == 1) {
            tmp_ptr = &(cursor->left);
            cursor = cursor->left;
        } else {
            tmp_ptr = &(cursor->right);
            cursor = cursor->right;
        }
    }

    *tmp_ptr = create_tree_node(word, dest->total_size);
    if (*tmp_ptr == NULL) {
        return ERROR_NODE_CREATING;
    }
    dest->total_size += 1;
    return INSERTED;
}

int delete_set(set_t *set) {
    if (set == NULL) {
        return -1;
    }

    delete_tree(set->root);
    free(set);
    return 0;
}

set_t *create_set_from_files(const files_t *files) {
    if (files == NULL) {
        return NULL;
    }

    set_t *new_set = create_set();
    if (new_set == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < files->amount; ++i) {
        FILE *fd = fopen(files->file_names[i], "r");
        if (fd == NULL) {
            free(new_set);
            return NULL;
        }

        char buff[MAXSIZE];
        int err;
        while ((err = naive_tokenizer(fd, buff)) != EOF) {
            if (err != 1) {
                set_include(new_set, buff);
            }
        }

        fclose(fd);
    }

    return new_set;
}

static long set_contains(const set_t *set, const char *word) {
    if (set == NULL) {
        return -1;
    }

    if (word == NULL) {
        return -1;
    }

    node_t *cursor = set->root;
    while (cursor != NULL) {
        int cmp = strcmp(cursor->word, word);
        if (cmp == 0) {
            return cursor->index;
        } else if (cmp == 1) {
            cursor = cursor->left;
        } else {
            cursor = cursor->right;
        }
    }

    return NOT_FOUND;
}

static void traverse(node_t *root, char **set) {
    if (root != NULL) {
        custom_strcpy(&(set[root->index]), root->word);
        traverse(root->left, set);
        traverse(root->right, set);
    }
}

expanded_set_t *expand_set(const set_t *set) {
    if (set == NULL) {
        return NULL;
    }

    expanded_set_t *new_ex_set = (expanded_set_t *) malloc(sizeof(expanded_set_t));
    if (new_ex_set == NULL) {
        return NULL;
    }

    new_ex_set->expanded_set = (char **) malloc(sizeof(char *) * set->total_size);
    if (new_ex_set->expanded_set == NULL) {
        free(new_ex_set);
        return NULL;
    }

    traverse(set->root, new_ex_set->expanded_set);
    new_ex_set->amount = set->total_size;

    return new_ex_set;
}

int delete_expand_set(expanded_set_t *set) {
    if (set == NULL) {
        return -1;
    }

    for (size_t i = 0; i < set->amount; ++i) {
        free(set->expanded_set[i]);
    }
    free(set->expanded_set);
    free(set);
    return 0;
}

bag_of_words_t *create_bag(const files_t *files, const set_t *total) {
    if (files == NULL) {
        return NULL;
    }

    if (files->amount == 0) {
        return NULL;
    }

    bag_of_words_t *new_bag = (bag_of_words_t *) malloc(sizeof(bag_of_words_t));
    if (new_bag == NULL) {
        return NULL;
    }

    // Создаем матрицу
    new_bag->rows = files->amount;
    new_bag->cols = total->total_size;
    new_bag->matrix = (double **) malloc(sizeof(size_t *) * new_bag->rows);
    if (new_bag->matrix == NULL) {
        free(new_bag);
        return NULL;
    }

    for (size_t i = 0; i < new_bag->rows; ++i) {
        new_bag->matrix[i] = (double *) malloc(sizeof(size_t) * new_bag->cols);
        if (new_bag->matrix[i] == NULL) {
            for (size_t j = 0; j < i; ++j) {
                free(new_bag->matrix[j]);
            }
            free(new_bag->matrix);
            free(new_bag);
            return NULL;
        }
        memset((void *) new_bag->matrix[i], 0, sizeof(size_t) * new_bag->cols);
    }

    // Заполняем матрицу
    for (size_t i = 0; i < files->amount; ++i) {
        FILE *fd = fopen(files->file_names[i], "r");
        if (fd == NULL) {
            for (size_t j = 0; j < new_bag->rows; ++j) {
                free(new_bag->matrix[j]);
            }
            free(new_bag->matrix);
            free(new_bag);
            return NULL;
        }
        printf("%s analize...\n", files->file_names[i]);

        char buff[MAXSIZE];
        int err;
        while ((err = naive_tokenizer(fd, buff)) != EOF) {
            if (err != 1) {
                long index = set_contains(total, buff);
                if (index != NOT_FOUND) {
                    new_bag->matrix[i][index] += 1;
                }
            }
        }

        fclose(fd);
    }

    return new_bag;
}

int delete_bag(bag_of_words_t *bag) {
    if (bag == NULL) {
        return -1;
    }

    for (size_t i = 0; i < bag->rows; ++i) {
        free(bag->matrix[i]);
    }

    free(bag->matrix);
    free(bag);
    return 0;
}
