#!/bin/bash

mkdir -p artifacts >/dev/null
pushd . >/dev/null
cd artifacts >/dev/null
# Config step: point to CMakeLists.txt
cmake ..
# Build: point to cache from config
cmake $* --build .
popd >/dev/null
