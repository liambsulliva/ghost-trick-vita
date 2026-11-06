import contextlib
import io
import os
import sys
import tempfile
import unittest
from unittest import mock

import numpy

import longplay_check
import vita_loop

HEIGHT = longplay_check.COMPARED_HEIGHT
WIDTH = longplay_check.COMPARED_WIDTH
OBJECT_POSE = ["hero", "hero_anim", 100, 50]
FIRST_FRAME = [0.0, 0.0, 40.0, 40.0]
SPRITE_TOP = 180
SPRITE_LEFT = 440
SPRITE_SIDE = 30


def noise_image(seed):
    return numpy.random.RandomState(seed).randint(30, 220, size=(HEIGHT, WIDTH, 3)).astype(numpy.float32)


def with_sprite(background, sprite_source):
    image = background.copy()
    image[SPRITE_TOP:SPRITE_TOP + SPRITE_SIDE, SPRITE_LEFT:SPRITE_LEFT + SPRITE_SIDE] = sprite_source[SPRITE_TOP:SPRITE_TOP + SPRITE_SIDE, SPRITE_LEFT:SPRITE_LEFT + SPRITE_SIDE]
    return image


def only_sprite_capture():
    return with_sprite(numpy.zeros((HEIGHT, WIDTH, 3), dtype=numpy.float32), noise_image(2))


def view_check():
    return {"name": "view", "camera": (0, 0), "time": "00:00:01.000", "measure": ["hero"], "max_error": 6.0, "expected_gain": 1.0, "fills_width": True, "placed_objects": [OBJECT_POSE]}


def screen_with_edges(brightness):
    screen = numpy.full((longplay_check.SCREEN_HEIGHT, longplay_check.SCREEN_WIDTH, 3), 128.0, dtype=numpy.float32)
    screen[:, :longplay_check.SCREEN_EDGE_COLUMNS] = brightness
    screen[:, -longplay_check.SCREEN_EDGE_COLUMNS:] = brightness
    return screen


class ScoringTestCase(unittest.TestCase):
    def setUp(self):
        self.captures = {}
        self.whole_screen = screen_with_edges(128.0)
        self.output = io.StringIO()
        for target, name, keywords in (
            (longplay_check, "read_capture", {"side_effect": lambda name: self.captures[name]}),
            (longplay_check, "read_whole_screen", {"side_effect": lambda name: self.whole_screen}),
            (longplay_check, "write_pair_image", {}),
            (longplay_check, "first_drawn_frame", {"return_value": FIRST_FRAME}),
        ):
            patcher = mock.patch.object(target, name, **keywords)
            self.addCleanup(patcher.stop)
            patcher.start()
        redirect = contextlib.redirect_stdout(self.output)
        redirect.__enter__()
        self.addCleanup(redirect.__exit__, None, None, None)


class ViewCheckTests(ScoringTestCase):
    def score(self, capture, reference, only_capture=None):
        self.captures["view"] = capture
        self.captures["view_only_hero"] = only_sprite_capture() if only_capture is None else only_capture
        with mock.patch.object(longplay_check, "read_clip_frames", return_value=[reference]):
            return longplay_check.score_view_check(view_check())

    def test_a_matching_capture_passes(self):
        reference = with_sprite(noise_image(1), noise_image(2))
        self.assertTrue(self.score(reference.copy(), reference))
        self.assertIn("brightness gain 1.00", self.output.getvalue())

    def test_a_sprite_missing_from_the_capture_fails_though_the_room_matches(self):
        background = noise_image(1)
        reference = with_sprite(background, noise_image(2))
        self.assertFalse(self.score(background.copy(), reference))

    def test_an_object_with_no_sprite_pixels_cannot_be_measured(self):
        reference = noise_image(1)
        self.assertFalse(self.score(reference.copy(), reference, numpy.zeros((HEIGHT, WIDTH, 3), dtype=numpy.float32)))
        self.assertIn("could not be measured", self.output.getvalue())

    def test_a_sprite_that_lines_up_but_looks_wrong_fails(self):
        reference = with_sprite(noise_image(1), noise_image(2))
        disturbance = numpy.random.RandomState(3).uniform(-50, 50, size=(HEIGHT, WIDTH, 3)).astype(numpy.float32)
        capture = with_sprite(reference, reference + disturbance)
        self.assertFalse(self.score(capture, reference))
        self.assertIn("shift +0.00 +0.00", self.output.getvalue())

    def test_a_badly_dimmed_playfield_fails(self):
        reference = with_sprite(noise_image(1), noise_image(2))
        capture = reference.copy()
        capture[:, longplay_check.PLAYFIELD_LEFT:longplay_check.PLAYFIELD_RIGHT] = reference[:, longplay_check.PLAYFIELD_LEFT:longplay_check.PLAYFIELD_RIGHT] * 0.5
        self.assertFalse(self.score(capture, reference))
        self.assertIn("brightness gain 2.00", self.output.getvalue())

    def test_black_edges_fail_when_the_room_should_fill_the_width(self):
        reference = with_sprite(noise_image(1), noise_image(2))
        self.whole_screen = screen_with_edges(0.0)
        self.assertFalse(self.score(reference.copy(), reference))
        self.assertIn("screen edges", self.output.getvalue())

    def test_art_at_the_edges_fails_when_the_shot_should_have_black_beside_it(self):
        reference = with_sprite(noise_image(1), noise_image(2))
        self.captures["view"] = reference.copy()
        self.captures["view_only_hero"] = only_sprite_capture()
        check = view_check()
        check["fills_width"] = False
        with mock.patch.object(longplay_check, "read_clip_frames", return_value=[reference]):
            self.assertFalse(longplay_check.score_view_check(check))


