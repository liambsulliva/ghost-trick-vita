import json
import os
import sys
import unittest

SRC_VITA = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
GAME_DESCRIPTION = os.path.join(SRC_VITA, "tools", "assets", "game.json")
DS_ROM = os.path.join(os.path.dirname(SRC_VITA), "extracted", "ds_rom", "game.nds")
TALK_BOXES = ("talk_box_top", "talk_box_bottom")
CHOICE_MARKS = (196, 197)


def description():
    with open(GAME_DESCRIPTION, encoding="utf-8") as source:
        return json.load(source)


def ds_entry(name):
    return next(entry for entry in description()["ds_textures"] if entry["name"] == name)


class DsButtonAndBoxArtTests(unittest.TestCase):
    def test_every_ds_choice_panel_wears_the_title_buttons_resting_and_selected_art(self):
        buttons = description()["pause"]["ds_art"]["title_buttons"]
        textures = sorted(part["texture"] for part in ds_entry("ui_choice_buttons")["parts"] if part["texture"] not in CHOICE_MARKS)
        self.assertEqual(textures, sorted([buttons["resting"], buttons["selected"]]))

    def test_every_ds_choice_panel_has_the_cartridge_check_and_circle(self):
        textures = [part["texture"] for part in ds_entry("ui_choice_buttons")["parts"]]
        self.assertEqual(sorted(texture for texture in textures if texture in CHOICE_MARKS), sorted(CHOICE_MARKS))

    @unittest.skipUnless(os.path.exists(DS_ROM), "needs the DS ROM")
    def test_the_ds_talk_corner_allowance_is_the_rounding_of_the_cartridge_box_art(self):
        sys.path.insert(0, os.path.join(SRC_VITA, "tools", "assets"))
        import ds_pause
        import ds_set
        pack = ds_set.Cartridge(DS_ROM).textures
        for name in TALK_BOXES:
            corner = ds_entry(name)["parts"][0]
            pixels = ds_pause.texture_pixels(pack, corner["texture"], 1)
            left, top, width, _ = corner["crop"]
            first_row = pixels[top, left:left + width, 3]
            rounding = int((first_row == 0).argmin())
            self.assertEqual(round(rounding * corner["width"] / width), description()["talk"]["ds_box_corner"], name)


if __name__ == "__main__":
    unittest.main()
