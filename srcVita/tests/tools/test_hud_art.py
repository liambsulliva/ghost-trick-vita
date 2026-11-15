import os
import unittest

import hud_art


class HudSheetTests(unittest.TestCase):
    def test_sheet_arguments_draw_every_button_and_word(self):
        arguments = hud_art.sheet_arguments("label.otf", "key.otf", "out.rgba")
        self.assertEqual(arguments[0], "magick")
        self.assertEqual(arguments[-1], "rgba:out.rgba")
        for word in hud_art.LABELS + hud_art.SWAP_LABELS + hud_art.LETTER_ORDER + ["START"]:
            self.assertIn(word, arguments)

    def test_face_buttons_fit_their_cells(self):
        total = len(hud_art.FACE_ORDER) * hud_art.GLYPH_CELL + len(hud_art.LETTER_ORDER) * hud_art.GLYPH_CELL + hud_art.START_WIDTH
        self.assertLessEqual(total, hud_art.SHEET_WIDTH)
        self.assertLessEqual(len(hud_art.LABELS) * hud_art.LABEL_WIDTH, hud_art.SHEET_WIDTH)
        self.assertLessEqual(hud_art.LABEL_ROW_Y + hud_art.LABEL_HEIGHT, hud_art.SHEET_HEIGHT)

    def test_swap_words_have_a_row_below_the_wide_word(self):
        self.assertGreaterEqual(hud_art.SWAP_LABEL_ROW_Y, hud_art.WIDE_LABEL_ROW_Y + hud_art.WIDE_LABEL_HEIGHT)
        self.assertLessEqual(hud_art.SWAP_LABEL_ROW_Y + hud_art.LABEL_HEIGHT, hud_art.SHEET_HEIGHT)
        self.assertLessEqual(len(hud_art.SWAP_LABELS) * hud_art.SWAP_LABEL_WIDTH, hud_art.SHEET_WIDTH)

    def test_the_hint_mark_has_a_cell_of_its_own_that_the_app_reads(self):
        arguments = hud_art.sheet_arguments("label.otf", "key.otf", "out.rgba")
        self.assertIn(hud_art.HINT_MARK, arguments)
        self.assertGreaterEqual(hud_art.HINT_MARK_X, len(hud_art.SWAP_LABELS) * hud_art.SWAP_LABEL_WIDTH)
        self.assertGreaterEqual(hud_art.HINT_MARK_Y, hud_art.WIDE_LABEL_ROW_Y + hud_art.WIDE_LABEL_HEIGHT)
        self.assertLessEqual(hud_art.HINT_MARK_X + hud_art.HINT_MARK_SIZE, hud_art.SHEET_WIDTH)
        self.assertLessEqual(hud_art.HINT_MARK_Y + hud_art.HINT_MARK_SIZE, hud_art.SHEET_HEIGHT)
        with open(os.path.join(os.path.dirname(__file__), "..", "..", "src", "app", "script_ui_view.c")) as source:
            app_code = source.read()
        self.assertIn("#define HINT_MARK_CELL_X %d.0f" % hud_art.HINT_MARK_X, app_code)
        self.assertIn("#define HINT_MARK_CELL_Y %d.0f" % hud_art.HINT_MARK_Y, app_code)
        self.assertIn("#define HINT_MARK_CELL_SIZE %d.0f" % hud_art.HINT_MARK_SIZE, app_code)


if __name__ == "__main__":
    unittest.main()