class CameraWindowTests(unittest.TestCase):
    WIDE_PLATE = "plate upper 0 0 1024 384\n"

    def test_a_card_with_no_room_sits_in_the_middle_of_the_screen(self):
        self.assertEqual(longplay_check.camera_window_on_screen("scene s\ncard c\n"), (120, 2))

    def test_the_left_end_of_a_room_starts_at_the_screen_edge(self):
        self.assertEqual(longplay_check.camera_window_on_screen(self.WIDE_PLATE + "camera 0 192\n"), (0, 4))

    def test_the_middle_of_a_room_keeps_the_camera_centred(self):
        self.assertEqual(longplay_check.camera_window_on_screen(self.WIDE_PLATE + "camera 344 96\n"), (120, 2))

    def test_the_right_end_of_a_room_stops_at_the_screen_edge(self):
        self.assertEqual(longplay_check.camera_window_on_screen(self.WIDE_PLATE + "camera 768 0\n"), (240, 0))

    def test_a_narrow_plate_is_centred_and_the_plate_above_it_is_ignored(self):
        self.assertEqual(longplay_check.camera_window_on_screen(self.WIDE_PLATE + "plate close 0 384 256 192\ncamera 0 384\n"), (120, 2))

    def test_two_plates_the_camera_straddles_count_as_one_piece_of_art(self):
        self.assertEqual(longplay_check.camera_window_on_screen(self.WIDE_PLATE + "plate lower 0 384 1024 384\ncamera 0 224\n"), (0, 2))


class MotionCheckTests(ScoringTestCase):
    def motion_check(self, name):
        return {"name": name, "object": "hero", "camera": (0, 0), "placed_objects": [OBJECT_POSE]}

    def score(self, first_capture, second_capture, frames):
        self.captures.update({"first": first_capture, "second": second_capture, "first_only_hero": only_sprite_capture(), "second_only_hero": only_sprite_capture()})
        with mock.patch.object(longplay_check, "read_clip_frames", return_value=frames):
            return longplay_check.score_motion_checks([self.motion_check("first"), self.motion_check("second")])

    def test_frames_that_move_forward_through_the_clip_pass(self):
        frames = [noise_image(1), noise_image(2), noise_image(3)]
        self.assertTrue(self.score(frames[0], frames[1], frames))

    def test_a_pose_stuck_on_one_clip_frame_fails(self):
        frames = [noise_image(1), noise_image(2), noise_image(3)]
        self.assertFalse(self.score(frames[1], frames[1], frames))
        self.assertIn("out of order", self.output.getvalue())

    def test_a_motion_check_with_no_sprite_pixels_cannot_be_measured(self):
        self.captures.update({"first": noise_image(1), "first_only_hero": numpy.zeros((HEIGHT, WIDTH, 3), dtype=numpy.float32)})
        with mock.patch.object(longplay_check, "read_clip_frames", return_value=[noise_image(1)]):
            self.assertFalse(longplay_check.score_motion_checks([self.motion_check("first")]))
        self.assertIn("could not be measured", self.output.getvalue())


