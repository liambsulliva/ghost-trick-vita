import glob
import json
import os
import re
import unittest

SRC_VITA = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
GAME_DESCRIPTION = os.path.join(SRC_VITA, "tools", "assets", "game.json")
UI_CLIPS = os.path.join(SRC_VITA, "tools", "assets", "ui_clips.py")
APP_SOURCES = os.path.join(SRC_VITA, "src", "app", "*.c")
SLOT_DEFINE = re.compile(r'#define \w*TEXTURE "([a-z_0-9]+)"')
BANNER_NAME = re.compile(r"banner_[a-z]+")
KNOWN_CARTRIDGE_GAPS = {
    "film_corner": "not found",
    "film_strip": "not found",
    "hud_button_art": "DS pills are whole sprites in hud_pills",
    "hud_sheet": "Vita button glyphs have no cartridge art",
    "narration_arrow": "the DS shows none",
    "phone_cap": "not found",
    "phone_strip": "not found",
    "side_label": "not in cpac_3d",
    "swirl_back": "not found",
    "swirl_particle": "not found",
    "talk_cursor": "the DS shows none",
    "ui_hourglass": "the DS has three whole frames, the app wants parts",
    "ui_mark": "not found",
}


def slots_the_app_asks_for():
    names = set()
    for path in glob.glob(APP_SOURCES):
        with open(path, encoding="utf-8") as source:
            names.update(SLOT_DEFINE.findall(source.read()))
    with open(UI_CLIPS, encoding="utf-8") as source:
        names.update(set(BANNER_NAME.findall(source.read())) - {"banner_clip"})
    return names


def ds_texture_names():
    with open(GAME_DESCRIPTION, encoding="utf-8") as source:
        return {entry["name"] for entry in json.load(source)["ds_textures"]}


class DsTextureSlotTests(unittest.TestCase):
    def test_every_texture_slot_the_app_draws_has_a_ds_entry_or_a_known_gap(self):
        asked = slots_the_app_asks_for()
        self.assertTrue({"soul_blob", "soul_glow", "soul_orb", "soul_beam", "banner_trick"} <= asked)
        self.assertEqual(asked - ds_texture_names(), set(KNOWN_CARTRIDGE_GAPS))

    def test_a_known_gap_is_a_slot_the_app_asks_for_and_has_no_entry(self):
        self.assertTrue(set(KNOWN_CARTRIDGE_GAPS) <= slots_the_app_asks_for())
        self.assertFalse(set(KNOWN_CARTRIDGE_GAPS) & ds_texture_names())


if __name__ == "__main__":
    unittest.main()
