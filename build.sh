#!/usr/bin/sh

[ ! -d build ] && mkdir build
[ ! -d plots ] && mkdir plots
cd build
cmake ..
cmake --build .
