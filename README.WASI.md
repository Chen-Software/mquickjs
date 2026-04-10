# MicroQuickJS WASI 0.2 Component

This project ports MicroQuickJS to a WASI 0.2 WebAssembly Component.

## Exported Interface

The component exports an `eval` function via the following WIT definition:

```wit
package local:microquickjs;

world microquickjs {
    export eval: func(code: string) -> result<string, string>;
}
```

## Building

Prerequisites:
- WASI SDK (set `WASI_SDK_PATH`)
- `wit-bindgen`
- `wasm-tools`
- `curl`

Run the build:
```bash
make -f Makefile.wasi
```

The final component will be at `build/microquickjs.component.wasm`.

## Usage

You can run the component using a WASM runtime like `wasmtime`:

```bash
wasmtime build/microquickjs.component.wasm "2 + 2"
# Output: 4
```

## Known Limitations

- **Error Handling**: Currently, JavaScript `throw` or syntax errors cause the component to trap (`unreachable`) because `setjmp/longjmp` are stubbed with `abort()` for WASI compatibility.
- **Filesystem/Network**: No access to guest filesystem or network.
- **Persistent State**: Each `eval` call uses a fresh JS engine instance.
- **Large Strings**: Result strings up to 100KB have been verified.
