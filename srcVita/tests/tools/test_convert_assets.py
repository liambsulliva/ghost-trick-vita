import contextlib
import io
import os
import shutil
import signal
import sys
import tempfile
import time
import unittest
from unittest import mock

import asset_writers
import convert_assets
import narration
import synthetic_files

DEFAULT_DS_ROM_PATH = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))), "extracted", "ds_rom", "game.nds")
SCREEN_PIXELS_PER_DS_PIXEL = 2.8125
VIEW_TOP_WITH_THE_CAMERA_AT_192 = 384 - 544 / SCREEN_PIXELS_PER_DS_PIXEL
SCREEN_LEFT_OF_THE_CENTRED_WINDOW = 120
SCREEN_TOP_OF_THE_CENTRED_WINDOW = 2
BURST_LEFT_ON_SCREEN = 237
BURST_TOP_ON_SCREEN = 98
MARK_LEFT_ON_SCREEN = 264
MARK_TOP_ON_SCREEN = 104
AN_HOUR_IN_SECONDS = 3600
JOB_COUNT = 20
SLOW_JOB_SECONDS = 0.2
TEXT_STEP_KINDS = ("narration_show", "narration_hide", "text_hide_all", "overlay", "overlay_hide")
MEASURED_TEXT_STEP_TICKS = [
    (613, ["narration_show", "m01_0000"]),
    (781, ["narration_show", "m01_0000"]),
    (948, ["narration_hide", "m01_0000"]),
    (1096, ["narration_show", "m01_0010"]),
    (1172, ["narration_show", "m01_0010"]),
    (1394, ["narration_hide", "m01_0010"]),
    (1597, ["narration_show", "m01_0020"]),
    (1672, ["narration_show", "m01_0020"]),
    (1914, ["narration_hide", "m01_0020"]),
    (2123, ["overlay", 1]),
    (2127, ["overlay", 2]),
    (2154, ["overlay_hide", 1]),
    (2154, ["overlay_hide", 2]),
    (2179, ["narration_show", "m01_0030"]),
    (2254, ["narration_show", "m01_0030"]),
    (2554, ["narration_hide", "m01_0030"]),
    (2757, ["narration_show", "m01_0040"]),
    (3019, ["text_hide_all"]),
    (3153, ["narration_show", "m01_0041"]),
    (3228, ["narration_show", "m01_0041"]),
    (3564, ["narration_hide", "m01_0041"]),
    (3908, ["overlay", 3]),
    (3914, ["overlay", 4]),
    (3995, ["narration_show", "m01_0060"]),
    (4080, ["narration_show", "m01_0060"]),
    (4162, ["narration_show", "m01_0060"]),
    (4465, ["text_hide_all"]),
    (4472, ["overlay_hide", 3]),
    (4472, ["overlay_hide", 4]),
    (4491, ["narration_show", "m01_0070"]),
    (4769, ["text_hide_all"]),
    (4797, ["narration_show", "m01_0071"]),
    (5037, ["narration_hide", "m01_0071"]),
]

