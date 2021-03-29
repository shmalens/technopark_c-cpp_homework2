#include "gtest/gtest.h"
#include <stdio.h>

#define PATH "../data/file"
#define TEST_STRING "striiiiing"
#define CAT1 "Hello "
#define CAT2 "Word"
#define CAT1CAT2 "Hello Word"
#define WORD "FIRST"
#define BUFF_SIZE 100

extern "C" {
#include "misc.h"
}

struct SomeString {
    char *str;

    SomeString() : str(NULL) {}
    ~SomeString();
};

struct SomeFile {
    FILE *fd;

    SomeFile() : fd(NULL) {}
    ~SomeFile();
};

SomeFile::~SomeFile() {
    if (fd != NULL) {
        fclose(fd);
    }
}

SomeString::~SomeString() {
    if (str != NULL) {
        free(str);
    }
}

TEST(CustomStrCpy, copyOK) {
SomeString string;
ASSERT_EQ(custom_strcpy(&string.str, TEST_STRING), 0);
ASSERT_TRUE(string.str);
ASSERT_STREQ(string.str, TEST_STRING);
}

TEST(CustomStrCpy, copyErr) {
SomeString string;
ASSERT_EQ(custom_strcpy(NULL, TEST_STRING), ERROR_EMPTY_DEST_PTR);
ASSERT_EQ(custom_strcpy(&string.str, NULL), ERROR_EMPTY_SRC_PTR);
}

TEST(CustomStrCat, catOK) {
SomeString string;
ASSERT_EQ(custom_strcat(&string.str, CAT1, CAT2), 0);
ASSERT_TRUE(string.str);
ASSERT_STREQ(string.str, CAT1CAT2);
}

TEST(CustomStrCat, catErr) {
SomeString string;
ASSERT_EQ(custom_strcat(NULL, CAT1, CAT2), ERROR_EMPTY_DEST_PTR);
ASSERT_EQ(custom_strcat(&string.str, NULL, CAT2), ERROR_EMPTY_SRC_PTR);
ASSERT_EQ(custom_strcat(&string.str, CAT1, NULL), ERROR_EMPTY_SRC_PTR);
}

TEST(Tokenizer, readOK) {
SomeFile file;
file.fd = fopen(PATH, "r");
ASSERT_TRUE(file.fd);
char buff[BUFF_SIZE];
ASSERT_EQ(naive_tokenizer(file.fd, buff), 0);
ASSERT_STREQ(buff, WORD);

fclose(file.fd);
file.fd = NULL;
}

TEST(Tokenizer, readErr) {
SomeFile file;
ASSERT_EQ(naive_tokenizer(file.fd, NULL), -1);
ASSERT_EQ(naive_tokenizer(NULL, NULL), -1);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}