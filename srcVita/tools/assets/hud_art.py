import os
import subprocess

import asset_writers
import bake_record

SHEET_NAME = "hud_sheet"
DISC_NAME = "hud_disc"
DISC_SIZE = 128
SHEET_WIDTH = 512
SHEET_HEIGHT = 256
SUPERSAMPLE = 4
GLYPH_CELL = 40
START_WIDTH = 72
SELECT_WIDTH = 84
LABEL_ROW_Y = 48
LABEL_WIDTH = 160
LABEL_HEIGHT = 64
INK_NAVY = "#1b2a3a"
WHITE = "#ffffff"
PAIRS = {"cross": "#8fb8ff", "circle": "#ff7a7a", "square": "#ff9ad5", "triangle": "#6fe0b0"}
FACE_ORDER = ["cross", "circle", "square", "triangle"]
LETTER_ORDER = ["L", "R"]
LABELS = ["Ghost", "Trick", "Back"]
SWAP_LABELS = ["Swap", "Unlock"]
SWAP_LABEL_ROW_Y = 192
SWAP_LABEL_WIDTH = 224
WIDE_LABEL = "Rewind Time"
WIDE_LABEL_ROW_Y = 112
WIDE_LABEL_WIDTH = 420
WIDE_LABEL_HEIGHT = 72
WIDE_LABEL_POINTS = 240
LABEL_POINTS = 180
LABEL_OUTLINE = 8
KEY_WORD_NUDGE_Y = 8
LETTER_POINTS = 112
START_POINTS = 64
SELECT_POINTS = 64


def scaled(value):
    return value * SUPERSAMPLE


