#include "gtest/gtest.h"
#include <iostream>

#define TEST_PATH "../data/"

extern "C" {
#include "directory_processing.h"
#include "files_processing.h"
#include "control_units.h"
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

TEST(InitBlock, InitBlockOk) {
Hash hash;
FilesSet files;
SomeBag bag;
ASSERT_EQ(init_block(TEST_PATH, &files.files, &hash.hash, &bag.bag), 0);
ASSERT_TRUE(files.files);
ASSERT_TRUE(hash.hash);
ASSERT_TRUE(bag.bag);
}

TEST(InitBlock, InitBlockErr) {
Hash hash;
FilesSet files;
SomeBag bag;
ASSERT_EQ(init_block(NULL, &files.files, &hash.hash, &bag.bag), EMPTY_PATH);
ASSERT_EQ(init_block("asdaas", &files.files, &hash.hash, &bag.bag), ERR_CREATING_FILES_SET);
ASSERT_FALSE(files.files);
ASSERT_FALSE(hash.hash);
ASSERT_FALSE(bag.bag);
}

TEST(DataProcessing, dataProcessing) {
Hash hash;
FilesSet files;
SomeBag bag;
ASSERT_EQ(init_block(TEST_PATH, &files.files, &hash.hash, &bag.bag), 0);
ASSERT_EQ(data_processing(files.files, hash.hash, bag.bag), 0);

ASSERT_EQ(data_processing(NULL, hash.hash, bag.bag), EMPTY_FILES);
ASSERT_EQ(data_processing(files.files, NULL, bag.bag), EMPTY_SET);
ASSERT_EQ(data_processing(files.files, hash.hash, NULL), EMPTY_BAG);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}