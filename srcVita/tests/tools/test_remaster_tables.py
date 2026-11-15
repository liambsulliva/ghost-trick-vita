import struct
import unittest

import remaster_tables

FIRST_SOUND_ID = 100
SOUND_NAMES = ["SE_MUON"] + ["SE_SYS_%03d" % number for number in range(1, 21)] + ["SE_GIM_%03d" % number for number in range(1, 21)]
SHARED_SOUND = 25
EMPTY_ID = 6
OTHER_NAMES = {EMPTY_ID: "", 10: "iEffect::CHAPTER_START_01", 11: "iPlace::ROOM", 12: "BGM_TITLE", 40: "iCutin::LYNNE", 141: "SE_REGIST_DONE"}
POOL_AT = 0x200
TABLE_AT = 0x1000


class FakeLibrary:
    def __init__(self, data):
        self.data = bytes(data)

    def section(self, name):
        return 0, 0, len(self.data)


def build(shared_elsewhere=True):
    data = bytearray(0x4000)
    position = POOL_AT
    offsets = {}

    def put(text):
        nonlocal position
        start = position
        data[start:start + len(text) + 1] = text.encode() + b"\0"
        position += len(text) + 1
        return start
    shared = put(SOUND_NAMES[SHARED_SOUND]) if shared_elsewhere else None
    put("Other")
    for name_id, text in OTHER_NAMES.items():
        offsets[name_id] = put(text)
    for index, text in enumerate(SOUND_NAMES):
        start = put(text)
        offsets[FIRST_SOUND_ID + index] = shared if index == SHARED_SOUND and shared_elsewhere else start
    base = POOL_AT - 1
    for name_id, start in offsets.items():
        struct.pack_into("<I", data, TABLE_AT + 4 * name_id, start - base)
    return FakeLibrary(data), base


class InternStringTests(unittest.TestCase):
    def locate(self, library):
        sound_ids = list(range(FIRST_SOUND_ID, FIRST_SOUND_ID + len(SOUND_NAMES)))
        return remaster_tables.locate_intern_strings(library, sound_ids, sorted(OTHER_NAMES))

    def test_finds_the_one_table_despite_an_empty_string_a_shared_name_and_a_neighbouring_sound_like_name(self):
        library, _ = build()
        text_of = self.locate(library)
        self.assertEqual(text_of(FIRST_SOUND_ID), "SE_MUON")
        self.assertEqual(text_of(FIRST_SOUND_ID + SHARED_SOUND), SOUND_NAMES[SHARED_SOUND])
        self.assertEqual(text_of(EMPTY_ID), "")
        self.assertEqual(text_of(10), "iEffect::CHAPTER_START_01")

    def test_refuses_when_the_sound_names_are_not_in_the_library(self):
        library, _ = build()
        data = bytearray(library.data)
        for index in range(len(data) - 3):
            if data[index:index + 3] == b"SE_":
                data[index:index + 3] = b"XX_"
        with self.assertRaises(remaster_tables.TablesError):
            self.locate(FakeLibrary(data))


if __name__ == "__main__":
    unittest.main()
