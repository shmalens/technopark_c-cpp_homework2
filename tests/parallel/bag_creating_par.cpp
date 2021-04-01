#include "gtest/gtest.h"
#include <iostream>

#define TEST_PATH1 "../tests/data/data1/"
#define TEST_PATH2 "../tests/data/data2/"

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

TEST(BagTest, babTestOK1) {
SomeBag bag;
Hash hash;
FilesSet files;
files.files = read_files(TEST_PATH1);
ASSERT_TRUE(files.files);
hash.hash = create_hash_from_files(files.files);
ASSERT_TRUE(hash.hash);
bag.bag = create_bag(files.files, hash.hash);
ASSERT_TRUE(bag.bag);
ASSERT_EQ(bag.bag->rows, files.files->amount);
ASSERT_EQ(bag.bag->cols, hash.hash->total_size);

ASSERT_EQ(delete_bag(bag.bag), 0);
bag.bag = NULL;
}

TEST(BagTest, babTestOK2) {
SomeBag bag;
Hash hash;
FilesSet files;
files.files = read_files(TEST_PATH2);
ASSERT_TRUE(files.files);
hash.hash = create_hash_from_files(files.files);
ASSERT_TRUE(hash.hash);
bag.bag = create_bag(files.files, hash.hash);
ASSERT_TRUE(bag.bag);
ASSERT_EQ(bag.bag->rows, files.files->amount);
ASSERT_EQ(bag.bag->cols, hash.hash->total_size);

ASSERT_EQ(delete_bag(bag.bag), 0);
bag.bag = NULL;
}

TEST(BagTest, babTestErr) {
Hash hash;
FilesSet files;
ASSERT_FALSE(create_bag(NULL, hash.hash));
ASSERT_FALSE(create_bag(files.files, NULL));
ASSERT_EQ(delete_bag(NULL), -1);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}