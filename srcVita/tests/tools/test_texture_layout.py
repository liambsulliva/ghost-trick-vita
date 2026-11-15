import os
import re
import shutil
import sys
import tempfile
import unittest

import bake_record
import output_folders

SRC_VITA_FOLDER = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(SRC_VITA_FOLDER, "tools"))

import vita_loop


def gtex(width=4, height=4):
    return bytes([0] * 20) + bytes(width * height * 4)


class TheSetHasTheVitaLayout(unittest.TestCase):
    def test_textures_use_the_same_buckets_as_the_app(self):
        self.assertEqual(output_folders.disk_path_of("textures/a.gtex"), "textures/03/a.gtex")
        self.assertEqual(output_folders.disk_path_of("textures/movie_time_jump_012.gtex"), "textures/30/movie_time_jump_012.gtex")
        with open(os.path.join(SRC_VITA_FOLDER, "Makefile"), encoding="utf-8") as source:
            buckets = int(re.search(r"^TEXTURE_FOLDER_BUCKETS := (\d+)$", source.read(), re.M).group(1))
        self.assertEqual(buckets, output_folders.TEXTURE_FOLDER_BUCKETS)

    def test_other_files_and_paths_keep_their_place(self):
        for path in ("music/theme.ogg", "manifest.txt", "textures/notes.txt", "lang/ja/scripts/talk.txt"):
            self.assertEqual(output_folders.disk_path_of(path), path)
            self.assertEqual(output_folders.relative_path_of(path), path)
        self.assertEqual(output_folders.relative_path_of("textures/03/a.gtex"), "textures/a.gtex")
        self.assertEqual(output_folders.relative_path_of("textures/04/a.gtex"), "textures/04/a.gtex")

    def test_a_language_overlay_texture_uses_the_same_bucket(self):
        self.assertEqual(output_folders.disk_path_of("lang/ja/textures/a.gtex"), "lang/ja/textures/03/a.gtex")
        self.assertEqual(output_folders.relative_path_of("lang/ja/textures/03/a.gtex"), "lang/ja/textures/a.gtex")

    def test_the_uploader_puts_each_file_where_the_set_has_it(self):
        self.assertEqual(vita_loop.device_path_of_asset("textures/a.gtex"), output_folders.disk_path_of("textures/a.gtex"))


class TheBakeWritesBuckets(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.mkdtemp()

    def tearDown(self):
        shutil.rmtree(self.folder)

    def test_a_stored_texture_lands_in_its_bucket_and_is_reused_from_there(self):
        record = bake_record.BakeRecord(self.folder)
        record.store("textures/a.gtex", "key", gtex())
        self.assertTrue(os.path.isfile(os.path.join(self.folder, "textures", "03", "a.gtex")))
        self.assertFalse(os.path.exists(os.path.join(self.folder, "textures", "a.gtex")))
        record.save()
        self.assertIsNotNone(bake_record.BakeRecord(self.folder).read_if_current("textures/a.gtex", "key"))

    def test_a_set_baked_flat_is_moved_into_buckets_and_still_reused(self):
        record = bake_record.BakeRecord(self.folder)
        record.store("textures/a.gtex", "key", gtex())
        record.save()
        os.replace(os.path.join(self.folder, "textures", "03", "a.gtex"), os.path.join(self.folder, "textures", "a.gtex"))
        reopened = bake_record.BakeRecord(self.folder)
        self.assertFalse(os.path.exists(os.path.join(self.folder, "textures", "a.gtex")))
        self.assertIsNotNone(reopened.read_if_current("textures/a.gtex", "key"))

    def test_the_sweep_keeps_listed_textures_in_their_bucket_and_removes_the_rest(self):
        for path in ("textures/03/a.gtex", "textures/30/stale.gtex", "textures/05/a.gtex", "textures/b.gtex"):
            os.makedirs(os.path.dirname(os.path.join(self.folder, path)), exist_ok=True)
            with open(os.path.join(self.folder, path), "wb") as output:
                output.write(b"x")
        removed = bake_record.remove_files_not_listed(self.folder, ["textures"], {"textures/a.gtex"})
        self.assertEqual(sorted(removed), ["textures/05/a.gtex", "textures/30/stale.gtex", "textures/b.gtex"])
        self.assertEqual(sorted(os.listdir(os.path.join(self.folder, "textures"))), ["03"])

    def test_a_flat_language_overlay_is_moved_into_buckets(self):
        flat = os.path.join(self.folder, "lang", "fr", "textures", "a.gtex")
        os.makedirs(os.path.dirname(flat))
        with open(flat, "wb") as output:
            output.write(gtex())
        bake_record.BakeRecord(self.folder)
        self.assertFalse(os.path.exists(flat))
        self.assertTrue(os.path.isfile(os.path.join(self.folder, "lang", "fr", "textures", "03", "a.gtex")))

    def test_the_overlay_sweep_keeps_listed_textures_in_their_bucket(self):
        import languages
        for path in ("lang/fr/textures/03/a.gtex", "lang/fr/textures/a.gtex", "lang/fr/textures/30/stale.gtex"):
            os.makedirs(os.path.dirname(os.path.join(self.folder, path)), exist_ok=True)
            with open(os.path.join(self.folder, path), "wb") as output:
                output.write(b"x")
        removed = languages.remove_overlay_files_not_listed(self.folder, {"lang/fr/textures/a.gtex"})
        self.assertEqual(sorted(removed), ["lang/fr/textures/30/stale.gtex", "lang/fr/textures/a.gtex"])

    def test_the_uploader_lists_a_bucketed_texture_by_its_name(self):
        record = bake_record.BakeRecord(self.folder)
        record.store("textures/a.gtex", "key", gtex())
        with open(os.path.join(self.folder, "manifest.txt"), "w", encoding="ascii") as output:
            output.write("animation a\n")
        listed = dict((relative_path, local_path) for local_path, relative_path in vita_loop.list_local_files(self.folder))
        self.assertEqual(listed["textures/a.gtex"], os.path.join(self.folder, "textures", "03", "a.gtex"))
        self.assertEqual(vita_loop.list_local_files(self.folder)[0][1], "manifest.txt")


if __name__ == "__main__":
    unittest.main()
