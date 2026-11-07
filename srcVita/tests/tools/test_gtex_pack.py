import os
import struct
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools", "assets"))

import asset_writers
import gtex_pack


def texture(width, height, pixels, texture_format=asset_writers.FORMAT_DXT1):
    return asset_writers.gtex_bytes(width, height, texture_format, pixels)


class PackTests(unittest.TestCase):
    def test_repeated_blocks_shrink_and_round_trip(self):
        pixels = bytes(8) * 30 + bytes(range(8)) * 2
        original = texture(32, 16, pixels)
        packed = gtex_pack.pack_gtex(original)
        self.assertLess(len(packed), len(original))
        self.assertEqual(struct.unpack_from("<I", packed, gtex_pack.GTEX_FORMAT_OFFSET)[0], asset_writers.FORMAT_DXT1 | gtex_pack.PACKED_FLAG)
        self.assertEqual(gtex_pack.unpack_gtex(packed), original)

    def test_dxt5_units_are_sixteen_bytes(self):
        pixels = bytes(range(16)) * 16
        original = texture(16, 16, pixels, asset_writers.FORMAT_DXT5)
        self.assertEqual(gtex_pack.unpack_gtex(gtex_pack.pack_gtex(original)), original)

    def test_data_that_cannot_shrink_stays_as_it_was(self):
        pixels = b"".join(struct.pack("<II", index, index * 7) for index in range(32))
        original = texture(32, 16, pixels)
        self.assertEqual(gtex_pack.pack_gtex(original), original)

    def test_unpacking_a_plain_texture_changes_nothing(self):
        original = texture(4, 4, bytes(8))
        self.assertEqual(gtex_pack.unpack_gtex(original), original)


if __name__ == "__main__":
    unittest.main()
