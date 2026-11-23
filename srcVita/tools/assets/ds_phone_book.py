import struct

import numpy as np

import asset_writers
import bake_record
import ds_sprites

CODE_FILES = ["ds_phone_book.py", "ds_sprites.py", "asset_writers.py"]
TEXTURE_FOLDER = "textures"
SHEET_NAME = "ds_phone_book"
TABLE_PATH = "pause/phone_book.txt"
ARM9_LOAD_ADDRESS = 0x02000000
PLATE_TABLE_ADDRESS = 0x0209a7cc
REVEAL_TABLE_ADDRESS = 0x0209a76c
TRACE_TABLE_ADDRESS = 0x0209a754
TRACE_CHOICE_COUNT = 2
PROMPT_BAR = 0x8009
PROMPT_WHERE_TO = 0x800e
PROMPT_TRACE = 0x800f
PROMPTS = (("bar", PROMPT_BAR), ("where_to", PROMPT_WHERE_TO), ("trace", PROMPT_TRACE))
PLACE_COUNT = 24
LEVEL_COUNT = 3
REVEAL_COUNT = LEVEL_COUNT - 1
NO_REVEAL = 0xff
RESTING = 0
SELECTED = 1
STATES = (RESTING, SELECTED)
CELL_GAP = 2
SHEET_WIDTH = 1024


class PhoneBookRefused(ValueError):
    pass


def read_places(arm9):
    places = []
    plate_offset = PLATE_TABLE_ADDRESS - ARM9_LOAD_ADDRESS
    reveal_offset = REVEAL_TABLE_ADDRESS - ARM9_LOAD_ADDRESS
    if plate_offset + PLACE_COUNT * LEVEL_COUNT * 2 > len(arm9) or reveal_offset + PLACE_COUNT * REVEAL_COUNT > len(arm9):
        raise PhoneBookRefused("the ARM9 binary is shorter than the phone book tables")
    for place in range(PLACE_COUNT):
        resources = struct.unpack_from("<%dH" % LEVEL_COUNT, arm9, plate_offset + place * LEVEL_COUNT * 2)
        reveals = tuple(arm9[reveal_offset + place * REVEAL_COUNT:reveal_offset + (place + 1) * REVEAL_COUNT])
        places.append({"place": place, "resources": resources, "reveals": reveals})
    return places


def read_trace_choices(arm9):
    offset = TRACE_TABLE_ADDRESS - ARM9_LOAD_ADDRESS
    if offset + TRACE_CHOICE_COUNT * 2 > len(arm9):
        raise PhoneBookRefused("the ARM9 binary is shorter than the traced call's choice table")
    return struct.unpack_from("<%dH" % TRACE_CHOICE_COUNT, arm9, offset)


def plate_resources(places, choices=()):
    listed = set(resource + state for row in places for resource in row["resources"] for state in STATES)
    listed.update(resource + state for resource in choices for state in STATES)
    listed.update(resource for _, resource in PROMPTS if choices)
    return sorted(listed)


def plate_parts(sprites, arm9, resource, language):
    return ds_sprites.resource_parts(sprites, ds_sprites.resolve(arm9, resource, language), language)


def plate_frame(parts):
    frames = ds_sprites.frames_of_parts(*parts)
    if not frames or frames[0][0] == 0:
        raise PhoneBookRefused("the plate has no drawn frame")
    return frames[0]


def pack_plates(frames):
    places = {}
    x = 0
    y = 0
    row_height = 0
    for resource, (width, height, _, _, _) in sorted(frames.items()):
        if width > SHEET_WIDTH:
            raise PhoneBookRefused("plate %#x is wider than the sheet" % resource)
        if x + width > SHEET_WIDTH:
            x = 0
            y = y + row_height + CELL_GAP
            row_height = 0
        places[resource] = (x, y)
        x = x + width + CELL_GAP
        row_height = max(row_height, height)
    height = asset_writers.round_up_to_power_of_two(max(y + row_height, 1))
    sheet = np.zeros((height, SHEET_WIDTH, 4), dtype=np.uint8)
    for resource, (width, plate_height, rgba, _, _) in frames.items():
        left, top = places[resource]
        sheet[top:top + plate_height, left:left + width] = np.frombuffer(rgba, dtype=np.uint8).reshape(plate_height, width, 4)
    return sheet, places


def table_lines(places, frames, spots, choices=()):
    lines = ["sheet " + SHEET_NAME]
    for resource in sorted(frames):
        width, height, _, left, top = frames[resource]
        x, y = spots[resource]
        lines.append("plate %d %d %d %d %d %d %d" % (resource, x, y, width, height, left, top))
    for row in places:
        lines.append("place %d %s %s" % (row["place"], " ".join("%d" % resource for resource in row["resources"]), " ".join("%d" % level for level in row["reveals"])))
    for kind, resource in enumerate(choices):
        lines.append("choice %d %d" % (kind, resource))
    for name, resource in PROMPTS if choices else ():
        lines.append("prompt %s %d" % (name, resource))
    return lines


def bake(arm9, sprites, language, record):
    code = bake_record.code_fingerprint(CODE_FILES)
    places = read_places(arm9)
    choices = read_trace_choices(arm9)
    parts = {}
    refused = []
    for resource in plate_resources(places, choices):
        try:
            parts[resource] = plate_parts(sprites, arm9, resource, language)
        except ds_sprites.SpriteRefused as error:
            refused.append("%#x (%s)" % (resource, error))
    source = b"".join(struct.pack("<I", resource) + b"".join(struct.pack("<I", len(part)) + part for part in parts[resource]) for resource in sorted(parts))
    key = record.key_for(code, {"places": [[row["resources"], row["reveals"]] for row in places], "choices": list(choices), "prompts": [list(prompt) for prompt in PROMPTS], "language": language}, source)
    relative_path = TEXTURE_FOLDER + "/" + SHEET_NAME + ".gtex"
    gtex = record.read_if_current(relative_path, key)
    table = record.read_if_current(TABLE_PATH, key)
    if gtex is None or table is None:
        frames = {}
        for resource in sorted(parts):
            try:
                frames[resource] = plate_frame(parts[resource])
            except (ds_sprites.SpriteRefused, PhoneBookRefused) as error:
                refused.append("%#x (%s)" % (resource, error))
        if not frames:
            print("ds phone book left out: no plate decodes")
            return {}, [], {}
        sheet, spots = pack_plates(frames)
        gtex = asset_writers.gtex_bytes(sheet.shape[1], sheet.shape[0], asset_writers.FORMAT_RGBA8888, np.ascontiguousarray(sheet).tobytes())
        table = ("\n".join(table_lines(places, frames, spots, choices)) + "\n").encode("ascii")
        record.store(relative_path, key, gtex)
        record.store(TABLE_PATH, key, table)
    if refused:
        print("ds phone book plates left out: %d (%s)" % (len(refused), ", ".join(refused[:12])))
    return {TABLE_PATH: table}, [relative_path], {SHEET_NAME: asset_writers.read_gtex_header(gtex)[:2]}
