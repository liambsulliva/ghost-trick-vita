import argparse
import pickle
import struct
import sys
import zlib

import numpy as np

SCREEN_WIDTH = 256
SCREEN_HEIGHT = 192
OBJ_WIDTHS = ((8, 16, 32, 64), (16, 32, 32, 64), (8, 8, 16, 32))
OBJ_HEIGHTS = ((8, 16, 32, 64), (8, 8, 16, 32), (16, 32, 32, 64))
TEXT_MAP_SIZES = ((256, 256), (512, 256), (256, 512), (512, 512))
BITMAP_SIZES = ((128, 128), (256, 256), (512, 256), (512, 512))


def write_png(path, rgb):
    height, width, _ = rgb.shape
    rows = b"".join(b"\x00" + rgb[row].astype(np.uint8).tobytes() for row in range(height))

    def chunk(kind, data):
        return struct.pack(">I", len(data)) + kind + data + struct.pack(">I", zlib.crc32(kind + data))

    with open(path, "wb") as out:
        out.write(b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 2, 0, 0, 0)) + chunk(b"IDAT", zlib.compress(rows, 6)) + chunk(b"IEND", b""))


def colors_of(palette_bytes):
    words = np.frombuffer(palette_bytes, dtype="<u2").astype(np.int32)
    return np.stack([(words & 31) * 255 // 31, ((words >> 5) & 31) * 255 // 31, ((words >> 10) & 31) * 255 // 31], axis=-1)


def half(data, offset):
    return struct.unpack_from("<H", data, offset)[0]


def word(data, offset):
    return struct.unpack_from("<I", data, offset)[0]


class Layer:
    def __init__(self):
        self.rgb = np.zeros((SCREEN_HEIGHT, SCREEN_WIDTH, 3), dtype=np.int32)
        self.shown = np.zeros((SCREEN_HEIGHT, SCREEN_WIDTH), dtype=bool)


def tile_pixels(vram, start, is_256):
    if is_256:
        return np.frombuffer(vram[start:start + 64].ljust(64, b"\x00"), dtype=np.uint8).reshape(8, 8).astype(np.int32)
    data = np.frombuffer(vram[start:start + 32].ljust(32, b"\x00"), dtype=np.uint8).astype(np.int32)
    pixels = np.empty(64, dtype=np.int32)
    pixels[0::2] = data & 15
    pixels[1::2] = data >> 4
    return pixels.reshape(8, 8)


def text_layer(vram, colors, control, char_base_extra, screen_base_extra, scroll):
    layer = Layer()
    char_base = ((control >> 2) & 15) * 0x4000 + char_base_extra
    screen_base = ((control >> 8) & 31) * 0x800 + screen_base_extra
    is_256 = (control >> 7) & 1
    map_width, map_height = TEXT_MAP_SIZES[control >> 14]
    canvas = np.zeros((map_height, map_width), dtype=np.int32)
    for block_y in range(map_height // 256):
        for block_x in range(map_width // 256):
            block = block_y * (map_width // 256) + block_x
            for cell in range(1024):
                entry = half(vram, screen_base + block * 0x800 + cell * 2) if screen_base + block * 0x800 + cell * 2 + 2 <= len(vram) else 0
                pixels = tile_pixels(vram, char_base + (entry & 0x3ff) * (64 if is_256 else 32), is_256)
                if entry & 0x400:
                    pixels = pixels[:, ::-1]
                if entry & 0x800:
                    pixels = pixels[::-1, :]
                if not is_256:
                    pixels = np.where(pixels > 0, pixels + (entry >> 12) * 16, 0)
                left = block_x * 256 + (cell % 32) * 8
                top = block_y * 256 + (cell // 32) * 8
                canvas[top:top + 8, left:left + 8] = pixels
    ys = (np.arange(SCREEN_HEIGHT) + scroll[1]) % map_height
    xs = (np.arange(SCREEN_WIDTH) + scroll[0]) % map_width
    indexes = canvas[ys][:, xs]
    layer.rgb = colors[indexes]
    layer.shown = indexes > 0
    return layer


def extended_layer(vram, colors, control, screen_base_extra, char_base_extra):
    layer = Layer()
    size = control >> 14
    if control & 0x80:
        width, height = BITMAP_SIZES[size]
        start = ((control >> 8) & 31) * 0x4000
        if control & 0x4:
            words = np.frombuffer(vram[start:start + width * height * 2].ljust(width * height * 2, b"\x00"), dtype="<u2").astype(np.int32).reshape(height, width)[:SCREEN_HEIGHT, :SCREEN_WIDTH]
            shown = (words & 0x8000) > 0
            rgb = np.stack([(words & 31) * 255 // 31, ((words >> 5) & 31) * 255 // 31, ((words >> 10) & 31) * 255 // 31], axis=-1)
        else:
            indexes = np.frombuffer(vram[start:start + width * height].ljust(width * height, b"\x00"), dtype=np.uint8).astype(np.int32).reshape(height, width)[:SCREEN_HEIGHT, :SCREEN_WIDTH]
            shown = indexes > 0
            rgb = colors[indexes]
        layer.rgb[:shown.shape[0], :shown.shape[1]] = rgb
        layer.shown[:shown.shape[0], :shown.shape[1]] = shown
        return layer
    cells = (16, 32, 64, 128)[size]
    char_base = ((control >> 2) & 15) * 0x4000 + char_base_extra
    screen_base = ((control >> 8) & 31) * 0x800 + screen_base_extra
    for cell_y in range(min(cells, SCREEN_HEIGHT // 8)):
        for cell_x in range(min(cells, SCREEN_WIDTH // 8)):
            entry = half(vram, screen_base + (cell_y * cells + cell_x) * 2)
            pixels = tile_pixels(vram, char_base + (entry & 0x3ff) * 64, True)
            if entry & 0x400:
                pixels = pixels[:, ::-1]
            if entry & 0x800:
                pixels = pixels[::-1, :]
            layer.rgb[cell_y * 8:cell_y * 8 + 8, cell_x * 8:cell_x * 8 + 8] = colors[pixels]
            layer.shown[cell_y * 8:cell_y * 8 + 8, cell_x * 8:cell_x * 8 + 8] = pixels > 0
    return layer


def obj_layers(obj_vram, colors, oam, display_control):
    layers = [Layer() for _ in range(4)]
    one_dimensional = (display_control >> 4) & 1
    tile_shift = (display_control >> 20) & 3 if one_dimensional else 0
    listing = []
    for index in range(127, -1, -1):
        attr0 = half(oam, index * 8)
        attr1 = half(oam, index * 8 + 2)
        attr2 = half(oam, index * 8 + 4)
        affine = (attr0 >> 8) & 1
        if not affine and (attr0 >> 9) & 1:
            continue
        mode = (attr0 >> 10) & 3
        shape = attr0 >> 14
        if shape == 3:
            continue
        size = attr1 >> 14
        width = OBJ_WIDTHS[shape][size]
        height = OBJ_HEIGHTS[shape][size]
        x = attr1 & 0x1ff
        if x >= 256:
            x = x - 512
        y = attr0 & 0xff
        if y >= 192:
            y = y - 256
        tile = attr2 & 0x3ff
        priority = (attr2 >> 10) & 3
        is_256 = (attr0 >> 13) & 1
        pixels = np.zeros((height, width), dtype=np.int32)
        if mode == 3:
            if (display_control >> 6) & 1:
                start = tile * (256 if (display_control >> 22) & 1 else 128)
            else:
                mask_x = 0x1f if not (display_control >> 5) & 1 else 0x0f
                start = (tile & mask_x) * 0x10 + (tile & ~mask_x) * 0x80
            words = np.frombuffer(obj_vram[start:start + width * height * 2].ljust(width * height * 2, b"\x00"), dtype="<u2").astype(np.int32).reshape(height, width)
            shown = (words & 0x8000) > 0
            rgb = np.stack([(words & 31) * 255 // 31, ((words >> 5) & 31) * 255 // 31, ((words >> 10) & 31) * 255 // 31], axis=-1)
        else:
            for tile_y in range(height // 8):
                for tile_x in range(width // 8):
                    if one_dimensional:
                        start = (tile << (5 + tile_shift)) + (tile_y * (width // 8) + tile_x) * (64 if is_256 else 32)
                    else:
                        start = (tile + tile_y * 32 + tile_x * (2 if is_256 else 1)) * 32
                    pixels[tile_y * 8:tile_y * 8 + 8, tile_x * 8:tile_x * 8 + 8] = tile_pixels(obj_vram, start, is_256)
            shown = pixels > 0
            if not is_256:
                pixels = np.where(shown, pixels + (attr2 >> 12) * 16, 0)
            rgb = colors[pixels]
        if not affine:
            if attr1 & 0x1000:
                rgb = rgb[:, ::-1]
                shown = shown[:, ::-1]
            if attr1 & 0x2000:
                rgb = rgb[::-1, :]
                shown = shown[::-1, :]
        listing.append((index, x, y, width, height, tile, priority, mode, affine, attr2 >> 12, is_256))
        left = max(0, x)
        top = max(0, y)
        right = min(SCREEN_WIDTH, x + width)
        bottom = min(SCREEN_HEIGHT, y + height)
        if right <= left or bottom <= top:
            continue
        part_shown = shown[top - y:bottom - y, left - x:right - x]
        target = layers[priority]
        target.rgb[top:bottom, left:right][part_shown] = rgb[top - y:bottom - y, left - x:right - x][part_shown]
        target.shown[top:bottom, left:right] |= part_shown
    return layers, listing


def render_engine(state, engine, notes):
    io = state["io_" + engine]
    display_control = word(io, 0)
    bg_vram = state["bg_" + engine]
    obj_vram = state["obj_" + engine]
    palette = state["palette"][0:0x400] if engine == "a" else state["palette"][0x400:0x800]
    bg_colors = colors_of(palette[0:0x200])
    obj_colors = colors_of(palette[0x200:0x400])
    oam = state["oam"][0:0x400] if engine == "a" else state["oam"][0x400:0x800]
    mode = display_control & 7
    char_extra = ((display_control >> 24) & 7) * 0x10000 if engine == "a" else 0
    screen_extra = ((display_control >> 27) & 7) * 0x10000 if engine == "a" else 0
    notes.append("engine %s dispcnt %08x mode %d display %d bg_enable %x obj %d win %x" % (engine, display_control, mode, (display_control >> 16) & 3, (display_control >> 8) & 15, (display_control >> 12) & 1, (display_control >> 13) & 7))
    picture = np.zeros((SCREEN_HEIGHT, SCREEN_WIDTH, 3), dtype=np.int32) + bg_colors[0]
    backgrounds = {}
    for bg in range(4):
        if not (display_control >> (8 + bg)) & 1:
            continue
        control = half(io, 8 + bg * 2)
        scroll = (half(io, 0x10 + bg * 4) & 0x1ff, half(io, 0x12 + bg * 4) & 0x1ff)
        notes.append("  bg%d control %04x priority %d scroll %s" % (bg, control, control & 3, scroll))
        if bg == 0 and engine == "a" and (display_control >> 3) & 1:
            notes.append("  bg0 is the 3D layer, not drawn")
            continue
        is_text = bg < 2 or mode == 0 or (bg == 2 and mode in (1, 3)) or (bg == 3 and mode == 0)
        if is_text:
            backgrounds[bg] = (control & 3, text_layer(bg_vram, bg_colors, control, char_extra, screen_extra, scroll))
        elif (bg == 3 and mode in (3, 4, 5)) or (bg == 2 and mode == 5):
            backgrounds[bg] = (control & 3, extended_layer(bg_vram, bg_colors, control, screen_extra, char_extra))
        else:
            notes.append("  bg%d is affine, not drawn" % bg)
    objects = [Layer() for _ in range(4)]
    if (display_control >> 12) & 1:
        objects, listing = obj_layers(obj_vram, obj_colors, oam, display_control)
        for row in listing:
            notes.append("  obj %3d x %4d y %4d %2dx%2d tile %4d prio %d mode %d affine %d pal %2d c256 %d" % row)
    for priority in range(3, -1, -1):
        for bg in range(3, -1, -1):
            if bg in backgrounds and backgrounds[bg][0] == priority:
                layer = backgrounds[bg][1]
                picture[layer.shown] = layer.rgb[layer.shown]
        layer = objects[priority]
        picture[layer.shown] = layer.rgb[layer.shown]
    brightness = half(io, 0x6c)
    factor = min(16, brightness & 31)
    if brightness >> 14 == 1:
        picture = picture + (255 - picture) * factor // 16
    elif brightness >> 14 == 2:
        picture = picture - picture * factor // 16
    notes.append("  master brightness %04x, blend control %04x alpha %04x" % (brightness, half(io, 0x50), half(io, 0x52)))
    if (display_control >> 16) & 3 == 0:
        picture = np.zeros_like(picture) + 255
    return picture


def main():
    parser = argparse.ArgumentParser(description="Draw both DS screens from a dump made by tools/ds_screen_dump.py. The 3D layer, windows, blending and affine transforms are not drawn.")
    parser.add_argument("dump")
    parser.add_argument("output", help="PNG path; the top screen is drawn above the bottom screen")
    parser.add_argument("--scale", type=int, default=2)
    parser.add_argument("--notes", action="store_true", help="print each engine's registers and sprites")
    arguments = parser.parse_args()
    with open(arguments.dump, "rb") as dump:
        state = pickle.load(dump)
    notes = []
    engine_a = render_engine(state, "a", notes)
    if "capture" in state:
        words = np.frombuffer(state["capture"], dtype="<u2").astype(np.int32).reshape(SCREEN_HEIGHT, SCREEN_WIDTH)
        engine_a = np.stack([(words & 31) * 255 // 31, ((words >> 5) & 31) * 255 // 31, ((words >> 10) & 31) * 255 // 31], axis=-1)
    engine_b = render_engine(state, "b", notes)
    picture = np.concatenate([engine_a, engine_b], axis=0)
    picture = np.repeat(np.repeat(picture, arguments.scale, axis=0), arguments.scale, axis=1)
    write_png(arguments.output, np.clip(picture, 0, 255))
    if arguments.notes:
        print("\n".join(notes))


if __name__ == "__main__":
    main()
