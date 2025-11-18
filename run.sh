#!/bin/bash

g++ -std=c++98 longasst.cpp -o longasst

if [ $? -eq 0 ]; then
    echo "Compilation successful!"
    ./longasst
else
    echo "Compilation failed!"
fi
