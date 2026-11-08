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


BOXED_THORN_SIZES = [dict(size, text_box=box) for size, box in zip(THORN_SIZES, ([34, 34, 145, 81], [35, 36, 254, 106], [28, 28, 308, 127]))]
THORN_ART_SIZES = {"thorn_s": (224, 144), "thorn_m": (324, 176), "thorn_l": (364, 180)}
STANDARD_SCALE = 27 / 44
ONE_LINE_HEIGHT = 60


def thorn_text_part(text_width, text_height):
    sizes = dict(THORN_ART_SIZES, bubble_said=(text_width, text_height))
    art = {"texture": "thorn_s", "dx": 0.5, "dy": 0.25, "after": 0, "grow": 6, "fits": "bubble_said", "thorn_sizes": BOXED_THORN_SIZES, "room_per_text": 30 / 44, "least_text_scale": STANDARD_SCALE, "text_line_height": ONE_LINE_HEIGHT}
    balloon = {"label": "said", "delay": 0, "parts": [art, {"texture": "bubble_said", "dx": -0.1333, "dy": -2.4444, "after": 6, "grow": 0}]}
    lines = [line.split() for line in script_assets.balloons_text([balloon], sizes).splitlines() if line.startswith("part ")]
    return lines[0][1], [float(value) for value in lines[1][2:6]]


class ThornTextTest(unittest.TestCase):
    def test_short_text_keeps_its_size_and_sits_on_the_solid_box_centre(self):
        art_name, (dx, dy, width, height) = thorn_text_part(80, 58)
        self.assertEqual(art_name, "thorn_s")
        self.assertAlmostEqual(dx, 0.5 + (34 + 72.5 - 112) / frame_layout.SCREEN_PIXELS_PER_DS_PIXEL, places=3)
        self.assertAlmostEqual(dy, 0.25 + (34 + 40.5 - 72) / frame_layout.SCREEN_PIXELS_PER_DS_PIXEL, places=3)
        self.assertAlmostEqual(width * frame_layout.SCREEN_PIXELS_PER_DS_PIXEL, 80, places=1)
        self.assertAlmostEqual(height * frame_layout.SCREEN_PIXELS_PER_DS_PIXEL, 58, places=1)

    def test_a_long_one_liner_keeps_the_standard_size_in_the_art_its_old_size_chose(self):
        art_name, (dx, dy, width, height) = thorn_text_part(300, 58)
        self.assertEqual(art_name, "thorn_m")
        self.assertAlmostEqual(width * frame_layout.SCREEN_PIXELS_PER_DS_PIXEL, 300 * STANDARD_SCALE, places=1)
        self.assertAlmostEqual(dy, 0.25 + (36 + 53 - 88) / frame_layout.SCREEN_PIXELS_PER_DS_PIXEL, places=3)

    def test_two_lines_keep_the_standard_size_even_in_the_smallest_art(self):
        art_name, (dx, dy, width, height) = thorn_text_part(140, 120)
        self.assertEqual(art_name, "thorn_s")
        self.assertAlmostEqual(width * frame_layout.SCREEN_PIXELS_PER_DS_PIXEL, 140 * STANDARD_SCALE, places=1)
        self.assertAlmostEqual(height * frame_layout.SCREEN_PIXELS_PER_DS_PIXEL, 120 * STANDARD_SCALE, places=1)


if __name__ == "__main__":
    unittest.main()
