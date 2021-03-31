#include "gtest/gtest.h"
#include <iostream>

#define TEST_PATH1 "./data/data1/"
#define TEST_PATH2 "../data/data2/"
#define TEST_PATH3 "../data/data3/"

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

TEST(HashTests, hashTests) {
FilesSet set1;
Hash hash1;
set1.files = read_files(TEST_PATH1);
ASSERT_TRUE(set1.files);
hash1.hash = create_hash_from_files(set1.files);
ASSERT_TRUE(hash1.hash);
ASSERT_EQ(delete_hash(hash1.hash), 0);
hash1.hash = NULL;

FilesSet set2;
Hash hash2;
set1.files = read_files(TEST_PATH2);
ASSERT_TRUE(set2.files);
hash1.hash = create_hash_from_files(set2.files);
ASSERT_TRUE(hash2.hash);
ASSERT_EQ(delete_hash(hash2.hash), 0);
hash2.hash = NULL;

FilesSet set3;
Hash hash3;
set1.files = read_files(TEST_PATH3);
ASSERT_TRUE(set3.files);
hash1.hash = create_hash_from_files(set3.files);
ASSERT_TRUE(hash3.hash);
ASSERT_EQ(delete_hash(hash3.hash), 0);
hash3.hash = NULL;

ASSERT_FALSE(create_hash_from_files(NULL));
ASSERT_EQ(delete_hash(NULL), -1);
}

TEST(BagTest, babTest) {
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

ASSERT_FALSE(create_bag(NULL, hash.hash));
ASSERT_FALSE(create_bag(files.files, NULL));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}