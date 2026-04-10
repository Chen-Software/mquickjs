# WasmEdge Compatibility Report

## Summary
I have tested the MicroQuickJS WASI Component against multiple versions of WasmEdge. While the component is spec-compliant and verified on Wasmtime, it currently fails to run on WasmEdge due to incomplete Component Model support in that runtime.

## Versions Tested
- **WasmEdge 0.14.1**: Fails with `malformed name (0x50b)`.
- **WasmEdge 0.17.0-alpha.1**: Fails with `instantiation failed: unknown import (0x302)` for `wasi:cli/environment@0.2.0`.

## Spec Compliance
- **wasm-tools validate**: ✅ Passes. The component is syntactically correct and adheres to the Component Model specification.
- **Wasmtime 23.0.1**: ✅ Passes. All tests (arithmetic, strings, large marshalling) pass as expected on the reference implementation.

## Conclusion
The issue lies within WasmEdge's experimental Component Model validation and instantiation logic. At this time, it is recommended to use **Wasmtime** for running the MicroQuickJS component. Compatibility with WasmEdge is expected to improve as their implementation matures towards a stable 0.17.0 release.
