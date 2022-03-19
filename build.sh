#!/usr/bin/sh

[ ! -d build ] && mkdir build
cd build
cmake ..
cmake --build . -j 4
