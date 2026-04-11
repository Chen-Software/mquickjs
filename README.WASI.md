# MicroQuickJS WASI Component

This is a port of MicroQuickJS to a WASI 0.2 WebAssembly Component.

## Features
- Exports a complete JS engine interface via the `local:microquickjs/engine` interface.
- Includes `js-value` resource for granular value manipulation.
- Supports type checking, conversions, property access, and function calling.
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

## Complete C API Support
The component now exports the following functions and resource methods:

### Engine Functions
- `eval(code: string) -> result<string, string>`
- `new-int32(val: s32) -> js-value`
- `new-float64(val: f64) -> js-value`
- `new-bool(val: bool) -> js-value`
- `new-string(val: string) -> js-value`
- `new-object() -> js-value`
- `new-array() -> js-value`
- `get-global-object() -> js-value`

### JS-Value Methods
- `is-int()`, `is-bool()`, `is-null()`, `is-undefined()`, `is-exception()`, `is-number()`, `is-string()`, `is-error()`, `is-function()`
- `to-string()`, `to-int32()`, `to-float64()`
- `get-property(name: string) -> js-value`
- `set-property(name: string, val: borrow<js-value>)`
- `call(args: list<borrow<js-value>>) -> js-value`

## Limitations
- **WasmEdge Compatibility:** Currently fails in WasmEdge 0.14.1 with `malformed name (0x50b)` during component validation. This is a known maturity issue in WasmEdge's Component Model implementation (Ref: WasmEdge Issue #4236).
- **WAMR Compatibility:** WAMR does not support the Component Model. The `build/core.wasm` module is binary-compatible with WAMR via its C embedding API but lacks wit-bindgen host glue.
- **Exceptions:** Requires the WebAssembly Exceptions proposal.
