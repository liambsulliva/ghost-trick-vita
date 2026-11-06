import sys
import unittest
from unittest import mock

import asset_writers
import convert_assets


class StepExpansionTests(unittest.TestCase):
    def test_expanding_a_set_description(self):
        set_description = {
            "message_hold_ticks": 30,
            "objects": {"a": {"layer": 5, "flip": 1}},
            "object_groups": {"group": [["a", "anim_a", 1, 2]]},
            "steps": [["plate", "p", 0, 8], ["object_group", "group"], ["message_hold"], ["object", "a", "anim_b", 3, 4], ["wait", 7]],
        }
        self.assertEqual(convert_assets.expand_steps(set_description, {"p": (16.0, 32.0)}), [
            ["plate", "p", 0, 8, 16.0, 32.0],
            ["object", "a", "anim_a", 1, 2, 5, 1],
            ["wait", 30],
            ["object", "a", "anim_b", 3, 4, 5, 1],
            ["wait", 7],
        ])

    def test_unknown_assets_are_refused(self):
        with self.assertRaises(ValueError):
            convert_assets.check_steps_refer_to_known_assets([["card", "missing"]], set(), set())
        with self.assertRaises(ValueError):
            convert_assets.check_steps_refer_to_known_assets([["object", "a", "missing", 0, 0, 0, 0]], set(), set())


class RealSetDescriptionTests(unittest.TestCase):
    def test_chapter1_opening_only_names_assets_it_declares(self):
        set_description = convert_assets.load_set_description("chapter1_opening")
        plate_names = [plate["name"] for plate in set_description["plates"]]
        screen_names = [texture["name"] for texture in set_description["screen_textures"]]
        animation_names = set(animation["name"] for animation in set_description["animations"])
        steps = convert_assets.expand_steps(set_description, {name: (1.0, 1.0) for name in plate_names})
        convert_assets.check_steps_refer_to_known_assets(steps, set(plate_names + screen_names), animation_names)
        self.assertLessEqual(len(steps), asset_writers.SCENE_STEP_MAX)


class TextureToolBuildTests(unittest.TestCase):
    def test_main_brings_the_texture_tool_up_to_date_first(self):
        calls = []
        with mock.patch.object(convert_assets.subprocess, "run", side_effect=lambda command, check: calls.append((command, check))):
            with mock.patch.object(convert_assets, "convert_set_description") as convert:
                with mock.patch.object(convert_assets, "load_set_description", return_value={}):
                    with mock.patch.object(sys, "argv", ["convert_assets.py", "--set", "tiny", "--output", "out"]):
                        self.assertEqual(convert_assets.main(), 0)
        self.assertEqual(calls, [(["make", "-C", convert_assets.ASSET_TOOLS_FOLDER], True)])
        convert.assert_called_once()


if __name__ == "__main__":
    unittest.main()
