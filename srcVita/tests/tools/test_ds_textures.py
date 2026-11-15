import struct
import unittest

import ds_textures

RED = 0x001f
GREEN = 0x03e0
BLUE = 0x7c00


def texture(texture_format, width_code, width, height, data, palette):
    palette_bytes = struct.pack("<%dH" % len(palette), *palette)
    header = struct.pack(ds_textures.HEADER, ds_textures.MAGIC, 1, texture_format, width_code, 0, ds_textures.HEADER_SIZE, len(data), ds_textures.HEADER_SIZE + len(data), len(palette_bytes), width, height)
    return header + data + palette_bytes


def pixel(rgba, width, x, y):
    return tuple(rgba[(y * width + x) * 4:(y * width + x) * 4 + 4])


class DecodeTests(unittest.TestCase):
    def test_sixteen_colors_read_low_nibble_first_and_color_zero_is_clear(self):
        data = bytes([0x21, 0x03, 0, 0]) + bytes(4)
        width, height, rgba = ds_textures.decode(texture(ds_textures.FORMAT_16_COLORS, 0, 8, 2, data, [0, RED, GREEN, BLUE]))
        self.assertEqual((width, height), (8, 2))
        self.assertEqual(pixel(rgba, 8, 0, 0), (255, 0, 0, 255))
        self.assertEqual(pixel(rgba, 8, 1, 0), (0, 255, 0, 255))
        self.assertEqual(pixel(rgba, 8, 2, 0), (0, 0, 255, 255))
        self.assertEqual(pixel(rgba, 8, 3, 0)[3], 0)

    def test_color_zero_stays_solid_when_the_texture_is_opaque(self):
        _, _, rgba = ds_textures.decode(texture(ds_textures.FORMAT_256_COLORS, 0, 8, 1, bytes(8), [RED]), first_color_is_clear=False)
        self.assertEqual(pixel(rgba, 8, 0, 0), (255, 0, 0, 255))

    def test_four_colors_pack_four_pixels_a_byte(self):
        _, _, rgba = ds_textures.decode(texture(ds_textures.FORMAT_4_COLORS, 0, 8, 1, bytes([0b11100100, 0]), [0, RED, GREEN, BLUE]))
        self.assertEqual([pixel(rgba, 8, x, 0)[:3] for x in range(1, 4)], [(255, 0, 0), (0, 255, 0), (0, 0, 255)])

    def test_translucent_formats_split_each_byte_into_alpha_and_color(self):
        _, _, rgba = ds_textures.decode(texture(ds_textures.FORMAT_A3I5, 0, 8, 1, bytes([(7 << 5) | 1, (3 << 5) | 2] + [0] * 6), [0, RED, GREEN]))
        self.assertEqual(pixel(rgba, 8, 0, 0), (255, 0, 0, 255))
        self.assertEqual(pixel(rgba, 8, 1, 0), (0, 255, 0, 3 * 255 // 7))
        _, _, rgba = ds_textures.decode(texture(ds_textures.FORMAT_A5I3, 0, 8, 1, bytes([(31 << 3) | 1, (16 << 3) | 2] + [0] * 6), [0, RED, GREEN]))
        self.assertEqual(pixel(rgba, 8, 0, 0), (255, 0, 0, 255))
        self.assertEqual(pixel(rgba, 8, 1, 0), (0, 255, 0, 16 * 255 // 31))

    def test_a_texture_narrower_than_its_row_keeps_only_its_own_columns(self):
        data = bytes([1] * 16 + [2] * 16)
        width, height, rgba = ds_textures.decode(texture(ds_textures.FORMAT_256_COLORS, 1, 12, 2, data, [0, RED, GREEN]))
        self.assertEqual((width, height, len(rgba)), (12, 2, 12 * 2 * 4))
        self.assertEqual(pixel(rgba, 12, 11, 1), (0, 255, 0, 255))

    def test_bad_textures_are_refused(self):
        good = texture(ds_textures.FORMAT_256_COLORS, 0, 8, 1, bytes(8), [RED])
        for broken in (good[:10], b"nope" + good[4:], good[:-1], texture(5, 0, 8, 1, bytes(8), [RED]), texture(ds_textures.FORMAT_256_COLORS, 0, 8, 2, bytes(8), [RED]), texture(ds_textures.FORMAT_256_COLORS, 0, 8, 1, bytes([4] * 8), [RED])):
            with self.assertRaises(ds_textures.TextureRefused):
                ds_textures.decode(broken)


if __name__ == "__main__":
    unittest.main()
