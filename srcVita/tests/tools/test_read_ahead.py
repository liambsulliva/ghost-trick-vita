import os
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools", "assets"))

import read_ahead


def write(folder, name, text):
    with open(os.path.join(folder, name), "w") as output:
        output.write(text)


class ReadAheadTest(unittest.TestCase):
    def test_first_use_of_each_page_is_keyed_to_the_latest_game_act(self):
        with tempfile.TemporaryDirectory() as folder:
            scripts = os.path.join(folder, "scripts")
            animations = os.path.join(folder, "animations")
            os.makedirs(scripts)
            os.makedirs(animations)
            write(scripts, "scene.motions", "motion 1a2b t_ball 1\nmotion 3c4d t_hand 1\n")
            write(scripts, "tiles.txt", "tile big_00 2840 2600 big_00_t00 0 0 2048 2048 0 0 2046 2046\ntile big_00 2840 2600 big_00_t10 2044 0 796 2048 2046 0 2840 2046\n")
            write(scripts, "effects.txt", "effect 90 movie sparkle 0 1 0 0 0\nfxmovie sparkle 1 0 0 -1 2\n")
            write(animations, "t_ball.anim", "animation t_ball\nframe ball_00 4 0 0 1 1 0 0 1 1\nframe ball_01 4 0 0 1 1 0 0 1 1\nframe - 4\n")
            write(animations, "t_hand.anim", "animation t_hand\nframe big_00 4 0 0 1 1 0 0 1 1\n")
            cues = os.path.join(folder, "cues.tsv")
            write(folder, "cues.tsv", "\n".join([
                "5\tmark script_begun scene",
                "5\tmark act_begun gameAct000",
                "6\tactor ball table=1a2b frame=0",
                "7\tactor ball table=1a2b frame=1",
                "50\tmark act_begun cicelAct010",
                "60\tmark act_begun gameAct010",
                "75\tactor hand table=3c4d frame=0",
                "80\teffect 90 sparkle 1.0 2.0",
            ]) + "\n")
            schedules = read_ahead.schedules_from_cues(cues, scripts, animations)
        self.assertEqual(schedules["scene"], [
            ("gameAct000", 1, "ball_00"),
            ("gameAct000", 5, "ball_01"),
            ("gameAct010", 15, "big_00_t00"),
            ("gameAct010", 15, "big_00_t10"),
            ("gameAct010", 20, "fx_sparkle_00"),
            ("gameAct010", 20, "fx_sparkle_01"),
        ])


    def test_each_page_of_a_long_animation_is_due_when_its_first_frame_is_shown(self):
        with tempfile.TemporaryDirectory() as folder:
            scripts = os.path.join(folder, "scripts")
            animations = os.path.join(folder, "animations")
            os.makedirs(scripts)
            os.makedirs(animations)
            write(scripts, "scene.motions", "motion 1a2b t_walk 1\n")
            write(scripts, "tiles.txt", "tile walk_01 2840 2600 walk_01_t00 0 0 2048 2048 0 0 2046 2046\ntile walk_01 2840 2600 walk_01_t10 2044 0 796 2048 2046 0 2840 2046\n")
            write(animations, "t_walk.anim", "animation t_walk\nloops 0\nframe walk_00 30 0 0 1 1 0 0 1 1\nframe - 10\nframe walk_00 20 0 0 1 1 0 0 1 1\nframe walk_01 40 0 0 1 1 0 0 1 1\nframe walk_02 5 0 0 1 1 0 0 1 1\nframe walk_01 5 0 0 1 1 0 0 1 1\n")
            cues = os.path.join(folder, "cues.tsv")
            write(folder, "cues.tsv", "\n".join([
                "100\tmark script_begun scene",
                "200\tactor walker table=1a2b frame=0",
                "201\tactor walker table=1a2b frame=0",
            ]) + "\n")
            schedules = read_ahead.schedules_from_cues(cues, scripts, animations)
        self.assertEqual(schedules["scene"], [
            ("*", 100, "walk_00"),
            ("*", 160, "walk_01_t00"),
            ("*", 160, "walk_01_t10"),
            ("*", 200, "walk_02"),
        ])


    def test_art_under_the_grayscale_filter_is_read_as_its_grayscale_copy_outside_ghost_view(self):
        with tempfile.TemporaryDirectory() as folder:
            scripts = os.path.join(folder, "scripts")
            animations = os.path.join(folder, "animations")
            os.makedirs(scripts)
            os.makedirs(animations)
            write(scripts, "scene.motions", "motion 1a2b t_ball 1\nmotion 3c4d t_hand 1\nmotion 5e6f t_cup 1\n")
            write(scripts, "tiles.txt", "tile big_00 2840 2600 big_00_t00 0 0 2048 2048 0 0 2046 2046\n")
            write(animations, "t_ball.anim", "animation t_ball\nframe ball_00 4 0 0 1 1 0 0 1 1\n")
            write(animations, "t_hand.anim", "animation t_hand\nframe big_00 4 0 0 1 1 0 0 1 1\n")
            write(animations, "t_cup.anim", "animation t_cup\nframe cup_00 4 0 0 1 1 0 0 1 1\n")
            cues = os.path.join(folder, "cues.tsv")
            write(folder, "cues.tsv", "\n".join([
                "5\tmark script_begun scene",
                "6\tscreen_filter 1",
                "7\tactor ball table=1a2b frame=0",
                "8\tactor hand table=3c4d frame=0",
                "9\tsoul_vision 1",
                "10\tactor cup table=5e6f frame=0",
            ]) + "\n")
            schedules = read_ahead.schedules_from_cues(cues, scripts, animations)
        self.assertEqual([texture for _, _, texture in schedules["scene"]], ["ball_00-mono", "big_00_t00-mono", "ball_00", "big_00_t00", "cup_00"])


    def test_a_grey_world_effect_reads_the_grayscale_copies_of_what_is_already_on_screen(self):
        with tempfile.TemporaryDirectory() as folder:
            scripts = os.path.join(folder, "scripts")
            animations = os.path.join(folder, "animations")
            os.makedirs(scripts)
            os.makedirs(animations)
            write(scripts, "scene.motions", "motion 1a2b t_ball 1\n")
            write(scripts, "effects.txt", "effect 53 grey_world - 0 1 0 0 0\n")
            write(animations, "t_ball.anim", "animation t_ball\nframe ball_00 4 0 0 1 1 0 0 1 1\n")
            cues = os.path.join(folder, "cues.tsv")
            write(folder, "cues.tsv", "\n".join([
                "5\tmark script_begun scene",
                "6\tactor ball table=1a2b frame=0",
                "40\teffect 53 efc_monochrome 0.0 0.0",
                "70\teffect_deleted efc_monochrome",
            ]) + "\n")
            schedules = read_ahead.schedules_from_cues(cues, scripts, animations)
        self.assertEqual([(offset, texture) for _, offset, texture in schedules["scene"]], [(1, "ball_00"), (35, "ball_00-mono")])


    def test_a_bundle_holds_each_motion_animation_once_in_order(self):
        with tempfile.TemporaryDirectory() as folder:
            scripts = os.path.join(folder, "scripts")
            animations = os.path.join(folder, "animations")
            os.makedirs(scripts)
            os.makedirs(animations)
            write(scripts, "scene.motions", "motion 1a2b t_ball 1\nmotion 3c4d t_hand 1\nmotion 5e6f t_ball 2\n")
            write(animations, "t_ball.anim", "animation t_ball\nloops 1\n")
            write(animations, "t_hand.anim", "animation t_hand\nloops 0")
            self.assertEqual(read_ahead.bundle_text(scripts, animations, "scene"), "animation t_ball\nloops 1\nanimation t_hand\nloops 0\n")


    def test_a_scenes_opening_art_is_also_read_during_the_scene_before(self):
        with tempfile.TemporaryDirectory() as folder:
            scripts = os.path.join(folder, "scripts")
            animations = os.path.join(folder, "animations")
            os.makedirs(scripts)
            os.makedirs(animations)
            write(scripts, "first.motions", "motion 1a2b t_ball 1\n")
            write(scripts, "second.motions", "motion 3c4d t_hand 1\n")
            write(animations, "t_ball.anim", "animation t_ball\nframe ball_00 4 0 0 1 1 0 0 1 1\n")
            write(animations, "t_hand.anim", "animation t_hand\nframe hand_00 4 0 0 1 1 0 0 1 1\nframe hand_01 4 0 0 1 1 0 0 1 1\n")
            cues = os.path.join(folder, "cues.tsv")
            write(folder, "cues.tsv", "\n".join([
                "0\tmark script_begun first",
                "100\tmark act_begun gameAct050",
                "110\tactor ball table=1a2b frame=0",
                "900\tmark script_begun second",
                "910\tactor hand table=3c4d frame=0",
                "2000\tactor hand table=3c4d frame=0",
            ]) + "\n")
            schedules = read_ahead.schedules_from_cues(cues, scripts, animations)
        self.assertIn(("gameAct050", 810, "hand_00"), schedules["first"])
        self.assertIn(("gameAct050", 814, "hand_01"), schedules["first"])
        self.assertIn(("*", 10, "hand_00"), schedules["second"])


if __name__ == "__main__":
    unittest.main()
