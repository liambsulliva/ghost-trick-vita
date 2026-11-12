import unittest

import numpy as np

import pause_art


def cat_table(rows):
    lines = ["pages 1"] + ["frame %s %d %d 0 0 0 4 4 0 0" % (name, index, ticks) for index, (name, ticks) in enumerate(rows)]
    return ("\n".join(lines) + "\n").encode("ascii")


def standing_cat(body_lift):
    cell = np.zeros((60, 60, 4), dtype=np.uint8)
    cell[50:60, 10:20, 3] = 255
    cell[20 - body_lift:52 - body_lift, 8:50, 3] = 255
    return cell


class PlayedTicksTest(unittest.TestCase):
    def test_ranges_run_forward_then_backward_inclusive(self):
        ticks = list(range(20))
        self.assertEqual(pause_art.played_ticks(ticks, [[0, 4], [3, 1], [6, 0]]), [0, 1, 2, 3, 4, 3, 2, 1, 6, 5, 4, 3, 2, 1, 0])


class CatTrackTextTest(unittest.TestCase):
    def test_idle_and_fidget_expand_each_frame_by_its_ticks(self):
        table = cat_table([("t_idle", 2), ("t_idle", 1), ("t_look", 3), ("t_look", 1)])
        animations = [{"name": "t_idle"}, {"name": "t_look"}]
        fidget = {"animation": 1, "play": [[0, 3], [2, 1]], "every": 9, "link": 1, "dissolve": 2}
        lines = pause_art.cat_track_text(table, animations, fidget).decode("ascii").splitlines()
        self.assertEqual(lines[:3], ["every 9", "link 1", "dissolve 2"])
        self.assertEqual([line for line in lines if line.startswith("idle")], ["idle 0", "idle 0", "idle 1"])
        self.assertEqual([line for line in lines if line.startswith("fidget")], ["fidget 2", "fidget 2", "fidget 2", "fidget 3", "fidget 2", "fidget 2"])


class GridTest(unittest.TestCase):
    def test_the_grid_covers_every_frame_and_its_anchor_scales_to_a_whole_pixel(self):
        placed = [(np.zeros((168, 204, 4)), -100, -168), (np.zeros((40, 300, 4)), -150, -10)]
        width, height, anchor_x, anchor_y = pause_art.grid_for(placed)
        factor = pause_art.CAT_SCREEN_PIXELS_PER_DS_PIXEL / pause_art.frame_layout.ART_PER_DS_PIXEL
        for pixels, left, top in placed:
            self.assertGreaterEqual(anchor_x + left, 0)
            self.assertGreaterEqual(anchor_y + top, 0)
            self.assertLessEqual(anchor_x + left + pixels.shape[1], width)
            self.assertLessEqual(anchor_y + top + pixels.shape[0], height)
        for value in (width, height, anchor_x, anchor_y):
            self.assertEqual(value * factor, round(value * factor))


class PinFramesTest(unittest.TestCase):
    def test_a_frame_placed_off_by_a_few_pixels_lands_back_on_the_same_paws(self):
        cells = [{"pixels": standing_cat(0), "left": -30 + dx, "top": -60 + dy} for dx, dy in ((0, 0), (1, 0), (0, 2), (-2, -1))]
        pins = pause_art.pin_frames(cells)
        self.assertEqual(len(set(pins)), 1)

    def test_a_body_that_bobs_is_held_on_one_row(self):
        cells = [{"pixels": standing_cat(lift), "left": -30, "top": -60} for lift in (0, 1, 2, 1)]
        pins = pause_art.pin_frames(cells)
        body_tops = [top + 20 - lift for (_, top), lift in zip(pins, (0, 1, 2, 1))]
        self.assertEqual(len(set(body_tops)), 1)


class SpotlightTest(unittest.TestCase):
    def test_the_pool_is_as_wide_as_the_art_and_fades_out_at_its_rim(self):
        art = np.zeros((40, 100, 4), dtype=np.uint8)
        art[:, 10:90] = (200, 180, 120, 255)
        pixels = pause_art.spotlight_pixels(art)
        self.assertEqual(pixels.shape[:2], (100, 100))
        self.assertEqual(pixels[50, 50, 3], 255)
        self.assertEqual(pixels[0, 0, 3], 0)


class FramesFromTextTest(unittest.TestCase):
    def test_frame_lines_become_fields(self):
        frames = pause_art.frames_from_text("name t_x\nframe page_00 3 1 2 30 40 0.5 0.25 0.125 0.0625\n")
        self.assertEqual(len(frames), 1)
        self.assertEqual(frames[0]["texture"], "page_00")
        self.assertEqual(frames[0]["ticks"], 3)
        self.assertEqual(frames[0]["width"], 30.0)
        self.assertEqual(frames[0]["v_size"], 0.0625)


if __name__ == "__main__":
    unittest.main()
