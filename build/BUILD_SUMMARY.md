# MicroQuickJS WASI Component Build Summary

## Environment
- WASI SDK: 25.0
- wit-bindgen: 0.55.0
- wasm-tools: 1.246.2
- WasmEdge: 0.14.1 (Verification target)
- Wasmtime: 29.0.1 (Fallback verification target)

## Build Artifacts
- `build/core.wasm`: Core WebAssembly module.
- `build/embedded.wasm`: Core module with WIT metadata embedded.
- `build/microquickjs.component.wasm`: Final WASI 0.2 component.

## WasmEdge Maturity Status (Issue #4236)
- **Component Model support:** 🔶 Partial
- **Simple types (string):** ✅ Stable
- **Result types:** 🔶 Partial
- **String marshalling:** 🔶 Partial (large strings may expose bugs)

## Known Runtime Limitations
- **Wasmtime v29.0.1:** Fails to parse the module with `exceptions proposal not enabled` at the `tags` section offset, despite `-W all-proposals=y`. This appears to be a regression or limitation in component-level exception handling support in this version.
- **WasmEdge v0.14.1:** Fails with `malformed name` (Code: 0x50b) when using `wasmedge run --enable-all`. This indicates ongoing stabilization of the Component Model parser for WASI 0.2.

## Mitigation & Recommendations
- Use **WasmEdge 0.15.0+** once available for improved Component Model stability.
- Ensure host runtimes strictly support the **WebAssembly Exception Handling** proposal for MicroQuickJS's internal `setjmp/longjmp` usage.
- Monitor WasmEdge issue #4236 for updates on `result<T, E>` and large string marshalling.
