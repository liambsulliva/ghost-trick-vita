import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools", "assets"))
import frame_layout
import script_assets

THORN_SIZES = [{"texture": "thorn_s", "widest_text": 170}, {"texture": "thorn_m", "widest_text": 238}, {"texture": "thorn_l", "widest_text": 100000}]


def balloon_with_text_part(art):
    return {"label": "said", "delay": 0, "parts": [art, {"texture": "bubble_said", "dx": 0, "dy": 0, "after": 6, "grow": 0}]}


def first_part_size(text):
    fields = [line for line in text.splitlines() if line.startswith("part ")][0].split()
    return float(fields[4]) * frame_layout.SCREEN_PIXELS_PER_DS_PIXEL, float(fields[5]) * frame_layout.SCREEN_PIXELS_PER_DS_PIXEL


class BubbleRoomTest(unittest.TestCase):
    def test_smaller_text_keeps_the_bubble_sized_for_the_room_text(self):
        sizes = {"oval": (40, 30), "bubble_said": (180, 54)}
        art = {"texture": "oval", "dx": 0, "dy": 0, "after": 0, "grow": 6, "fits": "bubble_said"}
        plain_width, plain_height = first_part_size(script_assets.balloons_text([balloon_with_text_part(art)], sizes))
        roomy_width, roomy_height = first_part_size(script_assets.balloons_text([balloon_with_text_part(dict(art, room_per_text=30 / 27))], sizes))
        self.assertAlmostEqual(plain_width, 180 + script_assets.BUBBLE_MARGIN_WIDTH, places=1)
        self.assertAlmostEqual(roomy_width, 200 + script_assets.BUBBLE_MARGIN_WIDTH, places=1)
        self.assertAlmostEqual(roomy_height, 60 + script_assets.BUBBLE_MARGIN_HEIGHT, places=1)
        self.assertGreater(roomy_height, plain_height)

    def test_the_thorn_bubble_is_chosen_by_the_room_text_width(self):
        sizes = {"thorn_s": (200, 80), "thorn_m": (260, 90), "thorn_l": (320, 100), "bubble_said": (160, 30)}
        art = {"texture": "thorn_s", "dx": 0, "dy": 0, "after": 0, "grow": 6, "fits": "bubble_said", "thorn_sizes": THORN_SIZES}
        self.assertEqual(script_assets.with_thorn_chosen_by_text(art, sizes)["texture"], "thorn_s")
        self.assertEqual(script_assets.with_thorn_chosen_by_text(dict(art, room_per_text=30 / 27), sizes)["texture"], "thorn_m")


if __name__ == "__main__":
    unittest.main()
