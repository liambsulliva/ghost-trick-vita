import os
import shutil
import struct
import tempfile
import unittest
from unittest import mock

import bake_record
import ds_phone_book
import ds_set
import ds_textures

SRC_VITA = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
ROM_PATH = os.path.join(os.path.dirname(SRC_VITA), "extracted", "ds_rom", "game.nds")
PHONE_BOOK_HEADER = os.path.join(SRC_VITA, "include", "phone_book_plates.h")
RED = (255, 0, 0, 255)


def fake_arm9(rows, reveals):
    size = ds_phone_book.PLATE_TABLE_ADDRESS - ds_phone_book.ARM9_LOAD_ADDRESS + ds_phone_book.PLACE_COUNT * ds_phone_book.LEVEL_COUNT * 2
    arm9 = bytearray(size)
    for place, resources in rows.items():
        struct.pack_into("<3H", arm9, ds_phone_book.PLATE_TABLE_ADDRESS - ds_phone_book.ARM9_LOAD_ADDRESS + place * 6, *resources)
    for place in range(ds_phone_book.PLACE_COUNT):
        offset = ds_phone_book.REVEAL_TABLE_ADDRESS - ds_phone_book.ARM9_LOAD_ADDRESS + place * 2
        arm9[offset:offset + 2] = bytes(reveals.get(place, (ds_phone_book.NO_REVEAL, ds_phone_book.NO_REVEAL)))
    return bytes(arm9)


def frame(width, height, color=RED, left=-64, top=-16):
    return width, height, bytes(color) * (width * height), left, top


class TableTests(unittest.TestCase):
    def test_each_place_reads_three_level_plates_and_two_reveals(self):
        arm9 = fake_arm9({1: (0x8011, 0x8011, 0x8011), 22: (0x8014, 0x8017, 0x801a)}, {1: (20, 0xff), 22: (20, 30)})
        places = ds_phone_book.read_places(arm9)
        self.assertEqual(len(places), ds_phone_book.PLACE_COUNT)
        self.assertEqual(places[22]["resources"], (0x8014, 0x8017, 0x801a))
        self.assertEqual(places[22]["reveals"], (20, 30))
        self.assertEqual(places[1]["reveals"], (20, 0xff))
        self.assertIn(0x801b, ds_phone_book.plate_resources(places))
        self.assertIn(0x8012, ds_phone_book.plate_resources(places))
        self.assertNotIn(0x801c, ds_phone_book.plate_resources(places))

    def test_a_short_arm9_is_refused(self):
        with self.assertRaises(ds_phone_book.PhoneBookRefused):
            ds_phone_book.read_places(b"\0" * 16)

    def test_plates_pack_without_overlap_on_a_power_of_two_sheet(self):
        frames = {resource: frame(128, 32, (resource & 0xff, 0, 0, 255)) for resource in range(0x8011, 0x8011 + 20)}
        sheet, spots = ds_phone_book.pack_plates(frames)
        self.assertEqual(sheet.shape[1], ds_phone_book.SHEET_WIDTH)
        self.assertEqual(sheet.shape[0] & (sheet.shape[0] - 1), 0)
        boxes = sorted((y, x) for x, y in spots.values())
        for (y, x), (next_y, next_x) in zip(boxes, boxes[1:]):
            self.assertTrue(next_y > y or next_x >= x + 128)
        for resource, (x, y) in spots.items():
            self.assertEqual(tuple(sheet[y + 5, x + 5]), (resource & 0xff, 0, 0, 255))

    def test_the_table_lines_match_what_the_app_reads(self):
        places = [{"place": 22, "resources": (0x8014, 0x8017, 0x801a), "reveals": (20, 30)}]
        frames = {0x8014: frame(128, 32)}
        lines = ds_phone_book.table_lines(places, frames, {0x8014: (4, 6)})
        self.assertEqual(lines, ["sheet ds_phone_book", "plate 32788 4 6 128 32 -64 -16", "place 22 32788 32791 32794 20 30"])
        with open(PHONE_BOOK_HEADER, encoding="utf-8") as header:
            text = header.read()
        self.assertIn("#define PHONE_BOOK_PLATES_PLACE_MOST %d" % ds_phone_book.PLACE_COUNT, text)
        self.assertIn("#define PHONE_BOOK_PLATES_LEVELS %d" % ds_phone_book.LEVEL_COUNT, text)


