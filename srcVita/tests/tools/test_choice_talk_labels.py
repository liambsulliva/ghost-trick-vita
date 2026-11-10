import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools", "assets"))
import chapter_assets


class ScriptWithStrings(chapter_assets.Script):
    def __init__(self, operations, strings):
        self.operations = operations
        self.strings = strings


class ChoicePanelTalkLabelsTest(unittest.TestCase):
    def test_a_talk_panel_and_a_turned_talk_panel_name_their_talks(self):
        strings = {10: "caption_one", 20: "talk_one", 30: "caption_two", 40: "talk_two"}
        operations = [
            (0, "op", 0x3c, [10, 0, 20, 0, 5, 0]),
            (14, "op", 0x3d, [2, 30, 0, 40, 0, 6, 0]),
        ]
        script = ScriptWithStrings(operations, strings)
        self.assertEqual(script.talk_labels_of_choice_panels(), ["talk_one", "talk_two"])

    def test_event_panels_name_no_talk(self):
        strings = {10: "caption_one", 50: "game", 60: "EVENT"}
        script = ScriptWithStrings([(0, "op", 0x40, [10, 0, 5, 0, 0, 50, 60, 70])], strings)
        self.assertEqual(script.talk_labels_of_choice_panels(), [])


class SoulBalloonTalkLabelsTest(unittest.TestCase):
    def test_the_soul_s_talking_balloon_names_a_talk_and_no_bubble(self):
        strings = {10: "talk_behind_the_balloon", 50: "game", 60: "EVENT", 70: "tapped"}
        operations = [
            (0, "op", 0x4c, [10, 0, 0xffff, 0, 5300, 0]),
            (14, "op", 0x4e, [0xffff, 0, 50, 60, 70, 5299, 0]),
        ]
        script = ScriptWithStrings(operations, strings)
        self.assertEqual(script.strings_of_ops((chapter_assets.SOUL_BALLOON_TALK_OP,)), ["talk_behind_the_balloon"])
        self.assertEqual(script.strings_of_ops(chapter_assets.BALLOON_LABEL_OPS), [])


if __name__ == "__main__":
    unittest.main()
