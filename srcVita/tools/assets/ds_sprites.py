import os
import struct
import sys

ASSET_TOOLS_FOLDER = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(ASSET_TOOLS_FOLDER))), "tools"))

import cell_resources
import cpac_unpack
import lz
import nds_unpack

PACK_PATH = "cpac_2d.bin"
ARM9_OFFSET_FIELD = 0x20
ARM9_SIZE_FIELD = 0x2c
HEADER_SIZE = 0x40
PALETTE_BLOCK = 0
SPRITE_BLOCK = 3
SMALL_PALETTE_SIZE = 0x20
BOUNDARY_BYTES = 128
TILE_SIZE = 8
OAM_WORDS = 3
NITRO_BANK_MAGIC = b"RNAN"
LAYOUT_STREAMED = "streamed"
LAYOUT_NITRO = "nitro"
SHAPE_WIDTHS = ((8, 16, 32, 64), (16, 32, 32, 64), (8, 8, 16, 32))
SHAPE_HEIGHTS = ((8, 16, 32, 64), (8, 8, 16, 32), (16, 32, 32, 64))


class SpriteRefused(ValueError):
    pass


def resolve(arm9_bytes, resource_id, language=1):
    row = cell_resources.resolve_resource(arm9_bytes, resource_id, language)
    if row["status"] != "resolved":
        return None
    return {name: row[name] for name in ("palette", "graphics", "cell", "animation", "sequence")}


class SpritePack:
    def __init__(self, pack_bytes):
        self.pack_bytes = pack_bytes
        self.blocks = cpac_unpack.find_blocks(pack_bytes)
        if len(self.blocks) <= SPRITE_BLOCK:
            raise SpriteRefused("cpac_2d.bin has no sprite block")
        self.tables = {}

    def entries(self, block_number):
        if block_number not in self.tables:
            limit = self.blocks[block_number + 1][0] if block_number + 1 < len(self.blocks) else len(self.pack_bytes)
            self.tables[block_number] = {row[0]: row for row in cpac_unpack.read_entries(self.pack_bytes, self.blocks[block_number], limit)}
        return self.tables[block_number]

    def entry(self, block_number, index, language=1):
        row = self.entries(block_number).get(index)
        if row is None:
            raise SpriteRefused("cpac_2d block " + str(block_number) + " has no entry " + str(index))
        if row[4]:
            row = self.entries(block_number).get(index + language - 1)
            if row is None:
                raise SpriteRefused("cpac_2d block " + str(block_number) + " has no language " + str(language) + " entry for " + str(index))
        _index, start, size, compressed, _localized, _key_size = row
        stored = self.pack_bytes[start:start + size]
        if not compressed:
            return stored
        plain = lz.try_decompress(stored)
        if plain is None:
            raise SpriteRefused("cpac_2d block " + str(block_number) + " entry " + str(index) + " does not decompress")
        return plain


