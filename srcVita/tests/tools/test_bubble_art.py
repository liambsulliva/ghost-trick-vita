import os
import shutil
import tempfile
import unittest

import asset_writers
import bubble_art
import synthetic_files
from test_texture_encoder import decode_dxt_image

TESTS_FOLDER = os.path.dirname(os.path.abspath(__file__))
SRC_VITA_FOLDER = os.path.dirname(os.path.dirname(TESTS_FOLDER))
TOOL_PATH = os.path.join(SRC_VITA_FOLDER, "build", "tools", "texture_tool_checked")
SOURCE_PATH = "page.tex"
HEADER_SIZE = 20


def two_color_page():
    columns = 16
    rows = 16
    white = synthetic_files.astc_constant_block(255, 255, 255, 255)
    empty = synthetic_files.astc_constant_block(0, 0, 0, 0)
    blocks = bytearray()
    for row in range(rows):
        for column in range(columns):
            blocks.extend(white if column < 8 else empty)
    return synthetic_files.tex_file(columns * 6, rows * 6, synthetic_files.FORMAT_ASTC_6X6, bytes(blocks), columns * 16)


def blank_part(x, y, width, height, crop, **flips):
    part = {"x": x, "y": y, "width": width, "height": height, "crop": crop}
    part.update(flips)
    return part


class PixelCopyingTests(unittest.TestCase):
    def test_flips_read_the_part_from_the_other_side(self):
        part = {"width": 4, "height": 2}
        self.assertEqual(bubble_art.part_pixel_offset(part, 0, 0), 0)
        self.assertEqual(bubble_art.part_pixel_offset(dict(part, flip_x=True), 0, 0), 3 * 4)
        self.assertEqual(bubble_art.part_pixel_offset(dict(part, flip_y=True), 0, 0), 4 * 4)
        self.assertEqual(bubble_art.part_pixel_offset(dict(part, flip_x=True, flip_y=True), 1, 1), 2 * 4)

    def test_an_opaque_pixel_replaces_what_is_below(self):
        canvas = bytearray([10, 20, 30, 255])
        bubble_art.blend_pixel_over(canvas, 0, bytes([200, 100, 50, 255]), 0)
        self.assertEqual(list(canvas), [200, 100, 50, 255])

    def test_a_transparent_pixel_changes_nothing(self):
        canvas = bytearray([10, 20, 30, 255])
        bubble_art.blend_pixel_over(canvas, 0, bytes([200, 100, 50, 0]), 0)
        self.assertEqual(list(canvas), [10, 20, 30, 255])

    def test_half_alpha_over_empty_keeps_the_color_and_the_alpha(self):
        canvas = bytearray(4)
        bubble_art.blend_pixel_over(canvas, 0, bytes([200, 100, 50, 128]), 0)
        self.assertEqual(list(canvas), [200, 100, 50, 128])

    def test_half_alpha_over_opaque_mixes_and_stays_opaque(self):
        canvas = bytearray([0, 0, 0, 255])
        bubble_art.blend_pixel_over(canvas, 0, bytes([200, 200, 200, 128]), 0)
        self.assertAlmostEqual(canvas[0], 100, delta=1)
        self.assertEqual(canvas[3], 255)

    def test_a_part_outside_the_canvas_is_clipped(self):
        canvas = bytearray(2 * 2 * 4)
        part = {"x": 1, "y": 1, "width": 2, "height": 2}
        bubble_art.paste_part(canvas, 2, 2, part, bytes([255, 255, 255, 255]) * 4)
        self.assertEqual(canvas[3::4], bytearray([0, 0, 0, 255]))


class ComposeTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, self.folder)

    def compose(self, parts):
        description = {"name": "bubble", "source": SOURCE_PATH, "width": 16, "height": 8, "parts": parts}
        return bubble_art.compose_gtex(TOOL_PATH, self.folder, description, two_color_page())

    def test_parts_land_where_they_are_asked_and_the_result_is_dxt5(self):
        gtex_bytes = self.compose([blank_part(8, 0, 8, 8, [0, 0, 8, 8])])
        self.assertEqual(asset_writers.read_gtex_header(gtex_bytes), (16, 8, asset_writers.FORMAT_DXT5))
        pixels = decode_dxt_image(gtex_bytes[HEADER_SIZE:], 16, 8, 16)
        self.assertEqual(pixels[0 * 16 + 0][3], 0)
        self.assertEqual(pixels[0 * 16 + 8][3], 255)
        self.assertEqual(pixels[0 * 16 + 8][:3], (255, 255, 255))
        self.assertEqual(pixels[0 * 16 + 0][:3], (255, 255, 255))

    def test_no_work_files_are_left_behind(self):
        self.compose([blank_part(0, 0, 8, 8, [0, 0, 8, 8])])
        self.assertEqual(sorted(os.listdir(self.folder)), ["bubble.dds", "bubble.rgba"])


if __name__ == "__main__":
    unittest.main()
