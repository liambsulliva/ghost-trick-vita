import struct
import unittest

import asset_writers
import convert_assets
import frame_layout
import synthetic_files
import timeline_file


class GtexWriterTests(unittest.TestCase):
    def test_header_fields_for_rgba(self):
        data = asset_writers.gtex_bytes(2, 1, asset_writers.FORMAT_RGBA8888, bytes(8))
        self.assertEqual(data[:4], b"GTEX")
        self.assertEqual(struct.unpack_from("<IIII", data, 4), (2, 1, 0, 8))
        self.assertEqual(len(data), 28)

    def test_dxt5_size_is_sixteen_bytes_per_block(self):
        data = asset_writers.gtex_bytes(8, 4, asset_writers.FORMAT_DXT5, bytes(32))
        self.assertEqual(struct.unpack_from("<IIII", data, 4), (8, 4, 1, 32))

    def test_dxt1_size_is_eight_bytes_per_block(self):
        data = asset_writers.gtex_bytes(8, 8, asset_writers.FORMAT_DXT1, bytes(32))
        self.assertEqual(struct.unpack_from("<IIII", data, 4), (8, 8, 2, 32))

    def test_wrong_size_is_refused(self):
        with self.assertRaises(ValueError):
            asset_writers.gtex_bytes(8, 8, asset_writers.FORMAT_DXT1, bytes(16))

    def test_sides_that_are_not_multiples_of_four_are_refused(self):
        with self.assertRaises(ValueError):
            asset_writers.gtex_bytes(6, 4, asset_writers.FORMAT_DXT5, bytes(16))


class NumberFormatTests(unittest.TestCase):
    def test_trailing_zeros_are_removed(self):
        self.assertEqual(asset_writers.format_number(-40.5), "-40.5")
        self.assertEqual(asset_writers.format_number(96), "96")
        self.assertEqual(asset_writers.format_number(0.25), "0.25")

    def test_negative_zero_is_plain_zero(self):
        self.assertEqual(asset_writers.format_number(-0.0), "0")
        self.assertEqual(asset_writers.format_number(0), "0")


class TargetSizeTests(unittest.TestCase):
    def test_exact_sizes_stay_exact(self):
        self.assertEqual(frame_layout.target_size(4096, 1536), (2880, 1080))
        self.assertEqual(frame_layout.target_size(1024, 768), (720, 540))

    def test_other_sizes_round_to_multiples_of_four(self):
        self.assertEqual(frame_layout.target_size(128, 128), (92, 92))
        self.assertEqual(frame_layout.target_size(32, 128), (24, 92))

    def test_tiny_sizes_stay_at_least_one_block(self):
        self.assertEqual(frame_layout.target_size(2, 2), (4, 4))


TEXTURE_00 = "st01_x_000_00"
TEXTURE_01 = "st01_x_000_01"
WHOLE_PAGE_CROPS = {TEXTURE_00: (0, 0, 512, 256), TEXTURE_01: (0, 0, 128, 64)}


def fake_timeline(records_per_ds_frame, placement_shift=(0, 0)):
    return {
        "textures": ["charfiles/2D/01_Human/st01/x/st01_x_000/st01_x_000_00_IML4.tex", "charfiles/2D/01_Human/st01/x/st01_x_000/st01_x_000_01_IML4.tex"],
        "atlas": (512, 256),
        "cell": (100, 50),
        "records_per_ds_frame": records_per_ds_frame,
        "ds_frame_count": 1,
        "placement_shift": placement_shift,
        "records": [],
    }


def fake_record(page, local_cell, position, anchor, point):
    return {"cell": local_cell, "page": page, "local_cell": local_cell, "position": position, "anchor": anchor, "point": point, "is_blank": False}


BLANK_RECORD = {"cell": 0, "page": None, "local_cell": None, "position": (0.0, 0.0), "anchor": (0, 0), "point": (0, 0), "is_blank": True}


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
        self.assertEqual([record["local_cell"] for record in selected], [4])
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


