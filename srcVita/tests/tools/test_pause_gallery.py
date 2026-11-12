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
        lines = pause_gallery.table_lines([("pause_art_000", "a"), ("pause_art_001", "b")])
        self.assertEqual(lines[1], "art pause_art_001 %d 0 %d %d" % (pause_gallery.ART_THUMB_PITCH[0], pause_gallery.ART_THUMB_SIZE[0], pause_gallery.ART_THUMB_SIZE[1]))


if __name__ == "__main__":
    unittest.main()
