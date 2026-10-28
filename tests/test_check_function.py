import json
import struct
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

TOOLS_DIR = Path(__file__).resolve().parents[1] / "tools"
sys.path.insert(0, str(TOOLS_DIR))

import check_function


def elf(body=b"\x00\x20\x70\x47", address=0x02001001, kind=2, undefined=False, duplicate=False):
    strings = b"\x00target\x00external\x00"
    symbols = bytes(16) + struct.pack("<IIIBBH", 1, address, len(body), 0x12, 0, 1)
    if undefined:
        symbols += struct.pack("<IIIBBH", 8, 0, 0, 0x10, 0, 0)
    if duplicate:
        symbols += symbols[16:32]
    text_offset = 52 + 4 * 40
    strings_offset = text_offset + len(body)
    symbols_offset = strings_offset + len(strings)
    header = struct.pack("<16sHHIIIIIHHHHHH", b"\x7fELF\x01\x01\x01".ljust(16, b"\x00"), kind, 40, 1, 0, 0, 52, 0, 52, 0, 0, 40, 4, 0)
    sections = bytes(40)
    sections += struct.pack("<10I", 0, 1, 6, address & ~3, text_offset, len(body), 0, 0, 2, 0)
    sections += struct.pack("<10I", 0, 3, 0, 0, strings_offset, len(strings), 0, 0, 1, 0)
    sections += struct.pack("<10I", 0, 2, 0, 0, symbols_offset, len(symbols), 2, 1, 4, 16)
    return header + sections + body + strings + symbols


class FunctionCheckTests(unittest.TestCase):
    def compare(self, first, second):
        return check_function.compare_functions(check_function.read_function(first, "target"), check_function.read_function(second, "target"))

    def test_identical_linked_thumb_function_passes(self):
        self.assertEqual(self.compare(elf(), elf())["status"], "pass")

    def test_two_byte_thumb_function_is_not_rounded_down_to_a_word(self):
        result = self.compare(elf(b"\x70\x47"), elf(b"\x70\x46"))
        self.assertEqual(result["status"], "fail")
        self.assertEqual(result["different_bytes"], 1)
        self.assertEqual(result["first_difference"], "0x2001001")

    def test_different_linked_call_destination_is_not_masked(self):
        result = self.compare(elf(b"\x00\xf0\x02\xf8\x70\x47"), elf(b"\x00\xf0\x03\xf8\x70\x47"))
        self.assertEqual(result["status"], "fail")
        self.assertIn("linked function bytes differ", result["problems"])

    def test_same_bytes_at_a_different_address_or_mode_fail(self):
        for address in [0x02001005, 0x02001000]:
            with self.subTest(address=address):
                self.assertEqual(self.compare(elf(), elf(address=address))["status"], "fail")

    def test_shorter_function_fails(self):
        result = self.compare(elf(), elf(b"\x00\x20"))
        self.assertEqual(result["status"], "fail")
        self.assertIn("size differs", result["problems"])

    def test_unlinked_objects_and_unresolved_symbols_are_rejected(self):
        for source in [elf(kind=1), elf(undefined=True)]:
            with self.assertRaises(ValueError):
                check_function.read_function(source, "target")

    def test_ambiguous_missing_and_truncated_functions_are_rejected(self):
        for source, name in [(elf(duplicate=True), "target"), (elf(), "missing"), (elf()[:-1], "target"), (b"", "target")]:
            with self.assertRaises(ValueError):
                check_function.read_function(source, name)

    def test_cli_records_failure_and_returns_nonzero(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            reference = root / "reference.elf"
            candidate = root / "candidate.elf"
            reference.write_bytes(elf())
            candidate.write_bytes(elf(b"\x01\x20\x70\x47"))
            report = root / "checks" / "result.json"
            command = [sys.executable, "-I", str(TOOLS_DIR / "check_function.py"), str(reference), str(candidate), "target", "--report", str(report)]
            result = subprocess.run(command, capture_output=True, text=True)
            self.assertNotEqual(result.returncode, 0)
            self.assertEqual(json.loads(report.read_text())["status"], "fail")


if __name__ == "__main__":
    unittest.main()
