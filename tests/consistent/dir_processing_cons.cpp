#include "gtest/gtest.h"
#include <iostream>

#define TEST_PATH_OK "../data/"
#define FILENAME "../data/file"

#define TEST_PATH_WRONG "../dataaa"


extern "C" {
#include "directory_processing.h"
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

TEST(DirectoryProcessing, readDirOK) {
    FilesSet set;
    set.files = read_files(TEST_PATH_OK);
    ASSERT_TRUE(set.files);
    ASSERT_EQ(set.files->amount, 1);
    ASSERT_STREQ(set.files->file_names[0], FILENAME);
}

TEST(DirectoryProcessing, readDirErr) {
FilesSet set;
set.files = read_files(TEST_PATH_WRONG);
ASSERT_FALSE(set.files);
}

TEST(DirectoryProcessing, deleteFilesSetOK) {
FilesSet set;
set.files = read_files(TEST_PATH_OK);
ASSERT_TRUE(set.files);

ASSERT_EQ(delete_files_set(set.files), 0);
set.files = NULL;
}

TEST(DirectoryProcessing, deleteFilesSetErr) {
ASSERT_EQ(delete_files_set(NULL), -1);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}