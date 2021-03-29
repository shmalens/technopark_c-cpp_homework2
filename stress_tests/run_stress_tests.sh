#!/usr/bin/env bash

echo "TEST1"
python3 ../stress_tests/generator.py
python3 ../stress_tests/timer.py
python3 ../stress_tests/diff.py
echo "end TEST1"

echo "TEST2"
python3 ../stress_tests/generator.py
python3 ../stress_tests/timer.py
python3 ../stress_tests/diff.py
echo "end TEST2"

echo "TEST3"
python3 ../stress_tests/generator.py
python3 ../stress_tests/timer.py
python3 ../stress_tests/diff.py
echo "end TEST2"
