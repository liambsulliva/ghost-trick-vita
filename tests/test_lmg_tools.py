import csv
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
    def test_wait_alias_can_have_a_different_script_object_name(self):
        folders = {"st03_02dbox_000": set()}
        self.assertEqual(map_motions.alias_candidates("st03", "02dbox_wait.mtbl.xml", folders), [("st03_02dbox_000", "wait_name")])

    def test_single_resource_wait_can_have_a_nonzero_animation_id(self):
        folders = {"st01_patolamp_010": set()}
        self.assertEqual(map_motions.alias_candidates("st01", "patolamp_wait.mtbl.xml", folders), [("st01_patolamp_010", "single_animation_wait")])

    def test_wait_alias_does_not_pick_between_multiple_numbered_resources(self):
        folders = {"st01_patolamp_010": set(), "st01_patolamp_020": set()}
        self.assertEqual(map_motions.alias_candidates("st01", "patolamp_wait.mtbl.xml", folders), [])

    def test_cross_stage_wait_preserves_alternatives(self):
        folders = {"st01_guitar_000": set(), "st01_guitar_999": set()}
        self.assertEqual(map_motions.alias_candidates("st13", "guitar_wait.mtbl.xml", folders),
                         [("st01_guitar_000", "cross_stage_wait"), ("st01_guitar_999", "cross_stage_wait")])

    def test_stage_prefix_and_primary_letter_variants_are_only_candidates(self):
        folders = {"st14_02lamp_000": set(), "st14_levlamp_000": set()}
        self.assertEqual(map_motions.alias_candidates("st14", "sr14_02lamp_000.mtbl.xml", folders), [("st14_02lamp_000", "stage_prefix_spelling")])
        self.assertEqual(map_motions.alias_candidates("st14", "st14_levlamp_00b.mtbl.xml", folders), [("st14_levlamp_000", "primary_letter_variant")])
        self.assertEqual(map_motions.alias_candidates("st14", "st14_levlamp_01b.mtbl.xml", folders), [])
        self.assertEqual(map_motions.alias_candidates("st14", "st14_02lamp_000.mtbl.xml", folders), [])

    def test_cli_reports_idle_candidates_without_promoting_them(self):
        code = struct.pack("<2HIH", 0x16, 0x22, 0x48, 0)
        code = code + struct.pack("<HIHH", 0x117, 0x52, 1, 0)
        code = code + struct.pack("<HIHH", 0x117, 0, 1, 0)
        code = code + struct.pack("<5H", 0xFFFD, 1, 0, 0x1000, 0xFFFE)
        data = container(code, label_table("lamp_wait.mtbl.xml", 0x52), b"lamp\x00")
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            source = root / "source"
            (source / "st01").mkdir(parents=True)
            (source / "st01" / "test.dec").write_bytes(data)
            file_list = root / "files.list"
            file_list.write_text("natives/android/charfiles/2d/group/st01/lamp/st01_lamp_000/st01_lamp_000.user.2\n")
            out = root / "out"
            command = [sys.executable, "-I", str(TOOLS_DIR / "map_motions.py"), str(source), str(file_list), str(out)]
            result = subprocess.run(command, capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            with (out / "idle_candidates.tsv").open() as f:
                candidates = list(csv.DictReader(f, delimiter="\t"))
            self.assertEqual(len(candidates), 1)
            self.assertEqual(candidates[0]["animation"], "st01_lamp_000")
            self.assertEqual(candidates[0]["play_offsets"], "0x3e")
            self.assertEqual(candidates[0]["frame_ids"], "0")
            self.assertEqual(candidates[0]["frame_ticks"], "4")
            with (out / "motion_uses.tsv").open() as f:
                uses = list(csv.DictReader(f, delimiter="\t"))
            self.assertEqual([row["object"] for row in uses], ["lamp", ""])
            self.assertEqual([row["status"] for row in uses], ["motion", "zero_target"])
            with (out / "motions.tsv").open() as f:
                mappings = list(csv.DictReader(f, delimiter="\t"))
            self.assertEqual(mappings[0]["match"], "unmatched")
            self.assertEqual(mappings[0]["remaster_folder"], "")
            with (out / "unmatched_remaster.tsv").open() as f:
                self.assertEqual(len(list(csv.DictReader(f, delimiter="\t"))), 1)
            with (out / "alias_candidates.tsv").open() as f:
                aliases = list(csv.DictReader(f, delimiter="\t"))
            self.assertEqual(aliases[0]["reason"], "wait_name")
            self.assertEqual(aliases[0]["alternative_animations"], "st01_lamp_000")

    def test_motion_owner_ends_at_object_boundary(self):
        items = [(0x34, "op", 0x16, [0x22, 0x70, 0, 0]),
                 (0x3E, "op", 0x117, [0x80, 0, 1, 0]),
                 (0x70, "op", 0x117, [0, 0, 1, 0])]
        self.assertEqual(map_motions.motion_uses(items, {0: "Lamp"}), [(0x3E, 0x80, "lamp"), (0x70, 0, "")])

    def test_shared_motion_keeps_each_callers_owner(self):
        items = [(0x34, "op", 0x16, [0x22, 0x70, 0, 0]),
                 (0x3E, "op", 0x117, [0xB0, 0, 1, 0]),
                 (0x70, "op", 0x16, [0x22, 0xA0, 0, 4]),
                 (0x7A, "op", 0x117, [0xB0, 0, 1, 0])]
        uses = map_motions.motion_uses(items, {0: "left", 4: "right"})
        self.assertEqual([use[2] for use in uses], ["left", "right"])

    def test_idle_candidate_requires_matching_owner_and_folder(self):
        folders = {"st01_lamp_000": set()}
        self.assertEqual(map_motions.idle_candidates("st01", "Lamp_wait.mtbl.xml", {"lamp": []}, folders), [("lamp", "st01_lamp_000")])
        self.assertEqual(map_motions.idle_candidates("st01", "Lamp_wait.mtbl.xml", {"other": []}, folders), [])
        self.assertEqual(map_motions.idle_candidates("st02", "Lamp_wait.mtbl.xml", {"lamp": []}, folders), [])
        self.assertEqual(map_motions.idle_candidates("st01", "Lamp_wait.mtbl.xml", {"": []}, folders), [])

    def test_plain_stage_object_name_is_an_idle_candidate(self):
        folders = {"st06_sofa_000": set()}
        self.assertEqual(map_motions.idle_candidates("st06", "st06_sofa.mtbl.xml", {"sofa": []}, folders), [("sofa", "st06_sofa_000")])
        self.assertEqual(map_motions.alias_candidates("st06", "st06_sofa.mtbl.xml", folders), [("st06_sofa_000", "unnumbered_default")])

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
