# MicroQuickJS WASI 0.2 Component

This project has been ported to a WebAssembly Component Model component.

## WIT Interface

The component exports an `eval` function:

```wit
package local:microquickjs;

world microquickjs {
    export eval: func(code: string) -> string;
}
```

## Building

Requires:
- `wasi-sdk` (v24.0)
- `wit-bindgen` (v0.35.0)
- `wasm-tools`
- `wasi_snapshot_preview1.reactor.wasm` adapter

Build steps:
1. Generate stdlib headers using native `mquickjs_build_native`.
2. Generate C bindings using `wit-bindgen`.
3. Compile object files using wasi-sdk clang with `-Oz`.
4. Link to `core.wasm`.
5. Embed WIT and lift to component.

A `Makefile.wasi` is provided to automate these steps.

## Verification

The component can be inspected using:
```sh
wasm-tools component wit microquickjs.component.wasm
```

Current limitation: WAMR (iwasm) support for the Component Model CLI flag (`\-\-component`) is still in development/experimental branches.

Verification can be done against a host implementing the WAMR or Wasmtime embedding API with Component Model support.
