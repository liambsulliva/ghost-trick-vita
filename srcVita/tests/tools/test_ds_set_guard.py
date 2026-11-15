import contextlib
import io
import os
import shutil
import sys
import tempfile
import unittest
from unittest import mock

import bake_record
import convert_assets
import ds_set
import pack_files


class ApkTripwireTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, self.folder)
        self.apk_folder = os.path.join(self.folder, "apk")
        self.rom_folder = os.path.join(self.folder, "rom")
        os.makedirs(self.apk_folder)
        os.makedirs(self.rom_folder)
        for folder in (self.apk_folder, self.rom_folder):
            with open(os.path.join(folder, "file.bin"), "wb") as output:
                output.write(b"bytes")
        self.addCleanup(convert_assets.allow_apk_reads)

    def test_while_refused_no_file_under_the_apk_folder_opens_or_lists(self):
        convert_assets.refuse_apk_reads_from([self.apk_folder])
        with self.assertRaises(convert_assets.ApkReadRefused):
            open(os.path.join(self.apk_folder, "file.bin"), "rb").close()
        with self.assertRaises(convert_assets.ApkReadRefused):
            os.listdir(self.apk_folder)
        with self.assertRaises(convert_assets.ApkReadRefused):
            pack_files.PackSet(self.apk_folder)

    def test_while_refused_the_cartridge_still_reads(self):
        convert_assets.refuse_apk_reads_from([self.apk_folder])
        with open(os.path.join(self.rom_folder, "file.bin"), "rb") as source:
            self.assertEqual(source.read(), b"bytes")
        self.assertEqual(os.listdir(self.rom_folder), ["file.bin"])

    def test_a_folder_whose_name_only_starts_like_the_apk_folder_still_reads(self):
        twin = self.apk_folder + "_twin"
        os.makedirs(twin)
        convert_assets.refuse_apk_reads_from([self.apk_folder])
        self.assertEqual(os.listdir(twin), [])

    def test_once_allowed_the_apk_folder_reads_again(self):
        convert_assets.refuse_apk_reads_from([self.apk_folder])
        convert_assets.allow_apk_reads()
        with open(os.path.join(self.apk_folder, "file.bin"), "rb") as source:
            self.assertEqual(source.read(), b"bytes")

    def run_main(self, convert):
        errors = io.StringIO()
        arguments = ["convert_assets.py", "--set", "tiny", "--output", os.path.join(self.folder, "out"), "--apk-folder", self.apk_folder, "--art", "ds", "--ds-rom", os.path.join(self.rom_folder, "file.bin")]
        with mock.patch.object(convert_assets.subprocess, "run"):
            with mock.patch.object(convert_assets, "convert_ds_set_description", side_effect=convert):
                with mock.patch.object(convert_assets, "load_set_description", return_value={}):
                    with mock.patch.object(sys, "argv", arguments):
                        with contextlib.redirect_stderr(errors):
                            code = convert_assets.main()
        return code, errors.getvalue()

    def test_a_ds_build_that_reaches_for_the_apk_stops_plainly(self):
        def reaches_for_the_apk(set_description, output, rom_path):
            open(os.path.join(self.apk_folder, "file.bin"), "rb").close()

        code, errors = self.run_main(reaches_for_the_apk)
        self.assertEqual(code, 1)
        self.assertIn("the DS set must read nothing from the APK", errors)
        self.assertNotIn("Traceback", errors)

    def test_a_ds_build_needs_no_text_key_and_reads_only_the_cartridge(self):
        def reads_the_cartridge(set_description, output, rom_path):
            with open(rom_path, "rb") as source:
                source.read()

        code, errors = self.run_main(reads_the_cartridge)
        self.assertEqual(code, 0)
        self.assertEqual(errors, "")

    def test_the_tripwire_is_lifted_after_a_ds_build(self):
        self.run_main(lambda set_description, output, rom_path: None)
        with open(os.path.join(self.apk_folder, "file.bin"), "rb") as source:
            self.assertEqual(source.read(), b"bytes")


