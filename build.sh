#!/bin/bash
set -e

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <platform>"
    exit 1
fi

rm -r build/ || true
mkdir build

if [ "$1" = "windows" ]; then
    USE_MINGW_TOOLCHAIN="-DCMAKE_TOOLCHAIN_FILE=extern/llvm-mingw.cmake"
    cp extern/libc++.dll extern/libunwind.dll build
else
    USE_MINGW_TOOLCHAIN=""
fi

cmake -S. -Bbuild -H. -DTARGET_PLATFORM="$1" $USE_MINGW_TOOLCHAIN
cmake --build build

cp build/compile_commands.json . || true
