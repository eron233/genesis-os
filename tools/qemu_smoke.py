#!/usr/bin/env python3
"""Real guest smoke test; absence of QEMU is NOT a successful result."""
from pathlib import Path
import argparse
import os
import select
import shutil
import subprocess
import sys
import time

MARKERS = (
    "GENESIS_BOOT_STAGE0: x86_64 long mode",
    "GENESIS_IDT_CHECK: OK",
    "GENESIS_SCHEDULER_CHECK: OK",
    "GENESIS_KERNEL_READY: stage0 exception bootstrap",
)

def verify_markers(output: str) -> None:
    cursor = 0
    for marker in MARKERS:
        position = output.find(marker, cursor)
        if position < 0:
            raise ValueError(f"Missing or out-of-order marker: {marker}")
        cursor = position + len(marker)
    if "GENESIS_PANIC" in output or "CHECK: FAIL" in output:
        raise ValueError("Guest signaled panic or failed check")

def smoke(kernel: Path, seconds: float = 15) -> str:
    binary = shutil.which("qemu-system-x86_64")
    if binary is None:
        raise RuntimeError("QEMU is not installed: no real boot verification")
    command = [binary, "-machine", "accel=tcg", "-m", "256M",
               "-kernel", str(kernel), "-display", "none",
               "-serial", "stdio", "-monitor", "none", "-no-reboot"]
    proc = subprocess.Popen(command, stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT, bufsize=0)
    assert proc.stdout is not None
    result = bytearray()
    deadline = time.monotonic() + seconds
    try:
        while time.monotonic() < deadline:
            ready, _, _ = select.select([proc.stdout], [], [], 0.2)
            if ready:
                data = os.read(proc.stdout.fileno(), 65536)
                if not data:
                    break
                result.extend(data)
                if len(result) > 1024 * 1024:
                    raise RuntimeError("Excessive guest output")
                decoded = result.decode("utf-8", errors="replace")
                if MARKERS[-1] in decoded:
                    verify_markers(decoded)
                    return decoded
            if proc.poll() is not None:
                break
        decoded = result.decode("utf-8", errors="replace")
        verify_markers(decoded)
        return decoded
    finally:
        if proc.poll() is None:
            proc.terminate()
        try:
            proc.wait(timeout=2)
        except subprocess.TimeoutExpired:
            proc.kill()
            proc.wait(timeout=2)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("kernel", type=Path)
    parser.add_argument("--timeout", type=float, default=15)
    args = parser.parse_args()
    try:
        out = smoke(args.kernel, args.timeout)
    except (ValueError, RuntimeError, OSError) as exc:
        print(f"QEMU SMOKE FAILED: {exc}", file=sys.stderr)
        raise SystemExit(1) from exc
    print(out)
    print("QEMU SMOKE PASSED")
