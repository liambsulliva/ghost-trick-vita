import os
import struct
import tempfile
import unittest

import asset_writers
import synthetic_files
import texture_encoder

TESTS_FOLDER = os.path.dirname(os.path.abspath(__file__))
SRC_VITA_FOLDER = os.path.dirname(os.path.dirname(TESTS_FOLDER))
TOOL_PATH = os.path.join(SRC_VITA_FOLDER, "build", "tools", "texture_tool_checked")


def alpha_table(alpha0, alpha1):
    table = [alpha0, alpha1]
    if alpha0 > alpha1:
        for step in range(1, 7):
            table.append(((7 - step) * alpha0 + step * alpha1) // 7)
    else:
        for step in range(1, 5):
            table.append(((5 - step) * alpha0 + step * alpha1) // 5)
        table.extend([0, 255])
    return table


def decode_alpha_block(block):
    table = alpha_table(block[0], block[1])
    indices = int.from_bytes(block[2:8], "little")
    return [table[(indices >> (3 * position)) & 7] for position in range(16)]


def decode_dxt_image(block_data, width, height, block_bytes):
    pixels = [None] * (width * height)
    block_index = 0
    for block_y in range(0, height, 4):
        for block_x in range(0, width, 4):
            block = block_data[block_index:block_index + block_bytes]
            block_index = block_index + block_bytes
            if block_bytes == 16:
                alphas = decode_alpha_block(block[:8])
                colors = synthetic_files.decode_color_block(block[8:])
            else:
                colors = synthetic_files.decode_color_block(block)
                alphas = [color[3] for color in colors]
            for position in range(16):
                x = block_x + position % 4
                y = block_y + position // 4
                color = colors[position]
                pixels[y * width + x] = (color[0], color[1], color[2], alphas[position])
    return pixels


def solid_quadrants_image(alpha_for_bottom):
    pixels = bytearray()
    for y in range(8):
        for x in range(8):
            if y < 4 and x < 4:
                pixels.extend([255, 0, 0, 255])
            elif y < 4:
                pixels.extend([0, 255, 0, 255])
            elif x < 4:
                pixels.extend([0, 0, 255, alpha_for_bottom])
            else:
                pixels.extend([255, 255, 255, alpha_for_bottom])
    return bytes(pixels)


def assert_close(test, actual, expected, tolerance=8):
    for actual_value, expected_value in zip(actual, expected):
        test.assertLessEqual(abs(actual_value - expected_value), tolerance, str((actual, expected)))


class DxtEncodingTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.TemporaryDirectory()
        self.addCleanup(self.folder.cleanup)

    def test_dxt1_blocks_run_from_the_top_left(self):
        rgba = solid_quadrants_image(255)
        data = texture_encoder.encode_rgba(self.folder.name, "image", 8, 8, rgba, "dxt1")
        self.assertEqual(len(data), 4 * 8)
        pixels = decode_dxt_image(data, 8, 8, 8)
        assert_close(self, pixels[0], (255, 0, 0, 255))
        assert_close(self, pixels[7], (0, 255, 0, 255))
        assert_close(self, pixels[56], (0, 0, 255, 255))
        assert_close(self, pixels[63], (255, 255, 255, 255))

    def test_dxt5_keeps_row_order_and_alpha(self):
        rgba = solid_quadrants_image(0)
        data = texture_encoder.encode_rgba(self.folder.name, "image", 8, 8, rgba, "dxt5")
        self.assertEqual(len(data), 4 * 16)
        pixels = decode_dxt_image(data, 8, 8, 16)
        assert_close(self, pixels[0], (255, 0, 0, 255))
        assert_close(self, pixels[7], (0, 255, 0, 255))
        assert_close(self, pixels[56], (0, 0, 255, 0))
        assert_close(self, pixels[63], (255, 255, 255, 0))

    def test_rows_inside_a_block_run_from_the_top(self):
        pixels = bytearray()
        for y in range(4):
            for x in range(4):
                if y < 2:
                    pixels.extend([255, 0, 0, 255])
                else:
                    pixels.extend([0, 0, 255, 255])
        data = texture_encoder.encode_rgba(self.folder.name, "image", 4, 4, bytes(pixels), "dxt1")
        decoded = decode_dxt_image(data, 4, 4, 8)
        assert_close(self, decoded[0], (255, 0, 0, 255))
        assert_close(self, decoded[15], (0, 0, 255, 255))

    def test_dxt1_opaque_gradient_has_no_transparent_pixels(self):
        pixels = bytearray()
        for y in range(16):
            for x in range(16):
                pixels.extend([x * 16, y * 16, 128, 255])
        data = texture_encoder.encode_rgba(self.folder.name, "image", 16, 16, bytes(pixels), "dxt1")
        for pixel in decode_dxt_image(data, 16, 16, 8):
            self.assertEqual(pixel[3], 255)

    def test_a_wrong_compression_header_is_refused(self):
        dds = b"DDS " + struct.pack("<III", 124, 0, 4) + struct.pack("<I", 4) + bytes(68) + b"DXT1" + bytes(40)
        with self.assertRaises(ValueError):
            texture_encoder.block_data_from_dds(dds, 4, 4, "dxt5")

    def test_scaling_colors_leaves_alpha_alone(self):
        self.assertEqual(texture_encoder.scale_colors(bytes([100, 200, 255, 77]), 0.5), bytes([50, 100, 128, 77]))


class TexToGtexTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.TemporaryDirectory()
        self.addCleanup(self.folder.cleanup)

    def test_opaque_art_becomes_dxt1(self):
        block = synthetic_files.etc2_constant_block(20, 10, 5)
        tex = synthetic_files.tex_file(4, 4, synthetic_files.FORMAT_ETC2_RGB, block, 8)
        data, texture_format = texture_encoder.convert_tex_to_gtex(TOOL_PATH, self.folder.name, "opaque", tex, 4, 4)
        self.assertEqual(texture_format, asset_writers.FORMAT_DXT1)
        self.assertEqual(struct.unpack_from("<IIII", data, 4), (4, 4, 2, 8))

    def test_transparent_art_becomes_dxt5(self):
        block = synthetic_files.astc_constant_block(200, 100, 50, 128)
        tex = synthetic_files.tex_file(6, 6, synthetic_files.FORMAT_ASTC_6X6, block, 16)
        data, texture_format = texture_encoder.convert_tex_to_gtex(TOOL_PATH, self.folder.name, "clear", tex, 8, 8)
        self.assertEqual(texture_format, asset_writers.FORMAT_DXT5)
        self.assertEqual(struct.unpack_from("<IIII", data, 4), (8, 8, 1, 64))
        decoded = decode_dxt_image(data[20:], 8, 8, 16)
        assert_close(self, decoded[0], (200, 100, 50, 128))

    def test_tex_size_reader(self):
        tex = synthetic_files.tex_file(4, 8, synthetic_files.FORMAT_RGBA8, bytes(128), 16)
        self.assertEqual(texture_encoder.read_tex_size(tex), (4, 8))
        with self.assertRaises(ValueError):
            texture_encoder.read_tex_size(b"NOPE")


if __name__ == "__main__":
    unittest.main()
