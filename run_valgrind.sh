#!/usr/bin/env bash

valgrind --tool=memcheck --leak-check=full --track-origins=yes  --show-leak-kinds=all ./parallel
valgrind --tool=memcheck --leak-check=full --track-origins=yes  --show-leak-kinds=all ./consistent