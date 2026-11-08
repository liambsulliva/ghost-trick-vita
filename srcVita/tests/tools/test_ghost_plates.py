import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools", "assets"))
import chapter_assets
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


if __name__ == "__main__":
    unittest.main()
