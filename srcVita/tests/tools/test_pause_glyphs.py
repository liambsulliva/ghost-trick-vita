import os
import re
import unittest

import hud_art
import pause_art

SOURCE_FOLDER = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "src", "app")
GLYPH_CALL = re.compile(r'pause_draw_(?:glyph|glyph_from_left|bar)\([^;]*?"([a-z]+)"')
CONTROL_TABLE = re.compile(r"control_glyphs\[CONTROLS_COUNT\]\[CONTROLS_GLYPH_MOST\] = \{(.*?)\};", re.S)


def named_glyphs():
    names = set()
    for file_name in os.listdir(SOURCE_FOLDER):
        if not file_name.startswith("pause_") or not file_name.endswith(".c"):
            continue
        with open(os.path.join(SOURCE_FOLDER, file_name), encoding="utf-8") as source:
            text = source.read()
        names.update(GLYPH_CALL.findall(text))
        table = CONTROL_TABLE.search(text)
        if table:
            names.update(re.findall(r'"([a-z]+)"', table.group(1)))
    return names


class PauseGlyphTest(unittest.TestCase):
    def test_every_glyph_the_pause_screens_draw_is_on_the_sheet(self):
        baked = {kind for kind, _, _, _ in hud_art.glyph_cells()}
        named = named_glyphs()
        self.assertIn("lstick", named)
        self.assertEqual(sorted(named - baked), [])

    def test_glyph_cells_stay_on_the_sheet_and_never_overlap(self):
        cells = hud_art.glyph_cells()
        boxes = [(x, y, x + width, y + hud_art.GLYPH_CELL) for _, x, y, width in cells]
        for left, top, right, bottom in boxes:
            self.assertTrue(0 <= left and right <= hud_art.SHEET_WIDTH and 0 <= top and bottom <= hud_art.SHEET_HEIGHT)
        for index, first in enumerate(boxes):
            for second in boxes[index + 1:]:
                self.assertFalse(first[0] < second[2] and second[0] < first[2] and first[1] < second[3] and second[1] < first[3])

    def test_the_glyph_table_gives_each_cell_its_own_width_and_height(self):
        lines = pause_art.glyph_lines()
        self.assertIn("glyph start %d 0 %d %d" % (hud_art.GLYPH_CELL * 6, hud_art.START_WIDTH, hud_art.GLYPH_CELL), lines)
        self.assertIn("glyph rstick 420 112 %d %d" % (hud_art.GLYPH_CELL, hud_art.GLYPH_CELL), lines)


if __name__ == "__main__":
    unittest.main()
