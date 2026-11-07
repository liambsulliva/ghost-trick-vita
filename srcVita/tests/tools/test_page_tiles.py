import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools", "assets"))

import asset_writers
import page_tiles


def numbered_dxt5_page(width, height):
    blocks = bytearray()
    for block_row in range(height // 4):
        for block_column in range(width // 4):
            blocks.extend(bytes([block_column % 256, block_row % 256]) * 8)
    return asset_writers.gtex_bytes(width, height, asset_writers.FORMAT_DXT5, bytes(blocks))


class PageTilesTest(unittest.TestCase):
    def test_a_page_that_fits_is_one_span(self):
        self.assertEqual(page_tiles.spans_along(2048), [(0, 2048, 0, 2048)])

    def test_a_wide_page_overlaps_its_tiles_and_draws_to_the_middle(self):
        self.assertEqual(page_tiles.spans_along(2840), [(0, 2048, 0, 2046), (2044, 2840, 2046, 2840)])

    def test_every_tile_fits_a_2048_slot(self):
        for length in (2052, 4096, 5000):
            for start, end, _, _ in page_tiles.spans_along(length):
                self.assertLessEqual(end - start, page_tiles.TILE_SIDE_MOST)

    def test_a_cut_keeps_the_blocks_of_its_area(self):
        tile = page_tiles.cut_gtex(numbered_dxt5_page(16, 8), 8, 4, 16, 8)
        width, height, texture_format = asset_writers.read_gtex_header(tile)
        self.assertEqual((width, height, texture_format), (8, 4, asset_writers.FORMAT_DXT5))
        self.assertEqual(tile[20:], bytes([2, 1]) * 8 + bytes([3, 1]) * 8)

    def test_the_table_names_each_tile_with_its_page(self):
        result = {"name": "big_t10", "width": 796, "height": 2048, "page": {"name": "big", "width": 2840, "height": 2600, "left": 2044, "top": 0, "draw": (2046, 0, 2840, 2046)}}
        self.assertEqual(page_tiles.table_text([result, {"name": "plain"}]), "tile big 2840 2600 big_t10 2044 0 796 2048 2046 0 2840 2046\n")


if __name__ == "__main__":
    unittest.main()