class AnimWriterTests(unittest.TestCase):
    def test_full_text_including_a_blank_frame(self):
        timeline = fake_timeline(2)
        timeline["records"] = [fake_record(0, 0, (100.0, 40.0), (0, 0), (0, 0)), BLANK_RECORD]
        text = asset_writers.anim_text("st01_x_000", 1, frame_layout.frames_for_animation(timeline, None, WHOLE_PAGE_CROPS))
        lines = text.splitlines()
        self.assertEqual(lines[0], "animation st01_x_000")
        self.assertEqual(lines[1], "ticks_per_second 60")
        self.assertEqual(lines[2], "loops 1")
        self.assertEqual(lines[3], "frame st01_x_000_00 2 -231 -118 25 12.5 0 0 0.195312 0.195312")
        self.assertEqual(lines[4], "frame - 2 0 0 0 0 0 0 0 0")

    def test_too_many_frames_are_refused(self):
        with self.assertRaises(ValueError):
            asset_writers.anim_text("x", 0, [asset_writers.blank_frame(1)] * 513)

    def test_an_animation_needs_frames(self):
        with self.assertRaises(ValueError):
            asset_writers.anim_text("x", 0, [])

    def test_bad_names_are_refused(self):
        with self.assertRaises(ValueError):
            asset_writers.anim_text("has space", 0, [asset_writers.blank_frame(1)])
        with self.assertRaises(ValueError):
            asset_writers.anim_text("x" * 96, 0, [asset_writers.blank_frame(1)])


class SceneWriterTests(unittest.TestCase):
    def test_steps_become_lines(self):
        steps = [["side_bars", "left", "right"], ["room", 11], ["plate", "st01-00", 0, 0, 1024, 384], ["object", "linne", "st01_linne_004", 281, 385.5, 900, 0], ["camera_pan", 0, 192, 60], ["hide_card"]]
        self.assertEqual(asset_writers.scene_text("opening", steps), "\n".join([
            "scene opening",
            "side_bars left right",
            "room 11",
            "plate st01-00 0 0 1024 384",
            "object linne st01_linne_004 281 385.5 900 0",
            "camera_pan 0 192 60",
            "hide_card",
        ]) + "\n")

    def test_too_many_steps_are_refused(self):
        with self.assertRaises(ValueError):
            asset_writers.scene_text("opening", [["wait", 1]] * 1025)

    def test_expanding_a_set_description(self):
        description = {
            "message_hold_ticks": 30,
            "objects": {"a": {"layer": 5, "flip": 1}},
            "object_groups": {"group": [["a", "anim_a", 1, 2]]},
            "steps": [["plate", "p", 0, 8], ["object_group", "group"], ["message_hold"], ["object", "a", "anim_b", 3, 4], ["wait", 7]],
        }
        self.assertEqual(convert_assets.expand_steps(description, {"p": (16.0, 32.0)}), [
            ["plate", "p", 0, 8, 16.0, 32.0],
            ["object", "a", "anim_a", 1, 2, 5, 1],
            ["wait", 30],
            ["object", "a", "anim_b", 3, 4, 5, 1],
            ["wait", 7],
        ])

    def test_unknown_assets_are_refused(self):
        with self.assertRaises(ValueError):
            convert_assets.check_steps_refer_to_known_assets([["card", "missing"]], set(), set())
        with self.assertRaises(ValueError):
            convert_assets.check_steps_refer_to_known_assets([["object", "a", "missing", 0, 0, 0, 0]], set(), set())


class RealSetDescriptionTests(unittest.TestCase):
    def test_chapter1_opening_only_names_assets_it_declares(self):
        description = convert_assets.load_set_description("chapter1_opening")
        plate_names = [plate["name"] for plate in description["plates"]]
        screen_names = [texture["name"] for texture in description["screen_textures"]]
        animation_names = set(animation["name"] for animation in description["animations"])
        steps = convert_assets.expand_steps(description, {name: (1.0, 1.0) for name in plate_names})
        convert_assets.check_steps_refer_to_known_assets(steps, set(plate_names + screen_names), animation_names)
        self.assertLessEqual(len(steps), asset_writers.SCENE_STEP_MAX)


