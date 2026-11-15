import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools", "assets"))

import chapter_assets


class FakeScript:
    def __init__(self, path, operations):
        self.path = path
        self.operations = [(position, "op", op, operands) for position, op, operands in operations]


def packs_with_screens(screens_of_pages):
    def exists(_, path):
        for page, count in screens_of_pages.items():
            for screen in range(1, count + 1):
                if path == chapter_assets.STAFF_SCREEN_SOURCE % (page, screen):
                    return True
        return False
    return exists


FIRST_PAGE = FakeScript("st15/demo000", [
    (10, 0x106, [35, 0]),
    (20, 0xf8, [0]),
    (24, 0xb, [15]),
    (28, 0xa, []),
    (30, 0xf9, []),
    (32, 0x2f, [120]),
    (36, 0xc, [45]),
    (40, 0xa, []),
    (42, 0xfa, []),
])

LATER_PAGE = FakeScript("st15/demo001", [
    (8, 0xf8, [1]),
    (12, 0x2d, [100, 0]),
    (16, 0xfa, []),
    (20, 0x0, []),
    (100, 0xb, [25]),
    (104, 0xa, []),
    (106, 0xf9, []),
    (108, 0xc, [45]),
    (112, 0xa, []),
    (114, 0x2e, []),
])


class StaffRollRowsTests(unittest.TestCase):
    def setUp(self):
        self.path_exists = chapter_assets.path_exists

    def tearDown(self):
        chapter_assets.path_exists = self.path_exists

    def rows(self, scripts, screens):
        chapter_assets.path_exists = packs_with_screens(screens)
        return chapter_assets.staff_roll_rows(None, dict((script.path, script) for script in scripts))

    def test_each_page_carries_the_fades_and_hold_its_script_plays_around_it(self):
        textures, text = self.rows([LATER_PAGE, FIRST_PAGE], {0: 1, 1: 2})
        self.assertEqual(text, "staffpage 0 1 15 120 45\nstaffpage 1 2 25 0 45\nstaffmusic bgm35_new\n")
        self.assertEqual([texture["name"] for texture in textures], ["staff_00_01", "staff_01_01", "staff_01_02"])

    def test_a_page_followed_by_something_other_than_its_fades_stops_the_bake(self):
        odd = FakeScript("st15/odd", [(0, 0xf8, [2]), (4, 0x2f, [30]), (8, 0xfa, [])])
        with self.assertRaises(ValueError):
            self.rows([odd], {2: 1})

    def test_a_page_started_twice_with_different_fades_stops_the_bake(self):
        again = FakeScript("st15/again", [(0, 0xf8, [0]), (4, 0xb, [30]), (6, 0xf9, []), (8, 0xc, [45])])
        with self.assertRaises(ValueError):
            self.rows([FIRST_PAGE, again], {0: 1})

    def test_no_music_row_without_a_music_start_before_the_first_page(self):
        _, text = self.rows([LATER_PAGE], {1: 1})
        self.assertNotIn("staffmusic", text)


if __name__ == "__main__":
    unittest.main()
