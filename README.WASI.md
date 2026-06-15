# MicroQuickJS WASI 0.2 Component

This project ports MicroQuickJS to a WASI 0.2 WebAssembly Component.

## Exported Interface

The component exports the `engine` interface via the following WIT definition:

```wit
package local:microquickjs;

interface engine {
    resource js-value {
        is-int:       func() -> bool;
        is-bool:      func() -> bool;
        is-null:      func() -> bool;
        is-undefined: func() -> bool;
        is-exception: func() -> bool;
        is-number:    func() -> bool;
        is-string:    func() -> bool;
        is-error:     func() -> bool;
        is-function:  func() -> bool;

        to-string:  func() -> string;
        to-int32:   func() -> s32;
        to-float64: func() -> f64;

        get-property: func(name: string) -> js-value;
        set-property: func(name: string, val: borrow<js-value>);

        call: func(args: list<borrow<js-value>>) -> js-value;
    }

    new-int32:  func(val: s32)    -> js-value;
    new-float64: func(val: f64)   -> js-value;
    new-bool:   func(val: bool)   -> js-value;
    new-string: func(val: string) -> js-value;
    new-object: func()            -> js-value;
    new-array:  func()            -> js-value;

    get-global-object: func() -> js-value;

    eval: func(code: string) -> result<string, string>;
}

world microquickjs {
    export engine;
}
```

## Building

Prerequisites:
- **WASI SDK**: [https://github.com/WebAssembly/wasi-sdk](https://github.com/WebAssembly/wasi-sdk) (Set `WASI_SDK_PATH` to your installation directory).
- **Tooling**: `wit-bindgen`, `wasm-tools`, `gcc` (for native helper).
- **Adapter**: `wasi_snapshot_preview1.reactor.wasm` should be present in the root directory.

Run the build:
```bash
make -f Makefile.wasi
```

The final component will be at `build/microquickjs.component.wasm`.

## Architecture & Implementation

- **Arena Memory**: Uses a fixed 4MiB static arena for JS heap, ensuring predictable memory footprint.
- **Error Handling**: Uses the WebAssembly Exception Handling proposal for robust `setjmp/longjmp` support.
- **Memory Safety**: Uses the Component Model Canonical ABI with `cabi_realloc` for all host-returned strings.
