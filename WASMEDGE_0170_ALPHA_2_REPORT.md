# WasmEdge 0.17.0-alpha.2 Compatibility Report

## Summary
Tested the MicroQuickJS WASI Component against WasmEdge 0.17.0-alpha.2 (released 2026-04-10).

## Results
- **Validation:** Bypassed the `0x50b` validator bug seen in 0.14.1.
- **Instantiation:** FAILED with `unknown import, Code: 0x302`.
- **Missing Import:** `wasi:cli/environment@0.2.0`.

## Analysis
The component uses the standard WASI Preview 1 adapter (Command profile) to bridge legacy WASI calls. This adapter requires `wasi:cli/environment@0.2.0`.
WasmEdge 0.17.0-alpha.2 implements parts of the Component Model but does not yet provide the full set of WASI 0.2 imports required by standard adapters.

## Recommendations
1. **Primary Runtime:** Use **Wasmtime 23.0.1+**. It is fully compatible and verified.
2. **WasmEdge Support:** Awaiting stable 0.17.0 release or further alpha updates that include `wasi:cli/environment`.
