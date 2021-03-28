import random
from os import listdir, remove

data_file_path = "../data/file"
generating_path = "../generated_data"

def prepare_words():
    words = []
    with open(data_file_path, "r") as file:
        words = file.read()

    words = words.split()
    return words

str_len = 10

if __name__ == '__main__':
    dir_files = listdir(generating_path)
    if len(dir_files) != 0:
        for f in dir_files:
            remove(generating_path + '/' + f)

    files_amount = random.randint(1, 25)
    list_of_words = prepare_words()
    words_min = 1
    words_max = len(list_of_words)
    for i in range(1, files_amount):
        words_amount = random.randint(words_min, words_max)
        with open(generating_path + '/' + str(i), "w") as file:
            for j in range(0, str_len):
                [file.write(random.choice(list_of_words) + " ") for i in range(int(words_amount / str_len))]

        print(f"in file {i} writen {words_amount} words")

