import json
import os
import unittest

import clock_scenes
import pause_clock

GAME_JSON = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))), "tools", "assets", "game.json")


class PauseClockTests(unittest.TestCase):
    def test_times_after_midnight_follow_the_evening(self):
        self.assertEqual(pause_clock.minutes_of("7:02 PM", 12), 19 * 60 + 2)
        self.assertEqual(pause_clock.minutes_of("12:10 AM", 12), 24 * 60 + 10)
        self.assertEqual(pause_clock.minutes_of("5:10 AM", 12), 29 * 60 + 10)
        with self.assertRaises(ValueError):
            pause_clock.minutes_of("7 PM", 12)

    def test_each_scene_steps_from_its_chapter_card_toward_the_next(self):
        clock = {"evening_starts_hour": 12, "chapter_times": {"1": "7:02 PM", "2": "7:31 PM"}, "night_ends": "6:00 AM", "scenes": {"1": ["a", "b", "c"], "2": ["d", "e"], "3": ["f"]}}
        lines = pause_clock.scene_minutes(clock)
        self.assertEqual(lines[:3], ["scene 1 a 1142", "scene 1 b 1151", "scene 1 c 1161"])
        self.assertEqual(lines[3:], ["scene 2 d 1171", "scene 2 e 1485"])

    def test_the_scene_lists_count_chapters_by_the_chapter_script(self):
        marks = ["mark script_begun st01-st01_root", "mark script_begun st01-a_Expand", "mark script_begun st01-a_Expand", "mark script_begun chapter", "mark script_begun phone", "mark script_begun st02-b_Expand", "mark core x"]
        self.assertEqual(clock_scenes.scenes_by_chapter(marks), {"1": ["st01-a_Expand"], "2": ["st02-b_Expand"]})

    def test_the_game_clock_has_a_card_time_for_every_chapter_with_one(self):
        with open(GAME_JSON, encoding="utf-8") as source:
            clock = json.load(source)["pause"]["clock"]
        times = [pause_clock.minutes_of(clock["chapter_times"][str(chapter)], clock["evening_starts_hour"]) for chapter in range(1, 18)]
        self.assertEqual(times, sorted(times))
        self.assertGreater(pause_clock.minutes_of(clock["night_ends"], clock["evening_starts_hour"]), times[-1])
        self.assertEqual(sorted(int(chapter) for chapter in clock["scenes"]), list(range(1, 19)))
        lines = pause_clock.scene_minutes(clock)
        self.assertEqual(lines[0], "scene 1 st01-st01_game000_Expand 1142")


if __name__ == "__main__":
    unittest.main()
