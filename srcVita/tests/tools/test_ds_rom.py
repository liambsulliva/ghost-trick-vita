import os
import struct
import tempfile
import unittest

import ds_rom
import synthetic_files

FILES = {"top.txt": b"top bytes", "area/inner.txt": b"inner bytes", "area/also.txt": b"also bytes", "area/room/deep.txt": b"deep bytes", "area/zlast.txt": b"after the room", "other/inner.txt": b"other inner bytes"}


class DsRomTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.path = os.path.join(temporary.name, "game.nds")

    def write_rom(self, data):
        with open(self.path, "wb") as output:
            output.write(data)

    def test_files_are_found_at_the_root_and_in_nested_folders(self):
        self.write_rom(synthetic_files.mini_rom_bytes(FILES))
        with ds_rom.DsRom(self.path) as rom:
            for path, data in FILES.items():
                self.assertEqual(rom.read_file(path), data, path)

    def test_wrong_game_code_refused_with_plain_message(self):
        self.write_rom(synthetic_files.mini_rom_bytes(FILES, b"XXXX"))
        with self.assertRaisesRegex(ds_rom.RomRefused, "USA") as caught:
            ds_rom.DsRom(self.path)
        self.assertNotIn("XXXX", str(caught.exception))

    def test_a_file_too_short_for_a_header_is_refused(self):
        self.write_rom(b"short")
        with self.assertRaisesRegex(ds_rom.RomRefused, "too short"):
            ds_rom.DsRom(self.path)

    def test_a_name_the_rom_does_not_hold_is_refused_by_name(self):
        self.write_rom(synthetic_files.mini_rom_bytes(FILES))
        with ds_rom.DsRom(self.path) as rom:
            for path in ("absent.txt", "absent/inner.txt", "area/absent.txt"):
                with self.assertRaisesRegex(ds_rom.RomRefused, path):
                    rom.read_file(path)

    def test_a_folder_is_not_a_file_and_a_file_is_not_a_folder(self):
        self.write_rom(synthetic_files.mini_rom_bytes(FILES))
        with ds_rom.DsRom(self.path) as rom:
            for path in ("area", "top.txt/inner.txt"):
                with self.assertRaisesRegex(ds_rom.RomRefused, "no file named"):
                    rom.read_file(path)

    def test_a_file_entry_past_the_end_of_the_rom_is_refused(self):
        rom_bytes = bytearray(synthetic_files.mini_rom_bytes(FILES))
        allocation_offset = struct.unpack_from("<I", rom_bytes, synthetic_files.ROM_TABLES_OFFSET + 8)[0]
        struct.pack_into("<I", rom_bytes, allocation_offset + 4, len(rom_bytes) + 1)
        self.write_rom(bytes(rom_bytes))
        with ds_rom.DsRom(self.path) as rom:
            with self.assertRaisesRegex(ds_rom.RomRefused, "outside the file"):
                rom.read_file("top.txt")

    def test_a_file_entry_that_ends_before_it_starts_is_refused(self):
        rom_bytes = bytearray(synthetic_files.mini_rom_bytes(FILES))
        allocation_offset = struct.unpack_from("<I", rom_bytes, synthetic_files.ROM_TABLES_OFFSET + 8)[0]
        start = struct.unpack_from("<I", rom_bytes, allocation_offset)[0]
        struct.pack_into("<I", rom_bytes, allocation_offset + 4, start - 1)
        self.write_rom(bytes(rom_bytes))
        with ds_rom.DsRom(self.path) as rom:
            with self.assertRaisesRegex(ds_rom.RomRefused, "outside the file"):
                rom.read_file("top.txt")

    def test_a_table_past_the_end_of_the_rom_is_refused(self):
        rom_bytes = bytearray(synthetic_files.mini_rom_bytes(FILES))
        struct.pack_into("<I", rom_bytes, synthetic_files.ROM_TABLES_OFFSET + 4, len(rom_bytes) + 1)
        self.write_rom(bytes(rom_bytes))
        with self.assertRaisesRegex(ds_rom.RomRefused, "outside the file"):
            ds_rom.DsRom(self.path)

    def test_a_file_table_that_stops_short_is_refused(self):
        rom_bytes = bytearray(synthetic_files.mini_rom_bytes(FILES))
        struct.pack_into("<I", rom_bytes, synthetic_files.ROM_TABLES_OFFSET + 12, 0)
        self.write_rom(bytes(rom_bytes))
        with ds_rom.DsRom(self.path) as rom:
            with self.assertRaisesRegex(ds_rom.RomRefused, "does not cover"):
                rom.read_file("top.txt")

    def test_a_damaged_name_table_is_refused(self):
        rom_bytes = bytearray(synthetic_files.mini_rom_bytes(FILES))
        names_offset, _ = struct.unpack_from("<II", rom_bytes, synthetic_files.ROM_TABLES_OFFSET)
        struct.pack_into("<I", rom_bytes, names_offset, 0x7FFFFF)
        self.write_rom(bytes(rom_bytes))
        with ds_rom.DsRom(self.path) as rom:
            with self.assertRaisesRegex(ds_rom.RomRefused, "damaged"):
                rom.read_file("top.txt")


if __name__ == "__main__":
    unittest.main()