class MemoryEstimateTests(unittest.TestCase):
    def test_compressed_sides_round_up_to_powers_of_two(self):
        self.assertEqual(asset_writers.memory_bytes(720, 540, asset_writers.FORMAT_DXT1), 524288)
        self.assertEqual(asset_writers.memory_bytes(2880, 2880, asset_writers.FORMAT_DXT5), 16777216)
        self.assertEqual(asset_writers.memory_bytes(16, 8, asset_writers.FORMAT_DXT5), 128)

    def test_rgba_is_its_own_size(self):
        self.assertEqual(asset_writers.memory_bytes(3, 3, asset_writers.FORMAT_RGBA8888), 36)


class ManifestWriterTests(unittest.TestCase):
    def test_manifest_lists_each_kind(self):
        self.assertEqual(asset_writers.manifest_text(["opening"], ["st01-00"], ["st01_x_000"]), "scene opening\nbackground st01-00\nanimation st01_x_000\n")


class TimelineFileTests(unittest.TestCase):
    def make_shorts(self, first, anchor, point):
        shorts = [0] * 20
        shorts[0] = first
        shorts[1] = anchor[0]
        shorts[2] = anchor[1]
        shorts[6] = point[0]
        shorts[7] = point[1]
        return shorts

    def test_decode_reads_records_pages_and_blanks(self):
        records = [
            (self.make_shorts(10, (-4, -6), (-2, 8)), (40.0, 80.0), 0),
            (self.make_shorts(10, (-4, -6), (-2, 8)), (40.0, 80.0), 3),
            (self.make_shorts(0, (0, 0), (0, 0)), (0.0, 0.0), 3),
            (self.make_shorts(10, (0, 0), (0, 0)), (1.0, 2.0), 4),
        ]
        data = synthetic_files.timeline_file(["charfiles/2D/a_00_IML4.tex", "charfiles/2D/a_01_IML4.tex"], (64, 32), (16, 16), [8, 2], records, 2)
        timeline = timeline_file.decode_timeline(data)
        self.assertEqual(timeline["atlas"], (64, 32))
        self.assertEqual(timeline["cell"], (16, 16))
        self.assertEqual(timeline["records_per_ds_frame"], 2)
        self.assertEqual(len(timeline["records"]), 4)
        self.assertEqual(timeline["records"][0]["page"], 0)
        self.assertEqual(timeline["records"][0]["anchor"], (-4, -6))
        self.assertEqual(timeline["records"][0]["point"], (-2, 8))
        self.assertEqual(timeline["records"][1]["local_cell"], 3)
        self.assertTrue(timeline["records"][2]["is_blank"])
        self.assertIsNone(timeline["records"][2]["page"])
        self.assertEqual(timeline["records"][3]["position"], (1.0, 2.0))

    def test_decode_reads_the_placement_shift_and_ignores_record_adjust(self):
        records = [(self.make_shorts(10, (0, 0), (0, 0)), (1.0, 2.0), 0, (12, -2))]
        data = synthetic_files.timeline_file(["a"], (64, 32), (16, 16), [8], records, 1, (24, -4))
        timeline = timeline_file.decode_timeline(data)
        self.assertEqual(timeline["placement_shift"], (24, -4))
        self.assertNotIn("adjust", timeline["records"][0])

    def test_records_per_ds_frame_comes_from_the_file(self):
        record = (self.make_shorts(10, (0, 0), (0, 0)), (1.0, 2.0), 0)
        data = synthetic_files.timeline_file(["a"], (64, 32), (16, 16), [8], [record] * 8, 2)
        self.assertEqual(timeline_file.decode_timeline(data)["records_per_ds_frame"], 4)

    def test_decode_finds_the_second_page(self):
        records = [(self.make_shorts(10, (0, 0), (0, 0)), (1.0, 2.0), 9)]
        data = synthetic_files.timeline_file(["a", "b"], (64, 32), (16, 16), [8, 2], records, 1)
        record = timeline_file.decode_timeline(data)["records"][0]
        self.assertEqual(record["page"], 1)
        self.assertEqual(record["local_cell"], 1)

    def test_not_a_user_file_is_refused(self):
        with self.assertRaises(timeline_file.TimelineError):
            timeline_file.decode_timeline(b"NOPE" + bytes(64))


if __name__ == "__main__":
    unittest.main()