def face_symbol(kind, center_x, center_y):
    if kind == "cross":
        reach = scaled(GLYPH_CELL) * 21 // 100
        return ["line %d,%d %d,%d" % (center_x - reach, center_y - reach, center_x + reach, center_y + reach), "line %d,%d %d,%d" % (center_x - reach, center_y + reach, center_x + reach, center_y - reach)]
    if kind == "circle":
        return ["circle %d,%d %d,%d" % (center_x, center_y, center_x, center_y + scaled(GLYPH_CELL) * 22 // 100)]
    if kind == "square":
        reach = scaled(GLYPH_CELL) * 19 // 100
        return ["rectangle %d,%d %d,%d" % (center_x - reach, center_y - reach, center_x + reach, center_y + reach)]
    reach = scaled(GLYPH_CELL) * 23 // 100
    return ["polygon %d,%d %d,%d %d,%d" % (center_x, center_y - reach, center_x + reach, center_y + reach * 3 // 4, center_x - reach, center_y + reach * 3 // 4)]


def face_arguments(kind, cell_x):
    center_x = scaled(cell_x) + scaled(GLYPH_CELL) // 2
    center_y = scaled(GLYPH_CELL) // 2
    radius = scaled(GLYPH_CELL) * 44 // 100
    arguments = ["-fill", INK_NAVY, "-stroke", WHITE, "-strokewidth", "8", "-draw", "circle %d,%d %d,%d" % (center_x, center_y, center_x, center_y + radius)]
    arguments += ["-fill", "none", "-stroke", PAIRS[kind], "-strokewidth", "18"]
    for shape in face_symbol(kind, center_x, center_y):
        arguments += ["-draw", shape]
    return arguments


def word_button_arguments(key_font_path, word, cell_x, cell_width, points):
    left = scaled(cell_x) + 8
    right = scaled(cell_x + cell_width) - 8
    bottom = scaled(GLYPH_CELL) * 88 // 100
    top = scaled(GLYPH_CELL) * 12 // 100
    plate = ["-fill", INK_NAVY, "-stroke", WHITE, "-strokewidth", "8", "-draw", "roundrectangle %d,%d %d,%d 28,28" % (left, top, right, bottom)]
    word_image = ["(", "-size", "%dx%d" % (scaled(cell_width), scaled(GLYPH_CELL)), "xc:none", "-font", key_font_path, "-pointsize", str(points), "-gravity", "center", "-fill", WHITE, "-stroke", "none", "-annotate", "+0+%d" % KEY_WORD_NUDGE_Y, word, ")", "-gravity", "NorthWest", "-geometry", "+%d+0" % scaled(cell_x), "-composite"]
    return plate + word_image


def label_arguments(label_font_path, word, cell_x, row_y, cell_width=LABEL_WIDTH):
    return ["(", "-size", "%dx%d" % (scaled(cell_width), scaled(LABEL_HEIGHT)), "xc:none", "-font", label_font_path, "-pointsize", str(LABEL_POINTS), "-gravity", "center", "-fill", INK_NAVY, "-stroke", INK_NAVY, "-strokewidth", str(LABEL_OUTLINE), "-annotate", "+0+0", word, "-fill", WHITE, "-stroke", "none", "-annotate", "+0+0", word, ")", "-gravity", "NorthWest", "-geometry", "+%d+%d" % (scaled(cell_x), scaled(row_y)), "-composite"]


def wide_label_arguments(label_font_path):
    return ["(", "-size", "%dx%d" % (scaled(WIDE_LABEL_WIDTH), scaled(WIDE_LABEL_HEIGHT)), "xc:none", "-font", label_font_path, "-pointsize", str(WIDE_LABEL_POINTS), "-gravity", "center", "-fill", WHITE, "-stroke", "none", "-annotate", "+0+0", WIDE_LABEL, ")", "-gravity", "NorthWest", "-geometry", "+0+%d" % scaled(WIDE_LABEL_ROW_Y), "-composite"]


def sheet_arguments(label_font_path, key_font_path, output_path):
    arguments = ["magick", "-respect-parentheses", "-size", "%dx%d" % (scaled(SHEET_WIDTH), scaled(SHEET_HEIGHT)), "xc:none"]
    cell_x = 0
    for kind in FACE_ORDER:
        arguments += face_arguments(kind, cell_x)
        cell_x += GLYPH_CELL
    for letter in LETTER_ORDER:
        arguments += word_button_arguments(key_font_path, letter, cell_x, GLYPH_CELL, LETTER_POINTS)
        cell_x += GLYPH_CELL
    arguments += word_button_arguments(key_font_path, "START", cell_x, START_WIDTH, START_POINTS)
    arguments += word_button_arguments(key_font_path, "SELECT", cell_x + START_WIDTH, SELECT_WIDTH, SELECT_POINTS)
    for index, word in enumerate(LABELS):
        arguments += label_arguments(label_font_path, word, index * LABEL_WIDTH, LABEL_ROW_Y)
    for index, word in enumerate(SWAP_LABELS):
        arguments += label_arguments(label_font_path, word, index * SWAP_LABEL_WIDTH, SWAP_LABEL_ROW_Y, SWAP_LABEL_WIDTH)
    arguments += wide_label_arguments(label_font_path)
    arguments += ["-filter", "Box", "-resize", "%dx%d!" % (SHEET_WIDTH, SHEET_HEIGHT), "-depth", "8", "rgba:" + output_path]
    return arguments


def bake_hud_disc(work_folder, record, code):
    relative_path = "textures/" + DISC_NAME + ".gtex"
    key = record.key_for(code, {"size": DISC_SIZE}, b"")
    gtex_bytes = record.read_if_current(relative_path, key)
    if gtex_bytes is None:
        rgba_path = os.path.join(work_folder, DISC_NAME + ".rgba")
        middle = scaled(DISC_SIZE) // 2
        subprocess.run(["magick", "-size", "%dx%d" % (scaled(DISC_SIZE), scaled(DISC_SIZE)), "xc:none", "-fill", WHITE, "-draw", "circle %d,%d %d,%d" % (middle, middle, middle, scaled(DISC_SIZE) - 1), "-filter", "Box", "-resize", "%dx%d!" % (DISC_SIZE, DISC_SIZE), "-depth", "8", "rgba:" + rgba_path], check=True, capture_output=True)
        with open(rgba_path, "rb") as source:
            pixels = source.read()
        os.remove(rgba_path)
        gtex_bytes = asset_writers.gtex_bytes(DISC_SIZE, DISC_SIZE, asset_writers.FORMAT_RGBA8888, pixels)
        record.store(relative_path, key, gtex_bytes)
    return bake_record.result_of_own_size(DISC_NAME, gtex_bytes)


def bake_hud_sheet(fonts, work_folder, record, code, label_font, key_font):
    settings = {"label_font": bake_record.sha256_of_bytes(fonts.pack_bytes(label_font)), "key_font": bake_record.sha256_of_bytes(fonts.pack_bytes(key_font)), "width": SHEET_WIDTH, "height": SHEET_HEIGHT}
    relative_path = "textures/" + SHEET_NAME + ".gtex"
    key = record.key_for(code, settings, b"")
    gtex_bytes = record.read_if_current(relative_path, key)
    if gtex_bytes is None:
        rgba_path = os.path.join(work_folder, SHEET_NAME + ".rgba")
        subprocess.run(sheet_arguments(fonts.decoded_path(label_font), fonts.decoded_path(key_font), rgba_path), check=True, capture_output=True)
        with open(rgba_path, "rb") as source:
            pixels = source.read()
        os.remove(rgba_path)
        gtex_bytes = asset_writers.gtex_bytes(SHEET_WIDTH, SHEET_HEIGHT, asset_writers.FORMAT_RGBA8888, pixels)
        record.store(relative_path, key, gtex_bytes)
    return bake_record.result_of_own_size(SHEET_NAME, gtex_bytes)
