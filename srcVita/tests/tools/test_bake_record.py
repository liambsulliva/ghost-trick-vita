import json
import os
import tempfile
import unittest

import asset_writers
import bake_record

CODE = "code"
SETTINGS = {"width": 4, "crop": [0, 0, 8, 8]}
SOURCE = b"source bytes"
OUTPUT_PATH = "textures/a.gtex"


class BakeRecordTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.folder = temporary.name
        os.makedirs(os.path.join(self.folder, "textures"))
        self.record = bake_record.BakeRecord(self.folder)
        self.key = self.record.key_for(CODE, SETTINGS, SOURCE)
        self.record.store(OUTPUT_PATH, self.key, b"baked")
        self.record.save()

    def reopened(self):
        return bake_record.BakeRecord(self.folder)

    def test_an_unchanged_file_is_reused_after_reopening(self):
        record = self.reopened()
        self.assertEqual(record.read_if_current(OUTPUT_PATH, self.key), b"baked")
        self.assertEqual(record.reused_in("textures"), 1)
        self.assertEqual(record.baked_in("textures"), 0)

    def test_a_key_changes_with_the_code_the_settings_or_the_source(self):
        self.assertNotEqual(self.record.key_for("other code", SETTINGS, SOURCE), self.key)
        self.assertNotEqual(self.record.key_for(CODE, {"width": 8, "crop": [0, 0, 8, 8]}, SOURCE), self.key)
        self.assertNotEqual(self.record.key_for(CODE, SETTINGS, b"other source"), self.key)

    def test_a_result_reads_its_size_and_format_from_the_file_and_counts_its_bytes(self):
        gtex_bytes = asset_writers.gtex_bytes(8, 4, asset_writers.FORMAT_DXT5, bytes(32))
        self.assertEqual(bake_record.texture_result("a", (16, 8), gtex_bytes), {"name": "a", "source_size": (16, 8), "width": 8, "height": 4, "format": asset_writers.FORMAT_DXT5, "byte_count": len(gtex_bytes)})
        self.assertEqual(bake_record.result_of_own_size("a", gtex_bytes)["source_size"], (8, 4))

    def test_details_stored_with_a_file_come_back_after_reopening(self):
        self.record.store(OUTPUT_PATH, self.key, b"baked", {"natural_width": 12.5})
        self.record.save()
        self.assertEqual(self.reopened().details_of(OUTPUT_PATH), {"natural_width": 12.5})

    def test_a_file_stored_without_details_has_none_in_its_entry(self):
        self.assertNotIn("details", self.record.entries[OUTPUT_PATH])

    def test_settings_order_does_not_change_the_key(self):
        self.assertEqual(self.record.key_for(CODE, {"crop": [0, 0, 8, 8], "width": 4}, SOURCE), self.key)

    def test_scripts_baked_do_not_count_as_textures(self):
        os.makedirs(os.path.join(self.folder, "scripts"))
        record = self.reopened()
        record.store("scripts/a.lmg", "script key", b"script")
        self.assertEqual(record.baked_in("scripts"), 1)
        self.assertEqual(record.baked_in("textures"), 0)
        record.read_if_current("scripts/a.lmg", "script key")
        self.assertEqual(record.reused_in("scripts"), 1)
        self.assertEqual(record.reused_in("textures"), 0)

    def test_a_different_key_is_not_reused(self):
        self.assertIsNone(self.reopened().read_if_current(OUTPUT_PATH, "other key"))

    def test_a_file_never_recorded_is_not_reused(self):
        self.assertIsNone(self.reopened().read_if_current("textures/b.gtex", self.key))

    def test_a_deleted_output_is_not_reused(self):
        os.remove(os.path.join(self.folder, OUTPUT_PATH))
        self.assertIsNone(self.reopened().read_if_current(OUTPUT_PATH, self.key))

    def test_an_edited_output_is_not_reused(self):
        with open(os.path.join(self.folder, OUTPUT_PATH), "wb") as output:
            output.write(b"edited")
        self.assertIsNone(self.reopened().read_if_current(OUTPUT_PATH, self.key))

    def test_a_damaged_record_starts_empty(self):
        with open(os.path.join(self.folder, bake_record.RECORD_FILE_NAME), "w", encoding="ascii") as output:
            output.write("not json")
        self.assertIsNone(self.reopened().read_if_current(OUTPUT_PATH, self.key))

    def write_record_text(self, text):
        with open(os.path.join(self.folder, bake_record.RECORD_FILE_NAME), "w", encoding="ascii") as output:
            output.write(text)

    def test_a_record_of_the_wrong_shape_starts_empty(self):
        for text in ("[]", "null", "3", '"text"'):
            self.write_record_text(text)
            self.assertEqual(self.reopened().entries, {}, text)

    def test_an_unusable_entry_is_dropped_and_the_good_ones_stay(self):
        good = {"key": self.key, "output": bake_record.sha256_of_bytes(b"baked")}
        unusable = [{}, "text", None, 3, [], {"key": self.key}, {"output": "o"}, {"key": 1, "output": "o"}, {"key": "k", "output": 1}, {"key": "k", "output": "o", "details": "text"}]
        entries = {"textures/bad_%d.gtex" % index: entry for index, entry in enumerate(unusable)}
        entries[OUTPUT_PATH] = good
        self.write_record_text(json.dumps(entries))
        record = self.reopened()
        self.assertEqual(list(record.entries), [OUTPUT_PATH])
        self.assertEqual(record.read_if_current(OUTPUT_PATH, self.key), b"baked")

    def test_details_of_a_file_that_has_none_are_empty(self):
        self.assertEqual(self.reopened().details_of(OUTPUT_PATH), {})
        self.assertEqual(self.reopened().details_of("textures/absent.gtex"), {})

    def test_forgetting_keeps_only_the_named_files(self):
        self.record.store("textures/b.gtex", "b key", b"b")
        self.record.forget_all_except({"textures/b.gtex"})
        self.assertEqual(list(self.record.entries), ["textures/b.gtex"])


