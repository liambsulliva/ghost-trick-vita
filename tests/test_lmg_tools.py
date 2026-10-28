import io
import struct
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

TOOLS_DIR = Path(__file__).resolve().parents[1] / "tools"
sys.path.insert(0, str(TOOLS_DIR))

import lmg_decode
import map_motions


def container(code, labels=b"", strings=b"*\x00"):
    header = b"1LMG" + struct.pack("<4I", 0, len(code), len(strings), len(labels))
    return header.ljust(0x34, b"\x00") + code + strings + labels


def label_table(name, target):
    return struct.pack("<3I", 1, 0, target) + name.encode() + b"\x00"


class DecoderTests(unittest.TestCase):
    def test_return_has_no_operand(self):
        data = container(struct.pack("<2H", 0x2E, 0))
        header, strings, labels, items, errors = lmg_decode.decode(data)
        self.assertEqual(errors, [])
        self.assertEqual([item[0] for item in items], [0x34, 0x36])

    def test_jump_into_operand_is_rejected(self):
        data = container(struct.pack("<HIH", 0x22, 0x36, 0))
        self.assertTrue(lmg_decode.decode(data)[4])

    def test_truncated_instruction_is_rejected(self):
        data = container(struct.pack("<H", 0x117))
        self.assertIn("truncated opcode", lmg_decode.decode(data)[4][0])

    def test_truncated_motion_is_rejected(self):
        data = container(struct.pack("<H", 0xFFF4), label_table("st01_test_004.mtbl.xml", 0x34))
        self.assertIn("truncated motion command", lmg_decode.decode(data)[4][0])

    def test_message_payload_is_omitted(self):
        payload = struct.pack("<H", 0xFF2D) + b"PRIVATE_PAYLOAD\x00"
        data = container(payload, label_table("m00_0000", 0x34))
        out = io.StringIO()
        errors, items = lmg_decode.write_tree("test.dec", data, out)
        self.assertEqual(errors, [])
        self.assertNotIn("PRIVATE_PAYLOAD", out.getvalue())
        self.assertIn("bytes (not shown)", out.getvalue())

    def test_header_sizes_are_checked(self):
        with self.assertRaises(ValueError):
            lmg_decode.decode(container(b"\x00\x00") + b"extra")

    def test_cli_rejects_empty_input_and_decode_errors(self):
        with tempfile.TemporaryDirectory() as folder:
            source = Path(folder) / "source"
            source.mkdir()
            command = [sys.executable, "-I", str(TOOLS_DIR / "lmg_decode.py"), str(source), str(Path(folder) / "out")]
            result = subprocess.run(command, capture_output=True)
            self.assertNotEqual(result.returncode, 0)
            (source / "test.dec").write_bytes(container(struct.pack("<H", 0xFFFF)))
            result = subprocess.run(command, capture_output=True)
            self.assertNotEqual(result.returncode, 0)


class MappingTests(unittest.TestCase):
    def test_primary_animation_id_is_preserved(self):
        folders = {"st01_test_004": set()}
        self.assertEqual(map_motions.match_motion("st01_test_004.mtbl.xml", folders), ("st01_test_004", "exact"))

    def test_variants_fold_to_existing_animation(self):
        folders = {"st01_test_004": set()}
        for suffix in ("001", "00a"):
            self.assertEqual(map_motions.match_motion("st01_test_004_" + suffix + ".mtbl.xml", folders), ("st01_test_004", "variant"))

    def test_exact_split_folder_takes_priority(self):
        folders = {"st01_test_004": set(), "st01_test_004_001": set()}
        self.assertEqual(map_motions.match_motion("st01_test_004_001.mtbl.xml", folders), ("st01_test_004_001", "exact"))

    def test_missing_folder_is_unmatched(self):
        self.assertEqual(map_motions.match_motion("st01_test_004_001.mtbl.xml", {}), ("", "unmatched"))

    def test_file_list_uses_2d_user_folders(self):
        with tempfile.TemporaryDirectory() as folder:
            path = Path(folder) / "files.list"
            path.write_text("natives/android/charfiles/2d/group/st01/test/st01_test_004/st01_test_004.user.2\n"
                            "natives/android/charfiles/2d/group/st01/test/st01_test_005/st01_test_005.tex.1\n"
                            "natives/android/charfiles/3d/group/st01/test/st01_test_006/st01_test_006.user.2\n")
            self.assertEqual(set(map_motions.read_remaster_folders(path)), {"st01_test_004"})


if __name__ == "__main__":
    unittest.main()
