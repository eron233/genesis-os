#!/usr/bin/env python3
"""Static ELF64 / Multiboot1 checks. NOT equivalent to a real boot."""
from pathlib import Path
import struct
import sys

def validate(blob: bytes) -> None:
    if len(blob) < 64 or blob[:4] != b"\x7fELF":
        raise ValueError("Not an ELF file")
    if blob[4] != 2 or blob[5] != 1:
        raise ValueError("Expected ELF64 little-endian")
    kind, machine = struct.unpack_from("<HH", blob, 16)
    if kind != 2 or machine != 62:
        raise ValueError("Expected x86-64 ET_EXEC")
    entry, phoff = struct.unpack_from("<QQ", blob, 24)
    phentsize, phnum = struct.unpack_from("<HH", blob, 54)
    if phentsize < 56 or not phnum or phoff + phentsize * phnum > len(blob):
        raise ValueError("Invalid program headers")
    if entry >= (1 << 32):
        raise ValueError("Entry above 4GiB unsupported by 32-bit bootstrap")
    executable = False
    loadable = 0
    for i in range(phnum):
        off = phoff + i * phentsize
        kind, flags = struct.unpack_from("<II", blob, off)
        vaddr, paddr = struct.unpack_from("<QQ", blob, off + 16)
        filesz, memsz = struct.unpack_from("<QQ", blob, off + 32)
        offset = struct.unpack_from("<Q", blob, off + 8)[0]
        if kind == 1:
            loadable += 1
            if filesz > memsz or offset + filesz > len(blob):
                raise ValueError("Truncated PT_LOAD")
            if not memsz or paddr >= (1 << 30) or memsz > (1 << 30) - paddr:
                raise ValueError("PT_LOAD outside temporary 1GiB identity map")
            if (flags & 1) and vaddr <= entry < vaddr + memsz:
                executable = True
    if not loadable or not executable:
        raise ValueError("Entry point is not in executable PT_LOAD")
    magic = 0x1BADB002
    for offset in range(0, min(len(blob) - 12, 8192), 4):
        a, flags, checksum = struct.unpack_from("<III", blob, offset)
        if a == magic and (a + flags + checksum) & 0xFFFFFFFF == 0:
            print(f"Multiboot header valid at 0x{offset:x}; ELF64 entry OK")
            return
    raise ValueError("Missing Multiboot header in first 8KiB")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        raise SystemExit("Usage: check_multiboot.py kernel.elf")
    try:
        validate(Path(sys.argv[1]).read_bytes())
    except (OSError, ValueError) as exc:
        raise SystemExit(str(exc)) from exc
