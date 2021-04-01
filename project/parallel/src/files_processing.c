#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/sysinfo.h>

#include "files_processing.h"
#include "misc.h"

static vector_t *create_vector() {
    vector_t *new_vector = (vector_t *) malloc(sizeof(vector_t));
    if (new_vector == NULL) {
        return NULL;
    }

    new_vector->amount = 0;
    new_vector->capacity = INIT_VECTOR_SIZE;
    new_vector->words = (char **) malloc(sizeof(char *) * new_vector->capacity);
    if (new_vector->words == NULL) {
        free(new_vector);
        return NULL;
    }
    new_vector->indexes = (size_t *) malloc(sizeof(size_t) * new_vector->capacity);
    if (new_vector->indexes == NULL) {
        free(new_vector->words);
        free(new_vector);
        return NULL;
    }
    return new_vector;
}

static int delete_vector(vector_t *vector) {
    if (vector == NULL) {
        return -1;
    }

    for (size_t i = 0; i < vector->amount; ++i) {
        free(vector->words[i]);
    }
    free(vector->indexes);
    free(vector->words);
    free(vector);
    return 0;
}

static int resize_vector(vector_t *vector) {
    if (vector == NULL) {
        return -1;
    }

    char **tmp = (char **) malloc(sizeof(char *) * vector->capacity * 2);
    if (tmp == NULL) {
        return -1;
    }
    memcpy(tmp, vector->words, sizeof(char *) * vector->capacity);
    free(vector->words);
    vector->words = tmp;

    size_t *tmp_indexes = (size_t *) malloc(sizeof(size_t) * vector->capacity * 2);
    if (tmp_indexes == NULL) {
        return -1;
    }

    memcpy(tmp_indexes, vector->indexes, sizeof(size_t) * vector->capacity);
    free(vector->indexes);
    vector->indexes = tmp_indexes;
    vector->capacity *= 2;

    return 0;
}

static int insert_in_vector(vector_t *vector, char *word, size_t index) {
    if (vector == NULL) {
        return -1;
    }

    if (word == NULL) {
        return -1;
    }

    for (int i = 0; i < vector->amount; ++i) {
        if (strcmp(vector->words[i], word) == 0) {
            return ALREADY_EXISTS;
        }
    }

    if (vector->amount == vector->capacity) {
        if (resize_vector(vector) != 0) {
            return -1;
        }
    }

    custom_strcpy(&(vector->words[vector->amount]), word);
    vector->indexes[vector->amount] = index;
    vector->amount += 1;
    return 0;
}

static hash_table_t *create_hash() {
    hash_table_t *hash = (hash_table_t *) malloc(sizeof(hash_table_t));
    if (hash == NULL) {
        return NULL;
    }
    memset((void *) hash->hash_table, 0, sizeof(vector_t *) * MAX_TABLE_SIZE);

    for (int i = 0; i < RAND_SIZE; ++i) {
        hash->rand[i] = abs((char) (random() % RAND_SIZE));
    }

    hash->total_size = 0;
    return hash;
}

static int hash_function(hash_table_t *hash, char *word) {
    if (hash == NULL) {
        return -1;
    }

    if (word == NULL) {
        return -1;
    }

    char h1 = *word;
    char h2 = *word + 1;
    ++word;
    while (*word) {
        h1 = hash->rand[h1 ^ *word];
        h2 = hash->rand[h2 ^ *word];
        ++word;
    }

    int h = ((int) h1 << 8) | (int) h2;
    return h % MAX_TABLE_SIZE;
}

int insert_in_hash(hash_table_t *hash, char *word) {
    if (hash == NULL) {
        return ERROR_EMPTY_DEST_PTR;
    }

    if (word == NULL) {
        return ERROR_EMPTY_SRC_PTR;
    }

    int index = hash_function(hash, word);
    if (index == -1) {
        return -1;
    }

    if (hash->hash_table[index] == NULL) {
        hash->hash_table[index] = create_vector();
        if (hash->hash_table[index] == NULL) {
            return ERROR_NODE_CREATING;
        }
        insert_in_vector(hash->hash_table[index], word, hash->total_size);
        hash->total_size += 1;
        return INSERTED;
    }

    int inf = insert_in_vector(hash->hash_table[index], word, hash->total_size);
    if (inf == ALREADY_EXISTS) {
        return ALREADY_EXISTS;
    }
    hash->total_size += 1;

    return INSERTED;
}

