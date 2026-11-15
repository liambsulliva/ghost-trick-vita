import os
import re
import struct
import unittest

import ds_set_text
import ds_talk_icons
import ds_text
import talk_assets
import test_ds_set_text
import test_ds_text

SRC_VITA = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..")
TALK_HEADER = os.path.join(SRC_VITA, "include", "script_talk.h")
TALK_VIEW = os.path.join(SRC_VITA, "src", "app", "script_view.c")
ICON_CODE = 279
ICON_WIDTH = 4
ICON_HEIGHT = 24
RED = b"\xff\x00\x00\xff"
CLEAR = b"\x00\x00\x00\x00"


def arm9_with_icon_rows(rows):
    arm9 = bytearray(ds_text.ICON_WIDTH_TABLE_ADDRESS - ds_text.ARM9_LOAD_ADDRESS + ds_text.ICON_WIDTH_ROW_SIZE * (len(rows) + 1))
    for index, row in enumerate(rows):
        struct.pack_into(ds_talk_icons.ICON_ROW_FORMAT, arm9, ds_text.ICON_WIDTH_TABLE_ADDRESS - ds_text.ARM9_LOAD_ADDRESS + ds_text.ICON_WIDTH_ROW_SIZE * index, *row)
    return bytes(arm9)


def red_column_art():
    rgba = bytearray(CLEAR * (ICON_WIDTH * ICON_HEIGHT))
    for y in range(ICON_HEIGHT):
        rgba[(y * ICON_WIDTH + 1) * 4:(y * ICON_WIDTH + 2) * 4] = RED
    return {ICON_CODE: ds_talk_icons.icon_art(ICON_CODE, {"height": ICON_HEIGHT}, ICON_WIDTH, ICON_HEIGHT, bytes(rgba))}


def font_with_icon():
    made = test_ds_text.make_font({1: test_ds_text.BAR, 2: test_ds_text.BAR}, {1: (0, 3), 2: (0, 5)}, icon_widths={ICON_CODE: ICON_WIDTH})
    made.shape["line_height"] = 21
    return made


def line_with_icon():
    return {"items": [("char", 1), ("char", ICON_CODE), ("char", 2)], "codes": [], "text": ""}


def define_of(text, name):
    return int(re.search(r"#define %s (\w+)" % name, text).group(1).rstrip("u"), 0)


class IconTableTests(unittest.TestCase):
    def test_rows_are_read_until_the_zero_code_with_texture_width_and_height(self):
        arm9 = arm9_with_icon_rows([(275, 987, 16, 24), (279, 972, 56, 24)])
        self.assertEqual(ds_talk_icons.read_icon_rows(arm9), {275: {"texture": 987, "width": 16, "height": 24}, 279: {"texture": 972, "width": 56, "height": 24}})

    def test_the_width_table_and_the_icon_rows_name_the_same_codes(self):
        arm9 = arm9_with_icon_rows([(275, 987, 16, 24), (279, 972, 56, 24)])
        self.assertEqual(set(ds_text.read_icon_widths(arm9)), set(ds_talk_icons.read_icon_rows(arm9)))


class TalkIconTests(unittest.TestCase):
    def rows_and_glyphs(self, icons):
        glyphs = ds_set_text.TalkGlyphs({1: 0, 2: 127, 3: 255}, True, icons)
        rows, _ = ds_set_text.talk_line_rows(font_with_icon(), glyphs, [line_with_icon()], test_ds_set_text.windows()[ds_set_text.BOTTOM_ROW], 1, 15)
        return [int(code) for code in rows[0].split()[4:]], glyphs.cells()

    def test_an_icon_in_talk_text_draws_its_cartridge_art_untinted(self):
        codes, cells = self.rows_and_glyphs(red_column_art())
        icon = cells[codes[1] & ((1 << talk_assets.COLOR_SHIFT) - 1)]
        self.assertEqual(codes[1] >> talk_assets.COLOR_SHIFT, talk_assets.COLOR_AS_IS)
        self.assertEqual(codes[0] >> talk_assets.COLOR_SHIFT, 0)
        self.assertGreater(icon["width"], 0)
        self.assertEqual(set(icon["pixels"][index:index + 4] for index in range(0, len(icon["pixels"]), 4)), {RED})

    def test_the_icon_sits_a_letter_gap_in_and_centred_seventeen_rows_below_the_row_top(self):
        codes, cells = self.rows_and_glyphs(red_column_art())
        icon = cells[codes[1] & ((1 << talk_assets.COLOR_SHIFT) - 1)]
        ds_y = 144 + 8
        top = ds_talk_icons.ICON_CENTER_BELOW_ROW - ICON_HEIGHT // 2
        self.assertEqual(icon["left"], ds_set_text.screen_of(ds_text.LETTER_GAP + 1))
        self.assertEqual(icon["top"], ds_set_text.screen_of(ds_y % ds_set_text.DS_STEP + top) - ds_set_text.screen_of(ds_y % ds_set_text.DS_STEP))
        self.assertEqual(icon["height"], ds_set_text.screen_of(ds_y % ds_set_text.DS_STEP + top + ICON_HEIGHT) - ds_set_text.screen_of(ds_y % ds_set_text.DS_STEP + top))

    def test_an_icon_with_no_art_keeps_its_gap_and_draws_nothing(self):
        codes, cells = self.rows_and_glyphs({})
        self.assertEqual(cells[codes[1] & ((1 << talk_assets.COLOR_SHIFT) - 1)]["width"], 0)


class TintFieldTests(unittest.TestCase):
    def test_the_baker_and_the_app_agree_on_the_tint_field_and_as_is_is_white(self):
        with open(TALK_HEADER, encoding="utf-8") as source:
            header = source.read()
        with open(TALK_VIEW, encoding="utf-8") as source:
            view = source.read()
        shift = define_of(header, "SCRIPT_TALK_COLOR_SHIFT")
        count = define_of(header, "SCRIPT_TALK_COLOR_COUNT")
        self.assertEqual(shift, talk_assets.COLOR_SHIFT)
        self.assertEqual(define_of(header, "SCRIPT_TALK_GLYPH_ID_MASK"), (1 << shift) - 1)
        self.assertGreaterEqual(1 << shift, define_of(header, "SCRIPT_TALK_GLYPH_MAX"))
        self.assertLessEqual((count - 1) << shift | ((1 << shift) - 1), 0xffff)
        table = view[view.index("text_colors[SCRIPT_TALK_COLOR_COUNT]"):]
        table = table[:table.index("};")]
        colors = re.findall(r"\{ ([0-9.f, ]+) \}", table)
        self.assertEqual(len(colors), count)
        self.assertLess(talk_assets.COLOR_AS_IS, count)
        self.assertEqual(colors[talk_assets.COLOR_AS_IS].replace(" ", ""), "1.0f,1.0f,1.0f,1.0f")


if __name__ == "__main__":
    unittest.main()
