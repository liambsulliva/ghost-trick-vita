import json
import os
import shutil
import tempfile
import unittest

import asset_writers
import bake_record
import ds_textures
import ds_ui

RED = bytes((255, 0, 0, 255))
BLUE = bytes((0, 0, 255, 255))
CLEAR = bytes(4)
GAME_DESCRIPTION = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))), "tools", "assets", "game.json")
TEXTURES_THE_APP_DRAWS_WITH_NO_PARTS_OF_ITS_OWN = ["talk_box_top", "talk_box_bottom", "ui_choice_buttons", "ui_select_button", "ui_tap_balloon", "rewind_rocks_far", "rewind_rocks_mid", "rewind_rocks_near", "soul_spokes"]


def image(width, height, pixels):
    return width, height, b"".join(pixels)


class FakePack:
    def __init__(self, images):
        self.images = images
        self.asked = []

    def texture_bytes(self, index, language=1):
        if index not in self.images:
            raise ds_textures.TextureRefused("no texture %d" % index)
        return repr((index, language)).encode("ascii")

    def texture(self, index, language=1, first_color_is_clear=True):
        self.asked.append((index, language, first_color_is_clear))
        return self.images[index]


def pixel(rgba, width, x, y):
    return rgba[(y * width + x) * 4:(y * width + x) * 4 + 4]


class ComposeTests(unittest.TestCase):
    def test_a_part_is_cropped_scaled_to_the_nearest_pixel_and_placed(self):
        source = image(2, 1, [RED, BLUE])
        _, _, rgba = ds_ui.compose({"width": 6, "height": 3, "parts": [{"texture": 1, "crop": [0, 0, 2, 1], "x": 1, "y": 1, "width": 4, "height": 2}]}, lambda index, opaque: source)
        self.assertEqual([pixel(rgba, 6, x, 1) for x in range(6)], [CLEAR, RED, RED, BLUE, BLUE, CLEAR])
        self.assertEqual(pixel(rgba, 6, 2, 2), RED)
        self.assertEqual(pixel(rgba, 6, 2, 0), CLEAR)

    def test_a_flipped_part_mirrors_its_columns(self):
        source = image(2, 1, [RED, BLUE])
        _, _, rgba = ds_ui.compose({"width": 2, "height": 1, "parts": [{"texture": 1, "flip_x": True}]}, lambda index, opaque: source)
        self.assertEqual([pixel(rgba, 2, 0, 0), pixel(rgba, 2, 1, 0)], [BLUE, RED])

    def test_nine_slices_keep_the_corners_and_stretch_the_middle(self):
        source = image(3, 3, [RED, BLUE, RED, BLUE, CLEAR, BLUE, RED, BLUE, RED])
        _, _, rgba = ds_ui.compose({"width": 6, "height": 6, "parts": [{"texture": 1, "crop": [0, 0, 3, 3], "corner": 1, "x": 0, "y": 0, "width": 6, "height": 6, "target_corner": 2}]}, lambda index, opaque: source)
        self.assertEqual([pixel(rgba, 6, x, 0) for x in range(6)], [RED, RED, BLUE, BLUE, RED, RED])
        self.assertEqual(pixel(rgba, 6, 3, 3), CLEAR)
        self.assertEqual(pixel(rgba, 6, 5, 5), RED)

    def test_a_part_outside_its_texture_or_a_crop_outside_its_source_is_refused(self):
        source = image(2, 1, [RED, BLUE])
        with self.assertRaises(ds_ui.DsUiError):
            ds_ui.compose({"width": 2, "height": 1, "parts": [{"texture": 1, "x": 1}]}, lambda index, opaque: source)
        with self.assertRaises(ds_ui.DsUiError):
            ds_ui.compose({"width": 2, "height": 1, "parts": [{"texture": 1, "crop": [1, 0, 2, 1]}]}, lambda index, opaque: source)


class BakeTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.mkdtemp()

    def tearDown(self):
        shutil.rmtree(self.folder)

    def test_textures_bake_once_in_the_set_s_language_and_a_missing_source_is_left_out(self):
        descriptions = [{"name": "talk_box_top", "width": 2, "height": 1, "parts": [{"texture": 7, "opaque": True}]}, {"name": "gone", "width": 2, "height": 1, "parts": [{"texture": 8}]}]
        pack = FakePack({7: image(2, 1, [RED, BLUE])})
        record = bake_record.BakeRecord(self.folder)
        paths, sizes = ds_ui.bake(descriptions, pack, 3, record)
        self.assertEqual(paths, ["textures/talk_box_top.gtex"])
        self.assertEqual(sizes, {"talk_box_top": (2, 1)})
        self.assertEqual(pack.asked, [(7, 3, False)])
        ds_ui.bake(descriptions, pack, 3, record)
        self.assertEqual(len(pack.asked), 1)
        ds_ui.bake(descriptions, pack, 2, record)
        self.assertEqual(pack.asked[-1], (7, 2, False))


class DescriptionTests(unittest.TestCase):
    def test_every_interface_texture_the_ds_set_names_has_parts_that_fit_it(self):
        with open(GAME_DESCRIPTION, encoding="utf-8") as source:
            descriptions = json.load(source)["ds_textures"]
        names = [description["name"] for description in descriptions]
        self.assertEqual(len(names), len(set(names)))
        for name in TEXTURES_THE_APP_DRAWS_WITH_NO_PARTS_OF_ITS_OWN:
            self.assertIn(name, names)
        for description in descriptions:
            asset_writers.check_name(description["name"])
            for part in ds_ui.plain_parts(description["parts"]):
                if "width" in part:
                    self.assertGreater(part["width"], 0, description["name"])
                    self.assertGreater(part["height"], 0, description["name"])
                    self.assertLessEqual(part.get("x", 0) + part["width"], description["width"], description["name"])
                    self.assertLessEqual(part.get("y", 0) + part["height"], description["height"], description["name"])


if __name__ == "__main__":
    unittest.main()