class DsSetNamesTests(unittest.TestCase):
    def test_a_table_name_two_resources_share_gets_each_resource_s_id(self):
        rows = [("a", "st01_door_000", 16, 7, [0]), ("b", "st01_door_000", 32, 9, [0]), ("a", "st01_cat_000", 48, 3, [0])]
        names = ds_set.animation_names(rows)
        self.assertEqual(names[("a", 16)], "t_st01_door_000_7")
        self.assertEqual(names[("b", 32)], "t_st01_door_000_9")
        self.assertEqual(names[("a", 48)], "t_st01_cat_000")

    def test_frames_pack_left_to_right_and_wrap_onto_a_new_row(self):
        frames = [(600, 10, b"", 0, 0), (600, 20, b"", 0, 0), (4, 4, b"", 0, 0)]
        places, page_sizes = ds_set.place_frames(frames)
        self.assertEqual(places, [(0, 0, 0), (0, 0, 11), (0, 601, 11)])
        self.assertEqual(page_sizes, [(605, 31)])

    def test_frames_that_would_pass_the_page_height_start_a_new_page(self):
        frames = [(1000, 1000, b"", 0, 0)] * 5
        places, page_sizes = ds_set.place_frames(frames)
        self.assertEqual([place[0] for place in places], [0, 0, 1, 1, 2])
        self.assertEqual(places[2], (1, 0, 0))
        self.assertEqual(page_sizes, [(1000, 2001), (1000, 2001), (1000, 1000)])

    def test_no_page_is_larger_than_the_app_s_texture_limit(self):
        header_path = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))), "include", "texture_file.h")
        with open(header_path, encoding="utf-8") as header:
            limit = int(header.read().split("#define TEXTURE_FILE_MAX_DIMENSION ")[1].split()[0])
        self.assertLessEqual(ds_set.PAGE_WIDTH, limit)
        self.assertLessEqual(ds_set.PAGE_HEIGHT, limit)
        frames = [(416, 240, b"", 0, 0)] * 400
        _, page_sizes = ds_set.place_frames(frames)
        self.assertGreater(len(page_sizes), 1)
        for width, height in page_sizes:
            self.assertLessEqual(width, limit)
            self.assertLessEqual(height, limit)

    def test_later_pages_carry_their_number_and_each_frame_names_its_own_page(self):
        self.assertEqual(ds_set.page_name(7, 0), "ds_sprite_7")
        self.assertEqual(ds_set.page_name(7, 2), "ds_sprite_7_2")
        frame = ds_set.anim_frame(7, (4, 2, b"", 1, 1), (1, 4, 2), [(8, 8), (16, 4)])
        self.assertEqual(frame["texture"], "ds_sprite_7_1")
        self.assertEqual((frame["u"], frame["v"], frame["u_size"], frame["v_size"]), (0.25, 0.5, 0.25, 0.5))

    def test_a_page_whose_cartridge_bytes_have_not_changed_is_not_decoded_again(self):
        folder = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, folder)
        record = bake_record.BakeRecord(folder)
        frames = [(2, 1, bytes(8), -1, -2)]
        with mock.patch.object(ds_set.ds_sprites, "resolve", return_value={}), \
                mock.patch.object(ds_set.ds_sprites, "resource_parts", return_value=(b"p", b"g", b"c", b"a")), \
                mock.patch.object(ds_set.ds_sprites, "frames_of_parts", return_value=frames) as decode:
            first = ds_set.pages_of_resource(5, mock.Mock(), b"", record, "code")
            second = ds_set.pages_of_resource(5, mock.Mock(), b"", record, "code")
        self.assertEqual(decode.call_count, 1)
        self.assertEqual(first, second)
        self.assertEqual(second[0], [(2, 1, None, -1, -2)])

    def test_a_sprite_page_is_read_back_by_the_name_an_animation_gives_it(self):
        red = bytes((255, 0, 0, 255)) * 2
        frames = [(2, 1, red, 0, 0)]
        with mock.patch.object(ds_set.ds_sprites, "resolve", return_value={}), \
                mock.patch.object(ds_set.ds_sprites, "resource_parts", return_value=(b"p", b"g", b"c", b"a")) as read, \
                mock.patch.object(ds_set.ds_sprites, "frames_of_parts", return_value=frames):
            pages = ds_set.SpritePages(mock.Mock(), b"")
            width, height, rgba = pages.pixels("ds_sprite_5")
            self.assertEqual(pages.source("ds_sprite_5"), ds_set.source_of_parts((b"p", b"g", b"c", b"a")))
            with self.assertRaises(ds_set.ds_sprites.SpriteRefused):
                pages.pixels("ds_sprite_5_3")
            with self.assertRaises(ds_set.ds_sprites.SpriteRefused):
                pages.pixels("talk_box_top")
        self.assertEqual(read.call_count, 1)
        self.assertEqual(rgba[:8], red)
        self.assertEqual(len(rgba), width * height * 4)

    def test_an_empty_frame_draws_nothing(self):
        frame = ds_set.anim_frame(1, (0, 0, b"", 0, 0), (0, 0, 0), [(8, 8)])
        self.assertEqual(frame["texture"], "-")


if __name__ == "__main__":
    unittest.main()
