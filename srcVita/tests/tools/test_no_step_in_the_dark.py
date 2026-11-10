import os
import sys
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import check_no_step_in_the_dark


class StepsInTheDark(unittest.TestCase):
    def test_a_step_begun_after_a_fade_out_with_no_fade_in_is_found(self):
        cues = ["1\tmark script_begun yard", "5\tbrightness -8", "6\tbrightness -16", "9\tmark route_step_began 4:trick:lamp:0"]
        self.assertEqual(check_no_step_in_the_dark.steps_begun_in_the_dark(cues), [(9, "yard", "4:trick:lamp:0")])

    def test_a_step_begun_after_the_fade_in_passes(self):
        cues = ["5\tbrightness -16", "7\tbrightness 0", "9\tmark route_step_began 4:trick:lamp:0"]
        self.assertEqual(check_no_step_in_the_dark.steps_begun_in_the_dark(cues), [])

    def test_a_new_script_starts_lit(self):
        cues = ["5\tbrightness -16", "6\tmark script_begun next", "9\tmark route_step_began 4:trick:lamp:0"]
        self.assertEqual(check_no_step_in_the_dark.steps_begun_in_the_dark(cues), [])


if __name__ == "__main__":
    unittest.main()