CARD_APPEARS_AT_CLIP_SECONDS = 8.267
MEASURED_OPENING_TICKS = 5119
MEASURED_DISSOLVE_START_TICK = 3030
MEASURED_DISSOLVE_TICKS = 32
MEASURED_DISSOLVE_END_ZOOM = 2
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
        steps = [["mask_on"], ["beam_create", "b", -16, 64, 108, 542, 60], ["beam_animate", "b", 118, 367, 30, 60], ["beam_delete", "b"], ["iris", 144, 359, 384, 120], ["dim_to", 0.49, 10], ["flash", 0.5, 8], ["dissolve", 32, 2], ["mask_off"]]
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
            "dissolve 32 2",
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
        with self.assertRaises(ValueError):
            convert_assets.check_steps_refer_to_known_assets([["text_show", 1, "missing", 480, 0, 60]], set(), set(), set(), set())
        with self.assertRaises(ValueError):
            convert_assets.check_steps_refer_to_known_assets([["overlay", 1, "missing", 0, 0, 4, 4, 6]], set(), set(), set(), set())

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

    def test_the_close_up_dissolves_in_from_its_measured_tick_and_zoom(self):
        ticks = 0
        ticks_of = {}
        for step in self.opening_steps():
            ticks_of.setdefault(tuple(step[:2]), ticks)
            ticks = ticks + ticks_held_by(step)
        self.assertEqual(ticks, MEASURED_OPENING_TICKS)
        self.assertEqual(ticks_of[("dissolve", MEASURED_DISSOLVE_TICKS)], MEASURED_DISSOLVE_START_TICK)
        for changed_with_the_cut in (("object", "cicel"), ("object", "cbag"), ("beam_delete", "spot_linne"), ("beam_delete", "spot_killer"), ("beam_create", "spot_linne2"), ("beam_create", "spot_killer2")):
            self.assertEqual(ticks_of[changed_with_the_cut], MEASURED_DISSOLVE_START_TICK, changed_with_the_cut)
        dissolve = [step for step in self.opening_steps() if step[0] == "dissolve"][0]
        self.assertEqual(dissolve[2], MEASURED_DISSOLVE_END_ZOOM)

    def test_the_cut_comes_right_after_the_dissolve_so_the_snapshot_holds_the_old_shot(self):
        steps = self.opening_steps()
        at = steps.index(["dissolve", MEASURED_DISSOLVE_TICKS, MEASURED_DISSOLVE_END_ZOOM])
        self.assertEqual(steps[at + 1], ["camera", 0, 384])

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
        composed_names = [texture["name"] for texture in set_description["composed_textures"]]
        bubble_text_names = [texture["name"] for texture in set_description["bubble_text"]]
        animation_names = set(animation["name"] for animation in set_description["animations"])
        music_names = set(entry["name"] for entry in set_description["music"])
        sound_names = set(entry["name"] for entry in set_description["sounds"])
        fake_paragraphs = [[{"id": 1, "texture": "text_line", "top_y": 0, "natural_width": 1.0}], [{"id": 2, "texture": "text_line", "top_y": 0, "natural_width": 1.0}], [{"id": 3, "texture": "text_line", "top_y": 0, "natural_width": 1.0}]]
        narration_lines = {request["name"]: {"paragraphs": fake_paragraphs} for request in set_description["narration"]}
        texture_names = plate_names + screen_names + composed_names + bubble_text_names + ["text_line"]
        steps = convert_assets.expand_steps(set_description, {name: (1.0, 1.0) for name in plate_names})
        steps = narration.expand_text_steps(steps, narration_lines, {name: (4, 4) for name in texture_names})
        convert_assets.check_steps_refer_to_known_assets(steps, set(texture_names), animation_names, music_names, sound_names)
        self.assertLessEqual(len(steps), asset_writers.SCENE_STEP_MAX)

    def test_chapter1_opening_reads_its_audio_from_the_streaming_folder(self):
        set_description = convert_assets.load_set_description("chapter1_opening")
        for entry in set_description["music"] + set_description["sounds"]:
            self.assertTrue(entry["source"].startswith("natives/android/streaming/sound/"), entry["source"])
            self.assertTrue(entry["source"].endswith(entry["name"] + ".asrc.31"), entry["source"])

    def test_chapter1_opening_names_every_narration_message_of_the_opening_once(self):
        set_description = convert_assets.load_set_description("chapter1_opening")
        names = [request["name"] for request in set_description["narration"]]
        self.assertEqual(names, ["m01_0000", "m01_0010", "m01_0020", "m01_0030", "m01_0040", "m01_0041", "m01_0060", "m01_0070", "m01_0071"])
        for request in set_description["narration"]:
            self.assertTrue(request["entry"].endswith(request["name"]))

    def text_step_ticks(self):
        moments = []
        ticks = 0
        for step in self.opening_steps():
            if step[0] in TEXT_STEP_KINDS:
                moments.append((ticks, step[:2]))
            ticks = ticks + ticks_held_by(step)
        return moments

    def test_every_text_step_sits_on_its_measured_tick(self):
        self.assertEqual(self.text_step_ticks(), MEASURED_TEXT_STEP_TICKS)

    def test_the_two_one_frame_undims_cut_their_text_on_the_same_tick(self):
        ticks = 0
        cut_ticks = []
        hide_ticks = []
        for step in self.opening_steps():
            if step == ["dim_to", 1, 0]:
                cut_ticks.append(ticks)
            if step[0] == "text_hide_all":
                hide_ticks.append(ticks)
            ticks = ticks + ticks_held_by(step)
        self.assertEqual(cut_ticks, hide_ticks[:2])

    def test_the_oval_leaves_with_the_black_cut(self):
        moments, _ = clip_seconds_at_each_step(self.opening_steps())
        cut = [seconds for seconds, step in moments if step == ["mask_on"]][1]
        hides = [seconds for seconds, step in moments if step[0] == "overlay_hide" and step[1] in (3, 4)]
        self.assertEqual(hides, [cut, cut])

    def test_no_dim_operand_is_zero_because_it_means_black(self):
        for step in self.opening_steps():
            if step[0] == "dim_to":
                self.assertGreater(step[1], 0)


