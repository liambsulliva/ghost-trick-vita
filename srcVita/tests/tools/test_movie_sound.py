import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools", "assets"))

import script_assets


class MovieSoundTest(unittest.TestCase):
    def test_a_movie_row_carries_its_sound_or_none(self):
        text = script_assets.movies_text([{"name": "rewind", "frames": 80, "fps": 20, "sound": 82}, {"name": "phone", "frames": 93, "fps": 20, "class_id": "41ee4e1e"}])
        self.assertEqual(text, "movie rewind 80 3 0 82\nmovie phone 93 3 41ee4e1e -1\n")


if __name__ == "__main__":
    unittest.main()
