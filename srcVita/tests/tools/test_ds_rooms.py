import struct
import unittest

import ds_rooms


def synthetic_plate(width, height, index_at):
    header = struct.pack("<HH", width, height) + bytes(ds_rooms.PALETTE_OFFSET - 4)
    palette = bytearray(ds_rooms.PALETTE_COLORS * 2)
    for index in range(ds_rooms.PALETTE_COLORS):
        struct.pack_into("<H", palette, index * 2, (index & 0x1f) | ((31 - (index & 0x1f)) << 10))
    pixels = bytearray(width * height)
    tile = 0
    for metatile_y in range(0, height, 16):
        for metatile_x in range(0, width, 16):
            for tile_row in range(2):
                for tile_column in range(2):
                    for y in range(8):
                        for x in range(8):
                            pixels[tile * 64 + y * 8 + x] = index_at(metatile_x + tile_column * 8 + x, metatile_y + tile_row * 8 + y)
                    tile = tile + 1
    return header + bytes(palette) + bytes(pixels)


def pattern(x, y):
    return 1 + (x // 8) + 4 * (y // 8)


class FakeRecord:
    def __init__(self):
        self.stored = {}

    def key_for(self, code, settings, data):
        return repr(settings)

    def read_if_current(self, path, key):
        return None

    def store(self, path, key, data, details=None):
        self.stored[path] = data


class DsRoomsTests(unittest.TestCase):
    def test_a_plate_unpacks_16_pixel_blocks_of_8_pixel_tiles_into_rows(self):
        width, height, rgba = ds_rooms.decode_plate(synthetic_plate(32, 16, pattern))
        self.assertEqual((width, height), (32, 16))
        for y in (0, 7, 8, 15):
            for x in (0, 7, 8, 15, 16, 24, 31):
                index = pattern(x, y)
                offset = (y * width + x) * 4
                self.assertEqual(rgba[offset], index * 255 // 31, (x, y))
                self.assertEqual(rgba[offset + 2], (31 - index) * 255 // 31, (x, y))
                self.assertEqual(rgba[offset + 3], 255)

    def test_colour_zero_is_transparent(self):
        _width, _height, rgba = ds_rooms.decode_plate(synthetic_plate(16, 16, lambda x, y: 0 if x < 8 else 5))
        self.assertEqual(rgba[3], 0)
        self.assertEqual(rgba[8 * 4 + 3], 255)

    def test_a_plate_whose_size_does_not_match_its_bytes_is_refused(self):
        plate = synthetic_plate(16, 16, pattern)
        with self.assertRaises(ds_rooms.PlateRefused):
            ds_rooms.decode_plate(plate[:-1])
        with self.assertRaises(ds_rooms.PlateRefused):
            ds_rooms.decode_plate(struct.pack("<HH", 12, 16) + plate[4:])

    def test_a_plate_gets_ds_art_only_when_the_ds_has_one_of_the_remasters_size(self):
        record = FakeRecord()
        plates = [{"name": "room-a", "ds_plate": 3}, {"name": "room-b", "ds_plate": 9}, {"name": "room-c", "ds_plate": 4}]
        original = ds_rooms.read_plate_entries
        ds_rooms.read_plate_entries = lambda pack_bytes: {3: synthetic_plate(16, 16, pattern), 4: synthetic_plate(16, 16, pattern)}
        try:
            paths, names, missing = ds_rooms.bake_plates(plates, b"", None, None, "unused", record, "code", {"room-a": (16.0, 16.0), "room-b": (16.0, 16.0), "room-c": (32.0, 16.0)})
        finally:
            ds_rooms.read_plate_entries = original
        self.assertEqual(paths, ["textures_ds/room-a.gtex"])
        self.assertEqual(names, ["room-a"])
        self.assertEqual(missing, ["room-b", "room-c"])
        stored = record.stored["textures_ds/room-a.gtex"]
        self.assertEqual(stored[:4], b"GTEX")
        self.assertEqual(struct.unpack_from("<III", stored, 4), (16, 16, 0))

    def test_the_names_file_lists_each_name_once_per_line_in_order(self):
        self.assertEqual(ds_rooms.names_text(["b", "a"]), "a\nb\n")


if __name__ == "__main__":
    unittest.main()
