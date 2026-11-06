import os
import subprocess
import tempfile
import unittest

import synthetic_files

TESTS_FOLDER = os.path.dirname(os.path.abspath(__file__))
SRC_VITA_FOLDER = os.path.dirname(os.path.dirname(TESTS_FOLDER))
TOOL_PATH = os.path.join(SRC_VITA_FOLDER, "build", "tools", "texture_tool_checked")


def rgba_tex(width, height, pixels):
    return synthetic_files.tex_file(width, height, synthetic_files.FORMAT_RGBA8, bytes(pixels), width * 4)


class TextureToolCase(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.TemporaryDirectory()
        self.addCleanup(self.folder.cleanup)
        self.input_path = os.path.join(self.folder.name, "input.tex")
        self.output_path = os.path.join(self.folder.name, "output.rgba")

    def run_tool(self, tex_bytes, width, height, crop=None):
        with open(self.input_path, "wb") as output:
            output.write(tex_bytes)
        command = [TOOL_PATH, self.input_path, self.output_path, str(width), str(height)]
        if crop is not None:
            command.extend(str(value) for value in crop)
        return subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    def convert(self, tex_bytes, width, height, crop=None):
        result = self.run_tool(tex_bytes, width, height, crop)
        self.assertEqual(result.returncode, 0, result.stderr.decode())
        with open(self.output_path, "rb") as source:
            return list(source.read()), result.stdout.decode().split()


class DecodeTests(TextureToolCase):
    def test_rgba_passes_through_unchanged(self):
        pixels = [10, 20, 30, 255, 40, 50, 60, 255, 70, 80, 90, 255, 100, 110, 120, 255]
        output, info = self.convert(rgba_tex(2, 2, pixels), 2, 2)
        self.assertEqual(output, pixels)
        self.assertEqual(info, ["2", "2", "28", "1"])

    def test_top_mip_is_read_from_its_recorded_offset(self):
        top_pixels = [10, 20, 30, 255]
        small_mip_pixels = [200, 210, 220, 255]
        tex = synthetic_files.tex_file_with_small_mip_stored_first(1, 1, synthetic_files.FORMAT_RGBA8, bytes(top_pixels), 4, bytes(small_mip_pixels))
        output, _ = self.convert(tex, 1, 1)
        self.assertEqual(output, top_pixels)

    def test_astc_void_extent_decodes_to_its_colour(self):
        block = synthetic_files.astc_constant_block(200, 100, 50, 255)
        tex = synthetic_files.tex_file(6, 6, synthetic_files.FORMAT_ASTC_6X6, block, 16)
        output, info = self.convert(tex, 6, 6)
        self.assertEqual(output[:4], [200, 100, 50, 255])
        self.assertEqual(output[-4:], [200, 100, 50, 255])
        self.assertEqual(info, ["6", "6", "1038", "1"])

    def test_astc_blocks_run_left_to_right(self):
        left = synthetic_files.astc_constant_block(255, 0, 0, 255)
        right = synthetic_files.astc_constant_block(0, 0, 255, 255)
        tex = synthetic_files.tex_file(12, 6, synthetic_files.FORMAT_ASTC_6X6, left + right, 32)
        output, _ = self.convert(tex, 12, 6)
        self.assertEqual(output[:4], [255, 0, 0, 255])
        self.assertEqual(output[11 * 4:12 * 4], [0, 0, 255, 255])

    def test_astc_alpha_marks_the_image_as_not_opaque(self):
        block = synthetic_files.astc_constant_block(200, 100, 50, 128)
        tex = synthetic_files.tex_file(6, 6, synthetic_files.FORMAT_ASTC_6X6, block, 16)
        output, info = self.convert(tex, 6, 6)
        self.assertEqual(output[:4], [200, 100, 50, 128])
        self.assertEqual(info[3], "0")

    def test_etc2_differential_block_decodes_with_the_modifier(self):
        block = synthetic_files.etc2_constant_block(20, 10, 5)
        tex = synthetic_files.tex_file(4, 4, synthetic_files.FORMAT_ETC2_RGB, block, 8)
        output, info = self.convert(tex, 4, 4)
        expected_pixel = [synthetic_files.expand_5_bits(20) + 2, synthetic_files.expand_5_bits(10) + 2, synthetic_files.expand_5_bits(5) + 2, 255]
        self.assertEqual(output[:4], expected_pixel)
        self.assertEqual(output[-4:], expected_pixel)
        self.assertEqual(info, ["4", "4", "1086", "1"])


class ResizeTests(TextureToolCase):
    def test_transparent_pixels_do_not_darken_the_average(self):
        pixels = [255, 0, 0, 255, 0, 0, 0, 0]
        output, info = self.convert(rgba_tex(2, 1, pixels), 1, 1)
        self.assertEqual(output, [255, 0, 0, 128])
        self.assertEqual(info[3], "0")

    def test_alpha_weights_the_colour_average(self):
        pixels = [200, 0, 0, 255, 0, 200, 0, 85]
        output, _ = self.convert(rgba_tex(2, 1, pixels), 1, 1)
        self.assertEqual(output, [150, 50, 0, 170])

    def test_fractional_area_average_uses_coverage(self):
        pixels = [30, 30, 30, 255, 90, 90, 90, 255, 150, 150, 150, 255]
        output, _ = self.convert(rgba_tex(3, 1, pixels), 2, 1)
        self.assertEqual(output, [50, 50, 50, 255, 130, 130, 130, 255])

    def test_two_dimensional_downscale_averages_a_block(self):
        pixels = []
        for value in (10, 20, 30, 40):
            pixels.extend([value, value, value, 255])
        output, _ = self.convert(rgba_tex(2, 2, pixels), 1, 1)
        self.assertEqual(output, [25, 25, 25, 255])

    def test_upscale_repeats_pixels(self):
        pixels = [10, 20, 30, 255]
        output, _ = self.convert(rgba_tex(1, 1, pixels), 2, 2)
        self.assertEqual(output, pixels * 4)

    def test_crop_uses_only_the_rectangle(self):
        pixels = [1, 1, 1, 255, 2, 2, 2, 255, 3, 3, 3, 255, 4, 4, 4, 255]
        output, _ = self.convert(rgba_tex(2, 2, pixels), 1, 2, crop=(1, 0, 1, 2))
        self.assertEqual(output, [2, 2, 2, 255, 4, 4, 4, 255])


class ColourFillTests(TextureToolCase):
    def test_fill_copies_the_nearest_visible_colour(self):
        pixels = [255, 0, 0, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 0, 200]
        output, _ = self.convert(rgba_tex(5, 1, pixels), 5, 1)
        self.assertEqual(output[4:8], [255, 0, 0, 0])
        self.assertEqual(output[12:16], [0, 255, 0, 0])
        self.assertEqual(output[16:20], [0, 255, 0, 200])

    def test_fill_keeps_transparent_pixels_transparent_in_both_directions(self):
        pixels = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 0, 0, 0, 0]
        output, _ = self.convert(rgba_tex(2, 2, pixels), 2, 2)
        self.assertEqual(output[0:4], [0, 0, 255, 0])
        self.assertEqual(output[4:8], [0, 0, 255, 0])
        self.assertEqual(output[12:16], [0, 0, 255, 0])

    def test_fully_transparent_image_stays_black(self):
        pixels = [0, 0, 0, 0] * 4
        output, info = self.convert(rgba_tex(2, 2, pixels), 2, 2)
        self.assertEqual(output, pixels)
        self.assertEqual(info[3], "0")


