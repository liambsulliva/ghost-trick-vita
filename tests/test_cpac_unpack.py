import csv
import struct
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

TOOLS_DIR = Path(__file__).resolve().parents[1] / "tools"
sys.path.insert(0, str(TOOLS_DIR))

import cpac_unpack


def pack(kind, keys, payload):
    header = struct.pack("<II4sI4sI", 24, 2, b"YEK" + kind, 24, b"TAD" + kind, 24 + len(keys))
    return header + keys + payload


class PackTests(unittest.TestCase):
    def extract(self, data):
        folder = tempfile.TemporaryDirectory()
        self.addCleanup(folder.cleanup)
        root = Path(folder.name)
        source = root / "fixture.bin"
        source.write_bytes(data)
        out = root / "out"
        out.mkdir()
        result = subprocess.run([sys.executable, "-I", str(TOOLS_DIR / "cpac_unpack.py"), str(source), str(out)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)
        with (out / "fixture_entries.tsv").open() as f:
            rows = list(csv.DictReader(f, delimiter="\t"))
        return out, rows

    def test_four_byte_palette_keys_extract_distinct_entries(self):
        keys = struct.pack("<3I", 0, 0x100, 0x10100)
        out, rows = self.extract(pack(b"P", keys, b"A" * 32 + b"B" * 32))
        self.assertEqual([row["index"] for row in rows], ["1", "2"])
        self.assertEqual((out / rows[0]["name"]).read_bytes(), b"A" * 32)
        self.assertEqual((out / rows[1]["name"]).read_bytes(), b"B" * 32)

    def test_single_palette_key_is_not_discarded(self):
        out, rows = self.extract(pack(b"P", struct.pack("<I", 0x100), b"A" * 32))
        self.assertEqual([row["index"] for row in rows], ["0"])
        self.assertEqual((out / rows[0]["name"]).read_bytes(), b"A" * 32)

    def test_manifest_preserves_quote_characters_in_magic(self):
        keys = struct.pack("<2I", 0x100, 0x10100)
        out, rows = self.extract(pack(b"P", keys, b'"ABC' + b"A" * 28 + b"B" * 32))
        self.assertEqual([row["index"] for row in rows], ["0", "1"])
        self.assertEqual(rows[0]["magic"], '"ABC')

    def test_eight_byte_keys_keep_their_byte_offsets_and_sizes(self):
        keys = struct.pack("<6I", 0, 0, 0, 4, 4, 4)
        out, rows = self.extract(pack(b"B", keys, b"AAAABBBB"))
        self.assertEqual([row["index"] for row in rows], ["1", "2"])
        self.assertEqual((out / rows[0]["name"]).read_bytes(), b"AAAA")
        self.assertEqual((out / rows[1]["name"]).read_bytes(), b"BBBB")

    def test_palette_language_flag_is_not_a_compression_flag(self):
        out, rows = self.extract(pack(b"P", struct.pack("<I", 0x180), b"A" * 32))
        self.assertEqual((out / rows[0]["name"]).read_bytes(), b"A" * 32)
        self.assertEqual(rows[0]["localized"], "true")
        self.assertEqual(rows[0]["key_size"], "4")
        self.assertEqual(rows[0]["status"], "raw")

    def test_byte_key_language_flag_is_removed_from_the_offset(self):
        compressed = b"\x10\x04\x00\x00\x00ABCD"
        keys = struct.pack("<2I", 0x80000000, 0x80000000 | len(compressed))
        out, rows = self.extract(pack(b"B", keys, compressed))
        self.assertEqual((out / rows[0]["name"]).read_bytes(), b"ABCD")
        self.assertEqual(rows[0]["localized"], "true")
        self.assertEqual(rows[0]["status"], "lz")

    def test_entry_cannot_read_past_its_block(self):
        data = pack(b"B", struct.pack("<2I", 4, 8), b"AAAA")
        block = cpac_unpack.find_blocks(data)[0]
        with self.assertRaises(ValueError):
            cpac_unpack.read_entries(data, block, len(data))

    def test_entry_cannot_read_into_the_next_block(self):
        first = pack(b"B", struct.pack("<2I", 0, 8), b"AAAA")
        second = pack(b"B", struct.pack("<2I", 0, 4), b"BBBB")
        data = first + second
        blocks = cpac_unpack.find_blocks(data)
        with self.assertRaises(ValueError):
            cpac_unpack.read_entries(data, blocks[0], blocks[1][0])

    def test_truncated_block_marker_is_ignored(self):
        self.assertEqual(cpac_unpack.find_blocks(b"\x00" * 8 + b"YEKP\x00\x00\x00\x00TADP"), [])


if __name__ == "__main__":
    unittest.main()
