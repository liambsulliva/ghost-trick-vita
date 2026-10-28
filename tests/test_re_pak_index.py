import csv
import io
import struct
import subprocess
import sys
import tempfile
import unittest
import zipfile
from pathlib import Path

TOOLS_DIR = Path(__file__).resolve().parents[1] / "tools"
sys.path.insert(0, str(TOOLS_DIR))

import re_pak_index
from re_names import path_hashes


def pack(path):
    first, second = path_hashes(path)
    header = struct.pack("<4sBBHII", b"KPKA", 4, 0, 0, 1, 0)
    entry = struct.pack("<IIQQQQQ", first, second, 64, 4, 4, 0, 0)
    return header + entry + b"TEST"


class PackIndexTests(unittest.TestCase):
    def test_truncated_entry_table_is_rejected(self):
        data = pack("example.user.2")[:48]
        with self.assertRaises(ValueError):
            re_pak_index.read_index(io.BytesIO(data), len(data))

    def test_data_outside_the_pack_is_rejected(self):
        data = pack("example.user.2")[:-1]
        with self.assertRaises(ValueError):
            re_pak_index.read_index(io.BytesIO(data), len(data))

    def test_entries_cannot_point_into_the_index(self):
        data = bytearray(pack("example.user.2"))
        struct.pack_into("<Q", data, 24, 16)
        with self.assertRaises(ValueError):
            re_pak_index.read_index(io.BytesIO(data), len(data))

    def test_cli_reads_pack_indexes_inside_zip_and_ignores_split_copies(self):
        path = "natives/android/charfiles/2d/group/st01/lamp/st01_lamp_000/st01_lamp_000.user.2"
        missing = path.replace("lamp", "switch")
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            (root / "files.list").write_text(path + "\n" + missing + "\n")
            archive_path = root / "data01"
            with zipfile.ZipFile(archive_path, "w") as archive:
                archive.writestr("assets/re_chunk_001.pak", pack(path))
                archive.writestr("assets/re_chunk_001.pak.0", pack(path)[:32])
            out = root / "presence.tsv"
            command = [sys.executable, "-I", str(TOOLS_DIR / "re_pak_index.py"), str(root / "files.list"), str(out), str(archive_path)]
            result = subprocess.run(command, capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            with out.open() as source:
                rows = {row["path"]: row for row in csv.DictReader(source, delimiter="\t")}
            self.assertEqual(rows[path]["status"], "found")
            self.assertEqual(rows[path]["entry"], "0")
            self.assertEqual(rows[path]["stored_size"], "4")
            self.assertIn("::assets/re_chunk_001.pak", rows[path]["source"])
            self.assertEqual(rows[missing]["status"], "missing")
            self.assertIn("packs: 1, entries: 1", result.stdout)


if __name__ == "__main__":
    unittest.main()
