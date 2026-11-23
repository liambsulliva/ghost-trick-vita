import struct

import asset_writers
import bake_record
import ds_sprites

CODE_FILES = ["ds_staff_roll.py", "ds_sprites.py", "asset_writers.py"]
TEXTURE_FOLDER = "textures"
TEXTURE_NAME = "staff_%02d_%02d"
ARM9_LOAD_ADDRESS = 0x02000000
PAGE_TABLE_ADDRESS = 0x0209e010
SCREEN_TABLE_ADDRESS = 0x0209e048
LONG_ROLL_TABLE_ADDRESS = 0x0209e028
PAGE_COUNT = 11
LONG_ROLL_PAGE = 9
LONG_ROLL_BANKS_MOST = 4
PICTURE_BLOCK = 2
PALETTE_BLOCK = 0
PALETTE_ENTRY = 0x82
SCREEN_WIDTH = 256
SCREEN_HEIGHT = 192
TILE_SIZE = 8
TILE_BYTES = 32
TILES_ACROSS = SCREEN_WIDTH // TILE_SIZE
SCREEN_TILE_BYTES = 0x6000
SCREEN_MAP_BYTES = 0x800
SCREEN_MAP_ENTRIES = TILES_ACROSS * (SCREEN_HEIGHT // TILE_SIZE)
TILE_INDEX_MASK = 0x3ff
FLIP_ACROSS = 0x400
FLIP_DOWN = 0x800
RGBA_BYTES = 4


class StaffRollRefused(Exception):
    pass


def table_bytes(arm9, address, size):
    offset = address - ARM9_LOAD_ADDRESS
    if offset < 0 or offset + size > len(arm9):
        raise StaffRollRefused("the ARM9 binary is shorter than the staff roll tables")
    return arm9[offset:offset + size]


def paged_screens(arm9):
    pages = table_bytes(arm9, PAGE_TABLE_ADDRESS, PAGE_COUNT * 2)
    screens = {}
    for page in range(PAGE_COUNT):
        first_row, count = pages[page * 2], pages[page * 2 + 1]
        if page == LONG_ROLL_PAGE or count == 0:
            continue
        rows = table_bytes(arm9, SCREEN_TABLE_ADDRESS + first_row * 8, count * 8)
        screens[page] = [struct.unpack_from("<II", rows, index * 8) for index in range(count)]
    return screens


def long_roll_banks(arm9):
    words = struct.unpack("<%dI" % (LONG_ROLL_BANKS_MOST * 2), table_bytes(arm9, LONG_ROLL_TABLE_ADDRESS, LONG_ROLL_BANKS_MOST * 8))
    banks = []
    for bank in range(LONG_ROLL_BANKS_MOST):
        if words[bank * 2] == 0:
            break
        banks.append((words[bank * 2], words[bank * 2 + 1]))
    return banks


def picture_rows(tiles, tile_map, entry_count, colors):
    if len(tile_map) < entry_count * 2 or entry_count % TILES_ACROSS != 0:
        raise StaffRollRefused("a staff roll map is %d bytes, too short for %d tiles" % (len(tile_map), entry_count))
    height = entry_count // TILES_ACROSS * TILE_SIZE
    pixels = bytearray(SCREEN_WIDTH * height * RGBA_BYTES)
    for index, entry in enumerate(struct.unpack_from("<%dH" % entry_count, tile_map)):
        tile = entry & TILE_INDEX_MASK
        if (tile + 1) * TILE_BYTES > len(tiles):
            raise StaffRollRefused("a staff roll map names tile %d, past its %d tiles" % (tile, len(tiles) // TILE_BYTES))
        left = index % TILES_ACROSS * TILE_SIZE
        top = index // TILES_ACROSS * TILE_SIZE
        for y in range(TILE_SIZE):
            source_y = TILE_SIZE - 1 - y if entry & FLIP_DOWN else y
            for x in range(TILE_SIZE):
                source_x = TILE_SIZE - 1 - x if entry & FLIP_ACROSS else x
                pair = tiles[tile * TILE_BYTES + source_y * 4 + source_x // 2]
                color = pair >> 4 if source_x & 1 else pair & 15
                if color >= len(colors):
                    raise StaffRollRefused("a staff roll tile names a color past its palette")
                at = ((top + y) * SCREEN_WIDTH + left + x) * RGBA_BYTES
                pixels[at:at + RGBA_BYTES] = colors[color]
    return bytes(pixels)


def decode_screen(tiles, tile_map, colors):
    if len(tiles) != SCREEN_TILE_BYTES or len(tile_map) != SCREEN_MAP_BYTES:
        raise StaffRollRefused("a staff roll screen is %d bytes of tiles and %d of map, not %d and %d" % (len(tiles), len(tile_map), SCREEN_TILE_BYTES, SCREEN_MAP_BYTES))
    return picture_rows(tiles, tile_map, SCREEN_MAP_ENTRIES, colors)


def cut_into_screens(strip):
    screen_bytes = SCREEN_WIDTH * SCREEN_HEIGHT * RGBA_BYTES
    if len(strip) == 0 or len(strip) % screen_bytes != 0:
        raise StaffRollRefused("the long staff roll is %d rows, not a whole number of screens" % (len(strip) // (SCREEN_WIDTH * RGBA_BYTES)))
    return [strip[start:start + screen_bytes] for start in range(0, len(strip), screen_bytes)]


def page_sources(arm9, sprites, language):
    sources = {}
    for page, rows in paged_screens(arm9).items():
        sources[page] = [(sprites.entry(PICTURE_BLOCK, tiles, language), sprites.entry(PICTURE_BLOCK, tile_map, language)) for tiles, tile_map in rows]
    banks = long_roll_banks(arm9)
    if banks:
        sources[LONG_ROLL_PAGE] = [(sprites.entry(PICTURE_BLOCK, tiles, language), sprites.entry(PICTURE_BLOCK, tile_map, language)) for tiles, tile_map in banks]
    return sources


def page_screens(page, source, colors):
    if page != LONG_ROLL_PAGE:
        return [decode_screen(tiles, tile_map, colors) for tiles, tile_map in source]
    return cut_into_screens(b"".join(picture_rows(tiles, tile_map, len(tile_map) // 2, colors) for tiles, tile_map in source))


def screens_of_pages_from(page, source):
    if page != LONG_ROLL_PAGE:
        return len(source)
    rows = sum(len(tile_map) // 2 // TILES_ACROSS * TILE_SIZE for _, tile_map in source)
    if rows == 0 or rows % SCREEN_HEIGHT != 0:
        raise StaffRollRefused("the long staff roll is %d rows, not a whole number of screens" % rows)
    return rows // SCREEN_HEIGHT


def screens_of_pages(arm9, sprites, language):
    return {page: screens_of_pages_from(page, source) for page, source in page_sources(arm9, sprites, language).items()}


def texture_path(page, screen):
    return TEXTURE_FOLDER + "/" + TEXTURE_NAME % (page, screen) + ".gtex"


def bake(arm9, sprites, language, record):
    code = bake_record.code_fingerprint(CODE_FILES)
    paths = []
    sizes = {}
    refused = []
    try:
        sources = page_sources(arm9, sprites, language)
        colors = ds_sprites.palette_rgba(sprites.entry(PALETTE_BLOCK, PALETTE_ENTRY))
    except (ds_sprites.SpriteRefused, StaffRollRefused) as error:
        print("ds staff roll left out: %s" % error)
        return paths, sizes
    for page in sorted(sources):
        source = sources[page]
        key = record.key_for(code, {"page": page, "language": language}, b"".join(colors) + b"".join(struct.pack("<II", len(tiles), len(tile_map)) + tiles + tile_map for tiles, tile_map in source))
        try:
            count = screens_of_pages_from(page, source)
            stored = [record.read_if_current(texture_path(page, screen + 1), key) for screen in range(count)]
            if any(data is None for data in stored):
                for screen, pixels in enumerate(page_screens(page, source, colors)):
                    record.store(texture_path(page, screen + 1), key, asset_writers.gtex_bytes(SCREEN_WIDTH, SCREEN_HEIGHT, asset_writers.FORMAT_RGBA8888, pixels))
        except StaffRollRefused as error:
            refused.append("%d (%s)" % (page, error))
            continue
        for screen in range(count):
            paths.append(texture_path(page, screen + 1))
            sizes[TEXTURE_NAME % (page, screen + 1)] = (SCREEN_WIDTH, SCREEN_HEIGHT)
    if refused:
        print("ds staff roll pages left out: %d (%s)" % (len(refused), ", ".join(refused)))
    return paths, sizes
