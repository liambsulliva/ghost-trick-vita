import os
import struct
import sys

ASSET_TOOLS_FOLDER = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(ASSET_TOOLS_FOLDER))), "tools"))

import asset_writers
import cpac_unpack
import lz
import texture_encoder

PACK_PATH = "cpac_2d.bin"
DS_TEXTURE_FOLDER = "textures"
PLATE_BLOCK = 4
PALETTE_OFFSET = 512
PALETTE_COLORS = 256
PIXELS_OFFSET = 1024
TILE_SIZE = 8
METATILE_SIZE = 16
TRANSPARENT_INDEX = 0


class PlateRefused(ValueError):
    pass


def read_plate_entries(pack_bytes):
    blocks = cpac_unpack.find_blocks(pack_bytes)
    if len(blocks) <= PLATE_BLOCK:
        raise PlateRefused("cpac_2d.bin has no room plate block")
    entries = {}
    for index, start, size, compressed, _localized, _key_size in cpac_unpack.read_entries(pack_bytes, blocks[PLATE_BLOCK], len(pack_bytes)):
        stored = pack_bytes[start:start + size]
        entries[index] = lz.try_decompress(stored) if compressed else stored
    return entries


def plate_size(plate_bytes):
    if len(plate_bytes) < PIXELS_OFFSET:
        raise PlateRefused("a room plate is shorter than its header")
    width, height = struct.unpack_from("<HH", plate_bytes, 0)
    if width == 0 or height == 0 or width % METATILE_SIZE or height % METATILE_SIZE or len(plate_bytes) != PIXELS_OFFSET + width * height:
        raise PlateRefused("a room plate's size does not match its bytes")
    return width, height


def palette_rgba(plate_bytes):
    colors = []
    for index in range(PALETTE_COLORS):
        value = struct.unpack_from("<H", plate_bytes, PALETTE_OFFSET + index * 2)[0]
        red = (value & 0x1f) * 255 // 31
        green = ((value >> 5) & 0x1f) * 255 // 31
        blue = ((value >> 10) & 0x1f) * 255 // 31
        alpha = 0 if index == TRANSPARENT_INDEX else 255
        colors.append(bytes((red, green, blue, alpha)))
    return colors


def decode_plate(plate_bytes):
    width, height = plate_size(plate_bytes)
    colors = palette_rgba(plate_bytes)
    pixels = plate_bytes[PIXELS_OFFSET:]
    rgba = bytearray(width * height * 4)
    tiles_per_metatile = METATILE_SIZE // TILE_SIZE
    tile = 0
    for metatile_y in range(0, height, METATILE_SIZE):
        for metatile_x in range(0, width, METATILE_SIZE):
            for tile_row in range(tiles_per_metatile):
                for tile_column in range(tiles_per_metatile):
                    source = tile * TILE_SIZE * TILE_SIZE
                    left = metatile_x + tile_column * TILE_SIZE
                    top = metatile_y + tile_row * TILE_SIZE
                    for y in range(TILE_SIZE):
                        row_start = ((top + y) * width + left) * 4
                        row = pixels[source + y * TILE_SIZE:source + (y + 1) * TILE_SIZE]
                        rgba[row_start:row_start + TILE_SIZE * 4] = b"".join(colors[value] for value in row)
                    tile = tile + 1
    return width, height, bytes(rgba)


def variants_of(plate, soul_ramp, ghost_ramps):
    variants = [(plate["name"], None)]
    if "soul_plate" in plate:
        variants.append((plate["soul_plate"], soul_ramp))
    for ghost, name in plate.get("ghost_plates", {}).items():
        variants.append((name, {key: value for key, value in (ghost_ramps or {})[ghost].items() if key != "commands"}))
    return variants


def ramped(work_folder, name, width, height, rgba, ramp):
    path = os.path.join(work_folder, name + ".ds.rgba")
    with open(path, "wb") as output:
        output.write(rgba)
    texture_encoder.apply_soul_ramp(path, width, height, ramp)
    with open(path, "rb") as source:
        result = source.read()
    os.remove(path)
    return result


def bake_variant(work_folder, record, code, plate_bytes, name, ramp, folder=DS_TEXTURE_FOLDER):
    relative_path = folder + "/" + name + ".gtex"
    key = record.key_for(code, {"soul_ramp": ramp}, plate_bytes)
    if record.read_if_current(relative_path, key) is not None:
        return relative_path
    width, height, rgba = decode_plate(plate_bytes)
    if ramp is not None:
        rgba = ramped(work_folder, name, width, height, rgba, ramp)
    record.store(relative_path, key, asset_writers.gtex_bytes(width, height, asset_writers.FORMAT_RGBA8888, rgba))
    return relative_path


def bake_plates(plates, pack_bytes, soul_ramp, ghost_ramps, work_folder, record, code, remaster_sizes=None, folder=DS_TEXTURE_FOLDER):
    entries = read_plate_entries(pack_bytes)
    paths = []
    names = []
    missing = []
    for plate in plates:
        plate_bytes = entries.get(plate.get("ds_plate"))
        if plate_bytes is None or (remaster_sizes is not None and tuple(remaster_sizes.get(plate["name"], ())) != plate_size(plate_bytes)):
            missing.append(plate["name"])
            continue
        for name, ramp in variants_of(plate, soul_ramp, ghost_ramps):
            paths.append(bake_variant(work_folder, record, code, plate_bytes, name, ramp, folder))
            names.append(name)
    return paths, names, missing
