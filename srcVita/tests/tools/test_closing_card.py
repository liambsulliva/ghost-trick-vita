import glob
import json
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools", "assets"))

import chapter_assets

SETS = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools", "assets", "*.json")


class ClosingCardTests(unittest.TestCase):
    def test_a_set_that_plays_the_game_s_own_cards_cannot_close_on_a_next_chapter_card(self):
        description = {"chapter": "st06", "chapter_cards": {}, "scripts": {"next_chapter_card_texture": "next_chapter_card"}}
        with self.assertRaises(chapter_assets.ChapterBakeError):
            chapter_assets.refuse_a_closing_card_beside_the_game_s_own_cards(description)

    def test_a_single_chapter_set_may_close_on_the_next_chapter_s_card(self):
        description = {"chapter": "st01", "scripts": {"next_chapter_card_texture": "next_chapter_card"}}
        chapter_assets.refuse_a_closing_card_beside_the_game_s_own_cards(description)

    def test_every_shipped_set_passes(self):
        paths = sorted(glob.glob(SETS))
        self.assertTrue(paths)
        for path in paths:
            with open(path) as file:
                description = json.load(file)
            if isinstance(description, dict):
                chapter_assets.refuse_a_closing_card_beside_the_game_s_own_cards(description)


if __name__ == "__main__":
    unittest.main()
