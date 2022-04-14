#!/usr/bin/sh

[ ! -d build ] && mkdir build
[ ! -d plots ] && mkdir plots
cd build

case "$1" in
  "" | "d" | "debug")
		echo Building in debug mode
		cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug ..
    break ;;
  "r" | "release")
		echo Building in release mode
		cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Release ..
    break ;;
esac

cmake --build .
