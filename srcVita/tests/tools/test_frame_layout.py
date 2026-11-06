import unittest

import frame_layout

TEXTURE_00 = "st01_x_000_00"
TEXTURE_01 = "st01_x_000_01"
WHOLE_PAGE_CROPS = {TEXTURE_00: (0, 0, 512, 256), TEXTURE_01: (0, 0, 128, 64)}


def fake_timeline(records_per_ds_frame, placement_shift=(0, 0)):
    return {
        "pages": ["charfiles/2D/01_Human/st01/x/st01_x_000/st01_x_000_00_IML4.tex", "charfiles/2D/01_Human/st01/x/st01_x_000/st01_x_000_01_IML4.tex"],
        "atlas_size": (512, 256),
        "cell_size": (100, 50),
        "records_per_ds_frame": records_per_ds_frame,
        "placement_shift": placement_shift,
        "records": [],
    }


def fake_record(page, cell_in_page, position, anchor, point):
    return {"page": page, "cell_in_page": cell_in_page, "position": position, "anchor": anchor, "point": point, "is_blank": False}


BLANK_RECORD = {"page": None, "cell_in_page": None, "position": (0.0, 0.0), "anchor": (0, 0), "point": (0, 0), "is_blank": True}


class TargetSizeTests(unittest.TestCase):
    def test_exact_sizes_stay_exact(self):
        self.assertEqual(frame_layout.target_size(4096, 1536), (2880, 1080))
        self.assertEqual(frame_layout.target_size(1024, 768), (720, 540))

    def test_other_sizes_round_to_multiples_of_four(self):
        self.assertEqual(frame_layout.target_size(128, 128), (92, 92))
        self.assertEqual(frame_layout.target_size(32, 128), (24, 92))

    def test_tiny_sizes_stay_at_least_one_block(self):
        self.assertEqual(frame_layout.target_size(2, 2), (4, 4))


