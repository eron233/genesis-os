from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
CHECK = ROOT / "tools" / "check_multiboot.py"
ELF = ROOT / "build" / "genesis.elf"

class ElfVerifierTest(unittest.TestCase):
    def check(self, data):
        with tempfile.TemporaryDirectory() as tmp:
            target = Path(tmp) / "kernel.elf"
            target.write_bytes(data)
            return subprocess.run([sys.executable, str(CHECK), str(target)],
                                  text=True, capture_output=True, check=False)

    def test_unchanged_elf(self):
        result = self.check(ELF.read_bytes())
        self.assertEqual(result.returncode, 0, result.stderr)
    def test_invalid_magic(self):
        self.assertNotEqual(self.check(b"garbage").returncode, 0)
    def test_invalid_machine(self):
        blob = bytearray(ELF.read_bytes())
        blob[18:20] = (40).to_bytes(2, "little")
        self.assertNotEqual(self.check(blob).returncode, 0)
    def test_invalid_entry_address(self):
        blob = bytearray(ELF.read_bytes())
        blob[24:32] = (1 << 33).to_bytes(8, "little")
        self.assertNotEqual(self.check(blob).returncode, 0)
    def test_truncated_load_segment(self):
        blob = bytearray(ELF.read_bytes())
        phoff = int.from_bytes(blob[32:40], "little")
        blob[phoff + 32:phoff + 40] = (4096).to_bytes(8, "little")
        self.assertNotEqual(self.check(blob).returncode, 0)
    def test_missing_multiboot_header(self):
        blob = bytearray(ELF.read_bytes())
        offset = blob.find(bytes.fromhex("02b0ad1b"))
        self.assertGreaterEqual(offset, 0)
        blob[offset:offset+4] = b"FAIL"
        self.assertNotEqual(self.check(blob).returncode, 0)

if __name__ == "__main__":
    unittest.main()
