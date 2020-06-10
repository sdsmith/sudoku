#!/bin/bash

BUILD_MODE=""

if [[ ! -z "$1" ]] ; then
    case "$1" in
        "debug") BUILD_MODE="-DCMAKE_BUILD_TYPE=Debug" ;;
    esac
fi

mkdir -p artifacts >/dev/null
pushd . >/dev/null
cd artifacts >/dev/null
# Config step: point to CMakeLists.txt
cmake ${BUILD_MODE} ..
# Build: point to cache from config
cmake --build .
popd >/dev/null
