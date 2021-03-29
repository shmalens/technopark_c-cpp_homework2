#include <stdio.h>
#include "gtest/gtest.h"

#define DATA_PATH "../test/data/comp_ok.txt"

#define TEST_LIMIT 480
#define TEST_BPM 150
#define TEST_TITLE "Metallica - Master of Puppets"

extern "C" {
#include ""
}

TEST(CompilationTest, Search_OK) {
FILE *fd = fopen(DATA_PATH, "r");
ASSERT_TRUE(fd);

playlist_t *playlist = read_data(fd);
ASSERT_TRUE(playlist);

playlist_t *search_res = search(playlist, TEST_LIMIT, TEST_BPM);
ASSERT_TRUE(search_res);
ASSERT_STREQ(search_res->compositions[0]->title.title, TEST_TITLE);
ASSERT_EQ(search_res->len, 1);

delete_playlist(search_res);
delete_playlist(playlist);
fclose(fd);
}

TEST(CompilationTest, Search_Err) {
ASSERT_FALSE(search(NULL, TEST_LIMIT, TEST_BPM));
playlist_t playlist;
playlist.len = 0;
ASSERT_FALSE(search(&playlist, TEST_LIMIT, TEST_BPM));
}

TEST(CompilationTest, GenCompilation_OK) {
FILE *fd = fopen(DATA_PATH, "r");
ASSERT_TRUE(fd);

playlist_t *playlist = read_data(fd);
ASSERT_TRUE(playlist);

playlist_t *search_res = search(playlist, TEST_LIMIT, TEST_BPM);
ASSERT_TRUE(search_res);

playlist_t *compilation = gen_compilation(search_res, 1, 1);
ASSERT_TRUE(compilation);
ASSERT_STREQ(compilation->compositions[0]->title.title, TEST_TITLE);
ASSERT_EQ(compilation->len, 1);

delete_playlist(playlist);
delete_playlist(search_res);
delete_playlist(compilation);
fclose(fd);
}

TEST(CompilationTest, GenCompilation_Err) {
ASSERT_FALSE(gen_compilation(NULL, TEST_LIMIT, 1));
playlist_t playlist;
playlist.len = 1;
ASSERT_FALSE(gen_compilation(&playlist, TEST_LIMIT, 1));

playlist.len = 0;
ASSERT_FALSE(gen_compilation(&playlist, TEST_LIMIT, 1));

playlist.len = 1;
playlist.compositions = NULL;
ASSERT_FALSE(gen_compilation(&playlist, TEST_LIMIT, 1));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}