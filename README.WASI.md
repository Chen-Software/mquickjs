# MicroQuickJS WASI Component

This is a WebAssembly Component built from MicroQuickJS, exporting a JavaScript evaluation function.

## Interface

The component exports a single function:
- `eval(code: string) -> string`

It returns the string representation of the result, or an error message starting with `Error: ` if a runtime exception occurs.

## Building

Run `./build.sh` to build the component. It requires the WASI SDK 24+ and standard WebAssembly component tools (`wit-bindgen`, `wasm-tools`).

## Runtime Requirements

This component is built with modern WebAssembly Exception Handling and SJLJ support (`-fwasm-exceptions`). It requires a runtime that supports these proposals.

For **Wasmtime**, run with:
```bash
wasmtime run -W all-proposals=y --invoke eval microquickjs.component.wasm '"1 + 1"'
```

## Status

- **Valid JavaScript**: Works perfectly.
- **Runtime Errors (e.g., `null.x`)**: Handled correctly, returns an error string.
- **Syntax Errors (e.g., `1 +`)**: May trigger a WebAssembly trap depending on the runtime's implementation of the Exception Handling proposal within the Component Model.
