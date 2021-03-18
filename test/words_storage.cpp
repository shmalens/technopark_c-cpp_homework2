#include <stdio.h>
#include "gtest/gtest.h"

extern "C" {
#include "words_storage.h"
}

TEST(CreatePair, create_ok) {

}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}