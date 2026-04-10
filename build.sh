#!/bin/bash
set -e

# Phase 1: Native build
cd mquickjs
gcc -O2 -I. mqjs_stdlib.c mquickjs_build.c cutils.c -o mqjs_stdlib_native -lm
./mqjs_stdlib_native -m32 > ../mqjs_stdlib.h
./mqjs_stdlib_native -m32 -a > ../mquickjs_atom.h
cd ..

# Phase 2: Component build
make -f Makefile.wasi clean
make -f Makefile.wasi
