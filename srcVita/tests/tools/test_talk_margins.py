import os
import re
import unittest

import talk_assets

SCRIPT_VIEW_SOURCE = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))), "src", "app", "script_view.c")
GLYPHS = ["glyph 0 0 10 20 0 5 12", "glyph 0 0 0 0 0 0 8"]


def page(box, lines):
    return ["page - 0 %d 0 0 %d 0 0 0 0 0" % (box, len(lines))] + ["line %d %d 2 0 1" % (x, y) for x, y in lines]


class TalkMarginTests(unittest.TestCase):
    def test_the_left_margin_matches_the_top_and_bottom_margin_of_a_full_page(self):
        top = talk_assets.BOTTOM_BOX_TOP
        rows = ["font f"] + GLYPHS + ["talk a"] + page(talk_assets.BOX_BOTTOM, [(120, top + 5), (120, top + 45), (120, top + 85)]) + ["talk b"] + page(talk_assets.BOX_BOTTOM, [(130, top + 45)])
        moved, margins = talk_assets.even_left_margins(rows)
        expected = ((10) + (talk_assets.BOTTOM_BOX_HEIGHT - 110)) / 2.0
        self.assertEqual(margins[talk_assets.BOX_BOTTOM], expected)
        self.assertEqual(int(moved[5].split()[1]), int(round(expected)))
        self.assertEqual(int(moved[10].split()[1]), int(round(expected)))
        self.assertEqual([row.split()[2] for row in moved if row.startswith("line")], [row.split()[2] for row in rows if row.startswith("line")])

    def test_a_top_box_page_keeps_its_margin_from_its_own_left_edge_and_fixed_talks_stay(self):
        top = talk_assets.TOP_BOX_TOP
        rows = ["font f"] + GLYPHS + ["talk a"] + page(talk_assets.BOX_TOP_WITH_HEAD, [(400, top + 10), (400, top + 50)]) + ["talk sys"] + page(talk_assets.BOX_BOTTOM, [(333, talk_assets.BOTTOM_BOX_TOP + 40)])
        moved, margins = talk_assets.even_left_margins(rows, ["sys"])
        self.assertEqual(int(moved[5].split()[1]), int(round(talk_assets.BOX_LEFT[talk_assets.BOX_TOP_WITH_HEAD] + margins[talk_assets.BOX_TOP_WITH_HEAD])))
        self.assertEqual(moved[-1], rows[-1])

    def test_a_page_spilling_out_of_its_box_does_not_set_the_margin(self):
        top = talk_assets.BOTTOM_BOX_TOP
        rows = ["font f"] + GLYPHS + ["talk a"] + page(talk_assets.BOX_BOTTOM, [(50, top + 5 + step * 40) for step in range(6)]) + ["talk b"] + page(talk_assets.BOX_BOTTOM, [(50, top + 20), (50, top + 60)])
        _, margins = talk_assets.even_left_margins(rows)
        self.assertGreater(margins[talk_assets.BOX_BOTTOM], 0)

    def test_the_box_edges_are_the_ones_the_app_draws(self):
        with open(SCRIPT_VIEW_SOURCE, encoding="utf-8") as source:
            edges = {name: float(value) for name, value in re.findall(r"#define (BOX_\w+) ([0-9.]+)f", source.read())}
        self.assertEqual(talk_assets.BOX_LEFT[talk_assets.BOX_BOTTOM], edges["BOX_BOTTOM_X"])
        self.assertEqual(talk_assets.BOX_LEFT[talk_assets.BOX_TOP_ALONE], edges["BOX_TOP_ALONE_X"])
        self.assertEqual(talk_assets.BOX_LEFT[talk_assets.BOX_TOP_WITH_HEAD], edges["BOX_TOP_WITH_HEAD_X"])
        self.assertEqual(talk_assets.BOTTOM_BOX_TOP, edges["BOX_BOTTOM_Y"])
        self.assertEqual(talk_assets.BOTTOM_BOX_HEIGHT, edges["BOX_HEIGHT"])
        self.assertEqual(talk_assets.TOP_BOX_TOP, edges["BOX_TOP_Y"])
        self.assertEqual(talk_assets.TOP_BOX_HEIGHT, edges["BOX_TOP_HEIGHT"])


if __name__ == "__main__":
    unittest.main()
