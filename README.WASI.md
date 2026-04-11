# MicroQuickJS WASI Component

This is a port of MicroQuickJS to a WASI 0.2 WebAssembly Component.

## Features
- Exports a complete JS engine interface via the `local:microquickjs/engine` interface.
- Includes `js-value` resource for granular value manipulation.
- Supports type checking, conversions, property access, and function calling.
- Uses a singleton JS context for persistent state between calls.
- Aggressively optimized for size using `-Oz`.

## Documentation
For deeper technical details, please refer to:
- [DESIGN.md](DESIGN.md) — Detailed architecture, design decisions, and implementation notes.
- [REQUIREMENTS.md](REQUIREMENTS.md) — Formal requirements and acceptance criteria for the WASI port.

## Build Environment
Requires the following tools:
- **WASI SDK:** 25.0
- **wit-bindgen:** 0.55.0
- **wasm-tools:** 1.246.2

### Build Command
```bash
make -f Makefile.wasi
```

### Build Artifacts
- `build/core.wasm`: Core WebAssembly module.
- `build/embedded.wasm`: Core module with WIT metadata embedded.
- `build/microquickjs.component.wasm`: Final WASI 0.2 component.

## Usage
### Wasmtime
Current versions of Wasmtime (v29) require the Exceptions proposal for `setjmp/longjmp` support used by the engine.
```bash
wasmtime run -W all-proposals=y build/microquickjs.component.wasm --invoke eval "1+1"
```

## C API Compatibility Table

The following table shows the correspondence between the MicroQuickJS C API and the WASI Component Model exports.

| MicroQuickJS C API | WASI Component Export | Ported |
| :--- | :--- | :---: |
| `JS_Eval` | `eval` | ✅ |
| `JS_NewInt32` | `new-int32` | ✅ |
| `JS_NewFloat64` | `new-float64` | ✅ |
| `JS_NewBool` | `new-bool` | ✅ |
| `JS_NewString` | `new-string` | ✅ |
| `JS_NewObject` | `new-object` | ✅ |
| `JS_NewArray` | `new-array` | ✅ |
| `JS_GetGlobalObject` | `get-global-object` | ✅ |
| `JS_IsInt` | `js-value.is-int` | ✅ |
| `JS_IsBool` | `js-value.is-bool` | ✅ |
| `JS_IsNull` | `js-value.is-null` | ✅ |
| `JS_IsUndefined` | `js-value.is-undefined` | ✅ |
| `JS_IsException` | `js-value.is-exception` | ✅ |
| `JS_IsNumber` | `js-value.is-number` | ✅ |
| `JS_IsString` | `js-value.is-string` | ✅ |
| `JS_IsError` | `js-value.is-error` | ✅ |
| `JS_IsFunction` | `js-value.is-function` | ✅ |
| `JS_ToString` | `js-value.to-string` | ✅ |
| `JS_ToInt32` | `js-value.to-int32` | ✅ |
| `JS_ToNumber` | `js-value.to-float64` | ✅ |
| `JS_GetPropertyStr` | `js-value.get-property` | ✅ |
| `JS_SetPropertyStr` | `js-value.set-property` | ✅ |
| `JS_Call` | `js-value.call` | ✅ |
| `JS_NewContext` | N/A (Internal Singleton) | 🛠️ |
| `JS_FreeContext` | N/A (Internal) | 🛠️ |
| `JS_Throw` | N/A (Host handles result) | 🛠️ |
| `JS_GC` | N/A (Internal) | 🛠️ |
| `JS_LoadBytecode` | N/A | ❌ |

## Complete WIT Interface Support
The component exports the following functions and resource methods:

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

## Runtime Limitations & WasmEdge Status

### WasmEdge Maturity Status (Issue #4236)
- **Component Model support:** 🔶 Partial
- **Simple types (string):** ✅ Stable
- **Result types:** 🔶 Partial
- **String marshalling:** 🔶 Partial (large strings may expose bugs)

### Known Runtime Limitations
- **Wasmtime v29.0.1:** Fails to parse the module with `exceptions proposal not enabled` at the `tags` section offset, despite `-W all-proposals=y`. This appears to be a regression or limitation in component-level exception handling support in this version.
- **WasmEdge v0.14.1:** Fails with `malformed name (Code: 0x50b)` when using `wasmedge run --enable-all`. This indicates ongoing stabilization of the Component Model parser for WASI 0.2.
- **WAMR Compatibility:** WAMR does not support the Component Model. The `build/core.wasm` module is binary-compatible with WAMR via its C embedding API but lacks wit-bindgen host glue.

## Mitigation & Recommendations
- Use **WasmEdge 0.16.1** (latest stable) or **0.17.0-alpha.x** for the latest Component Model improvements.
- Note: WasmEdge 0.17.x is currently in alpha (latest: 0.17.0-alpha.2).
- Ensure host runtimes strictly support the **WebAssembly Exception Handling** proposal for MicroQuickJS's internal `setjmp/longjmp` usage.
- Monitor WasmEdge issue #4236 for updates on `result<T, E>` and large string marshalling.
