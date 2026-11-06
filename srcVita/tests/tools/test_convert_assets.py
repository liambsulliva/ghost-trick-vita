import sys
import unittest
from unittest import mock

import asset_writers
import convert_assets

CARD_APPEARS_AT_CLIP_SECONDS = 8.267
ONE_FRAME_AND_A_BIT = 0.02
LESS_THAN_A_FRAME = 0.01


class StepExpansionTests(unittest.TestCase):
    def test_expanding_a_set_description(self):
        set_description = {
            "objects": {"a": {"layer": 5, "flip": 1}},
            "object_groups": {"group": [["a", "anim_a", 1, 2]]},
            "steps": [["plate", "p", 0, 8], ["object_group", "group"], ["object", "a", "anim_b", 3, 4], ["wait", 7]],
        }
        self.assertEqual(convert_assets.expand_steps(set_description, {"p": (16.0, 32.0)}), [
            ["plate", "p", 0, 8, 16.0, 32.0],
            ["object", "a", "anim_a", 1, 2, 5, 1],
            ["object", "a", "anim_b", 3, 4, 5, 1],
            ["wait", 7],
        ])

    def test_effect_steps_pass_through_unchanged(self):
        steps = [["mask_on"], ["beam_create", "b", -16, 64, 108, 542, 60], ["beam_animate", "b", 118, 367, 30, 60], ["beam_delete", "b"], ["iris", 144, 359, 384, 120], ["dim_to", 0.49, 10], ["flash", 0.5, 8], ["mask_off"]]
        set_description = {"objects": {}, "object_groups": {}, "steps": steps}
        self.assertEqual(convert_assets.expand_steps(set_description, {}), steps)
        self.assertEqual(asset_writers.scene_text("s", steps), "\n".join([
            "scene s",
            "mask_on",
            "beam_create b -16 64 108 542 60",
            "beam_animate b 118 367 30 60",
            "beam_delete b",
            "iris 144 359 384 120",
            "dim_to 0.49 10",
            "flash 0.5 8",
            "mask_off",
        ]) + "\n")

    def test_the_old_message_hold_step_is_gone(self):
        set_description = {"objects": {}, "object_groups": {}, "steps": [["message_hold"]]}
        self.assertEqual(convert_assets.expand_steps(set_description, {}), [["message_hold"]])
        self.assertNotIn("message_hold", convert_assets.load_set_description("chapter1_opening"))

    def test_unknown_assets_are_refused(self):
        with self.assertRaises(ValueError):
            convert_assets.check_steps_refer_to_known_assets([["card", "missing"]], set(), set(), set(), set())
        with self.assertRaises(ValueError):
            convert_assets.check_steps_refer_to_known_assets([["object", "a", "missing", 0, 0, 0, 0]], set(), set(), set(), set())
        with self.assertRaises(ValueError):
            convert_assets.check_steps_refer_to_known_assets([["music", "missing", 1]], set(), set(), set(), set())
        with self.assertRaises(ValueError):
            convert_assets.check_steps_refer_to_known_assets([["sound", "missing"]], set(), set(), set(), set())

    def test_declared_audio_is_accepted(self):
        convert_assets.check_steps_refer_to_known_assets([["music", "song", 1], ["music_stop", 4], ["sound", "ping"]], set(), set(), {"song"}, {"ping"})


def ticks_held_by(step):
    return step[-1] if step[0] in ("wait", "fade_in", "fade_out", "camera_pan") else 0


def clip_seconds_at_each_step(steps):
    ticks_at_card = None
    ticks = 0
    moments = []
    for step in steps:
        if step[0] == "card" and ticks_at_card is None:
            ticks_at_card = ticks
        moments.append((ticks, step))
        ticks = ticks + ticks_held_by(step)
    return [(CARD_APPEARS_AT_CLIP_SECONDS + (at_ticks - ticks_at_card) / 60.0, step) for at_ticks, step in moments], ticks


