#!/bin/bash
set -e
export WASI_SDK_PATH=/home/jules/wasi-sdk
make -f Makefile.wasi clean
make -f Makefile.wasi
# wasmtime 23.0.1 might need flags to enable exceptions if not enabled by default
wasmtime -W all-proposals=y build/microquickjs.component.wasm "2 + 2" | grep -q "4"
echo "✓ Success"