class FrameLayoutTests(unittest.TestCase):
    def test_uv_comes_from_the_grid_position(self):
        frame = frame_layout.frame_for_record(fake_timeline(2), fake_record(0, 7, (0.0, 0.0), (0, 0), (0, 0)), 2, WHOLE_PAGE_CROPS)
        self.assertEqual(frame["texture"], TEXTURE_00)
        self.assertEqual(frame["u"], 200 / 512)
        self.assertEqual(frame["v"], 50 / 256)
        self.assertEqual(frame["u_size"], 100 / 512)
        self.assertEqual(frame["v_size"], 50 / 256)

    def test_a_single_cell_crop_fills_the_texture(self):
        crops = {TEXTURE_00: (200, 50, 100, 50)}
        frame = frame_layout.frame_for_record(fake_timeline(2), fake_record(0, 7, (0.0, 0.0), (0, 0), (0, 0)), 2, crops)
        self.assertEqual((frame["u"], frame["v"], frame["u_size"], frame["v_size"]), (0, 0, 1, 1))

    def test_uv_is_measured_from_the_crop_corner(self):
        crops = {TEXTURE_00: (100, 0, 300, 100)}
        frame = frame_layout.frame_for_record(fake_timeline(2), fake_record(0, 7, (0.0, 0.0), (0, 0), (0, 0)), 2, crops)
        self.assertEqual(frame["u"], 100 / 300)
        self.assertEqual(frame["v"], 50 / 100)
        self.assertEqual(frame["u_size"], 100 / 300)
        self.assertEqual(frame["v_size"], 50 / 100)

    def test_crops_cover_every_used_cell_of_each_page(self):
        timeline = fake_timeline(1)
        timeline["records"] = [fake_record(0, 1, (0.0, 0.0), (0, 0), (0, 0)), fake_record(0, 7, (0.0, 0.0), (0, 0), (0, 0)), BLANK_RECORD, fake_record(1, 0, (0.0, 0.0), (0, 0), (0, 0))]
        crops = {}
        frame_layout.add_used_cells_to_crops(crops, timeline, None)
        self.assertEqual(crops, {TEXTURE_00: (100, 0, 200, 100), TEXTURE_01: (0, 0, 100, 50)})

    def test_crops_only_count_the_selected_ds_frames(self):
        timeline = fake_timeline(1)
        timeline["records"] = [fake_record(0, 0, (0.0, 0.0), (0, 0), (0, 0)), fake_record(0, 7, (0.0, 0.0), (0, 0), (0, 0))]
        crops = {}
        frame_layout.add_used_cells_to_crops(crops, timeline, [1])
        self.assertEqual(crops, {TEXTURE_00: (200, 50, 100, 50)})

    def test_crops_grow_across_animations_sharing_a_page(self):
        timeline = fake_timeline(1)
        timeline["records"] = [fake_record(0, 0, (0.0, 0.0), (0, 0), (0, 0)), fake_record(0, 7, (0.0, 0.0), (0, 0), (0, 0))]
        crops = {}
        frame_layout.add_used_cells_to_crops(crops, timeline, [0])
        frame_layout.add_used_cells_to_crops(crops, timeline, [1])
        self.assertEqual(crops, {TEXTURE_00: (0, 0, 300, 100)})

    def test_placement_and_size_are_in_ds_pixels(self):
        frame = frame_layout.frame_for_record(fake_timeline(2), fake_record(0, 0, (100.0, 40.0), (-339, -51), (-336, 14)), 2, WHOLE_PAGE_CROPS)
        self.assertEqual(frame["x"], 25 - 256 + 1.5)
        self.assertEqual(frame["y"], 10 - 128 + 32.5)
        self.assertEqual(frame["width"], 25)
        self.assertEqual(frame["height"], 12.5)

    def test_a_difference_of_one_half_pixel_is_rounding_noise(self):
        frame = frame_layout.frame_for_record(fake_timeline(2), fake_record(0, 0, (100.0, 40.0), (-355, 13), (-356, 14)), 2, WHOLE_PAGE_CROPS)
        self.assertEqual(frame["x"], 25 - 256)
        self.assertEqual(frame["y"], 10 - 128)

    def test_placement_shift_is_in_art_pixels(self):
        frame = frame_layout.frame_for_record(fake_timeline(2, (24, -4)), fake_record(0, 0, (0.0, 0.0), (0, 0), (0, 0)), 2, WHOLE_PAGE_CROPS)
        self.assertEqual(frame["x"], -256 + 6)
        self.assertEqual(frame["y"], -128 - 1)

    def test_ticks_per_record(self):
        self.assertEqual(frame_layout.ticks_per_record(1), 4)
        self.assertEqual(frame_layout.ticks_per_record(2), 2)
        self.assertEqual(frame_layout.ticks_per_record(4), 1)
        with self.assertRaises(ValueError):
            frame_layout.ticks_per_record(3)

    def test_selected_ds_frame_picks_its_records(self):
        timeline = fake_timeline(2)
        timeline["records"] = [fake_record(0, index, (0.0, 0.0), (0, 0), (0, 0)) for index in range(6)]
        selected = frame_layout.select_records(timeline, [2])
        self.assertEqual([record["cell_in_page"] for record in selected], [4])
        self.assertEqual(len(frame_layout.select_records(timeline, None)), 6)

    def test_a_held_pose_lasts_one_ds_frame(self):
        timeline = fake_timeline(4)
        timeline["records"] = [fake_record(0, index, (0.0, 0.0), (0, 0), (0, 0)) for index in range(8)]
        self.assertEqual(frame_layout.frames_for_animation(timeline, [1], WHOLE_PAGE_CROPS)[0]["ticks"], 4)
        self.assertEqual(frame_layout.frames_for_animation(timeline, None, WHOLE_PAGE_CROPS)[0]["ticks"], 1)

    def test_blank_records_become_blank_frames_and_use_no_crop(self):
        timeline = fake_timeline(2)
        timeline["records"] = [BLANK_RECORD, fake_record(0, 1, (0.0, 0.0), (0, 0), (0, 0))]
        frames = frame_layout.frames_for_animation(timeline, None, WHOLE_PAGE_CROPS)
        self.assertEqual(frames[0]["texture"], "-")
        self.assertEqual(frames[0]["ticks"], 2)
        crops = {}
        frame_layout.add_used_cells_to_crops(crops, timeline, None)
        self.assertEqual(list(crops.keys()), [TEXTURE_00])

    def test_names_and_pack_paths(self):
        self.assertEqual(frame_layout.texture_name_for("charfiles/2D/a/ST01_A_000_00_IML4.tex"), "st01_a_000_00")
        self.assertEqual(frame_layout.pack_path_for("charfiles/2D/A/B_IML4.tex"), "natives/android/charfiles/2d/a/b_iml4.tex.691230823")


if __name__ == "__main__":
    unittest.main()
