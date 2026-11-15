import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools"))

import chapter_starts_check


def note_levels_with(places):
    levels = bytearray(3 * 64)
    for place in places:
        levels[place] = 1
    return levels.hex()


class ChapterStartsCheckTests(unittest.TestCase):
    def test_the_story_scene_of_a_chapter_is_the_first_scene_after_chapter_xml(self):
        cues = [
            "0\tchapter_mark 3\n",
            "1\tmark script_begun chapter\n",
            "2\tchapter_mark 4\n",
            "3\tmark script_begun st02-st02_root\n",
            "4\tmark script_begun st02-st02_game000_Expand\n",
            "5\tmark script_begun phone\n",
            "6\tmark script_begun st01-st01_game020_Expand\n",
        ]
        self.assertEqual(chapter_starts_check.story_scenes(cues), {4: "st02-st02_game000_Expand"})

    def test_each_section_is_keyed_by_its_chapter(self):
        text = "chapter_start 2\nscript chapter\nchapter_mark 1\nchapter_start 3\nscript chapter\nchapter_mark 2\n"
        starts = chapter_starts_check.chapter_starts(text)
        self.assertEqual(sorted(starts), [2, 3])
        self.assertEqual(chapter_starts_check.mark_of(starts[3]), 2)

    def test_the_book_holds_place_one_the_noted_places_and_the_telephone_place(self):
        state = "phone_book_place 18\nnote_levels %s\n" % note_levels_with([4, 11, 30])
        self.assertEqual(chapter_starts_check.book_places(state), [1, 4, 11, 18])

    def test_a_ride_takes_the_place_it_is_given(self):
        self.assertEqual(chapter_starts_check.with_place("script chapter\nphone_book_place 2\n", 15), "script chapter\nphone_book_place 15\n")


if __name__ == "__main__":
    unittest.main()
