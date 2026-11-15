import json
import os
import struct
import unittest

import numpy

import chapter_assets
import ds_effect_movies
import ds_effects
import ds_textures

SRC_VITA = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
GAME_DESCRIPTION = os.path.join(SRC_VITA, "tools", "assets", "game.json")
ROM_PATH = os.path.join(os.path.dirname(SRC_VITA), "extracted", "ds_rom", "game.nds")
FIXED = 4096
RAW_HEADER = bytes([0x80]) + bytes(7)
SOUL_FLOWER_EFFECT = 31
RAY_LIGHT_EFFECTS = (84, 91)


def table(entries):
    offsets = []
    body = b""
    for entry in entries:
        offsets.append(len(body))
        body += entry
    return struct.pack("<%dI" % (len(entries) + 1), len(entries), *offsets) + body


def fixed(values):
    return struct.pack("<6i", *[int(round(value * FIXED)) for value in values])


def item(slot, character, depth, matrix, rgb, alpha_percent):
    return struct.pack("<HHhhi", slot, character, depth, -1, 1) + fixed(matrix) + bytes(rgb) + b"\0" + struct.pack("<i", alpha_percent * FIXED)


def frame(items, label=-1):
    return struct.pack("<ii", label, len(items)) + b"".join(items)


def push(values):
    return bytes([0x96, 0, len(values), 0]) + b"".join(struct.pack("<BBHi", kind, index, 0, value) for kind, index, value in values)


def call(pops):
    return bytes([0x3d, 0, 0, pops])


def do_action(frame_number, actions):
    return struct.pack("<iii", frame_number, -1, 1) + actions + bytes(4)


def sprite_with_scripts(sprite_id, frames, scripts):
    frames_table = table(frames)
    return struct.pack("<HHii", len(frames), sprite_id, len(frames_table), len(frames_table) + 4) + frames_table + struct.pack("<i", 0) + table(scripts)


def movie(shapes, sprites, names=(b"loopStart",), bitmap_ids=(1,), fps=60):
    bitmaps = table([struct.pack("<i", bitmap) for bitmap in bitmap_ids])
    shape_table = table(shapes)
    buttons = struct.pack("<i", 0)
    sprite_table = table(sprites)
    name_bytes = b""
    name_offsets = []
    for name in names:
        name_offsets.append(len(name_bytes))
        name_bytes += name + b"\0"
    name_table = struct.pack("<%dI" % (len(names) + 1), len(names), *name_offsets) + name_bytes
    shapes_at = len(bitmaps)
    buttons_at = shapes_at + len(shape_table)
    sprites_at = buttons_at + len(buttons)
    names_at = sprites_at + len(sprite_table)
    header = bytearray(0x40)
    header[0:4] = ds_effect_movies.MAGIC
    struct.pack_into("<IHH", header, 4, 1, 1, fps)
    struct.pack_into("<iiii", header, 0x30, shapes_at, buttons_at, sprites_at, names_at)
    return bytes(header) + bitmaps + shape_table + buttons + sprite_table + name_table


def shape(shape_id, rect, bitmap, fill_matrix):
    return struct.pack("<HH4i", shape_id, 1, *rect) + struct.pack("<i", bitmap) + fixed(fill_matrix)


