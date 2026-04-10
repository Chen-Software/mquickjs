# Testing Newer WasmEdge Versions

This document provides detailed instructions for testing the MicroQuickJS component on new WasmEdge releases.

## Automated Testing
Run the provided script:
```bash
./test_microquickjs_wasmedge_versions.sh
```

## Manual Testing Steps
1. **Download**: Obtain the latest Linux x86_64 tarball from the WasmEdge releases page.
2. **Extract**: `tar -xzf WasmEdge-<version>.tar.gz`
3. **Set Path**: `export PATH=$(pwd)/WasmEdge-<version>/bin:$PATH`
4. **Run**: `wasmedge --enable-component --reactor build/microquickjs.component.wasm eval "2+2"`

## Verification with Wasmtime
If WasmEdge fails, always verify the component still works in Wasmtime to ensure it's a runtime-specific issue:
```bash
wasmtime build/microquickjs.component.wasm "2 + 2"
```
