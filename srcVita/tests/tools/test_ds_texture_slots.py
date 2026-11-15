import glob
import json
import os
import re
import unittest

import ui_clips

SRC_VITA = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
GAME_DESCRIPTION = os.path.join(SRC_VITA, "tools", "assets", "game.json")
APP_SOURCES = os.path.join(SRC_VITA, "src", "app", "*.c")
SLOT_DEFINE = re.compile(r'#define \w*TEXTURE "([a-z_0-9]+)"')
APP_HEADERS = os.path.join(SRC_VITA, "include", "*.h")
FLIPBOOK_DEFINE = re.compile(r'#define \w*FLIPBOOK "([a-z_0-9]+)"')
NO_TEXTURE = "-"
KNOWN_CARTRIDGE_GAPS = {
    "card_hand_seconds": "the DS watch has two hands, textures 190 and 191",
    "digit_0": "death count not traced",
    "digit_1": "death count not traced",
    "digit_2": "death count not traced",
    "digit_3": "death count not traced",
    "digit_label": "death count not traced",
    "film_corner": "not found",
    "film_strip": "not found",
    "hud_button_art": "DS pills are whole sprites in hud_pills",
    "hud_disc": "the DS card backdrop is one texture, effect 281 item 1",
    "hud_sheet": "Vita button glyphs have no cartridge art",
    "narration_arrow": "the DS shows none",
    "phone_cap": "not found",
    "phone_strip": "not found",
    "side_label": "not in cpac_3d",
    "talk_cursor": "the DS shows none",
    "ui_hourglass": "the DS has three whole frames, the app wants parts",
    "ui_mark": "not found",
    "ui_wave_quadrant": "DS 80 is a half arc",
    "wipe_time_passing": "not in cpac_3d block 0",
}


def slots_the_app_asks_for():
    names = set()
    for path in glob.glob(APP_SOURCES):
        with open(path, encoding="utf-8") as source:
            names.update(SLOT_DEFINE.findall(source.read()))
    names.update(element["texture"] for clip in ui_clips.all_clips() for element in clip["elements"] if element["texture"] != NO_TEXTURE)
    return names


def flipbooks_the_app_asks_for():
    names = set()
    for path in glob.glob(APP_SOURCES) + glob.glob(APP_HEADERS):
        with open(path, encoding="utf-8") as source:
            names.update(FLIPBOOK_DEFINE.findall(source.read()))
    return names


def ds_flipbook_names():
    with open(GAME_DESCRIPTION, encoding="utf-8") as source:
        return {entry["name"] for entry in json.load(source).get("ds_backdrops", [])}


def ds_texture_names():
    with open(GAME_DESCRIPTION, encoding="utf-8") as source:
        return {entry["name"] for entry in json.load(source)["ds_textures"]}


class DsTextureSlotTests(unittest.TestCase):
    def test_every_texture_slot_the_app_draws_has_a_ds_entry_or_a_known_gap(self):
        asked = slots_the_app_asks_for()
        self.assertTrue({"soul_blob", "soul_glow", "soul_orb", "soul_beam", "banner_trick", "card_clock", "digit_label"} <= asked)
        self.assertEqual(asked - ds_texture_names(), set(KNOWN_CARTRIDGE_GAPS))

    def test_every_flipbook_the_app_draws_by_name_has_a_ds_source(self):
        asked = flipbooks_the_app_asks_for()
        self.assertIn("swirl_waves", asked)
        self.assertEqual(asked - ds_flipbook_names(), set())

    def test_a_known_gap_is_a_slot_the_app_asks_for_and_has_no_entry(self):
        self.assertTrue(set(KNOWN_CARTRIDGE_GAPS) <= slots_the_app_asks_for())
        self.assertFalse(set(KNOWN_CARTRIDGE_GAPS) & ds_texture_names())


if __name__ == "__main__":
    unittest.main()