class SetWiringTests(unittest.TestCase):
    def test_the_text_and_menu_pass_of_every_language_bakes_the_phone_book_in_that_language(self):
        cartridge = mock.Mock()
        baked = {"talk_files": {}, "texture_paths": [], "texture_sizes": {}, "narration_lines": [], "talk_textures": {}}
        for code, language in sorted(ds_textures.LANGUAGE_NUMBERS.items()):
            with mock.patch.object(ds_set.ds_set_text, "bake_text", return_value=dict(baked)), \
                    mock.patch.object(ds_set.ds_ui, "bake", return_value=([], {})), \
                    mock.patch.object(ds_set.ds_balloons, "bake", return_value=([], [], {})), \
                    mock.patch.object(ds_set.ds_phone_book, "bake", return_value=({ds_phone_book.TABLE_PATH: b"t"}, ["textures/ds_phone_book.gtex"], {ds_phone_book.SHEET_NAME: (1024, 512)})) as phone_book:
                art = ds_set.bake_text_and_menus({"texture_paths": [], "texture_sizes": {}, "animation_texts": {}}, {"scripts": {"language": code}}, {}, b"arm9", cartridge, "rom", "record", "work")
            phone_book.assert_called_once_with(b"arm9", cartridge.sprites, language, "record")
            self.assertEqual(art["files"][ds_phone_book.TABLE_PATH], b"t")
            self.assertIn("textures/ds_phone_book.gtex", art["texture_paths"])
            self.assertEqual(art["texture_sizes"][ds_phone_book.SHEET_NAME], (1024, 512))


@unittest.skipUnless(os.path.isfile(ROM_PATH), "needs the cartridge")
class CartridgePhoneBookTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        import ds_rom
        import ds_sprites
        with ds_rom.DsRom(ROM_PATH) as rom:
            pack, cls.arm9 = ds_sprites.read_rom(rom)
        cls.sprites = ds_sprites.SpritePack(pack)

    def setUp(self):
        self.folder = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, self.folder)

    def test_every_place_at_every_level_has_a_resting_and_a_selected_plate_in_every_cartridge_language(self):
        places = ds_phone_book.read_places(self.arm9)
        for code, language in sorted(ds_textures.LANGUAGE_NUMBERS.items()):
            record = bake_record.BakeRecord(os.path.join(self.folder, code))
            files, paths, sizes = ds_phone_book.bake(self.arm9, self.sprites, language, record)
            self.assertEqual(paths, ["textures/ds_phone_book.gtex"])
            width, height = sizes[ds_phone_book.SHEET_NAME]
            plates = {}
            listed = {}
            for line in files[ds_phone_book.TABLE_PATH].decode("ascii").splitlines():
                words = line.split()
                if words[0] == "plate":
                    resource, x, y, plate_width, plate_height = (int(word) for word in words[1:6])
                    self.assertLessEqual((x + plate_width, y + plate_height), (width, height))
                    plates[resource] = (plate_width, plate_height)
                elif words[0] == "place":
                    listed[int(words[1])] = [int(word) for word in words[2:5]]
            self.assertEqual(sorted(listed), list(range(ds_phone_book.PLACE_COUNT)))
            for row in places:
                self.assertEqual(listed[row["place"]], list(row["resources"]))
                for resource in row["resources"]:
                    for state in ds_phone_book.STATES:
                        self.assertIn(resource + state, plates, "%s place %d plate %#x" % (code, row["place"], resource + state))

    def test_the_junkyard_plate_differs_between_languages(self):
        junkyard = ds_phone_book.read_places(self.arm9)[1]["resources"][0]
        drawn = {code: ds_phone_book.plate_frame(ds_phone_book.plate_parts(self.sprites, self.arm9, junkyard, language))[2] for code, language in ds_textures.LANGUAGE_NUMBERS.items()}
        self.assertEqual(len(set(drawn.values())), len(drawn))


if __name__ == "__main__":
    unittest.main()
