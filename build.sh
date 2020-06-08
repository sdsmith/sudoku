#!/bin/bash

if ! [[ -d "artifacts" ]]; then
    echo "WARNING: run config.sh"
    exit 1
fi

pushd . >/dev/null
cd artifacts
make $*
popd >/dev/null