class RealSetDescriptionTests(unittest.TestCase):
    def opening_steps(self):
        return convert_assets.load_set_description("chapter1_opening")["steps"]

    def seconds_of_first(self, wanted, steps=None, skip=0):
        moments, _ = clip_seconds_at_each_step(self.opening_steps() if steps is None else steps)
        matches = [seconds for seconds, step in moments if step[:len(wanted)] == wanted]
        return matches[skip]

    def test_the_opening_has_no_invented_camera_pans_and_no_flat_holds(self):
        steps = self.opening_steps()
        self.assertEqual([step for step in steps if step[0] == "camera_pan"], [])
        self.assertEqual([step for step in steps if step[0] == "message_hold"], [])

    def test_the_opening_lasts_as_long_as_the_measured_stretch_of_the_clip(self):
        moments, total_ticks = clip_seconds_at_each_step(self.opening_steps())
        self.assertAlmostEqual(moments[0][0] + total_ticks / 60.0, 92.067, delta=ONE_FRAME_AND_A_BIT)

    def test_beam_steps_come_one_frame_before_the_clip_first_changes(self):
        self.assertAlmostEqual(self.seconds_of_first(["beam_animate", "spot_linne", 56]), 23.817, delta=LESS_THAN_A_FRAME)
        self.assertAlmostEqual(self.seconds_of_first(["beam_animate", "spot_killer", 200]), 32.167, delta=LESS_THAN_A_FRAME)
        self.assertAlmostEqual(self.seconds_of_first(["beam_animate", "spot_linne", 118]), 50.667, delta=LESS_THAN_A_FRAME)
        self.assertAlmostEqual(self.seconds_of_first(["beam_animate", "spot_killer", 138]), 50.667, delta=LESS_THAN_A_FRAME)

    def test_both_beams_of_the_close_up_close_together_and_are_gone_when_the_clip_goes_black(self):
        closing_start = self.seconds_of_first(["beam_animate", "spot_linne2"])
        self.assertAlmostEqual(closing_start, 67.717, delta=LESS_THAN_A_FRAME)
        self.assertEqual(closing_start, self.seconds_of_first(["beam_animate", "spot_killer2"]))
        closing_ticks = [step[-1] for step in self.opening_steps() if step[:2] == ["beam_animate", "spot_linne2"]][0]
        self.assertAlmostEqual(closing_start + closing_ticks / 60.0, 67.85, delta=LESS_THAN_A_FRAME)

    def test_moving_the_beam_steps_leaves_the_room_change_where_it_was(self):
        self.assertAlmostEqual(self.seconds_of_first(["beam_delete", "spot_linne2"]), 67.883, delta=LESS_THAN_A_FRAME)
        self.assertAlmostEqual(self.seconds_of_first(["room", 7]), 67.883, delta=LESS_THAN_A_FRAME)

    def test_figures_start_moving_at_the_measured_clip_times(self):
        self.assertAlmostEqual(self.seconds_of_first(["object", "killer", "st01_killera_011"]), 40.567, delta=ONE_FRAME_AND_A_BIT)
        self.assertAlmostEqual(self.seconds_of_first(["object", "linne", "st01_linne_005"]), 41.633, delta=ONE_FRAME_AND_A_BIT)

    def test_rooms_iris_cuts_and_flashes_come_at_the_measured_clip_times(self):
        self.assertAlmostEqual(self.seconds_of_first(["iris"]), 68.867, delta=ONE_FRAME_AND_A_BIT)
        self.assertAlmostEqual(self.seconds_of_first(["mask_off"]), 70.883, delta=ONE_FRAME_AND_A_BIT)
        self.assertAlmostEqual(self.seconds_of_first(["mask_on"], skip=1), 81.283, delta=ONE_FRAME_AND_A_BIT)
        self.assertAlmostEqual(self.seconds_of_first(["flash", 0.5]), 81.333, delta=ONE_FRAME_AND_A_BIT)
        self.assertAlmostEqual(self.seconds_of_first(["flash", 1]), 81.483, delta=ONE_FRAME_AND_A_BIT)
        self.assertAlmostEqual(self.seconds_of_first(["flash", 0.5], skip=1), 86.4, delta=ONE_FRAME_AND_A_BIT)
        self.assertAlmostEqual(self.seconds_of_first(["flash", 1], skip=1), 86.55, delta=ONE_FRAME_AND_A_BIT)

    def test_every_dim_starts_at_its_measured_clip_time(self):
        moments, _ = clip_seconds_at_each_step(self.opening_steps())
        dims = [seconds for seconds, step in moments if step[0] == "dim_to"]
        measured = [24.833, 30.967, 33.183, 39.65, 42.883, 50.3, 52.517, 57.067, 59.117, 67.15, 73.133, 81.167]
        self.assertEqual(len(dims), len(measured))
        for seconds, expected in zip(dims, measured):
            self.assertAlmostEqual(seconds, expected, delta=ONE_FRAME_AND_A_BIT)

    def test_each_beam_is_created_before_it_is_used_and_at_most_a_few_exist_at_once(self):
        alive = set()
        for step in self.opening_steps():
            if step[0] == "beam_create":
                alive.add(step[1])
            elif step[0] == "beam_animate":
                self.assertIn(step[1], alive)
            elif step[0] == "beam_delete":
                alive.remove(step[1])
            self.assertLessEqual(len(alive), 4)
        self.assertEqual(alive, set())

    def test_the_room_is_never_shown_lit_before_the_iris_opens_it(self):
        steps = self.opening_steps()
        kinds = [step[0] for step in steps]
        self.assertLess(kinds.index("mask_on"), kinds.index("fade_in", kinds.index("room")))
        self.assertLess(kinds.index("iris"), kinds.index("mask_off"))
        self.assertEqual(kinds.count("mask_off"), 1)

    def test_chapter1_opening_only_names_assets_it_declares(self):
        set_description = convert_assets.load_set_description("chapter1_opening")
        plate_names = [plate["name"] for plate in set_description["plates"]]
        screen_names = [texture["name"] for texture in set_description["screen_textures"]]
        animation_names = set(animation["name"] for animation in set_description["animations"])
        music_names = set(entry["name"] for entry in set_description["music"])
        sound_names = set(entry["name"] for entry in set_description["sounds"])
        steps = convert_assets.expand_steps(set_description, {name: (1.0, 1.0) for name in plate_names})
        convert_assets.check_steps_refer_to_known_assets(steps, set(plate_names + screen_names), animation_names, music_names, sound_names)
        self.assertLessEqual(len(steps), asset_writers.SCENE_STEP_MAX)

    def test_chapter1_opening_reads_its_audio_from_the_streaming_folder(self):
        set_description = convert_assets.load_set_description("chapter1_opening")
        for entry in set_description["music"] + set_description["sounds"]:
            self.assertTrue(entry["source"].startswith("natives/android/streaming/sound/"), entry["source"])
            self.assertTrue(entry["source"].endswith(entry["name"] + ".asrc.31"), entry["source"])


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
