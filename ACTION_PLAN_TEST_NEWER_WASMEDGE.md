# Action Plan: Testing Newer WasmEdge Versions

If a new version of WasmEdge is released, follow these steps to test compatibility:

1. **Download New Version**:
   ```bash
   curl -sSf https://raw.githubusercontent.com/WasmEdge/WasmEdge/master/utils/install_v2.sh | bash -s -- -v <version> -p ~/wasmedge-new
   ```

2. **Run Compatibility Script**:
   Update `test_microquickjs_wasmedge_versions.sh` to include the new path and run it.

3. **Verify with wasm-tools**:
   Always ensure `wasm-tools validate build/microquickjs.component.wasm` passes. If it does, and WasmEdge fails, it is likely a WasmEdge bug.

4. **Update Documentation**:
   Reflect findings in `WASMEDGE_COMPATIBILITY_REPORT.md`.