class OnlyObjectSceneTests(unittest.TestCase):
    def test_the_scene_holds_only_the_object(self):
        set_description = {"objects": {"hero": {"layer": 5, "flip": 1}}}
        check = {"name": "view", "room": 7, "camera": (16, 32), "placed_objects": [["other", "other_anim", 1, 2], OBJECT_POSE]}
        self.assertEqual(longplay_check.only_object_scene_text(set_description, check, "hero"), "\n".join([
            "scene view_only_hero",
            "room 7",
            "camera 16 32",
            "object hero hero_anim 100 50 5 1",
            "wait %d" % longplay_check.HOLD_FOREVER_TICKS,
        ]) + "\n")

    def test_view_checks_and_motion_checks_measure_different_keys(self):
        self.assertEqual(longplay_check.measured_objects({"measure": ["a", "b"]}), ["a", "b"])
        self.assertEqual(longplay_check.measured_objects({"object": "c"}), ["c"])


class CaptureFlowTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.TemporaryDirectory()
        self.addCleanup(self.folder.cleanup)
        self.output = io.StringIO()
        with open(os.path.join(self.folder.name, "eboot.bin"), "wb") as output:
            output.write(b"eboot")
        with open(os.path.join(self.folder.name, "manifest.txt"), "wb") as output:
            output.write(b"manifest")
        self.remote_files = {vita_loop.APP_FOLDER + "/eboot.bin": b"eboot", vita_loop.ASSETS_FOLDER + "/manifest.txt": b"manifest"}
        self.quit_and_wait = self.patch(vita_loop, "quit_and_wait")
        self.launch = self.patch(vita_loop, "launch", return_value=0)
        self.patch(vita_loop, "wait_for_text", return_value="a line")
        self.patch(vita_loop, "write_app_commands")
        self.request_capture = self.patch(vita_loop, "request_capture", return_value=0)
        self.patch(vita_loop, "download_or_none", side_effect=lambda path: self.remote_files.get(path))
        self.latest_run_lines = self.patch(vita_loop, "latest_run_lines", return_value=["[ 1.000] scene ready name=x preload_us=1 texture_bytes=1 missing=0"])
        self.patch(vita_loop, "BUILD_FOLDER", self.folder.name)
        self.patch(longplay_check, "SET_FOLDER", self.folder.name)
        self.patch(longplay_check.time, "sleep")
        redirect = contextlib.redirect_stdout(self.output)
        redirect.__enter__()
        self.addCleanup(redirect.__exit__, None, None, None)

    def patch(self, target, name, *arguments, **keywords):
        patcher = mock.patch.object(target, name, *arguments, **keywords)
        self.addCleanup(patcher.stop)
        return patcher.start()

    def test_a_clean_run_captures_every_scene_and_quits_at_the_end(self):
        self.assertTrue(longplay_check.capture_on_device(["one", "two"]))
        self.assertEqual(self.request_capture.call_count, 2)
        self.assertEqual(self.quit_and_wait.call_count, 2)

    def test_an_old_eboot_on_the_device_stops_before_launch(self):
        self.remote_files[vita_loop.APP_FOLDER + "/eboot.bin"] = b"old eboot"
        self.assertFalse(longplay_check.capture_on_device(["one"]))
        self.launch.assert_not_called()
        self.assertIn("upload-eboot", self.output.getvalue())

    def test_a_manifest_missing_from_the_device_stops_before_launch(self):
        del self.remote_files[vita_loop.ASSETS_FOLDER + "/manifest.txt"]
        self.assertFalse(longplay_check.capture_on_device(["one"]))
        self.launch.assert_not_called()
        self.assertIn("upload-assets", self.output.getvalue())

    def test_a_load_failure_in_the_log_fails_the_run_and_is_printed(self):
        for marker in longplay_check.LOAD_FAILURE_MARKERS:
            self.latest_run_lines.return_value = ["[ 1.000] " + marker + " name=x"]
            self.assertFalse(longplay_check.capture_on_device(["one"]))
            self.assertIn(marker, self.output.getvalue())

    def test_the_app_is_quit_when_a_capture_raises(self):
        self.request_capture.side_effect = RuntimeError
        with self.assertRaises(RuntimeError):
            longplay_check.capture_on_device(["one"])
        self.assertEqual(self.quit_and_wait.call_count, 2)

    def test_check_files_are_deleted_when_the_run_raises(self):
        self.patch(longplay_check, "OUTPUT_FOLDER", self.folder.name)
        self.patch(longplay_check, "write_check_files", return_value=([], [], []))
        self.patch(longplay_check, "upload_check_files")
        delete_check_files = self.patch(longplay_check, "delete_check_files_from_device")
        self.patch(longplay_check, "capture_on_device", side_effect=RuntimeError)
        with mock.patch.object(sys, "argv", ["longplay_check.py"]):
            with self.assertRaises(RuntimeError):
                longplay_check.main()
        delete_check_files.assert_called_once()


if __name__ == "__main__":
    unittest.main()
