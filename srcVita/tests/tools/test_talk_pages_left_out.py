import os
import sys
import unittest
from unittest import mock

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools", "assets"))

import message_file
import talk_assets

FILE = "text/st01_0000.msg"
PAGES = {"m01_0460": ["one", "two", "three", "four"]}


def section_leaving_out(names):
    return {
        "entries": {"m01_0460": [(FILE, "st01_0000_m01_0460_%02d" % index) for index in range(4)]},
        "language_files": {},
        "pages_left_out": names,
    }


def page_texts(parsed):
    return ["".join(character for line in sections[0]["lines"] for character, _ in line) for sections, _, _, _, _ in parsed["m01_0460"]]


class TalkPagesLeftOutTests(unittest.TestCase):
    def parse(self, section):
        with mock.patch.object(talk_assets, "read_all_pages", return_value=PAGES):
            return talk_assets.parse_all_pages(None, section, None)[0]

    def test_a_page_left_out_by_entry_name_is_not_baked(self):
        self.assertEqual(page_texts(self.parse(section_leaving_out(["st01_0000_m01_0460_01", "st01_0000_m01_0460_02"]))), ["one", "four"])

    def test_the_pages_after_one_left_out_keep_their_own_ds_layout(self):
        seen = []

        def layout(section, layouts, label, page_index):
            seen.append(page_index)
            return None, 0, talk_assets.BOX_BOTTOM, 0

        with mock.patch.object(talk_assets, "layout_of_page", side_effect=layout):
            self.parse(section_leaving_out(["st01_0000_m01_0460_01"]))
        self.assertEqual(seen, [0, 2, 3])

    def test_a_name_that_is_no_talk_page_stops_the_bake(self):
        with self.assertRaises(message_file.MissingEntryError):
            self.parse(section_leaving_out(["st01_0000_m01_0460_09"]))

    def test_the_game_leaves_out_ray_s_line_that_tricks_need_the_living_world(self):
        import json
        with open(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools", "assets", "game.json"), encoding="utf-8") as source:
            talk = json.load(source)["talk"]
        self.assertEqual(talk["pages_left_out"], ["st01_0000_m01_0460_05", "st01_0000_m01_0460_06", "st01_0000_m01_0460_07"])


if __name__ == "__main__":
    unittest.main()
