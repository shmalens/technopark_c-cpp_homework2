#include "gtest/gtest.h"
#include <iostream>

#define TEST_PATH "../data/"
#define POSITIONS 5

extern "C" {
#include "directory_processing.h"
#include "files_processing.h"
#include "control_units.h"
#include "calculations.h"
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

TEST(TfIdf, tfIdfOK) {
Hash hash;
FilesSet files;
SomeBag bag;
ASSERT_EQ(init_block(TEST_PATH, &files.files, &hash.hash, &bag.bag), 0);
ASSERT_TRUE(files.files);
ASSERT_TRUE(hash.hash);
ASSERT_TRUE(bag.bag);

ASSERT_EQ(tf_idf_transform(bag.bag), 0);
}

TEST(TfIdf, tfIdfErr) {
ASSERT_EQ(tf_idf_transform(NULL), EMPTY_PTR);
}

TEST(GetTop, topOk) {
index_val_t **top;
hash_table_t *hash;
files_t *files;
bag_of_words_t *bag;
ASSERT_EQ(init_block(TEST_PATH, &files, &hash, &bag), 0);
ASSERT_EQ(tf_idf_transform(bag), 0);
top = get_top(bag);
ASSERT_TRUE(top);

ASSERT_EQ(show_top(hash, files, top, bag->rows, POSITIONS), 0);

ASSERT_EQ(delete_top(bag, top), 0);
top = NULL;
}

TEST(GetTop, topErr) {
ASSERT_FALSE(get_top(NULL));

ASSERT_EQ(delete_top(NULL, NULL), -1);
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}