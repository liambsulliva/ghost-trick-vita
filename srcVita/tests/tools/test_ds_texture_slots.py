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


def absent_textures():
    with open(GAME_DESCRIPTION, encoding="utf-8") as source:
        return json.load(source)["ds_absent_textures"]


def absent_slots():
    return {name for name in absent_textures() if not name.endswith("*")}


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
        self.assertEqual(asked - ds_texture_names(), absent_slots() & asked)
        self.assertTrue(asked - ds_texture_names() <= absent_slots())

    def test_every_flipbook_the_app_draws_by_name_has_a_ds_source(self):
        asked = flipbooks_the_app_asks_for()
        self.assertIn("swirl_waves", asked)
        self.assertEqual(asked - ds_flipbook_names(), set())

    def test_no_absent_texture_has_a_ds_entry(self):
        self.assertFalse(absent_slots() & ds_texture_names())

    def test_the_converter_writes_every_absent_texture_into_the_set(self):
        import convert_assets
        with open(GAME_DESCRIPTION, encoding="utf-8") as source:
            text = convert_assets.absent_textures_text(json.load(source))
        self.assertEqual(text.splitlines(), sorted(absent_textures()))

    def test_every_absent_texture_has_a_reason(self):
        for name, reason in absent_textures().items():
            self.assertTrue(reason.strip(), name)


if __name__ == "__main__":
    unittest.main()