def palette_rgba(palette_bytes):
    colors = []
    for index in range(len(palette_bytes) // 2):
        value = struct.unpack_from("<H", palette_bytes, index * 2)[0]
        red = (value & 0x1f) * 255 // 31
        green = ((value >> 5) & 0x1f) * 255 // 31
        blue = ((value >> 10) & 0x1f) * 255 // 31
        colors.append(bytes((red, green, blue, 255)))
    return colors


def cell_frame_count(cell_bytes):
    if len(cell_bytes) < 2:
        raise SpriteRefused("a cell file is shorter than its frame count")
    return struct.unpack_from("<H", cell_bytes, 0)[0]


def cell_records(cell_bytes, frame):
    if frame >= cell_frame_count(cell_bytes):
        raise SpriteRefused("frame " + str(frame) + " is past the cell file's frame table")
    start = struct.unpack_from("<H", cell_bytes, frame * 2)[0] * 2
    if start + 2 > len(cell_bytes):
        raise SpriteRefused("frame " + str(frame) + "'s cell layout is outside the cell file")
    count = struct.unpack_from("<H", cell_bytes, start)[0]
    end = start + 2 + count * OAM_WORDS * 2
    if end > len(cell_bytes):
        raise SpriteRefused("frame " + str(frame) + "'s cell layout runs past the cell file")
    return [struct.unpack_from("<HHH", cell_bytes, start + 2 + i * OAM_WORDS * 2) for i in range(count)]


def oam_sprite(attr0, attr1, attr2):
    shape = attr0 >> 14
    size = attr1 >> 14
    if shape == 3:
        raise SpriteRefused("a cell uses the prohibited OAM shape 3")
    x = attr1 & 0x1ff
    if x & 0x100:
        x = x - 0x200
    y = attr0 & 0xff
    if y & 0x80:
        y = y - 0x100
    affine = (attr0 >> 8) & 1
    return {
        "x": x,
        "y": y,
        "width": SHAPE_WIDTHS[shape][size],
        "height": SHAPE_HEIGHTS[shape][size],
        "affine": affine,
        "double": affine and (attr0 >> 9) & 1,
        "hidden": not affine and (attr0 >> 9) & 1,
        "flip_x": not affine and (attr1 >> 12) & 1,
        "flip_y": not affine and (attr1 >> 13) & 1,
        "eight_bit": (attr0 >> 13) & 1,
        "tile": attr2 & 0x3ff,
        "palette": attr2 >> 12,
    }


def sprite_indices(graphics, sprite, eight_bit):
    width = sprite["width"]
    height = sprite["height"]
    tile_bytes = 64 if eight_bit else 32
    base = sprite["tile"] * BOUNDARY_BYTES
    tiles_across = width // TILE_SIZE
    if base + tiles_across * (height // TILE_SIZE) * tile_bytes > len(graphics):
        raise SpriteRefused("a sprite's tiles run past its frame graphics")
    indices = bytearray(width * height)
    for y in range(height):
        for x in range(width):
            tile = (y // TILE_SIZE) * tiles_across + x // TILE_SIZE
            pixel = (y % TILE_SIZE) * TILE_SIZE + x % TILE_SIZE
            if eight_bit:
                value = graphics[base + tile * 64 + pixel]
            else:
                byte = graphics[base + tile * 32 + pixel // 2]
                value = byte >> 4 if pixel & 1 else byte & 0xf
            source_x = width - 1 - x if sprite["flip_x"] else x
            source_y = height - 1 - y if sprite["flip_y"] else y
            indices[source_y * width + source_x] = value
    return indices


def compose_frame(records, graphics, colors, eight_bit):
    sprites = [oam_sprite(*record) for record in records]
    sprites = [sprite for sprite in sprites if not sprite["hidden"]]
    if any(sprite["affine"] for sprite in sprites):
        raise SpriteRefused("a cell uses an affine OAM entry")
    if not sprites:
        return 0, 0, b"", 0, 0
    left = min(sprite["x"] for sprite in sprites)
    top = min(sprite["y"] for sprite in sprites)
    width = max(sprite["x"] + sprite["width"] for sprite in sprites) - left
    height = max(sprite["y"] + sprite["height"] for sprite in sprites) - top
    rgba = bytearray(width * height * 4)
    for sprite in reversed(sprites):
        indices = sprite_indices(graphics, sprite, eight_bit)
        color_base = 0 if eight_bit else sprite["palette"] * 16
        for y in range(sprite["height"]):
            for x in range(sprite["width"]):
                value = indices[y * sprite["width"] + x]
                if value == 0:
                    continue
                color = color_base + value
                if color >= len(colors):
                    raise SpriteRefused("a sprite's colour is past its palette")
                offset = ((sprite["y"] - top + y) * width + sprite["x"] - left + x) * 4
                rgba[offset:offset + 4] = colors[color]
    return width, height, bytes(rgba), left, top


def bank_layout(bank_bytes):
    return LAYOUT_NITRO if bank_bytes[:4] == NITRO_BANK_MAGIC else LAYOUT_STREAMED


def streamed_chunk(graphics_file, bank_bytes, frame):
    if (frame + 1) * 8 > len(bank_bytes):
        raise SpriteRefused("frame " + str(frame) + " is past the frame table")
    offset, size = struct.unpack_from("<II", bank_bytes, frame * 8)
    if offset + size > len(graphics_file):
        raise SpriteRefused("frame " + str(frame) + "'s graphics are outside the graphics file")
    chunk = lz.try_decompress(graphics_file[offset:offset + size])
    if chunk is None:
        raise SpriteRefused("frame " + str(frame) + "'s graphics do not decompress")
    return chunk


def resource_parts(pack, resolved, language=1):
    if resolved is None or 0 in (resolved["palette"], resolved["graphics"], resolved["cell"], resolved["animation"]):
        raise SpriteRefused("the resource has no complete sprite set")
    return (pack.entry(PALETTE_BLOCK, resolved["palette"], language), pack.entry(SPRITE_BLOCK, resolved["graphics"], language),
            pack.entry(SPRITE_BLOCK, resolved["cell"], language), pack.entry(SPRITE_BLOCK, resolved["animation"], language))


def frames_of_parts(palette_bytes, graphics_file, cell_bytes, bank_bytes):
    eight_bit = len(palette_bytes) > SMALL_PALETTE_SIZE
    colors = palette_rgba(palette_bytes)
    layout = bank_layout(bank_bytes)
    frames = []
    for frame in range(cell_frame_count(cell_bytes)):
        records = cell_records(cell_bytes, frame)
        graphics = graphics_file if layout == LAYOUT_NITRO else streamed_chunk(graphics_file, bank_bytes, frame)
        frames.append(compose_frame(records, graphics, colors, eight_bit))
    return frames


def frames_of_resource(pack_bytes, arm9_bytes, resource_id, language=1):
    pack = pack_bytes if isinstance(pack_bytes, SpritePack) else SpritePack(pack_bytes)
    return frames_of_parts(*resource_parts(pack, resolve(arm9_bytes, resource_id, language), language))


def read_rom(rom):
    header = rom.read_range(0, HEADER_SIZE)
    offset = struct.unpack_from("<I", header, ARM9_OFFSET_FIELD)[0]
    size = struct.unpack_from("<I", header, ARM9_SIZE_FIELD)[0]
    return rom.read_file(PACK_PATH), nds_unpack.blz_decompress(rom.read_range(offset, size))