class CodeFingerprintTests(unittest.TestCase):
    def test_a_fingerprint_changes_with_the_files_and_the_extra_text(self):
        first = bake_record.code_fingerprint(["bake_record.py"])
        self.assertEqual(bake_record.code_fingerprint(["bake_record.py"]), first)
        self.assertNotEqual(bake_record.code_fingerprint(["pack_files.py"]), first)
        self.assertNotEqual(bake_record.code_fingerprint(["bake_record.py"], "magick 7"), first)


class FileHelperTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.folder = temporary.name
        self.path = os.path.join(self.folder, "a.txt")

    def test_a_new_file_is_written(self):
        self.assertTrue(bake_record.write_if_different(self.path, b"one"))
        with open(self.path, "rb") as source:
            self.assertEqual(source.read(), b"one")
        self.assertFalse(os.path.exists(self.path + ".new"))

    def test_the_same_bytes_leave_the_file_untouched(self):
        bake_record.write_if_different(self.path, b"one")
        os.utime(self.path, (1000, 1000))
        self.assertFalse(bake_record.write_if_different(self.path, b"one"))
        self.assertEqual(os.path.getmtime(self.path), 1000)

    def test_different_bytes_replace_the_file(self):
        bake_record.write_if_different(self.path, b"one")
        self.assertTrue(bake_record.write_if_different(self.path, b"two"))
        with open(self.path, "rb") as source:
            self.assertEqual(source.read(), b"two")

    def test_a_leftover_temporary_file_is_removed_and_its_absence_is_fine(self):
        with open(self.path + bake_record.TEMPORARY_SUFFIX, "wb") as output:
            output.write(b"part")
        bake_record.remove_leftover_temporary_file(self.path)
        self.assertFalse(os.path.exists(self.path + bake_record.TEMPORARY_SUFFIX))
        bake_record.remove_leftover_temporary_file(self.path)

    def test_only_listed_files_stay(self):
        os.makedirs(os.path.join(self.folder, "textures"))
        for name in ("keep.gtex", "stale.gtex"):
            with open(os.path.join(self.folder, "textures", name), "wb") as output:
                output.write(b"x")
        removed = bake_record.remove_files_not_listed(self.folder, ["textures"], {"textures/keep.gtex"})
        self.assertEqual(removed, ["textures/stale.gtex"])
        self.assertEqual(os.listdir(os.path.join(self.folder, "textures")), ["keep.gtex"])


if __name__ == "__main__":
    unittest.main()
