import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools", "assets"))

import message_file
import talk_assets


class TalkPageEffectTests(unittest.TestCase):
    def test_a_flash_tag_marks_its_section(self):
        section = talk_assets.parse_page("<TRCK mesflash />word")[0]
        self.assertEqual((section["flashes"], section["quake_level"], section["quake_frames"]), (1, 0, 0))

    def test_a_plain_page_has_no_effects(self):
        section = talk_assets.parse_page("word")[0]
        self.assertEqual((section["flashes"], section["quake_level"], section["quake_frames"]), (0, 0, 0))

    def test_a_bare_quake_tag_takes_the_handheld_s_level_and_frames(self):
        section = talk_assets.parse_page("<TRCK mesquake />word")[0]
        self.assertEqual((section["quake_level"], section["quake_frames"]), (1, 40))

    def test_a_named_quake_takes_its_level_and_frames(self):
        section = talk_assets.parse_page('<TRCK mesquake type="iQuake::LEVEL_L" frame="30"/>word')[0]
        self.assertEqual((section["quake_level"], section["quake_frames"]), (2, 30))
        section = talk_assets.parse_page('<TRCK mesquake type="iQuake::LEVEL_M" frame="40" />word')[0]
        self.assertEqual((section["quake_level"], section["quake_frames"]), (1, 40))

    def test_a_quake_level_the_tool_does_not_know_stops_the_page(self):
        with self.assertRaises(message_file.MessageFileError):
            talk_assets.parse_page('<TRCK mesquake type="iQuake::LEVEL_X" frame="30"/>word')

    def test_an_effect_after_an_automatic_page_break_marks_the_later_section(self):
        sections = talk_assets.parse_page("one<TRCK auto_pf /><TRCK mesflash />two")
        self.assertEqual([section["flashes"] for section in sections], [0, 1])


if __name__ == "__main__":
    unittest.main()
