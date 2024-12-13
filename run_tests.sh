#!/bin/bash
g++ -std=c++17 -o scheduler_tests.o \
    tests/scheduler_tests.cpp simpletest_test/simpletest/simpletest.cpp context/context.s \
    -Iinclude -Isimpletest_test/simpletest \
    -lstdc++

if [ $? -eq 0 ]; then
    ./scheduler_tests.o
else
    echo "Build failed."
fi
