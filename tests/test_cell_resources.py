import csv
import struct
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

TOOLS_DIR = Path(__file__).resolve().parents[1] / "tools"
sys.path.insert(0, str(TOOLS_DIR))

import cell_resources


def segment():
    data = bytearray(0xB900)
    for offset, address in [(0xB750, 0x0200B800), (0xB754, 0x0200B820), (0xB758, 0x0200B840)]:
        struct.pack_into("<I", data, offset, address)
    struct.pack_into("<5H", data, 0xB800, 1, 2, 1, 2, 0)
    struct.pack_into("<2H", data, 0xB822, 4, 7)
    struct.pack_into("<4H", data, 0xB848, 10, 20, 30, 40)
    return data


class CellResourceTests(unittest.TestCase):
    def test_id_selects_descriptor_and_sequence_separately(self):
        row = cell_resources.resolve_resource(segment(), 2)
        self.assertEqual(row["descriptor"], 1)
        self.assertEqual(row["sequence"], 3)
        self.assertEqual([row[name] for name in ["palette", "graphics", "cell", "animation"]], [10, 20, 30, 40])

    def test_localized_id_selects_one_of_five_language_entries(self):
        first = cell_resources.resolve_resource(segment(), 0x8000, 1)
        second = cell_resources.resolve_resource(segment(), 0x8000, 2)
        self.assertEqual(first["resolved_id"], 1)
        self.assertEqual(first["sequence"], 0)
        self.assertEqual(second["resolved_id"], 2)
        self.assertEqual(second["sequence"], 3)
        self.assertEqual(cell_resources.resolve_resource(segment(), 0x8000, 5)["status"], "none")

    def test_zero_id_has_no_resource(self):
        row = cell_resources.resolve_resource(segment(), 0)
        self.assertEqual(row["status"], "none")
        self.assertEqual(row["palette"], "")

    def test_invalid_id_or_language_is_rejected(self):
        for resource_id, language in [(-1, 1), (0x10000, 1), (1, 0), (1, 6)]:
            with self.subTest(resource_id=resource_id, language=language):
                with self.assertRaises(ValueError):
                    cell_resources.resolve_resource(segment(), resource_id, language)

    def test_lookup_cannot_cross_table_boundaries(self):
        for resource_id in [16, 0x8003]:
            with self.subTest(resource_id=resource_id):
                with self.assertRaises(ValueError):
                    cell_resources.resolve_resource(segment(), resource_id)

    def test_truncated_or_invalid_segment_is_rejected(self):
        data = segment()
        struct.pack_into("<H", data, 0xB822, 0xFFFC)
        for source in [b"", bytes(0xB900), data]:
            with self.assertRaises(ValueError):
                cell_resources.resolve_resource(source, 1)

    def test_cli_writes_metadata_for_explicit_ids(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            source = root / "main.bin"
            source.write_bytes(segment())
            output = root / "out" / "resources.tsv"
            command = [sys.executable, "-I", str(TOOLS_DIR / "cell_resources.py"), str(source), str(output), "1", "0x8000", "--language", "2"]
            result = subprocess.run(command, capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            with output.open() as f:
                rows = list(csv.DictReader(f, delimiter="\t"))
            self.assertEqual([row["resolved_id"] for row in rows], ["1", "2"])


if __name__ == "__main__":
    unittest.main()
