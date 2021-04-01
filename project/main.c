#include "files_processing.h"
#include "directory_processing.h"
#include "control_units.h"

#define PATH "../generated_data/"

int main() {
    files_t *files;
    hash_table_t *set;
    bag_of_words_t *bag;

    if (init_block(PATH, &files, &set, &bag) != 0) {
        return -1;
    }

    if (data_processing(files, set, bag) != 0) {
        free_resources(files, set, bag);
        return -1;
    }

    free_resources(files, set, bag);
}
