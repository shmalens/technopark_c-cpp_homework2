#include <stdio.h>
#include <dirent.h>

#include "files.h"
#include "files_processing.h"

#define DIR_PATH "../data"

#define FILE_NAME1 "file1"
#define FILE_NAME2 "file1"
#define FILE_NAME3 "file3"
#define FILE_NAME4 "file4"

#include "words_storage.h"
#include <string.h>

int main() {
    words_storage_t *storage = create_storage("AB");
    if (storage == NULL) {
        return -1;
    }

    insert_in_storage(storage, "AAA");
    insert_in_storage(storage, "AAC");
    insert_in_storage(storage, "ABA");
    insert_in_storage(storage, "ACCA");

    show_storage(storage);

    delete_storage(storage);
    return 0;
}
