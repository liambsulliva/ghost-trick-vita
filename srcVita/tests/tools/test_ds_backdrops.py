import json
import os
import struct
import unittest

import ds_backdrops
import ds_rom
import ds_sprites
import script_assets

SRC_VITA = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
GAME_DESCRIPTION = os.path.join(SRC_VITA, "tools", "assets", "game.json")
ROM_PATH = os.path.join(os.path.dirname(SRC_VITA), "extracted", "ds_rom", "game.nds")
RED = 0x001f
GREEN = 0x03e0
BLUE = 0x7c00
SMALL = {"name": "test_waves", "block": 4, "entry": 1, "palette_count": 4, "map_columns": 2, "ticks_per_frame": 16}


def palette(*colors):
    words = list(colors) + [0] * (ds_backdrops.COLORS_PER_PALETTE - len(colors))
    return struct.pack("<16H", *words)


def small_entry(tile_indexes):
    palettes = palette(0, RED, GREEN) + palette(0, GREEN, BLUE) + palette(0, BLUE, RED) + palette(0, RED, RED)
    tiles = b"".join(bytes([index]) * ds_backdrops.TILE_BYTES for index in tile_indexes)
    return bytes(4) + palettes + tiles


def pixel(rgba, width, x, y):
    start = (y * width + x) * 4
    return tuple(rgba[start:start + 4])


def game_backdrops():
    with open(GAME_DESCRIPTION, encoding="utf-8") as source:
        return json.load(source).get("ds_backdrops", [])


class BackdropLayoutTests(unittest.TestCase):
    def test_each_frame_is_the_tile_map_in_the_next_palette(self):
        width, rgba = ds_backdrops.page_rgba(small_entry([1, 2, 0, 1]), SMALL)
        self.assertEqual(width, 32)
        self.assertEqual(pixel(rgba, width, 0, 0), (255, 0, 0, 255))
        self.assertEqual(pixel(rgba, width, 8, 0), (0, 255, 0, 255))
        self.assertEqual(pixel(rgba, width, 0, 8)[3], 0)
        self.assertEqual(pixel(rgba, width, 16, 0), (0, 255, 0, 255))
        self.assertEqual(pixel(rgba, width, 24, 0), (0, 0, 255, 255))
        self.assertEqual(pixel(rgba, width, 0, 16), (0, 0, 255, 255))
        self.assertEqual(pixel(rgba, width, 16, 16), (255, 0, 0, 255))

    def test_the_flipbook_holds_one_frame_per_palette(self):
        row = ds_backdrops.flipbook_row(SMALL)
        self.assertEqual((row["frames"], row["columns"], row["rows"], row["cell_width"]), (4, 2, 2, 0.5))
        self.assertEqual(script_assets.flipbooks_text([row]), "flipbook test_waves 16 4 2 2 0.5 0.5 test_waves_00\n")

    def test_a_short_entry_or_a_colour_past_the_palette_is_refused(self):
        with self.assertRaises(ds_backdrops.BackdropRefused):
            ds_backdrops.page_rgba(small_entry([1, 2, 0]), SMALL)
        with self.assertRaises(ds_backdrops.BackdropRefused):
            ds_backdrops.page_rgba(small_entry([1, 2, 0, 16]), SMALL)


@unittest.skipUnless(os.path.exists(ROM_PATH), "needs the DS ROM")
class CartridgeBackdropTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with ds_rom.DsRom(ROM_PATH) as rom:
            cls.pack = ds_sprites.SpritePack(rom.read_file("cpac_2d.bin"))

    def test_every_backdrop_the_set_names_decodes_with_one_frame_per_palette(self):
        self.assertIn("swirl_waves", [description["name"] for description in game_backdrops()])
        for description in game_backdrops():
            entry_bytes = self.pack.entry(description["block"], description["entry"])
            width, rgba = ds_backdrops.page_rgba(entry_bytes, description)
            side = ds_backdrops.frame_side(description)
            self.assertEqual(width, side * ds_backdrops.page_columns(description))
            columns = ds_backdrops.page_columns(description)
            frames = set()
            for frame in range(description["palette_count"]):
                left = (frame % columns) * side
                top = (frame // columns) * side
                frames.add(b"".join(rgba[((top + row) * width + left) * 4:((top + row) * width + left + side) * 4] for row in range(side)))
            self.assertEqual(len(frames), description["palette_count"])

    def test_the_swirl_waves_are_red_with_a_dark_middle(self):
        description = [entry for entry in game_backdrops() if entry["name"] == "swirl_waves"][0]
        width, rgba = ds_backdrops.page_rgba(self.pack.entry(description["block"], description["entry"]), description)
        middle = pixel(rgba, width, 128, 128)
        self.assertLess(sum(middle[:3]), 60)
        corner = pixel(rgba, width, 4, 4)
        self.assertGreater(corner[0], corner[1] + 40)


if __name__ == "__main__":
    unittest.main()
