#!/bin/bash
g++ -std=c++17 -o main.o main.cpp context/context.s

if [ $? -eq 0 ]; then
    chmod +x main.o
    ./main.o
else 
    echo "Build failed"
fi