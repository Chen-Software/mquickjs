# MicroQuickJS WASI Component

This is a port of MicroQuickJS to a WASI 0.2 WebAssembly Component.

## Features
- Exports `eval(code: string) -> result<string, string>`.
- Uses a singleton JS context for persistent state between calls.
- Aggressively optimized for size using `-Oz`.

## Build
Requires WASI SDK 25.0 and `wasm-tools`, `wit-bindgen`.
```bash
make -f Makefile.wasi
```

## Usage
### Wasmtime
Current versions of Wasmtime (v29) require the Exceptions proposal for `setjmp/longjmp` support used by the engine.
```bash
wasmtime run -W all-proposals=y build/microquickjs.component.wasm --invoke eval "1+1"
```

## Limitations
- **WasmEdge Compatibility:** Currently fails in WasmEdge 0.14.1 with `malformed name (0x50b)` during component validation. This is a known maturity issue in WasmEdge's Component Model implementation (Ref: WasmEdge Issue #4236).
- **No Filesystem/Network:** The component is a pure logic reactor.
- **Exceptions:** Requires the WebAssembly Exceptions proposal.
