#!/bin/bash

export PATH=$HOME/wasmedge-0.14.1/bin:$PATH
echo "Testing WasmEdge 0.14.1..."
wasmedge --enable-component --reactor build/microquickjs.component.wasm eval "2+2" 2>&1 | grep "Code"

export PATH=$HOME/wasmedge-alpha/bin:$PATH
echo "Testing WasmEdge 0.17.0-alpha.1..."
wasmedge --enable-component --reactor build/microquickjs.component.wasm eval "2+2" 2>&1 | grep "Code"
