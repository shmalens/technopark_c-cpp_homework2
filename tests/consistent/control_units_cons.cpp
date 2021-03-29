#include "gtest/gtest.h"
#include <iostream>

#define TEST_PATH "../data/"

extern "C" {
#include "directory_processing.h"
#include "files_processing.h"
}

struct Hash {
    hash_table_t *hash;

    Hash() : hash(NULL) {}
    ~Hash();
};

Hash::~Hash() {
    if (hash != NULL) {
        delete_hash(hash);
    }
}

struct FilesSet {
    files_t *files;

    FilesSet() : files(NULL) {}
    ~FilesSet();
};

FilesSet::~FilesSet() {
    if (files != NULL) {
        delete_files_set(files);
    }
}

struct SomeBag {
    bag_of_words_t *bag;

    SomeBag() : bag(NULL) {}
    ~SomeBag();
};

SomeBag::~SomeBag() {
    if (bag == NULL) {
        delete bag;
    }
}

//int init_block(const char *dir_path, files_t **files, hash_table_t **set, bag_of_words_t **bag);
//int data_processing(files_t *files, hash_table_t *set, bag_of_words_t *bag);
//void free_resources(files_t *files, hash_table_t *set, bag_of_words_t *bag);

TEST(InitBlock, InitBlockOk) {

}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}