class BadInputTests(TextureToolCase):
    def assert_rejected(self, tex_bytes, width=2, height=2, crop=None):
        result = self.run_tool(tex_bytes, width, height, crop)
        self.assertEqual(result.returncode, 1, result.stderr.decode())
        self.assertIn(b"texture_tool:", result.stderr)

    def test_bad_magic_is_rejected(self):
        self.assert_rejected(b"NOPE" + bytes(100))

    def test_short_file_is_rejected(self):
        self.assert_rejected(b"TEX\0")

    def test_truncated_mip_is_rejected(self):
        tex = rgba_tex(2, 2, bytes(16))
        self.assert_rejected(tex[:-1])

    def test_wrong_mip_size_is_rejected(self):
        tex = synthetic_files.tex_file(6, 6, synthetic_files.FORMAT_ASTC_6X6, bytes(16), 16)
        self.assert_rejected(tex[:52] + bytes([8]) + tex[53:])

    def test_unknown_format_is_rejected(self):
        self.assert_rejected(synthetic_files.tex_file(2, 2, 9999, bytes(16), 8))

    def test_crop_outside_the_image_is_rejected(self):
        self.assert_rejected(rgba_tex(2, 2, bytes(16)), crop=(1, 1, 2, 2))

    def test_zero_output_size_is_rejected(self):
        self.assert_rejected(rgba_tex(2, 2, bytes(16)), width=0)


if __name__ == "__main__":
    unittest.main()
