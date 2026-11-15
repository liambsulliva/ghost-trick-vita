import json
import os
import struct
import unittest

import numpy

import chapter_assets
import class_movies
import ds_movies
import ds_rom

SRC_VITA = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
GAME_DESCRIPTION = os.path.join(SRC_VITA, "tools", "assets", "game.json")
ROM_PATH = os.path.join(os.path.dirname(SRC_VITA), "extracted", "ds_rom", "game.nds")
NAMED_GAPS = {}


class FakeScript:
    def __init__(self, path, class_ids):
        self.path = path
        self.operations = [(0, "op", class_movies.MOVIE_OP, [class_id & 0xFFFF, class_id >> 16]) for class_id in class_ids]


def thumb_call(source, target):
    offset = (target - (source + 4)) >> 1
    return struct.pack("<HH", 0xF000 | ((offset >> 11) & 0x7FF), 0xF800 | (offset & 0x7FF))


def literal_load(register, source, slot):
    return struct.pack("<H", 0x4800 | (register << 8) | ((slot - ((source + 4) & ~3)) // 4))


class ConversionTest(unittest.TestCase):
    def test_ycgco_grey_stays_grey_and_green_chroma_turns_green(self):
        planes = numpy.array([100] * 4 + [128, 128], dtype=numpy.uint8)
        self.assertEqual(ds_movies.ycgco_to_rgba(planes, 2, 2)[:4], bytes([100, 100, 100, 255]))
        planes = numpy.array([100] * 4 + [168, 128], dtype=numpy.uint8)
        self.assertEqual(ds_movies.ycgco_to_rgba(planes, 2, 2)[:4], bytes([60, 140, 60, 255]))

    def test_the_header_gives_the_frame_rate_and_the_audio_track(self):
        header = struct.pack(ds_movies.MODS_HEADER, b"MODS", b"N3\n\0", 60, 256, 192, 0x0EFC2ED9, 0, 0)
        read = ds_movies.read_header(header)
        self.assertEqual((read["frames"], read["width"], read["height"], round(read["fps"])), (60, 256, 192, 15))
        self.assertEqual(read["audio_channels"], 0)

    def test_a_file_that_is_not_mobiclip_is_refused(self):
        with self.assertRaises(ds_movies.DsMovieRefused):
            ds_movies.read_header(b"RIFF" + bytes(40))


class NativeWalkTest(unittest.TestCase):
    def test_a_class_name_leads_through_its_registration_factory_and_vtable_to_its_file(self):
        base = 0x02000000
        code = bytearray(0x400)
        name_at, file_at, pool, register, methods, factory, constructor, vtable, start = 0x300, 0x340, 0x40, 0x80, 0x100, 0x120, 0x160, 0x200, 0x1A0
        code[name_at - 1:name_at + 16] = b"\0pGameMainMovieX\0"
        code[file_at:file_at + 11] = b"movie.mods\0"
        struct.pack_into("<I", code, pool, base + name_at)
        code[0:2] = literal_load(1, base, base + pool)
        code[2:6] = thumb_call(base + 2, base + register)
        code[register:register + 2] = literal_load(0, base + register, base + register + 8)
        struct.pack_into("<I", code, register + 8, base + methods)
        struct.pack_into("<I", code, methods, base + factory + 1)
        code[factory:factory + 4] = thumb_call(base + factory, base + 0x380)
        code[factory + 4:factory + 8] = thumb_call(base + factory + 4, base + constructor)
        code[constructor:constructor + 2] = literal_load(0, base + constructor, base + constructor + 4)
        struct.pack_into("<I", code, constructor + 4, base + vtable)
        struct.pack_into("<I", code, vtable + ds_movies.START_SLOT * 4, base + start + 1)
        code[start:start + 2] = literal_load(1, base + start, base + start + 4)
        struct.pack_into("<I", code, start + 4, base + file_at)
        memory = ds_movies.Memory([(base, bytes(code))])
        self.assertEqual(ds_movies.native_movie_file(memory, "pGameMainMovieX"), "movie.mods")
        self.assertIsNone(ds_movies.native_movie_file(memory, "pGameMainMovieY"))


class RequestedMovieTest(unittest.TestCase):
    def test_a_class_the_scripts_start_with_no_baked_frames_is_named(self):
        movies = class_movies.add_to_set({"movies": [{"name": "meteor", "class": "pGameMainMovieMeteor", "frames": 10}, {"name": "torpedo", "class": "pGameMainMovieTorpedo", "frames": 0}]})["movies"]
        scripts = {path: FakeScript(path, class_ids) for path, class_ids in (("st13_game020", [0x14F605FB]), ("st14_game021", [0x2E50609F]))}
        self.assertEqual(ds_movies.classes_with_no_baked_movie(movies, scripts), {0x2E50609F: "st14_game021"})


@unittest.skipUnless(os.path.isfile(ROM_PATH), "needs the cartridge")
class CartridgeMovieTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with open(GAME_DESCRIPTION) as source:
            cls.description = json.load(source)
        with ds_rom.DsRom(ROM_PATH) as rom:
            cls.memory = ds_movies.cartridge_memory(chapter_assets.read_arm9(rom), rom)
            cls.plan = ds_movies.movie_plan(cls.description["movies"], cls.memory)
            cls.headers = {}
            for file_name, _ in cls.plan.values():
                if file_name is not None:
                    cls.headers[file_name] = ds_movies.read_header(rom.read_file(file_name))

    def test_every_movie_row_finds_its_cartridge_file_through_native_code_or_is_a_named_gap(self):
        unmapped = {name: reason for name, (file_name, reason) in self.plan.items() if file_name is None}
        self.assertEqual(set(unmapped), set(NAMED_GAPS), unmapped)

    def test_every_class_the_ds_scripts_start_has_a_movie_row_with_a_cartridge_file(self):
        _, scripts, _, _, _ = chapter_assets.read_chain(self.description, ROM_PATH)
        movies = class_movies.add_to_set({"movies": [dict(row, frames=1 if self.plan[row["name"]][0] else 0) for row in self.description["movies"]]})["movies"]
        missing = ds_movies.classes_with_no_baked_movie(movies, scripts)
        self.assertEqual(missing, {}, ", ".join("%08x (%s)" % item for item in missing.items()))

    def test_the_native_class_table_matches_the_cartridge_start_functions(self):
        self.assertEqual(self.plan["time_jump"][0], "timejump.mods")
        self.assertEqual(self.plan["long_jump"][0], "timejump.mods")
        self.assertEqual(self.plan["final_jump"][0], "longjump.mods")
        self.assertEqual(self.plan["tell_move"][0], "telmove.mods")

    def test_cartridge_movies_carry_no_audio_track_so_their_sound_is_the_rows_sound(self):
        for file_name, header in self.headers.items():
            self.assertEqual(header["audio_channels"], 0, file_name)
            self.assertEqual(round(header["fps"]), 15, file_name)


if __name__ == "__main__":
    unittest.main()