class BubblePlacementTests(unittest.TestCase):
    def test_the_oval_bubble_and_its_text_land_on_whole_screen_pixels(self):
        camera = None
        places = {}
        for step in convert_assets.load_set_description("chapter1_opening")["steps"]:
            if step[0] == "camera":
                camera = step[1:]
            elif step[0] == "overlay":
                places[step[2]] = (camera, step[3], step[4])
        wanted_screen_places = {"oval_bubble": (278, 147), "oval_bubble_text": (320, 170)}
        for name, (screen_x, screen_y) in wanted_screen_places.items():
            camera, x, y = places[name]
            self.assertEqual(camera, [0, 192])
            self.assertAlmostEqual(x * SCREEN_PIXELS_PER_DS_PIXEL, screen_x, delta=0.001)
            self.assertAlmostEqual((y - VIEW_TOP_WITH_THE_CAMERA_AT_192) * SCREEN_PIXELS_PER_DS_PIXEL, screen_y, delta=0.001)


class BurstPlacementTests(unittest.TestCase):
    def test_the_burst_sits_where_the_clip_has_it_in_the_centred_view_the_mask_uses(self):
        camera = None
        mask_is_on = False
        place = None
        for step in convert_assets.load_set_description("chapter1_opening")["steps"]:
            if step[0] == "camera":
                camera = step[1:]
            elif step[0] == "mask_on":
                mask_is_on = True
            elif step[:3] == ["overlay", 1, "alert_burst"]:
                place = (camera, mask_is_on, step[3], step[4])
        camera, mask_is_on, x, y = place
        self.assertEqual(camera, [0, 192])
        self.assertTrue(mask_is_on)
        self.assertAlmostEqual((x - camera[0]) * SCREEN_PIXELS_PER_DS_PIXEL + SCREEN_LEFT_OF_THE_CENTRED_WINDOW, BURST_LEFT_ON_SCREEN, delta=0.5)
        self.assertAlmostEqual((y - camera[1]) * SCREEN_PIXELS_PER_DS_PIXEL + SCREEN_TOP_OF_THE_CENTRED_WINDOW, BURST_TOP_ON_SCREEN, delta=0.5)


