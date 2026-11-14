import argparse
import os
import re
import subprocess
import sys

import numpy as np

TOOLS_FOLDER = os.path.dirname(os.path.abspath(__file__))
SRC_VITA_FOLDER = os.path.dirname(TOOLS_FOLDER)
sys.path.insert(0, os.path.join(TOOLS_FOLDER, "assets"))

import asset_writers
import gtex_pack

RENDER_BINARY = os.path.join(SRC_VITA_FOLDER, "build", "tests", "golden", "pause_render")
RENDERER_SOURCE = os.path.join(SRC_VITA_FOLDER, "src", "vita", "renderer.c")


def renderer_constant(name):
    with open(RENDERER_SOURCE, encoding="utf-8") as source:
        return float(re.search(r"#define %s ([0-9.]+)f" % name, source.read()).group(1))


CONTRAST_FLOOR = renderer_constant("CONTRAST_FLOOR")
CONTRAST_GAIN = renderer_constant("CONTRAST_GAIN")
SCREEN_WIDTH = 960
SCREEN_HEIGHT = 544
HEADER_SIZE = 20
PACKED_FLAG = 0x100


def expand_565(colors):
    red = ((colors >> 11) & 31) * 255 // 31
    green = ((colors >> 5) & 63) * 255 // 63
    blue = (colors & 31) * 255 // 31
    return np.stack([red, green, blue], axis=-1)


