import struct
import unittest

import numpy as np

import ds_people

OVERLAY_ADDRESS = 0x020d0000
OVERLAY_SIZE = 0x10000


def put_text(overlay, address, text):
    overlay[address - OVERLAY_ADDRESS:address - OVERLAY_ADDRESS + len(text) + 1] = text.encode("ascii") + b"\0"


def put_words(overlay, address, kind, values):
    struct.pack_into("<%d%s" % (len(values), kind), overlay, address - OVERLAY_ADDRESS, *values)


def fake_overlay():
    overlay = bytearray(OVERLAY_SIZE)
    free = OVERLAY_ADDRESS + 0x100
    person_names = []
    for index in range(ds_people.PERSON_COUNT):
        put_text(overlay, free, "person%02d" % index)
        person_names.append(free)
        free += 16
    place_names = []
    for index in range(ds_people.PLACE_NAME_COUNT):
        put_text(overlay, free, "place%02d" % index)
        place_names.append(free)
        free += 16
    put_words(overlay, ds_people.PERSON_NAMES_ADDRESS, "I", person_names)
    put_words(overlay, ds_people.PLACE_NAMES_ADDRESS, "I", place_names)
    for index in range(ds_people.PERSON_COUNT):
        struct.pack_into("<h", overlay, ds_people.PORTRAIT_TABLE_ADDRESS + index * ds_people.PORTRAIT_ROW_SIZE - OVERLAY_ADDRESS, 200 + index)
    put_words(overlay, ds_people.PLACE_PICTURE_TABLE_ADDRESS, "H", [0x1b4e + index for index in range(ds_people.PLACE_PICTURE_COUNT)])
    person_plates, place_plates, person_levels, place_levels, level_words = free, free + 0x200, free + 0x400, free + 0x600, free + 0x800
    put_words(overlay, ds_people.PLATE_TABLES_ADDRESS, "I", [person_plates, place_plates])
    put_words(overlay, ds_people.LEVEL_TABLES_ADDRESS, "I", [person_levels, place_levels])
    put_words(overlay, person_plates, "I", [0x7a + index for index in range(ds_people.PERSON_COUNT)])
    put_words(overlay, place_plates, "I", [0x9d + index for index in range(ds_people.PLACE_PICTURE_COUNT)])
    put_words(overlay, level_words, "H", [10, 20, 150])
    struct.pack_into("<iI", overlay, person_levels + 2 * 8 - OVERLAY_ADDRESS, 3, level_words)
    struct.pack_into("<iI", overlay, place_levels - OVERLAY_ADDRESS, 1, level_words)
    return bytes(overlay)


class TableTests(unittest.TestCase):
    def test_the_tables_are_read_from_the_overlay_by_address(self):
        tables = ds_people.read_tables(ds_people.Memory(bytes(16), OVERLAY_ADDRESS, fake_overlay()))
        self.assertEqual(tables["person_names"][2], "person02")
        self.assertEqual(tables["place_names"][22], "place22")
        self.assertEqual(tables["portraits"][44], 244)
        self.assertEqual(tables["place_pictures"][24], 0x1b4e + 24)
        self.assertEqual(tables["person_plates"][1], 0x7b)
        self.assertEqual(tables["person_levels"][2], [10, 20, 150])
        self.assertEqual(tables["person_levels"][0], [])
        self.assertEqual(tables["place_levels"][0], [10])

    def test_an_address_outside_the_program_is_refused(self):
        with self.assertRaises(ds_people.PeopleRefused):
            ds_people.Memory(bytes(16), OVERLAY_ADDRESS, bytes(16)).read(0x02001000, 4)

    def test_ids_follow_the_set_description_order(self):
        self.assertEqual(ds_people.ids_by_name(["b", "a"], ["a", "b", "c"]), {"a": 2, "b": 1})


class PictureTests(unittest.TestCase):
    def test_pasting_blends_by_alpha_and_clips_at_the_edges(self):
        canvas = ds_people.black_canvas((4, 4))
        image = np.zeros((2, 2, 4), dtype=np.uint8)
        image[...] = (255, 0, 0, 255)
        image[0, 0] = (255, 0, 0, 0)
        ds_people.pasted(canvas, image, 3, 3)
        ds_people.pasted(canvas, image, -1, -1)
        self.assertEqual(tuple(canvas[3, 3]), (0, 0, 0, 255))
        self.assertEqual(tuple(canvas[0, 0]), (255, 0, 0, 255))
        self.assertEqual(tuple(canvas[1, 1]), (0, 0, 0, 255))

    def test_a_stretch_repeats_rows_and_columns(self):
        image = np.arange(2 * 2 * 4, dtype=np.uint8).reshape(2, 2, 4)
        wide = ds_people.stretched(image, 4, 6)
        self.assertEqual(wide.shape, (6, 4, 4))
        self.assertEqual(tuple(wide[5, 3]), tuple(image[1, 1]))


if __name__ == "__main__":
    unittest.main()
