#!/bin/bash
set -e

# Configuration
if [ -z "$WASI_SDK_PATH" ]; then
    if [ -d "/opt/wasi-sdk" ]; then
        WASI_SDK_PATH="/opt/wasi-sdk"
    elif [ -d "/home/jules/wasi-sdk" ]; then
        WASI_SDK_PATH="/home/jules/wasi-sdk"
    else
        echo "Error: WASI_SDK_PATH not set and not found in default locations."
        exit 1
    fi
fi

CC="${WASI_SDK_PATH}/bin/clang"

# Environment Check (Requirement 1)
MISSING_TOOLS=0
command -v wit-bindgen >/dev/null 2>&1 || { echo "Error: wit-bindgen not found on PATH"; MISSING_TOOLS=1; }
command -v wasm-tools >/dev/null 2>&1 || { echo "Error: wasm-tools not found on PATH"; MISSING_TOOLS=1; }
command -v wasmtime >/dev/null 2>&1 || { echo "Error: wasmtime not found on PATH"; MISSING_TOOLS=1; }

if [ ! -f "$CC" ]; then
    echo "Error: WASI SDK clang not found at $CC"
    MISSING_TOOLS=1
fi

if [ ! -f "wasi_snapshot_preview1.reactor.wasm" ]; then
    echo "Error: wasi_snapshot_preview1.reactor.wasm not found in workspace root"
    MISSING_TOOLS=1
fi

if [ $MISSING_TOOLS -ne 0 ]; then
    echo "Environment check failed."
    exit 1
fi

echo "=== Phase 1: Generate Stdlib Header ==="
cc -O2 -I. mqjs_stdlib.c mquickjs_build.c cutils.c -o mqjs_stdlib_native
./mqjs_stdlib_native -m32 > mqjs_stdlib.h
./mqjs_stdlib_native -m32 -a > mquickjs_atom.h

echo "=== Phase 2: WIT Bindings ==="
wit-bindgen c ./microquickjs.wit --world microquickjs

echo "=== Phase 3: Compile WASM Objects ==="
# Modern WASM Exception Handling and SJLJ flags
CFLAGS="-Oz -D_WASI_EMULATED_SIGNAL -I. --sysroot=${WASI_SDK_PATH}/share/wasi-sysroot -fwasm-exceptions -mllvm -wasm-enable-sjlj"

$CC $CFLAGS -c mquickjs.c      -o mquickjs.o
$CC $CFLAGS -c cutils.c        -o cutils.o
$CC $CFLAGS -c dtoa.c          -o dtoa.o
$CC $CFLAGS -c libm.c          -o libm.o
$CC $CFLAGS -c microquickjs.c  -o microquickjs.o
$CC $CFLAGS -c glue.c          -o glue.o
$CC $CFLAGS -c wasi_glue.c     -o wasi_glue.o

echo "=== Phase 4: Link core.wasm ==="
$CC \
  mquickjs.o cutils.o dtoa.o libm.o microquickjs.o glue.o wasi_glue.o \
  microquickjs_component_type.o \
  -Wl,--no-entry \
  -Wl,--export=cabi_realloc \
  -Wl,--export=__wasm_call_ctors \
  -lwasi-emulated-signal \
  -lsetjmp \
  -o core.wasm

echo "=== Phase 5: Assemble Component ==="
wasm-tools component embed ./microquickjs.wit --world microquickjs core.wasm --output embedded.wasm
# Skip validation if EH is not yet fully supported in wasm-tools validator
wasm-tools component new embedded.wasm --adapt wasi_snapshot_preview1=wasi_snapshot_preview1.reactor.wasm --output microquickjs.component.wasm || \
wasm-tools component new embedded.wasm --adapt wasi_snapshot_preview1=wasi_snapshot_preview1.reactor.wasm --skip-validation --output microquickjs.component.wasm

echo "=== Build Successful: microquickjs.component.wasm ==="
