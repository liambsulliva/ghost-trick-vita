import unittest

import hud_art


class HudSheetTests(unittest.TestCase):
    def test_sheet_arguments_draw_every_button_and_word(self):
        arguments = hud_art.sheet_arguments("label.otf", "key.otf", "out.rgba")
        self.assertEqual(arguments[0], "magick")
        self.assertEqual(arguments[-1], "rgba:out.rgba")
        for word in hud_art.LABELS + hud_art.LETTER_ORDER + ["START"]:
            self.assertIn(word, arguments)

    def test_face_buttons_fit_their_cells(self):
        total = len(hud_art.FACE_ORDER) * hud_art.GLYPH_CELL + len(hud_art.LETTER_ORDER) * hud_art.GLYPH_CELL + hud_art.START_WIDTH
        self.assertLessEqual(total, hud_art.SHEET_WIDTH)
        self.assertLessEqual(len(hud_art.LABELS) * hud_art.LABEL_WIDTH, hud_art.SHEET_WIDTH)
        self.assertLessEqual(hud_art.LABEL_ROW_Y + hud_art.LABEL_HEIGHT, hud_art.SHEET_HEIGHT)


if __name__ == "__main__":
    unittest.main()
