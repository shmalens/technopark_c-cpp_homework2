#include <dirent.h>
#include <stdlib.h>

#include "files.h"
#include "files_processing.h"

//file_data_t *create_file_data() {
//    file_data_t *new_data = (file_data_t *) malloc(sizeof(file_data_t));
//}

files_t *read_files(DIR *directory) {
    if (directory == NULL) {
        return NULL;
    }

    files_t *directory_files_collection = create_files_collection();
    if (directory_files_collection == NULL) {
        return NULL;
    }

    struct dirent *entry_dir = NULL;
    while ((entry_dir = readdir(directory)) != NULL) {
        if (entry_dir->d_type == DT_REG) {
            if (add_file(directory_files_collection, entry_dir->d_name) != 0) {
                delete_files_collection(directory_files_collection);
                return NULL;
            }
        }
    }

    return directory_files_collection;
}