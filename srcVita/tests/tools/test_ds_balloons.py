import json
import os
import struct
import unittest

import ds_balloons
import ds_text
import ds_textures
import frame_layout
import script_assets

SRC_VITA = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
ROM_PATH = os.path.join(os.path.dirname(SRC_VITA), "extracted", "ds_rom", "game.nds")
GAME_DESCRIPTION = os.path.join(SRC_VITA, "tools", "assets", "game.json")
FILL = 3
INK = 15
LETTER = 10
LETTER_WIDTH = 5
LETTER_ROWS = range(2, 10)
FONT_SHAPE = {"cell_width": 16, "cell_height": 16, "advance": 12, "line_height": 12, "glyph_bytes": 128, "two_bits": False, "kind": 2}


def sdt_texture(width, height, rows):
    width_code = (width // 8).bit_length() - 1
    data = bytearray()
    for row in rows:
        for column in range(0, width, 2):
            data.append(row[column] | (row[column + 1] << 4))
    palette = bytes(32)
    header_size = ds_textures.HEADER_SIZE
    header = struct.pack(ds_textures.HEADER, ds_textures.MAGIC, 0, ds_textures.FORMAT_16_COLORS, width_code, 0, header_size, len(data), header_size + len(data), len(palette), width, height)
    return header + bytes(data) + palette


def filled(width, height):
    return sdt_texture(width, height, [[FILL] * width for _ in range(height)])


def tail(width, height):
    return sdt_texture(width, height, [[FILL if column < 4 else 0 for column in range(width)] for _ in range(height)])


class FakeTables:
    sizes = [(0, 0, 0, 0), (1, 0, 0, 0), (108, 1, 86, 32), (107, 1, 108, 32), (114, 1, 52, 32)]
    spaces = [(4, 16, 9, 6, 0), (4, 16, 9, 6, 0), (4, 0, 9, 2, -4), (5, 0, 8, 1, -4), (2, 0, 9, 2, -4)]
    frames = [(8, 32, 3, 16, 8), (8, 44, 3, 16, 12)]
    pieces = [(110, 111, 112, 113), (116, 117, 118, 119)]


TEXTURES = {
    109: sdt_texture(128, 64, [[0] * 128 for _ in range(64)]),
    115: sdt_texture(256, 64, [[0] * 256 for _ in range(64)]),
    110: filled(16, 32), 111: filled(16, 32), 112: filled(16, 32), 113: tail(32, 8),
    116: filled(16, 48), 117: filled(16, 48), 118: filled(16, 48), 119: tail(32, 16),
    107: filled(128, 64), 108: filled(128, 64), 114: filled(128, 64),
}


def pictures(entry):
    return ds_balloons.texture_indexes(TEXTURES[entry])


def fake_font():
    glyphs = bytearray(FONT_SHAPE["glyph_bytes"] * 0x100)
    for row in LETTER_ROWS:
        for column in range(0, 4, 2):
            glyphs[LETTER * 128 + row * 8 + column // 2] = 3 | (3 << 4)
    widths = bytearray(2 * 0x100)
    widths[2 * LETTER + 1] = LETTER_WIDTH
    return ds_text.DsFont(ds_text.BALLOON_FONT, FONT_SHAPE, bytes(glyphs), bytes(widths), bytes(0x100 * 32))


def message(*lines):
    return {"pages": [[[{"items": items} for items in lines]]]}


def letters(count, centered=False):
    return ([("control", ds_balloons.CENTER_LINE_CODE, ())] if centered else []) + [("char", LETTER)] * count


def solid_columns(picture, row):
    return [column for column in range(picture.width) if picture.at(column, row)]


def compose_one(kind, *lines):
    rows, images, refused = ds_balloons.compose({("said", kind): "script"}, lambda label, path: message(*lines), fake_font(), FakeTables(), pictures)
    return rows, images, refused


class OvalTests(unittest.TestCase):
    def test_a_one_line_oval_is_centred_on_its_canvas_and_wide_enough_for_its_text(self):
        rows, images, _ = compose_one(0, letters(6))
        shape = images[rows[0]["parts"][0]["texture"]]
        segments = ds_balloons.segment_count(6 * LETTER_WIDTH)
        middle = solid_columns(shape, ds_balloons.ONE_LINE_TOP + 16)
        self.assertEqual((shape.width, shape.height), (128, 64))
        self.assertEqual(len(middle), 32 + segments * 6)
        self.assertLessEqual(abs((middle[0] + middle[-1] + 1) / 2 - 64), 1)

    def test_kind_zero_puts_the_tail_left_of_centre_and_kind_one_right_of_it(self):
        tail_row = ds_balloons.ONE_LINE_TOP + 32
        for kind, is_left in ((0, True), (1, False)):
            rows, images, _ = compose_one(kind, letters(6))
            tail_columns = solid_columns(images[rows[0]["parts"][0]["texture"]], tail_row)
            self.assertTrue(tail_columns)
            self.assertEqual((tail_columns[0] + tail_columns[-1]) / 2 < 64, is_left)

    def test_two_lines_use_the_tall_pieces_and_a_long_line_the_wide_canvas(self):
        rows, images, _ = compose_one(0, letters(2), letters(2))
        self.assertNotEqual(solid_columns(images[rows[0]["parts"][0]["texture"]], ds_balloons.MORE_LINES_TOP + 43), [])
        rows, images, _ = compose_one(0, letters(30))
        self.assertEqual(images[rows[0]["parts"][0]["texture"]].width, 256)

    def test_a_centred_line_sits_in_the_middle_and_a_plain_line_at_the_left(self):
        text = ds_balloons.text_picture(fake_font(), message(letters(2, centered=True), letters(2))["pages"][0][0], ds_balloons.shape_for(0, [30, 10], FakeTables(), pictures))
        first = solid_columns(text, ds_balloons.MORE_LINES_TOP + 6 + LETTER_ROWS[0])
        second = solid_columns(text, ds_balloons.MORE_LINES_TOP + 6 + 16 + LETTER_ROWS[0])
        self.assertGreater(first[0], second[0])


class ThornTests(unittest.TestCase):
    def test_the_thorn_texture_is_chosen_by_the_widest_line(self):
        for count, entry in ((2, 114), (12, 108), (30, 107)):
            rows, _, _ = compose_one(2, letters(count))
            self.assertEqual(rows[0]["parts"][0]["texture"], ds_balloons.SHAPE_NAME % ("thorn_%d" % entry))


class RowTests(unittest.TestCase):
    def test_the_shape_grows_first_and_the_text_shows_when_it_is_whole(self):
        rows, images, refused = compose_one(1, letters(4))
        self.assertEqual(refused, [])
        shape_part, text_part = rows[0]["parts"]
        self.assertEqual((rows[0]["kind"], rows[0]["label"]), (1, "said"))
        self.assertEqual((shape_part["after"], shape_part["grow"]), (0, ds_balloons.GROW_TICKS))
        self.assertEqual((text_part["after"], text_part["grow"]), (ds_balloons.GROW_TICKS, 0))
        self.assertIn(text_part["texture"], images)

    def test_balloons_text_draws_the_shape_at_its_ds_size(self):
        rows, images, _ = compose_one(1, letters(4))
        sizes = {name: (picture.width, picture.height) for name, picture in images.items()}
        line = script_assets.balloons_text(rows, sizes).splitlines()[1].split()
        self.assertEqual((float(line[4]), float(line[5])), (128.0, 64.0))
        self.assertEqual(ds_balloons.TEXTURE_SCALE, frame_layout.SCREEN_PIXELS_PER_DS_PIXEL)

    def test_a_kind_with_no_native_shape_is_left_out_and_named(self):
        rows, images, refused = compose_one(5, letters(4))
        self.assertEqual((rows, images), ([], {}))
        self.assertEqual(len(refused), 1)


@unittest.skipUnless(os.path.isfile(ROM_PATH), "needs the cartridge")
class CartridgeTests(unittest.TestCase):
    def test_every_worded_balloon_of_the_game_gets_a_row_for_each_kind_it_opens_with(self):
        import ds_rom
        import ds_set
        import ds_set_text
        with open(GAME_DESCRIPTION, encoding="utf-8") as source:
            description = json.load(source)
        cartridge = ds_set.Cartridge(ROM_PATH)
        _, scripts, arm9, *_ = ds_set.expand(description, ROM_PATH, cartridge)
        with ds_rom.DsRom(ROM_PATH) as rom:
            font = ds_text.font_from_rom(rom, ds_text.BALLOON_FONT, arm9)
            book = ds_set_text.MessageBook(rom, scripts, "en")

        def cartridge_pictures(entry):
            return ds_balloons.texture_indexes(cartridge.textures.texture_bytes(entry))

        wanted = ds_balloons.balloon_labels(scripts)
        rows, images, refused = ds_balloons.compose(wanted, book.find, font, ds_balloons.Tables(arm9), cartridge_pictures)
        self.assertEqual(refused, [])
        self.assertEqual({(row["label"], row["kind"]) for row in rows}, set(wanted))
        self.assertTrue(all(part["texture"] in images for row in rows for part in row["parts"]))
        said = next(row for row in rows if row["label"] == "m01_0050")
        shape = images[said["parts"][0]["texture"]]
        self.assertEqual(solid_columns(shape, 30)[0], 2)
        self.assertEqual(solid_columns(shape, 30)[-1], 123)


if __name__ == "__main__":
    unittest.main()
