import struct
import unittest

import asset_writers
import frame_layout

WHOLE_PAGE_CROP = {"st01_x_000_00": (0, 0, 512, 256)}
PAGE_PATH = "charfiles/2D/01_Human/st01/x/st01_x_000/st01_x_000_00_IML4.tex"
PLACED_RECORD = {"page": 0, "cell_in_page": 0, "position": (100.0, 40.0), "anchor": (0, 0), "origin": (1024, 512), "point": (0, 0), "is_blank": False}
BLANK_RECORD = {"page": None, "cell_in_page": None, "position": (0.0, 0.0), "anchor": (0, 0), "origin": (0, 0), "point": (0, 0), "is_blank": True}


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


class AnimWriterTests(unittest.TestCase):
    def test_full_text_including_a_blank_frame(self):
        timeline = {"pages": [PAGE_PATH], "atlas_size": (512, 256), "cell_size": (100, 50), "records_per_ds_frame": 2, "placement_shift": (0, 0), "records": [PLACED_RECORD, BLANK_RECORD]}
        text = asset_writers.anim_text("st01_x_000", 1, frame_layout.frames_for_animation(timeline, None, WHOLE_PAGE_CROP))
        lines = text.splitlines()
        self.assertEqual(lines[0], "animation st01_x_000")
        self.assertEqual(lines[1], "loops 1")
        self.assertEqual(lines[2], "frame st01_x_000_00 2 -231 -118 24.822222 12.322222 0 0 0.193924 0.192535")
        self.assertEqual(lines[3], "frame - 2 0 0 0 0 0 0 0 0")

    def test_too_many_frames_are_refused(self):
        with self.assertRaises(ValueError):
            asset_writers.anim_text("x", 0, [asset_writers.blank_frame(1)] * 1025)

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
        steps = [["room", 11], ["plate", "st01-00", 0, 0, 1024, 384], ["object", "linne", "st01_linne_004", 281, 385.5, 900, 0], ["camera_pan", 0, 192, 60], ["hide_card"]]
        self.assertEqual(asset_writers.scene_text("opening", steps), "\n".join([
            "scene opening",
            "room 11",
            "plate st01-00 0 0 1024 384",
            "object linne st01_linne_004 281 385.5 900 0",
            "camera_pan 0 192 60",
            "hide_card",
        ]) + "\n")

    def test_too_many_steps_are_refused(self):
        with self.assertRaises(ValueError):
            asset_writers.scene_text("opening", [["wait", 1]] * 1025)


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
        self.assertEqual(asset_writers.manifest_text([], ["st01-00"], [], ["remastered", "ds"]), "art remastered\nart ds\nbackground st01-00\n")


if __name__ == "__main__":
    unittest.main()
