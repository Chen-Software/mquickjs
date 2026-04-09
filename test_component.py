import subprocess
import json
import sys

WASMTIME_BIN = "/home/jules/wasmtime-v29.0.1-x86_64-linux/wasmtime"

def test_eval(code):
    cmd = [
        WASMTIME_BIN,
        "run",
        "-W", "all-proposals=y",
        "build/microquickjs.component.wasm",
        "--invoke", "local:microquickjs/microquickjs#eval",
        code
    ]
    # print(f"Running: {' '.join(cmd)}")
    result = subprocess.run(cmd, capture_output=True, text=True)
    return result.stdout.strip(), result.stderr.strip()

print("Test 1: simple addition")
stdout, stderr = test_eval("1 + 2")
print(f"Stdout: {stdout}")
print(f"Stderr: {stderr}")

print("\nTest 2: error handling")
stdout, stderr = test_eval("nonexistent_var")
print(f"Stdout: {stdout}")
print(f"Stderr: {stderr}")
