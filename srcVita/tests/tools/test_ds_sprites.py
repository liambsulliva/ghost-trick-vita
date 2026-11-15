import struct
import unittest

import ds_sprites
import lz


def lz11_literal(data):
    out = bytearray(struct.pack("<I", 0x11 | (len(data) << 8)))
    for start in range(0, len(data), 8):
        out.append(0)
        out += data[start:start + 8]
    return bytes(out)


def palette(count):
    colors = bytearray(count * 2)
    for index in range(count):
        struct.pack_into("<H", colors, index * 2, (index & 0x1f) | (((index >> 5) & 0x1f) << 5) | 0x7c00)
    return bytes(colors)


def red_of(index):
    return (index & 0x1f) * 255 // 31


def cell_file(frames):
    offsets = []
    body = bytearray()
    for records in frames:
        offsets.append(len(frames) + len(body) // 2)
        body += struct.pack("<H", len(records))
        for record in records:
            body += struct.pack("<HHH", *record)
    return struct.pack("<" + "H" * len(frames), *offsets) + bytes(body)


def attrs(x, y, shape, size, tile, flip_x=0, flip_y=0, palette_bank=0, eight_bit=0):
    attr0 = (y & 0xff) | (eight_bit << 13) | (shape << 14)
    attr1 = (x & 0x1ff) | (flip_x << 12) | (flip_y << 13) | (size << 14)
    return attr0, attr1, tile | (palette_bank << 12)


def four_bit_tiles(index_at, width, height):
    out = bytearray()
    for tile_y in range(height // 8):
        for tile_x in range(width // 8):
            for y in range(8):
                for x in range(0, 8, 2):
                    low = index_at(tile_x * 8 + x, tile_y * 8 + y)
                    high = index_at(tile_x * 8 + x + 1, tile_y * 8 + y)
                    out.append(low | (high << 4))
    return bytes(out)


def eight_bit_tiles(index_at, width, height):
    out = bytearray()
    for tile_y in range(height // 8):
        for tile_x in range(width // 8):
            for y in range(8):
                for x in range(8):
                    out.append(index_at(tile_x * 8 + x, tile_y * 8 + y))
    return bytes(out)


def pixel(frame, x, y):
    width, _height, rgba, left, top = frame
    offset = ((y - top) * width + x - left) * 4
    return rgba[offset:offset + 4]


def gradient(x, y):
    return 1 + (x + 2 * y) % 15


class DsSpritesTests(unittest.TestCase):
    def test_shape_and_size_follow_the_native_size_tables(self):
        self.assertEqual(ds_sprites.SHAPE_WIDTHS, ((8, 16, 32, 64), (16, 32, 32, 64), (8, 8, 16, 32)))
        self.assertEqual(ds_sprites.SHAPE_HEIGHTS, ((8, 16, 32, 64), (8, 8, 16, 32), (16, 32, 32, 64)))
        sprite = ds_sprites.oam_sprite(*attrs(-160, -72, 1, 1, 0))
        self.assertEqual((sprite["x"], sprite["y"], sprite["width"], sprite["height"]), (-160, -72, 32, 8))

    def test_a_streamed_frame_decompresses_its_own_chunk_and_places_tiles_in_128_byte_units(self):
        first = four_bit_tiles(lambda x, y: 3, 16, 8) + bytes(64)
        second = four_bit_tiles(gradient, 16, 16) + four_bit_tiles(lambda x, y: 9, 8, 8)
        chunks = [lz11_literal(first), lz11_literal(second)]
        graphics = chunks[0] + chunks[1]
        bank = struct.pack("<IIII", 0, len(chunks[0]), len(chunks[0]), len(chunks[1]))
        cell = cell_file([[attrs(-8, -4, 1, 0, 0)], [attrs(-16, -16, 0, 1, 0), attrs(0, -8, 0, 0, 1)]])
        frames = ds_sprites.frames_of_parts(palette(16), graphics, cell, bank)
        self.assertEqual([frame[:2] + frame[3:] for frame in frames], [(16, 8, -8, -4), (24, 16, -16, -16)])
        self.assertEqual(pixel(frames[0], -8, -4)[0], red_of(3))
        for x, y in ((0, 0), (5, 3), (15, 15)):
            self.assertEqual(pixel(frames[1], x - 16, y - 16)[0], red_of(gradient(x, y)))
        self.assertEqual(pixel(frames[1], 0, -8)[0], red_of(9))
        self.assertEqual(pixel(frames[1], 0, -16)[3], 0)

    def test_colour_zero_is_clear_and_earlier_oam_entries_draw_on_top(self):
        graphics = four_bit_tiles(lambda x, y: 0 if x < 4 else 5, 8, 8) + bytes(96) + four_bit_tiles(lambda x, y: 7, 8, 8)
        bank = b"RNAN" + bytes(12)
        cell = cell_file([[attrs(0, 0, 0, 0, 0), attrs(0, 0, 0, 0, 1)]])
        frame = ds_sprites.frames_of_parts(palette(16), graphics, cell, bank)[0]
        self.assertEqual(pixel(frame, 1, 1)[0], red_of(7))
        self.assertEqual(pixel(frame, 6, 1)[0], red_of(5))
        lone = ds_sprites.frames_of_parts(palette(16), graphics, cell_file([[attrs(0, 0, 0, 0, 0)]]), bank)[0]
        self.assertEqual(pixel(lone, 1, 1)[3], 0)

    def test_flips_mirror_each_sprite_in_place(self):
        graphics = four_bit_tiles(lambda x, y: 1 + x // 2, 16, 8)
        bank = b"RNAN" + bytes(12)
        frame = ds_sprites.frames_of_parts(palette(16), graphics, cell_file([[attrs(4, 2, 1, 0, 0, flip_x=1, flip_y=1)]]), bank)[0]
        self.assertEqual(frame[3:], (4, 2))
        self.assertEqual(pixel(frame, 4, 2)[0], red_of(8))
        self.assertEqual(pixel(frame, 19, 9)[0], red_of(1))

    def test_a_palette_larger_than_32_bytes_selects_8_bit_tiles(self):
        graphics = eight_bit_tiles(lambda x, y: 20 + x + y, 8, 8)
        frame = ds_sprites.frames_of_parts(palette(256), graphics, cell_file([[attrs(0, 0, 0, 0, 0)]]), b"RNAN" + bytes(12))[0]
        self.assertEqual(pixel(frame, 3, 4)[0], red_of(27))

    def test_a_4_bit_sprite_naming_a_bank_its_one_bank_palette_lacks_is_refused(self):
        graphics = four_bit_tiles(lambda x, y: 2, 8, 8)
        with self.assertRaises(ds_sprites.SpriteRefused):
            ds_sprites.frames_of_parts(palette(16), graphics, cell_file([[attrs(0, 0, 0, 0, 0, palette_bank=1)]]), b"RNAN" + bytes(12))

    def test_a_frame_without_sprites_is_empty(self):
        frame = ds_sprites.frames_of_parts(palette(16), b"", cell_file([[]]), b"RNAN" + bytes(12))[0]
        self.assertEqual(frame, (0, 0, b"", 0, 0))

    def test_tiles_past_the_graphics_are_refused(self):
        with self.assertRaises(ds_sprites.SpriteRefused):
            ds_sprites.frames_of_parts(palette(16), bytes(32), cell_file([[attrs(0, 0, 0, 1, 0)]]), b"RNAN" + bytes(12))

    def test_a_frame_table_entry_outside_the_graphics_is_refused(self):
        with self.assertRaises(ds_sprites.SpriteRefused):
            ds_sprites.frames_of_parts(palette(16), bytes(8), cell_file([[attrs(0, 0, 0, 0, 0)]]), struct.pack("<II", 0, 64))

    def test_the_literal_helper_round_trips_through_the_shared_decoder(self):
        data = bytes(range(40))
        self.assertEqual(lz.try_decompress(lz11_literal(data)), data)


if __name__ == "__main__":
    unittest.main()
