import unittest
from tools.qemu_smoke import MARKERS, verify_markers

class MarkerTest(unittest.TestCase):
    def test_all_markers(self):
        verify_markers("\r\n".join(MARKERS) + "\r\n")
    def test_missing_idt(self):
        with self.assertRaises(ValueError):
            verify_markers("\n".join(m for m in MARKERS if "IDT" not in m))
    def test_wrong_order(self):
        with self.assertRaises(ValueError):
            verify_markers("\n".join(reversed(MARKERS)))
    def test_guest_panic(self):
        with self.assertRaises(ValueError):
            verify_markers("\n".join(MARKERS) + "\nGENESIS_PANIC")

if __name__ == "__main__":
    unittest.main()
