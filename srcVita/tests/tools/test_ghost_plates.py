import json
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools", "assets"))
import chapter_assets
import convert_assets
import script_assets


class ScriptWithRoomsAndStrings(chapter_assets.Script):
    def __init__(self, rooms, strings):
        self.operations = [(index * 4, "op", chapter_assets.ROOM_OP, [room]) for index, room in enumerate(rooms)]
        self.strings = dict(enumerate(strings))


class GhostPlatesTest(unittest.TestCase):
    def test_only_rooms_of_scripts_naming_a_ghost_get_its_plates(self):
        scripts = {
            "a": ScriptWithRoomsAndStrings([3, 4], ["GameApp", "Exception", "Missile:On"]),
            "b": ScriptWithRoomsAndStrings([5], ["GameApp", "Exception", "Boss:On"]),
            "c": ScriptWithRoomsAndStrings([6], ["GameApp", "Button", "Trick:On"]),
        }
        self.assertEqual(chapter_assets.rooms_of_scripts_naming(scripts, {"Missile:On"}), {3, 4})
        self.assertEqual(chapter_assets.rooms_of_scripts_naming(scripts, {"Boss:On"}), {5})

    def test_each_plate_line_names_the_ghost_variants_or_nothing(self):
        section = {"room_plates": {"3": [["bg_a", 0, 0]], "6": [["bg_b", 0, 0]]}}
        sizes = {"bg_a": (256, 192), "bg_b": (256, 192)}
        soul = {"bg_a": "bg_a-soul", "bg_b": "bg_b-soul"}
        ghosts = {"bg_a": {"missile": "bg_a-soul-missile", "boss": "bg_a-soul-boss"}, "bg_b": {}}
        lines = [line.split() for line in script_assets.rooms_text(section, sizes, soul, ghosts).splitlines() if line.startswith("plate ")]
        self.assertEqual(lines[0][7:], ["bg_a-soul", "bg_a-soul-missile", "bg_a-soul-boss"])
        self.assertEqual(lines[1][7:], ["bg_b-soul", "-", "-"])


class GhostTurnTest(unittest.TestCase):
    def test_only_soul_red_classes_turn_with_the_ghost(self):
        soul_view = {"owner": "a", "ring": "b", "classes": {"0000000a": [0.85, 0.12, 0.07, 1, 0], "0000000b": [0.7, 0.38, 0.03, 1, 1], "0000000c": [0.17, 0.41, 0.66, 1, 1]}}
        flags = {line.split()[1]: line.split()[-1] for line in script_assets.soul_text(soul_view).splitlines() if line.startswith("class ")}
        self.assertEqual(flags, {"0000000a": "1", "0000000b": "0", "0000000c": "0"})

    def test_every_red_class_of_the_game_turns_and_no_other(self):
        game_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools", "assets", "game.json")
        with open(game_path) as source:
            soul_view = json.load(source)["soul_view"]
        lines = [line.split() for line in script_assets.soul_text(soul_view).splitlines() if line.startswith("class ")]
        turning = [line[1] for line in lines if line[-1] == "1"]
        self.assertTrue(turning)
        for line in lines:
            red, green, blue = (float(value) for value in line[2:5])
            self.assertEqual(line[-1] == "1", red >= 2 * max(green, blue), line[1])

    def test_a_texture_with_ghost_variants_bakes_a_turned_copy_for_each_ghost(self):
        jobs = {}
        convert_assets.add_screen_texture_jobs(jobs, [{"name": "beam", "source": "s", "width": 8, "height": 4, "ghost_variants": True}, {"name": "plain", "source": "p", "width": 8, "height": 4}])
        self.assertEqual(sorted(jobs), ["beam", "beam-boss", "beam-missile", "plain"])
        self.assertNotIn("ghost_variants", jobs["beam"])
        self.assertNotIn("hue_turn", jobs["beam"])
        self.assertEqual(jobs["beam-missile"]["hue_turn"], 120)
        self.assertEqual(jobs["beam-boss"]["hue_turn"], 240)
        self.assertEqual(jobs["beam-boss"]["source"], "s")


if __name__ == "__main__":
    unittest.main()
