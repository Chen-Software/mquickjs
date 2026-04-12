#!/bin/bash
set -e

# Build the component using Makefile.wasi
# This handles both native tool compilation and WASM cross-compilation
make -f Makefile.wasi clean
make -f Makefile.wasi
