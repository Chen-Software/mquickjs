# MicroQuickJS WASI 0.2 Component

This project ports MicroQuickJS to a WASI 0.2 WebAssembly Component.

## Exported Interface

The component exports an `eval` function via the following WIT definition:

```wit
package local:microquickjs;

world microquickjs {
    /// Evaluate JavaScript code and return result as string.
    /// On error (syntax, runtime), returns Err(error-message).
    export eval: func(code: string) -> result<string, string>;
}
```

## Building

Prerequisites:
- **WASI SDK**: [https://github.com/WebAssembly/wasi-sdk](https://github.com/WebAssembly/wasi-sdk) (Set `WASI_SDK_PATH` to your installation directory).
- **Tooling**: `wit-bindgen`, `wasm-tools`, `curl`.

Run the build:
```bash
make -f Makefile.wasi
```

The final component will be at `build/microquickjs.component.wasm`.

## Usage

You can run the component using a WASI 0.2 compliant runtime like `wasmtime`:

```bash
wasmtime build/microquickjs.component.wasm "2 + 2"
# Output: 4
```

## Runtime Compatibility

| Runtime | Version | Status | Notes |
| :--- | :--- | :--- | :--- |
| **Wasmtime** | 23.0.1+ | ✅ **Fully Supported** | Reference implementation. All tests pass. |
| **WasmEdge** | 0.17.0-alpha.2 | ❌ **Incompatible** | Fails instantiation due to missing `wasi:cli/environment@0.2.0`. |
| **WasmEdge** | 0.14.1 / 0.16.1 | ❌ **Broken** | Known validator bugs in experimental Component Model support. |

### Note on WasmEdge
While the component is 100% spec-compliant (verified via `wasm-tools component validate`), WasmEdge's current alpha implementation of the Component Model does not yet provide the mandatory WASI 0.2 imports required by the standard Preview 1 adapter. Wasmtime is the recommended runtime for this component.

## Architecture & Implementation

- **Error Handling**: Uses the WebAssembly Exception Handling proposal (`-mllvm -wasm-enable-sjlj`) for robust `setjmp/longjmp` support. JavaScript exceptions are caught and returned as the `err` variant of the WIT `result` type.
- **Portability**: Includes internal shims for WASI Preview 1 functions (`args`, `environ`, `fd`) to reduce external host dependencies and improve compatibility across different WASI 0.2 environments.
- **Memory Safety**: Uses the Component Model Canonical ABI with `cabi_realloc` for all host-returned strings.
- **Standard Library**: Fully integrated including Math, JSON, and RegExp support.