hash_table_t *create_hash_from_files(files_t *files) {
    if (files == NULL) {
        return NULL;
    }

    hash_table_t *new_hash = create_hash();
    if (new_hash == NULL) {
        return NULL;
    }

    int pipes[2];
    if (pipe(pipes) == -1) {
        delete_hash(new_hash);
        return NULL;
    }

    size_t aviable = get_nprocs();
    size_t max_processes = aviable > files->amount ? files->amount : aviable;
    if (max_processes == 0) {
        delete_hash(new_hash);
        return NULL;
    }
    size_t max_cycles = files->amount / max_processes + (files->amount % max_processes == 0 ? 0 : 1);
    size_t left = files->amount;

    for (size_t cycle_num = 0; cycle_num < max_cycles; ++cycle_num) {
        pid_t parent = getpid();
        size_t process_num;
        size_t max_processes_in_cycle = left > max_processes ? max_processes : left;
        for (process_num = 0; process_num < max_processes_in_cycle; ++process_num) {
            pid_t tmp = fork();
            if (tmp == -1) {
                for (size_t j = 0; j < process_num; ++j) {
                    close(pipes[0]);
                    close(pipes[1]);
                    wait(NULL);
                    free(new_hash);
                    return NULL;
                }
            }
            if (tmp == 0) {
                break;
            }
        }

        pid_t child = getpid();
        if (child != parent) {
            close(pipes[0]);
            size_t file_number = cycle_num * max_processes + process_num;
            FILE *fd = fopen(files->file_names[file_number], "r");
            if (fd == NULL) {
                close(pipes[1]);
                delete_hash(new_hash);
                return NULL;
            }

            int err;
            char buff[MAXSIZE];
            memset(buff, 0, sizeof(char) * MAXSIZE);
            while ((err = naive_tokenizer(fd, buff)) != EOF) {
                if (err != 1) {
                    write(pipes[1], buff, sizeof(char) * MAXSIZE);
                }
            }

            close(pipes[1]);
            fclose(fd);
            delete_hash(new_hash);
            exit(0);
        }

        if (child == parent) {
            close(pipes[1]);
            char buff[MAXSIZE];
            memset(buff, 0, sizeof(char) * MAXSIZE);

            while (read(pipes[0], buff, sizeof(char) * MAXSIZE) > 0) {
                insert_in_hash(new_hash, buff);
            }

            for (size_t i = 0; i < process_num; ++i) {
                wait(NULL);
            }

            close(pipes[0]);
            left -= max_processes_in_cycle;
        }
    }

    return new_hash;
}

int delete_hash(hash_table_t *hash) {
    if (hash == NULL) {
        return -1;
    }

    for (int i = 0; i < MAX_TABLE_SIZE; ++i) {
        delete_vector(hash->hash_table[i]);
    }
    free(hash);
    return 0;
}

long get_index(hash_table_t *hash, char *word) {
    if (hash == NULL) {
        return -1;
    }

    if (word == NULL) {
        return -1;
    }

    int index = hash_function(hash, word);
    if (index == -1) {
        return NOT_FOUND;
    }

    if (hash->hash_table[index] == NULL) {
        return NOT_FOUND;
    }

    for (int i = 0; i < hash->hash_table[index]->amount; ++i) {
        if (strcmp(hash->hash_table[index]->words[i], word) == 0) {
            return hash->hash_table[index]->indexes[i];
        }
    }
    return NOT_FOUND;
}

bag_of_words_t *create_bag(const files_t *files, hash_table_t *total) {
    if (files == NULL) {
        return NULL;
    }

    if (files->amount == 0) {
        return NULL;
    }

    if (total == NULL) {
        return NULL;
    }

    bag_of_words_t *new_bag = (bag_of_words_t *) malloc(sizeof(bag_of_words_t));
    if (new_bag == NULL) {
        return NULL;
    }

    new_bag->rows = files->amount;
    new_bag->cols = total->total_size;

    size_t shared_size = sizeof(double) * new_bag->cols * new_bag->rows;
    double *shared = mmap(NULL, shared_size,
                        PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (!shared) {
        free(new_bag);
        return NULL;
    }
    memset(shared, 0, shared_size);

    pid_t parent = getpid();
    size_t process_num;
    for (process_num = 0; process_num < new_bag->rows; ++process_num) {
        pid_t tmp = fork();
        if (tmp == -1) {
            for (size_t i = 0; i < process_num; ++i) {
                wait(NULL);
            }
            free(new_bag);
            return NULL;
        }

        if (getpid() != parent) {
            break;
        }
    }
    pid_t child = getpid();

    if (child != parent) {
        FILE *fd = fopen(files->file_names[process_num], "r");
        if (fd == NULL) {
            munmap(shared, shared_size);
            free(new_bag);
            return NULL;
        }

        int err;
        char buff[MAXSIZE];
        while ((err = naive_tokenizer(fd, buff)) != EOF) {
            if (err != 1) {
                long index = get_index(total, buff);
                if (index != NOT_FOUND) {
                    *(shared + (new_bag->cols * process_num + index)) += 1;
                }
            }
        }

        fclose(fd);
        free(new_bag);
        exit(0);
    }

    if (child == parent) {
        for (size_t i = 0; i < process_num; ++i) {
            wait(NULL);
        }

        new_bag->matrix = (double **) malloc(sizeof(double *) * new_bag->rows);
        if (new_bag->matrix == NULL) {
            free(new_bag);
            munmap(shared, shared_size);
            return NULL;
        }

        for (size_t i = 0; i < new_bag->rows; ++i) {
            new_bag->matrix[i] = (double *) malloc(sizeof(double) * new_bag->cols);
            if (new_bag->matrix[i] == NULL) {
                for (size_t j = 0; j < i; ++j) {
                    free(new_bag->matrix[j]);
                }
                free(new_bag->matrix);
                free(new_bag);
                munmap(shared, shared_size);
                return NULL;
            }
        }

        for (size_t i = 0; i < new_bag->rows; ++i) {
            memcpy(new_bag->matrix[i], shared + (new_bag->cols * i), sizeof(double) * new_bag->cols);
        }

        munmap(shared, shared_size);
        return new_bag;
    }

    free(new_bag);
    return NULL;
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