def decode_colors(colour_bytes, opaque_four):
    color0 = colour_bytes[:, 0].astype(np.int32) | (colour_bytes[:, 1].astype(np.int32) << 8)
    color1 = colour_bytes[:, 2].astype(np.int32) | (colour_bytes[:, 3].astype(np.int32) << 8)
    first = expand_565(color0)
    second = expand_565(color1)
    four = opaque_four | (color0 > color1)
    third = np.where(four[:, None], (2 * first + second) // 3, (first + second) // 2)
    fourth = np.where(four[:, None], (first + 2 * second) // 3, 0)
    table = np.stack([first, second, third, fourth], axis=1)
    bits = colour_bytes[:, 4].astype(np.uint32) | (colour_bytes[:, 5].astype(np.uint32) << 8) | (colour_bytes[:, 6].astype(np.uint32) << 16) | (colour_bytes[:, 7].astype(np.uint32) << 24)
    rows = np.arange(len(colour_bytes))
    colors = np.zeros((len(colour_bytes), 16, 3), dtype=np.uint8)
    holes = np.zeros((len(colour_bytes), 16), dtype=bool)
    for pixel in range(16):
        index = ((bits >> np.uint32(2 * pixel)) & np.uint32(3)).astype(np.int64)
        colors[:, pixel] = table[rows, index]
        holes[:, pixel] = ~four & (index == 3)
    return colors, holes


def tile(out, width, height):
    return out.reshape(height // 4, width // 4, 4, 4, 4).transpose(0, 2, 1, 3, 4).reshape(height, width, 4)


def decode_dxt1(body, width, height):
    blocks = np.frombuffer(body, dtype=np.uint8)[:(width // 4) * (height // 4) * 8].reshape(-1, 8)
    colors, holes = decode_colors(blocks, np.zeros(len(blocks), dtype=bool))
    out = np.zeros((len(blocks), 16, 4), dtype=np.uint8)
    out[:, :, :3] = colors
    out[:, :, 3] = np.where(holes, 0, 255)
    return tile(out, width, height)


def decode_dxt5(body, width, height):
    blocks = np.frombuffer(body, dtype=np.uint8)[:(width // 4) * (height // 4) * 16].reshape(-1, 16)
    alpha0 = blocks[:, 0].astype(np.int32)
    alpha1 = blocks[:, 1].astype(np.int32)
    alpha_bits = np.zeros(len(blocks), dtype=np.uint64)
    for index in range(6):
        alpha_bits |= blocks[:, 2 + index].astype(np.uint64) << np.uint64(8 * index)
    wide = alpha0 > alpha1
    alpha_table = np.zeros((len(blocks), 8), dtype=np.int32)
    alpha_table[:, 0] = alpha0
    alpha_table[:, 1] = alpha1
    for step in range(2, 8):
        narrow = ((6 - step) * alpha0 + (step - 1) * alpha1) // 5 if step < 6 else (0 if step == 6 else 255)
        alpha_table[:, step] = np.where(wide, ((8 - step) * alpha0 + (step - 1) * alpha1) // 7, narrow)
    colors, _ = decode_colors(blocks[:, 8:16], np.ones(len(blocks), dtype=bool))
    rows = np.arange(len(blocks))
    out = np.zeros((len(blocks), 16, 4), dtype=np.uint8)
    out[:, :, :3] = colors
    for pixel in range(16):
        out[:, pixel, 3] = alpha_table[rows, ((alpha_bits >> np.uint64(3 * pixel)) & np.uint64(7)).astype(np.int64)]
    return tile(out, width, height)


def read_texture(path):
    with open(path, "rb") as source:
        data = source.read()
    if int.from_bytes(data[12:16], "little") & PACKED_FLAG:
        data = gtex_pack.unpack_gtex(data)
    width, height, texture_format = asset_writers.read_gtex_header(data)
    body = data[HEADER_SIZE:]
    if texture_format == asset_writers.FORMAT_RGBA8888:
        return np.frombuffer(body, dtype=np.uint8)[:width * height * 4].reshape(height, width, 4)
    if texture_format == asset_writers.FORMAT_DXT5:
        return decode_dxt5(body, width, height)
    if texture_format == asset_writers.FORMAT_DXT1:
        return decode_dxt1(body, width, height)
    raise ValueError("unsupported texture format %d in %s" % (texture_format, path))


def blend(canvas, pixels, x, y, alpha):
    height, width = pixels.shape[:2]
    left, top = max(0, x), max(0, y)
    right, bottom = min(SCREEN_WIDTH, x + width), min(SCREEN_HEIGHT, y + height)
    if right <= left or bottom <= top:
        return
    part = pixels[top - y:bottom - y, left - x:right - x].astype(np.float32)
    weight = part[:, :, 3:4] / 255.0 * alpha
    canvas[top:bottom, left:right] = canvas[top:bottom, left:right] * (1.0 - weight) + part[:, :, :3] * weight


def draw_sprite(canvas, texture, fields):
    x, y, width, height, u, v, u_size, v_size, alpha = (float(value) for value in fields)
    target_width, target_height = int(round(width)), int(round(height))
    if target_width <= 0 or target_height <= 0:
        return
    texture_height, texture_width = texture.shape[:2]
    columns = np.clip(((u + (np.arange(target_width) + 0.5) / target_width * u_size) * texture_width).astype(int), 0, texture_width - 1)
    rows = np.clip(((v + (np.arange(target_height) + 0.5) / target_height * v_size) * texture_height).astype(int), 0, texture_height - 1)
    blend(canvas, texture[rows][:, columns], int(round(x)), int(round(y)), alpha)


def draw_rect(canvas, fields):
    x, y, width, height, red, green, blue, alpha = (float(value) for value in fields)
    patch = np.zeros((int(round(height)), int(round(width)), 4), dtype=np.uint8)
    patch[:, :] = (round(red * 255), round(green * 255), round(blue * 255), 255)
    blend(canvas, patch, int(round(x)), int(round(y)), alpha)


def raise_contrast(canvas, fields):
    x, y, w, h = [int(round(float(value))) for value in fields]
    area = canvas[y:y + h, x:x + w]
    area[:] = np.clip(area - CONTRAST_FLOOR * 255.0, 0.0, 255.0) * (1.0 + CONTRAST_GAIN)


def render(assets_folder, buttons, output_path, notes=None, clock=None):
    extra = [notes or ""] + ([clock] if clock else []) if notes or clock else []
    result = subprocess.run([RENDER_BINARY, assets_folder, buttons] + extra, check=True, capture_output=True, text=True)
    canvas = np.zeros((SCREEN_HEIGHT, SCREEN_WIDTH, 3), dtype=np.float32)
    textures = {}
    for line in result.stdout.splitlines():
        parts = line.split()
        if not parts:
            continue
        if parts[0] == "contrast":
            raise_contrast(canvas, parts[1:])
        elif parts[0] == "rect":
            draw_rect(canvas, parts[1:])
        elif parts[0] == "sprite":
            name = parts[1]
            if name not in textures:
                textures[name] = read_texture(os.path.join(assets_folder, "textures", name + ".gtex"))
            draw_sprite(canvas, textures[name], parts[2:])
    raw_path = output_path + ".rgb"
    with open(raw_path, "wb") as output:
        output.write(canvas.clip(0, 255).astype(np.uint8).tobytes())
    subprocess.run(["magick", "-size", "%dx%d" % (SCREEN_WIDTH, SCREEN_HEIGHT), "-depth", "8", "rgb:" + raw_path, output_path], check=True)
    os.remove(raw_path)


def main():
    parser = argparse.ArgumentParser(description="Draws the pause menu on the Mac from the baked assets, after a list of button presses.")
    parser.add_argument("assets_folder", help="a converted set folder, such as ../extracted/vita_assets/game")
    parser.add_argument("buttons", help="presses after the menu opens: u d l r x o L R, and . to wait")
    parser.add_argument("--title", choices=["none", "save", "cleared"], help="draw the title screen instead, with no save, a save, or a save of a cleared game")
    parser.add_argument("output", help="the PNG to write")
    parser.add_argument("--notes", help="notebook entries known, as list:id:level,... (1 people, 2 places)")
    parser.add_argument("--clock", help="the scene the clock reads, as chapter:script:plate")
    arguments = parser.parse_args()
    buttons = arguments.buttons
    if arguments.title is not None:
        buttons = {"none": "title:", "save": "title_save:", "cleared": "title_clear:"}[arguments.title] + buttons
    render(arguments.assets_folder, buttons, arguments.output, arguments.notes, arguments.clock)
    return 0


if __name__ == "__main__":
    sys.exit(main())
