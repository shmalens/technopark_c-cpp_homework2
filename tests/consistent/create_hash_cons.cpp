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

TEST(HashTests, hashTestOK1) {
FilesSet files;
Hash hash;

files.files = read_files(TEST_PATH1);
ASSERT_TRUE(files.files);
hash.hash = create_hash_from_files(files.files);
ASSERT_TRUE(hash.hash);

ASSERT_EQ(delete_hash(hash.hash), 0);

ASSERT_EQ(delete_files_set(files.files), 0);
hash.hash = NULL;
files.files = NULL;
}

TEST(HashTests, hashTestOK2) {
FilesSet files;
Hash hash;

files.files = read_files(TEST_PATH2);
ASSERT_TRUE(files.files);
hash.hash = create_hash_from_files(files.files);
ASSERT_TRUE(hash.hash);

ASSERT_EQ(delete_hash(hash.hash), 0);

ASSERT_EQ(delete_files_set(files.files), 0);
hash.hash = NULL;
files.files = NULL;
}

TEST(HashTests, hashTestErr) {
Hash hash;
hash.hash = create_hash_from_files(NULL);
ASSERT_FALSE(hash.hash);

ASSERT_EQ(delete_hash(NULL), -1);

}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}