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
TEXTURES_THE_APP_DRAWS_WITH_NO_PARTS_OF_ITS_OWN = ["talk_box_top", "talk_box_bottom", "ui_choice_buttons", "ui_select_button", "ui_tap_balloon", "rewind_rocks_far", "rewind_rocks_mid", "rewind_rocks_near", "soul_spokes", "hud_pills", "pause_backdrop", "title_logo"]


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

    def test_a_cut_out_keeps_what_is_not_the_red_backdrop_and_a_rim_around_it(self):
        dark = bytes((120, 8, 8, 255))
        white = bytes((250, 250, 250, 255))
        bright = bytes((230, 0, 0, 255))
        source = image(7, 1, [dark, dark, dark, white, dark, dark, bright])
        _, _, rgba = ds_ui.compose({"width": 7, "height": 1, "parts": [{"texture": 1, "cut_out": {"other_channels_above": 40, "red_above": 180, "grow": 1}}]}, lambda index, opaque: source)
        self.assertEqual([pixel(rgba, 7, x, 0) for x in range(7)], [CLEAR, CLEAR, dark, white, dark, dark, bright])

    def test_a_fill_colors_the_whole_texture_with_no_part(self):
        _, _, rgba = ds_ui.compose({"width": 2, "height": 2, "fill": [123, 8, 8], "parts": []}, lambda index, opaque: None)
        self.assertEqual(rgba, bytes((123, 8, 8, 255)) * 4)

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

    def test_a_cell_part_is_trimmed_to_its_ink_and_set_against_the_side_its_alignment_names(self):
        source = image(3, 2, [CLEAR, RED, CLEAR, CLEAR, CLEAR, CLEAR])
        _, _, rgba = ds_ui.compose({"width": 8, "height": 2, "parts": [{"texture": 1, "cell": [0, 0, 4, 2], "align": [1, 1]}, {"texture": 1, "cell": [4, 0, 4, 2], "align": [0, 0]}]}, lambda index, opaque: source)
        self.assertEqual([pixel(rgba, 8, x, 1) for x in range(4)], [CLEAR, CLEAR, CLEAR, RED])
        self.assertEqual([pixel(rgba, 8, x, 0) for x in range(4, 8)], [RED, CLEAR, CLEAR, CLEAR])
        with self.assertRaises(ds_ui.DsUiError):
            ds_ui.compose({"width": 8, "height": 2, "parts": [{"texture": 1, "cell": [0, 0, 4, 2]}]}, lambda index, opaque: image(1, 1, [CLEAR]))
        with self.assertRaises(ds_ui.DsUiError):
            ds_ui.compose({"width": 8, "height": 2, "parts": [{"texture": 1, "cell": [0, 0, 1, 1]}]}, lambda index, opaque: image(2, 1, [RED, RED]))

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
                if "cell" in part:
                    self.assertLessEqual(part["cell"][0] + part["cell"][2], description["width"], description["name"])
                    self.assertLessEqual(part["cell"][1] + part["cell"][3], description["height"], description["name"])
                if "width" in part:
                    self.assertGreater(part["width"], 0, description["name"])
                    self.assertGreater(part["height"], 0, description["name"])
                    self.assertLessEqual(part.get("x", 0) + part["width"], description["width"], description["name"])
                    self.assertLessEqual(part.get("y", 0) + part["height"], description["height"], description["name"])


class PillSheetTests(unittest.TestCase):
    def test_the_pill_sheet_has_a_resting_and_a_pressed_cell_for_every_pill_the_app_draws(self):
        with open(GAME_DESCRIPTION, encoding="utf-8") as source:
            sheet = [description for description in json.load(source)["ds_textures"] if description["name"] == "hud_pills"][0]
        with open(os.path.join(os.path.dirname(GAME_DESCRIPTION), "..", "..", "include", "script_outputs.h"), encoding="utf-8") as header:
            text = header.read()
        pills = text[text.index("enum script_pill_id {"):text.index("SCRIPT_PILL_COUNT")].count("SCRIPT_PILL_")
        cells = sorted(tuple(part["cell"]) for part in sheet["parts"])
        self.assertEqual(len(cells), pills * 2)
        cell_width = sheet["width"] // 2
        cell_height = sheet["height"] // pills
        self.assertEqual(cells, sorted((column * cell_width, row * cell_height, cell_width, cell_height) for row in range(pills) for column in range(2)))


if __name__ == "__main__":
    unittest.main()
