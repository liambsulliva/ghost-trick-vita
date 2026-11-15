import json

import asset_writers
import bake_record
import ds_sprites
import ds_textures

TEXTURE_FOLDER = "textures"
CODE_FILES = ["ds_backdrops.py", "ds_sprites.py", "ds_textures.py", "asset_writers.py"]
HEADER_SIZE = 4
COLORS_PER_PALETTE = 16
PALETTE_BYTES = COLORS_PER_PALETTE * 2
TILE_SIDE = 8
TILE_BYTES = TILE_SIDE * TILE_SIDE
CLEAR_INDEX = 0
RGBA_BYTES = 4


class BackdropRefused(ValueError):
    pass


def page_name(name):
    return "%s_00" % name


def page_path(name):
    return TEXTURE_FOLDER + "/" + page_name(name) + ".gtex"


def frame_side(description):
    return description["map_columns"] * TILE_SIDE


def frame_count(description):
    return description["palette_count"]


def page_columns(description):
    columns = 1
    while columns * columns < frame_count(description):
        columns = columns * 2
    return columns


def palettes(entry_bytes, description):
    start = HEADER_SIZE
    count = description["palette_count"]
    if len(entry_bytes) < start + count * PALETTE_BYTES:
        raise BackdropRefused("a DS backdrop is shorter than its %d palettes" % count)
    rows = []
    for palette in range(count):
        row = []
        for color in range(COLORS_PER_PALETTE):
            place = start + palette * PALETTE_BYTES + color * 2
            row.append(ds_textures.color_of(entry_bytes[place] | entry_bytes[place + 1] << 8))
        rows.append(row)
    return rows


def indexes(entry_bytes, description):
    start = HEADER_SIZE + description["palette_count"] * PALETTE_BYTES
    columns = description["map_columns"]
    tile_count = columns * columns
    if len(entry_bytes) < start + tile_count * TILE_BYTES:
        raise BackdropRefused("a DS backdrop is shorter than its %d tiles" % tile_count)
    side = columns * TILE_SIDE
    pixels = bytearray(side * side)
    for tile in range(tile_count):
        left = (tile % columns) * TILE_SIDE
        top = (tile // columns) * TILE_SIDE
        for row in range(TILE_SIDE):
            source = start + tile * TILE_BYTES + row * TILE_SIDE
            target = (top + row) * side + left
            pixels[target:target + TILE_SIDE] = entry_bytes[source:source + TILE_SIDE]
    if max(pixels) >= COLORS_PER_PALETTE:
        raise BackdropRefused("a DS backdrop uses a colour past its 16-colour palette")
    return bytes(pixels)


def frame_rgba(pixels, palette):
    table = [bytes(color) + bytes((255,)) for color in palette]
    table[CLEAR_INDEX] = bytes(RGBA_BYTES)
    return b"".join(table[index] for index in pixels)


def page_rgba(entry_bytes, description):
    side = frame_side(description)
    columns = page_columns(description)
    width = side * columns
    pixels = indexes(entry_bytes, description)
    canvas = bytearray(width * width * RGBA_BYTES)
    for frame, palette in enumerate(palettes(entry_bytes, description)):
        rgba = frame_rgba(pixels, palette)
        left = (frame % columns) * side
        top = (frame // columns) * side
        for row in range(side):
            start = ((top + row) * width + left) * RGBA_BYTES
            canvas[start:start + side * RGBA_BYTES] = rgba[row * side * RGBA_BYTES:(row + 1) * side * RGBA_BYTES]
    return width, bytes(canvas)


def flipbook_row(description):
    columns = page_columns(description)
    return {"name": description["name"], "ticks_per_frame": description["ticks_per_frame"], "frames": frame_count(description), "columns": columns, "rows": columns, "cell_width": 1.0 / columns, "cell_height": 1.0 / columns, "pages": [page_name(description["name"])]}


def bake(descriptions, sprite_pack, record):
    code = bake_record.code_fingerprint(CODE_FILES)
    flipbooks = []
    paths = []
    refused = []
    for description in descriptions:
        name = description["name"]
        asset_writers.check_name(name)
        try:
            entry_bytes = sprite_pack.entry(description["block"], description["entry"])
            key = record.key_for(code, {"description": json.dumps(description, sort_keys=True)}, entry_bytes)
            if record.read_if_current(page_path(name), key) is None:
                width, rgba = page_rgba(entry_bytes, description)
                record.store(page_path(name), key, asset_writers.gtex_bytes(width, width, asset_writers.FORMAT_RGBA8888, rgba))
        except (ds_sprites.SpriteRefused, BackdropRefused) as error:
            refused.append("%s (%s)" % (name, error))
            continue
        flipbooks.append(flipbook_row(description))
        paths.append(page_path(name))
    if refused:
        print("ds backdrops left out: %d (%s)" % (len(refused), ", ".join(refused)))
    return flipbooks, paths


def add_to_set(set_description, cartridge, record):
    flipbooks, paths = bake(set_description.get("ds_backdrops", []), cartridge.sprites, record)
    expanded = dict(set_description)
    expanded["flipbooks"] = list(set_description.get("flipbooks", [])) + flipbooks
    return expanded, paths
