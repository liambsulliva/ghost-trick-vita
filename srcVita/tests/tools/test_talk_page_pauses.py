import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools", "assets"))

import talk_assets


def pauses_and_ending(raw_text):
    return [(section["pauses_before"], section["ending"]) for section in talk_assets.parse_page(raw_text)]


class TalkPagePauseTests(unittest.TestCase):
    def test_a_pause_after_the_text_ends_the_page(self):
        self.assertEqual(pauses_and_ending("word<TRCK mes_pause />"), [(0, talk_assets.ENDING_PAUSE_THEN_NEXT)])

    def test_a_pause_before_the_text_waits_before_it_and_the_page_ends_on_a_press(self):
        self.assertEqual(pauses_and_ending('<TRCK meswin disp="off" /><TRCK mes_pause /><TRCK meswin disp="on" />word'), [(1, talk_assets.ENDING_PRESS)])

    def test_two_pauses_before_the_text_are_both_kept(self):
        self.assertEqual(pauses_and_ending('<TRCK mes_pause /><TRCK cutin_fadein id="x" frame="1" /><TRCK mes_pause /><TRCK mesflash />word'), [(2, talk_assets.ENDING_PRESS)])

    def test_a_pause_on_each_side_of_the_text_keeps_both(self):
        self.assertEqual(pauses_and_ending("<TRCK mes_pause />word<TRCK mes_pause />"), [(1, talk_assets.ENDING_PAUSE_THEN_NEXT)])

    def test_a_page_with_a_pause_and_no_text_ends_on_it(self):
        self.assertEqual(pauses_and_ending('<TRCK meswin disp="off" /><TRCK mes_pause /><TRCK auto_end/>'), [(0, talk_assets.ENDING_PAUSE_THEN_CLOSE)])

    def test_a_pause_before_an_icon_counts_as_before_the_text(self):
        self.assertEqual(pauses_and_ending("<TRCK mes_pause /><ICON GHOST>"), [(1, talk_assets.ENDING_PRESS)])

    def test_each_section_of_an_automatic_page_break_counts_its_own_pauses(self):
        self.assertEqual(pauses_and_ending('one<TRCK mes_pause /><TRCK auto_pf time="0"/><TRCK mes_pause />two'), [(0, talk_assets.ENDING_PAUSE_THEN_NEXT), (1, talk_assets.ENDING_PRESS)])


if __name__ == "__main__":
    unittest.main()
