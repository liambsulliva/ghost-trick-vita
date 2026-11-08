import os
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools", "assets"))
import read_ahead


class CardReadAheadTest(unittest.TestCase):
    def test_card_effects_read_their_own_texture_ahead(self):
        with tempfile.TemporaryDirectory() as folder:
            path = os.path.join(folder, "effects.txt")
            with open(path, "w") as handle:
                handle.write("effect 66 card chapter_card_00 0 1 0 0 0\neffect 68 card chapter_card_02 0 1 0 0 0\neffect 7 movie spark 0 1 0 0 0\nfxmovie spark 0 1 2 0 2\n")
            textures = read_ahead.effect_textures(path)
            self.assertEqual(textures["66"], ["chapter_card_00"])
            self.assertEqual(textures["68"], ["chapter_card_02"])
            self.assertEqual(textures["7"], ["fx_spark_00", "fx_spark_01"])


if __name__ == "__main__":
    unittest.main()