class MaskedBubblePlacementTests(unittest.TestCase):
    def test_the_burst_and_mark_land_on_whole_screen_pixels_in_the_masked_view(self):
        wanted_screen_places = {"alert_burst": (BURST_LEFT_ON_SCREEN, BURST_TOP_ON_SCREEN), "alert_mark": (MARK_LEFT_ON_SCREEN, MARK_TOP_ON_SCREEN)}
        steps = convert_assets.load_set_description("chapter1_opening")["steps"]
        places = {step[2]: (step[3], step[4]) for step in steps if step[0] == "overlay"}
        for name, (screen_x, screen_y) in wanted_screen_places.items():
            x, y = places[name]
            self.assertAlmostEqual(x * SCREEN_PIXELS_PER_DS_PIXEL + SCREEN_LEFT_OF_THE_CENTRED_WINDOW, screen_x, delta=0.001)
            self.assertAlmostEqual((y - 192) * SCREEN_PIXELS_PER_DS_PIXEL + SCREEN_TOP_OF_THE_CENTRED_WINDOW, screen_y, delta=0.001)


class TextureNameTests(unittest.TestCase):
    def test_names_that_differ_pass(self):
        convert_assets.check_texture_names_differ(["a", "b", "c"])

    def test_a_repeated_name_fails_and_is_named(self):
        with self.assertRaises(convert_assets.ConversionFailed) as caught:
            convert_assets.check_texture_names_differ(["a", "b", "a"])
        self.assertEqual(caught.exception.failures, [("a", "another texture has this name")])

    def test_names_that_differ_only_by_case_fail_because_the_files_would_clash(self):
        with self.assertRaises(convert_assets.ConversionFailed):
            convert_assets.check_texture_names_differ(["art", "ART"])
        with self.assertRaises(convert_assets.ConversionFailed):
            convert_assets.check_texture_names_differ(["ART", "art"])


class OldWorkFolderTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.folder = temporary.name
        patcher = mock.patch.object(tempfile, "tempdir", self.folder)
        patcher.start()
        self.addCleanup(patcher.stop)

    def make_folder(self, name, age_seconds):
        path = os.path.join(self.folder, name)
        os.makedirs(path)
        with open(os.path.join(path, "font_0.otf"), "wb") as output:
            output.write(b"font")
        stamp = time.time() - age_seconds
        os.utime(path, (stamp, stamp))
        return path

    def test_only_old_folders_of_the_converter_are_removed(self):
        old = self.make_folder(convert_assets.WORK_FOLDER_PREFIX + "old", AN_HOUR_IN_SECONDS + 60)
        new = self.make_folder(convert_assets.WORK_FOLDER_PREFIX + "new", AN_HOUR_IN_SECONDS - 60)
        other = self.make_folder("other_tool_old", AN_HOUR_IN_SECONDS + 60)
        convert_assets.remove_old_work_folders()
        self.assertFalse(os.path.exists(old))
        self.assertTrue(os.path.exists(new))
        self.assertTrue(os.path.exists(other))

    def test_a_file_with_the_same_prefix_is_left_alone(self):
        path = os.path.join(self.folder, convert_assets.WORK_FOLDER_PREFIX + "file")
        with open(path, "wb") as output:
            output.write(b"x")
        stamp = time.time() - AN_HOUR_IN_SECONDS - 60
        os.utime(path, (stamp, stamp))
        convert_assets.remove_old_work_folders()
        self.assertTrue(os.path.exists(path))

    def test_a_folder_that_vanishes_while_it_is_checked_is_not_old(self):
        self.assertFalse(convert_assets.work_folder_is_old(os.path.join(self.folder, "absent"), time.time()))
        with mock.patch.object(convert_assets.os.path, "getmtime", side_effect=OSError):
            path = self.make_folder(convert_assets.WORK_FOLDER_PREFIX + "racing", 0)
            self.assertFalse(convert_assets.work_folder_is_old(path, time.time()))


class WorkFolderDeletionTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.work_folder = os.path.join(temporary.name, "work")
        os.makedirs(self.work_folder)

    def test_a_stop_during_the_deletion_still_deletes_the_folder_and_still_stops(self):
        real_rmtree = shutil.rmtree
        calls = []

        def stop_on_the_first_call(path, ignore_errors=False):
            calls.append(path)
            if len(calls) == 1:
                raise SystemExit(143)
            real_rmtree(path, ignore_errors=ignore_errors)

        with mock.patch.object(convert_assets.shutil, "rmtree", side_effect=stop_on_the_first_call):
            with self.assertRaises(SystemExit):
                convert_assets.delete_work_folder(self.work_folder)
        self.assertEqual(len(calls), 2)
        self.assertFalse(os.path.exists(self.work_folder))

    def test_a_plain_deletion_deletes_the_folder_once(self):
        convert_assets.delete_work_folder(self.work_folder)
        self.assertFalse(os.path.exists(self.work_folder))


class StopSignalTests(unittest.TestCase):
    def setUp(self):
        for stop_signal in convert_assets.STOP_SIGNALS:
            self.addCleanup(signal.signal, stop_signal, signal.getsignal(stop_signal))
        self.addCleanup(setattr, convert_assets, "stop_signal_number", None)

    def test_the_first_stop_ignores_every_later_stop_and_exits_with_the_signal_code(self):
        with self.assertRaises(SystemExit) as caught:
            convert_assets.exit_so_the_work_folder_is_deleted(signal.SIGTERM, None)
        self.assertEqual(caught.exception.code, 128 + signal.SIGTERM)
        for stop_signal in (signal.SIGINT, signal.SIGTERM, signal.SIGHUP):
            self.assertEqual(signal.getsignal(stop_signal), signal.SIG_IGN)

    def run_main_with_a_conversion_that(self, conversion):
        arguments = ["convert_assets.py", "--set", "s", "--output", "o"]
        with mock.patch.object(sys, "argv", arguments):
            with mock.patch.object(convert_assets, "load_set_description", return_value={}):
                with mock.patch.object(convert_assets.subprocess, "run"):
                    with mock.patch.object(convert_assets, "convert_set_description", side_effect=conversion):
                        return convert_assets.main()

    def test_a_stop_python_swallowed_still_sets_the_exit_code(self):
        def stop_inside_a_destructor(*_arguments):
            try:
                convert_assets.exit_so_the_work_folder_is_deleted(signal.SIGTERM, None)
            except SystemExit:
                pass

        self.assertEqual(self.run_main_with_a_conversion_that(stop_inside_a_destructor), 128 + signal.SIGTERM)

    def test_a_run_nothing_stopped_exits_with_zero(self):
        self.assertEqual(self.run_main_with_a_conversion_that(lambda *_arguments: None), 0)

    def test_main_catches_all_three_stop_signals(self):
        self.run_main_with_a_conversion_that(lambda *_arguments: None)
        for stop_signal in (signal.SIGINT, signal.SIGTERM, signal.SIGHUP):
            self.assertEqual(signal.getsignal(stop_signal), convert_assets.exit_so_the_work_folder_is_deleted)


class TextureJobTests(unittest.TestCase):
    def run_jobs(self, job_body):
        jobs = {"job_%d" % index: {"name": "job_%d" % index} for index in range(JOB_COUNT)}
        with mock.patch.object(convert_assets.os, "cpu_count", return_value=1):
            with mock.patch.object(convert_assets, "convert_one_texture", side_effect=job_body):
                with mock.patch.object(convert_assets, "texture_code_fingerprint", return_value="code"):
                    return convert_assets.convert_all_textures(None, jobs, "work", None)

    def test_an_interrupted_run_cancels_the_jobs_still_waiting(self):
        started = []

        def interrupt_first_and_run_the_rest_slowly(packs, work_folder, job, record, code):
            started.append(job["name"])
            if job["name"] == "job_0":
                raise SystemExit(143)
            time.sleep(SLOW_JOB_SECONDS)

        with self.assertRaises(SystemExit):
            self.run_jobs(interrupt_first_and_run_the_rest_slowly)
        self.assertLess(len(started), JOB_COUNT)

    def test_a_failed_job_lets_the_others_finish_and_is_reported_by_name(self):
        started = []

        def fail_first(packs, work_folder, job, record, code):
            started.append(job["name"])
            if job["name"] == "job_0":
                raise ValueError("bad texture")
            return {"name": job["name"]}

        results, failures = self.run_jobs(fail_first)
        self.assertEqual(len(started), JOB_COUNT)
        self.assertEqual(len(results), JOB_COUNT - 1)
        self.assertEqual(failures, [("job_0", "bad texture")])


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


class TextKeyTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, self.folder)
        self.calls = []

    def run_main(self, set_description):
        errors = io.StringIO()
        with mock.patch.object(convert_assets.subprocess, "run", side_effect=lambda command, check: self.calls.append("make")):
            with mock.patch.object(convert_assets, "convert_set_description", side_effect=lambda *arguments: self.calls.append(arguments)):
                with mock.patch.object(convert_assets, "load_set_description", return_value=set_description):
                    with mock.patch.object(sys, "argv", ["convert_assets.py", "--set", "tiny", "--output", "out", "--apk-folder", self.folder]):
                        with contextlib.redirect_stderr(errors):
                            code = convert_assets.main()
        return code, errors.getvalue()

    def test_a_set_with_text_and_no_key_file_stops_before_any_other_work(self):
        for set_description in ({"narration": [{}]}, {"bubble_text": [{}]}):
            code, errors = self.run_main(set_description)
            self.assertEqual(code, 1)
            self.assertEqual(self.calls, [])
            self.assertIn("no text key at " + os.path.join(self.folder, "text_key.bin"), errors)
            self.assertIn("extract_text_key.py with your own copy of the Android package", errors)

    def test_a_key_file_of_the_wrong_size_stops_before_any_other_work(self):
        synthetic_files.write_text_key(self.folder, bytes(15))
        code, errors = self.run_main({"narration": [{}]})
        self.assertEqual(code, 1)
        self.assertEqual(self.calls, [])
        self.assertIn("is the wrong size", errors)

    def test_the_key_reaches_the_conversion_after_the_tool_is_built(self):
        synthetic_files.write_text_key(self.folder)
        set_description = {"narration": [{}]}
        code, errors = self.run_main(set_description)
        self.assertEqual(code, 0)
        self.assertEqual(errors, "")
        self.assertEqual(self.calls, ["make", (set_description, "out", self.folder, synthetic_files.FAKE_TEXT_KEY, DEFAULT_DS_ROM_PATH)])

    def test_a_set_without_text_runs_with_no_key_file(self):
        code, errors = self.run_main({})
        self.assertEqual(code, 0)
        self.assertEqual(errors, "")
        self.assertEqual(self.calls, ["make", ({}, "out", self.folder, None, DEFAULT_DS_ROM_PATH)])

    def test_a_key_file_that_cannot_be_read_stops_before_any_other_work(self):
        os.makedirs(os.path.join(self.folder, "text_key.bin"))
        code, errors = self.run_main({"narration": [{}]})
        self.assertEqual(code, 1)
        self.assertEqual(self.calls, [])
        self.assertIn("could not read " + os.path.join(self.folder, "text_key.bin"), errors)
        self.assertNotIn("Traceback", errors)

    def test_a_decoding_failure_during_the_conversion_is_a_plain_message(self):
        synthetic_files.write_text_key(self.folder)
        with mock.patch.object(convert_assets.subprocess, "run"):
            with mock.patch.object(convert_assets, "convert_set_description", side_effect=narration.NarrationError("the plain words")):
                with mock.patch.object(convert_assets, "load_set_description", return_value={"narration": [{}]}):
                    with mock.patch.object(sys, "argv", ["convert_assets.py", "--set", "tiny", "--output", "out", "--apk-folder", self.folder]):
                        errors = io.StringIO()
                        with contextlib.redirect_stderr(errors):
                            self.assertEqual(convert_assets.main(), 1)
        self.assertEqual(errors.getvalue(), "error: the plain words\n")


if __name__ == "__main__":
    unittest.main()
