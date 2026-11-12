import struct
import unittest

import pause_gallery


class GalleryOrderTest(unittest.TestCase):
    def test_each_image_is_listed_once_in_the_gallery_order(self):
        text = "GUI/ui000000/tex/Thumbnail_Image01_IML4.tex\0GUI/ui040100/tex/Image01_IML4.tex\0GUI/ui040100/tex/Image161_IML4.tex\0GUI/ui040100/tex/Image01_IML4.tex\0"
        self.assertEqual(pause_gallery.gallery_order(text.encode("utf-16-le")), ["Image01", "Image161"])


class ThumbSheetTest(unittest.TestCase):
    def test_the_sheet_and_every_cell_keep_block_aligned_sides(self):
        width = pause_gallery.ART_THUMB_COLUMNS * pause_gallery.ART_THUMB_PITCH[0]
        self.assertEqual(width % 4, 0)
        self.assertEqual(pause_gallery.ART_THUMB_PITCH[1] % 4, 0)
        self.assertLessEqual(width, 2048)
        places = pause_gallery.thumb_places(40)
        self.assertEqual(places[pause_gallery.ART_THUMB_COLUMNS], (0, pause_gallery.ART_THUMB_PITCH[1]))
        self.assertEqual(len(set(places)), 40)

    def test_table_lines_name_each_texture_and_its_thumb_cell(self):
        lines = pause_gallery.table_lines([("pause_art_000", "a", 1), ("pause_art_001", "b", 5)])
        self.assertEqual(lines[1], "art pause_art_001 %d 0 %d %d 5" % (pause_gallery.ART_THUMB_PITCH[0], pause_gallery.ART_THUMB_SIZE[0], pause_gallery.ART_THUMB_SIZE[1]))

    def test_a_key_unlocks_the_picture_once_its_chapter_is_cleared(self):
        self.assertEqual(pause_gallery.chapter_that_unlocks(0), 1)
        self.assertEqual(pause_gallery.chapter_that_unlocks(4), 5)
        self.assertEqual(pause_gallery.chapter_that_unlocks(18), 18)
        self.assertEqual(pause_gallery.chapter_that_unlocks(30), 18)

    def test_the_key_after_each_thumbnail_path_is_read(self):
        def path(text):
            encoded = (text + "\0").encode("utf-16-le")
            padded = encoded + b"\0" * (-(4 + len(encoded)) % 4)
            return struct.pack("<I", len(text) + 1) + padded

        records = path("GUI/ui000000/tex/Thumbnail_Image02_IML4.tex") + struct.pack("<II", 1, 0) + path("GUI/ui040100/tex/Image02_IML4.tex") + struct.pack("<I", 0)
        records += path("GUI/ui000000/tex/Thumbnail_Image26_IML4.tex") + struct.pack("<II", 29, 0) + path("GUI/ui040100/tex/Image26_IML4.tex")
        table = b"\0" * (pause_gallery.RECORD_START + pause_gallery.RECORD_DATA) + records
        self.assertEqual(pause_gallery.unlock_keys(table), {"Image02": 1, "Image26": 29})


if __name__ == "__main__":
    unittest.main()
