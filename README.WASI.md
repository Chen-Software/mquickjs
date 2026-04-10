# MicroQuickJS WASI Component

This is a port of MicroQuickJS to a WASI 0.2 Component.

## Build

Prerequisites:
- WASI SDK 25+
- wit-bindgen
- wasm-tools

Run:
```bash
make -f Makefile.wasi
```

## Usage

The component exports an `eval` function:
```wit
eval: func(code: string) -> result<string, string>;
```

## Limitations

### Error Handling (setjmp/longjmp)

MicroQuickJS uses `setjmp`/`longjmp` for error handling during parsing (e.g., syntax errors).
Standard WASI 0.2 does not yet fully support these primitives without the WebAssembly Exception Handling proposal.

In this port, `longjmp` is stubbed to call `abort()`.
- **Valid JavaScript:** Executes normally and returns the result as a string.
- **Syntax Errors / Parser Errors:** Will cause the component to trap (`unreachable`).
- **Runtime Errors:** Correctly handled via `JS_GetException` and returned as an `Err` result.

### Other WASI Limitations
- No filesystem access ( `load()` is disabled).
- No subprocesses (`system()`, `fork()` are not available).
- Timers (`setTimeout`) are not currently supported in the component export.
