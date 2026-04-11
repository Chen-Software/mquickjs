#!/bin/bash
set -e

# Resolve WASI_SDK_PATH
if [ -z "$WASI_SDK_PATH" ]; then
    if [ -d "/opt/wasi-sdk" ]; then
        export WASI_SDK_PATH="/opt/wasi-sdk"
    elif [ -d "/home/jules/wasi-sdk" ]; then
        export WASI_SDK_PATH="/home/jules/wasi-sdk"
    elif [ -d "packages/wasi-sdk" ]; then
        export WASI_SDK_PATH="$(pwd)/packages/wasi-sdk"
    else
        echo "ERROR: WASI_SDK_PATH not set"
        exit 1
    fi
fi

make -f Makefile.wasi clean WASI_SDK_PATH="$WASI_SDK_PATH"
make -f Makefile.wasi WASI_SDK_PATH="$WASI_SDK_PATH"
