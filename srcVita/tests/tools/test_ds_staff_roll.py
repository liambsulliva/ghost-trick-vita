import struct
import unittest

import chapter_assets
import ds_staff_roll

BLACK = bytes((0, 0, 0, 255))
WHITE = bytes((255, 255, 255, 255))
COLORS = [BLACK, WHITE] + [BLACK] * 14
TILE_WITH_ONE_WHITE_PIXEL_AT_ITS_LEFT_TOP = bytes((0x01,)) + bytes(31)


class FakeSprites:
    def __init__(self, entries):
        self.entries = entries

    def entry(self, block, index, language=1):
        return self.entries[(block, index)]


def fake_arm9(pages, screen_rows, banks):
    arm9 = bytearray(ds_staff_roll.SCREEN_TABLE_ADDRESS - ds_staff_roll.ARM9_LOAD_ADDRESS + 8 * 32)
    for page, (first_row, count) in pages.items():
        offset = ds_staff_roll.PAGE_TABLE_ADDRESS - ds_staff_roll.ARM9_LOAD_ADDRESS + page * 2
        arm9[offset:offset + 2] = bytes((first_row, count))
    for row, pair in enumerate(screen_rows):
        struct.pack_into("<II", arm9, ds_staff_roll.SCREEN_TABLE_ADDRESS - ds_staff_roll.ARM9_LOAD_ADDRESS + row * 8, *pair)
    for bank, pair in enumerate(banks):
        struct.pack_into("<II", arm9, ds_staff_roll.LONG_ROLL_TABLE_ADDRESS - ds_staff_roll.ARM9_LOAD_ADDRESS + bank * 8, *pair)
    return bytes(arm9)


def screen_tiles():
    return TILE_WITH_ONE_WHITE_PIXEL_AT_ITS_LEFT_TOP + bytes(ds_staff_roll.SCREEN_TILE_BYTES - ds_staff_roll.TILE_BYTES)


def screen_map(entries):
    words = list(entries) + [1] * (ds_staff_roll.SCREEN_MAP_ENTRIES - len(entries))
    return struct.pack("<%dH" % len(words), *words) + bytes(ds_staff_roll.SCREEN_MAP_BYTES - 2 * len(words))


def pixel(pixels, x, y):
    at = (y * ds_staff_roll.SCREEN_WIDTH + x) * 4
    return pixels[at:at + 4]


class DecodeTests(unittest.TestCase):
    def test_a_screen_places_each_tile_and_honors_both_flips(self):
        pixels = ds_staff_roll.decode_screen(screen_tiles(), screen_map([0, ds_staff_roll.FLIP_ACROSS, ds_staff_roll.FLIP_DOWN]), COLORS)
        self.assertEqual(len(pixels), 256 * 192 * 4)
        self.assertEqual(pixel(pixels, 0, 0), WHITE)
        self.assertEqual(pixel(pixels, 1, 0), BLACK)
        self.assertEqual(pixel(pixels, 15, 0), WHITE)
        self.assertEqual(pixel(pixels, 16, 7), WHITE)
        self.assertEqual(pixel(pixels, 16, 0), BLACK)

    def test_a_screen_of_the_wrong_size_is_refused(self):
        with self.assertRaises(ds_staff_roll.StaffRollRefused):
            ds_staff_roll.decode_screen(screen_tiles()[:-32], screen_map([0]), COLORS)

    def test_a_map_naming_a_tile_past_its_tiles_is_refused(self):
        with self.assertRaises(ds_staff_roll.StaffRollRefused):
            ds_staff_roll.picture_rows(TILE_WITH_ONE_WHITE_PIXEL_AT_ITS_LEFT_TOP, struct.pack("<32H", *([5] * 32)), 32, COLORS)


class TableTests(unittest.TestCase):
    def setUp(self):
        long_map = struct.pack("<%dH" % (32 * 48), *([0] * (32 * 48)))
        self.arm9 = fake_arm9({0: (0, 1), 4: (1, 2), 9: (3, 9)}, [(0x16, 0x1b), (0x5c, 0x61), (0x66, 0x6b)], [(0x110, 0x115)])
        self.sprites = FakeSprites({(2, 0x16): screen_tiles(), (2, 0x1b): screen_map([0]), (2, 0x5c): screen_tiles(), (2, 0x61): screen_map([0]), (2, 0x66): screen_tiles(), (2, 0x6b): screen_map([0]), (2, 0x110): TILE_WITH_ONE_WHITE_PIXEL_AT_ITS_LEFT_TOP, (2, 0x115): long_map})

    def test_pages_read_their_screens_and_the_long_roll_is_cut_into_screens(self):
        self.assertEqual(ds_staff_roll.paged_screens(self.arm9), {0: [(0x16, 0x1b)], 4: [(0x5c, 0x61), (0x66, 0x6b)]})
        self.assertEqual(ds_staff_roll.long_roll_banks(self.arm9), [(0x110, 0x115)])
        self.assertEqual(ds_staff_roll.screens_of_pages(self.arm9, self.sprites, 1), {0: 1, 4: 2, 9: 2})
        screens = ds_staff_roll.page_screens(9, ds_staff_roll.page_sources(self.arm9, self.sprites, 1)[9], COLORS)
        self.assertEqual(len(screens), 2)
        self.assertEqual(pixel(screens[1], 8, 0), WHITE)

    def test_the_rows_take_the_cartridge_screen_counts(self):
        text = chapter_assets.staff_roll_text({0: (15, 120, 45), 4: (25, 0, 45)}, {0: "bgm_end", 4: None}, {0: 1, 4: 2, 9: 2})
        self.assertEqual(text, "staffpage 0 1 15 120 45\nstaffpage 4 2 25 0 45\nstaffmusic bgm_end\n")

    def test_a_page_a_script_starts_with_no_screens_is_refused(self):
        with self.assertRaises(ValueError):
            chapter_assets.staff_roll_text({7: (25, 0, 45)}, {7: None}, {})


if __name__ == "__main__":
    unittest.main()
