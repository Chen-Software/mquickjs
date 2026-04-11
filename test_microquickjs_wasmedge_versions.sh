#!/bin/bash
COMPONENT="build/microquickjs.component.wasm"
TIMESTAMP=$(date +%Y%m%d_%H%M%S)
RESULTS_FILE="test_results_${TIMESTAMP}.txt"

declare -A VERSIONS=(
  ["0.14.1"]="/home/jules/wasmedge-0.14.1/bin/wasmedge"
  ["0.16.1"]="/home/jules/wasmedge-0.16.1/bin/wasmedge"
  ["0.17.0-alpha.1"]="/home/jules/wasmedge-alpha/bin/wasmedge"
)

{
  echo "MicroQuickJS WasmEdge Compatibility Matrix"
  echo "=========================================="
  for v in "${!VERSIONS[@]}"; do
    bin="${VERSIONS[$v]}"
    echo "Testing $v ($bin)..."
    if [ ! -f "$bin" ]; then echo "$v: Not found"; continue; fi
    OUT=$($bin --enable-component "$COMPONENT" eval "2+2" 2>&1 || true)
    if echo "$OUT" | grep -q "4"; then echo "$v: ✅ PASS";
    else echo "$v: ❌ FAIL (Error: $(echo "$OUT" | head -n 1))"; fi
  done
} | tee "$RESULTS_FILE"
