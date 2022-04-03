#!/usr/bin/sh

[ ! -d build ] && mkdir build
[ ! -d plots ] && mkdir plots
cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
cmake --build .
