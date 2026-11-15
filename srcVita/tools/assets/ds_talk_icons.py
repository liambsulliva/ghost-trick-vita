import struct

import ds_text
import ds_textures

ICON_ROW_FORMAT = "<4H"
ICON_CENTER_BELOW_ROW = 17
RGBA_BYTES = 4


def read_icon_rows(arm9):
    rows = {}
    offset = ds_text.ICON_WIDTH_TABLE_ADDRESS - ds_text.ARM9_LOAD_ADDRESS
    while True:
        code, texture, width, height = struct.unpack_from(ICON_ROW_FORMAT, arm9, offset)
        if code == 0:
            return rows
        rows[code] = {"texture": texture, "width": width, "height": height}
        offset = offset + ds_text.ICON_WIDTH_ROW_SIZE


def icon_art(code, row, width, height, rgba):
    return {"code": code, "width": width, "height": height, "rgba": rgba, "left": ds_text.LETTER_GAP, "top": ICON_CENTER_BELOW_ROW - row["height"] // 2}


def read_icon_art(textures, rows, language):
    art = {}
    for code, row in sorted(rows.items()):
        try:
            width, height, rgba = textures.texture(row["texture"], language)
        except ds_textures.TextureRefused as error:
            print("ds talk icon %d (texture %d) draws nothing: %s" % (code, row["texture"], error))
            continue
        art[code] = icon_art(code, row, width, height, rgba)
    return art


def icon_cell(art, phase_y, screen_of):
    width = art["width"]
    screen_pixels = []
    for y in range(art["height"]):
        for x in range(width):
            color = art["rgba"][(y * width + x) * RGBA_BYTES:(y * width + x + 1) * RGBA_BYTES]
            if color[3] == 0:
                continue
            ds_x = art["left"] + x
            ds_y = art["top"] + y
            for column in range(screen_of(ds_x), screen_of(ds_x + 1)):
                for row in range(screen_of(phase_y + ds_y), screen_of(phase_y + ds_y + 1)):
                    screen_pixels.append((column, row - screen_of(phase_y), color))
    if not screen_pixels:
        return {"width": 0, "height": 0, "left": 0, "top": 0, "pixels": b""}
    left = min(column for column, _, _ in screen_pixels)
    top = min(row for _, row, _ in screen_pixels)
    cell_width = max(column for column, _, _ in screen_pixels) + 1 - left
    cell_height = max(row for _, row, _ in screen_pixels) + 1 - top
    rgba = bytearray(b"\xff\xff\xff\x00" * (cell_width * cell_height))
    for column, row, color in screen_pixels:
        start = ((row - top) * cell_width + column - left) * RGBA_BYTES
        rgba[start:start + RGBA_BYTES] = color
    return {"width": cell_width, "height": cell_height, "left": left, "top": top, "pixels": bytes(rgba)}
