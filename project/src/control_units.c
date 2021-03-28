#include "control_units.h"
#include "files_processing.h"
#include "directory_processing.h"
#include "calculations.h"

int init_block(const char *dir_path, files_t **files, set_t **set, bag_of_words_t **bag) {
    if (dir_path == NULL) {
        return EMPTY_PATH;
    }

    *files = read_files(dir_path);
    if (*files == NULL) {
        return ERR_CREATING_FILES_SET;
    }

    *set = create_set_from_files(*files);
    if (*set == NULL) {
        delete_files_set(*files);
        return ERR_CREATING_SET;
    }

    *bag = create_bag(*files, *set);
    if (*bag == NULL) {
        delete_set(*set);
        delete_files_set(*files);
        return ERR_CREATING_BAG;
    }

    return 0;
}

int data_processing(files_t *files, set_t *set, bag_of_words_t *bag) {
    if (files == NULL) {
        return EMPTY_FILES;
    }

    if (set == NULL) {
        return EMPTY_SET;
    }

    if (bag == NULL) {
        return EMPTY_BAG;
    }

    if (tf_idf_transform(bag) != 0) {
        return ERR_TF_IDF;
    }

    index_val_t **top_table = get_top(bag);
    if (top_table == NULL) {
        return ERR_GETTING_TOP;
    }

//    for (size_t i = 0; i < bag->rows; ++i) {
//        for (size_t j = 0; j < bag->cols; ++j) {
//            printf("%.2f ", bag->matrix[i][j]);
//        }
//        printf("\n");
//    }

    show_top(set, files, top_table, bag->rows, TOP);

    delete_top(bag, top_table);

    return 0;
}


void free_resources(files_t *files, set_t *set, bag_of_words_t *bag) {
    delete_files_set(files);
    delete_set(set);
    delete_bag(bag);
}