def texture(width, height, rgb):
    palette = struct.pack("<2H", 0, (rgb[0] >> 3) | ((rgb[1] >> 3) << 5) | ((rgb[2] >> 3) << 10))
    pixels = bytes([0x11]) * (width * height // 2)
    header_size = struct.calcsize(ds_textures.HEADER)
    width_code = (width // ds_textures.SMALLEST_ROW).bit_length() - 1
    header = struct.pack(ds_textures.HEADER, ds_textures.MAGIC, 1, ds_textures.FORMAT_16_COLORS, width_code, 0, header_size, len(pixels), header_size + len(pixels), len(palette), width, height)
    return header + pixels + palette


def effect(movie_bytes, textures):
    return table([bytes(8) + movie_bytes] + [RAW_HEADER + each for each in textures])


IDENTITY = (1, 1, 0, 0, 0, 0)


class SyntheticMovieTests(unittest.TestCase):
    def test_a_shape_is_drawn_where_its_matrix_puts_it_with_its_color_and_alpha(self):
        root = sprite_with_scripts(0, [frame([item(0, 2, 1, (1, 1, 0, 0, 120, 90), (31, 0, 0), 50)])], [])
        data = effect(movie([shape(2, (0, 8, 0, 4), 1, IDENTITY)], [root]), [texture(8, 4, (255, 255, 255))])
        lines, pages = ds_effect_movies.bake_movie({"effect": "test"}, data, 1)
        self.assertTrue(lines[0].startswith("fxmovie test 1 2 0 -1 1"))
        self.assertIn("fxframe 0 1 -8 -6 0 -6 0 -2 -8 -2", lines)
        page = numpy.frombuffer(pages[0][20:], dtype=numpy.uint8).reshape(-1, ds_effect_movies.PAGE_WIDTH, 4)
        cell = page[ds_effect_movies.CELL_GUTTER, ds_effect_movies.CELL_GUTTER]
        self.assertEqual(tuple(cell), (255, 0, 0, 128))

    def test_a_quarter_turned_fill_lands_in_the_turned_quarter(self):
        turned = (0, 0, 1, -1, 8, 0)
        root = sprite_with_scripts(0, [frame([item(0, 2, 1, (1, 1, 0, 0, 128, 96), (31, 31, 31), 100)])], [])
        data = effect(movie([shape(2, (-8, 8, -8, 8), 1, turned)], [root]), [texture(8, 4, (255, 255, 255))])
        lines, _ = ds_effect_movies.bake_movie({"effect": "test"}, data, 1)
        self.assertIn("fxframe 0 1 4 0 8 0 8 8 4 8", lines)

    def test_four_set_vertices_become_the_frame_corners(self):
        actions = b"".join(push([(1, 0, y), (1, 0, x), (1, 0, corner), (1, 0, 3), (4, 5, 0x8006)]) + call(5) + bytes([0x17, 0, 0, 0]) for corner, x, y in ((0, 0, 0), (1, 5, 30), (2, 40, 20), (3, 30, -5)))
        root = sprite_with_scripts(0, [frame([item(0, 2, 1, (1, 1, 0, 0, 128, 96), (31, 31, 31), 100)])], [do_action(0, actions)])
        data = effect(movie([shape(2, (0, 8, 0, 4), 1, IDENTITY)], [root]), [texture(8, 4, (255, 255, 255))])
        lines, _ = ds_effect_movies.bake_movie({"effect": "test"}, data, 1)
        self.assertIn("fxframe 0 1 0 0 30 -5 40 20 5 30", lines)

    def test_a_held_root_plays_its_nested_sprite_until_it_removes_the_movie(self):
        nested_frames = [frame([item(0, 2, 1, (1, 1, 0, 0, offset, 0), (31, 31, 31), 100)]) for offset in range(3)]
        remove = push([(1, 0, 0), (4, 2, 0), (4, 5, 0x8005)]) + bytes([0x52, 0, 0, 3])
        nested = sprite_with_scripts(3, nested_frames, [do_action(2, remove)])
        root = sprite_with_scripts(0, [frame([item(0, 3, 1, (1, 1, 0, 0, 128, 96), (31, 31, 31), 100)])], [])
        data = effect(movie([shape(2, (0, 8, 0, 4), 1, IDENTITY)], [root, nested]), [texture(8, 4, (255, 255, 255))])
        lines, _ = ds_effect_movies.bake_movie({"effect": "test"}, data, 1)
        self.assertTrue(lines[0].startswith("fxmovie test 1 0 0 -1 1"))
        frames = [line for line in lines if line.startswith("fxframe")]
        self.assertEqual([line.split()[3] for line in frames], ["0", "1", "2"])

    def test_a_movie_that_is_not_a_movie_is_refused(self):
        with self.assertRaises(ds_effect_movies.EffectMovieRefused):
            ds_effect_movies.read_movie(b"xxxx" + bytes(0x40))


@unittest.skipUnless(os.path.isfile(ROM_PATH), "needs the cartridge")
class CartridgeMovieTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        import ds_rom
        with ds_rom.DsRom(ROM_PATH) as rom:
            cls.textures = ds_textures.TexturePack(rom.read_file(ds_textures.PACK_PATH))
            cls.arm9 = chapter_assets.read_arm9(rom)
        cls.effects = ds_effects.EffectPack(cls.textures.pack)

    def baked(self, effect_id):
        kind = chapter_assets.effect_kind_of(self.arm9, effect_id)
        return ds_effect_movies.bake_movie({"effect": "test"}, self.effects.effect_bytes(kind), 1)

    def test_every_effect_movie_on_the_cartridge_reads(self):
        for kind in sorted(self.textures.pack.entries(ds_effects.EFFECT_BLOCK)):
            effect_bytes = self.effects.effect_bytes(kind)
            start, end = ds_effects.item_offsets(effect_bytes)[ds_effects.SCRIPT_ITEM]
            movie = ds_effect_movies.read_movie(effect_bytes[start + ds_effects.ITEM_HEADER_SIZE:end])
            self.assertTrue(any(each["sprite_id"] == ds_effect_movies.ROOT_SPRITE for each in movie["sprites"]), kind)

    def test_every_sprite_effect_the_scripts_spawn_gets_a_movie_row_or_a_hand_row(self):
        with open(GAME_DESCRIPTION, encoding="utf-8") as source:
            description = json.load(source)
        _, scripts, arm9, _, _ = chapter_assets.read_chain(description, ROM_PATH)
        spawned = set()
        for script in scripts.values():
            spawned.update(chapter_assets.spawned_effect_ids(script))
        sprite_kinds = set(effect_id for effect_id in spawned if effect_id and chapter_assets.effect_kind_of(arm9, effect_id) < chapter_assets.FIRST_CLASS_EFFECT_KIND)
        rows = set(row["id"] for row in ds_effect_movies.movie_rows(description, scripts, arm9))
        hand = set(row["id"] for row in description.get("effects", []))
        self.assertEqual(sprite_kinds - hand, rows)
        flower = next(row for row in ds_effect_movies.movie_rows(description, scripts, arm9) if row["id"] == SOUL_FLOWER_EFFECT)
        self.assertEqual(flower.get("only_in_soul_view"), 1)

    def test_the_soul_flower_is_four_turned_copies_of_one_quarter(self):
        lines, pages = self.baked(SOUL_FLOWER_EFFECT)
        piece = [line for line in lines if line.startswith("fxpiece")][0].split()
        page = numpy.frombuffer(pages[int(piece[1])][20:], dtype=numpy.uint8).reshape(-1, ds_effect_movies.PAGE_WIDTH, 4)
        height, width = page.shape[:2]
        left, top, right, bottom = [int(round(float(value) * size)) for value, size in zip(piece[2:6], (width, height, width, height))]
        cell = page[top:bottom, left:right, 3] > 0
        first = [line for line in lines if line.startswith("fxframe")][0].split()
        corners = [float(value) for value in first[3:]]
        centre_column = int(round(-corners[0]))
        centre_row = int(round(-corners[1]))
        self.assertTrue(cell.any())
        rows, columns = numpy.nonzero(cell)
        turned = set(zip(columns - centre_column, rows - centre_row))
        quarter_turned = set((-(y + 1), x) for x, y in turned)
        self.assertGreater(len(turned & quarter_turned), 0.95 * len(turned))

    def test_rays_light_draws_from_cartridge_art(self):
        for effect_id in RAY_LIGHT_EFFECTS:
            lines, pages = self.baked(effect_id)
            self.assertTrue(pages, effect_id)
            self.assertTrue(any(line.startswith("fxframe") and line.split()[2] == "1" for line in lines), effect_id)


if __name__ == "__main__":
    unittest.main()
