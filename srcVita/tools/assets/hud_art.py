import os
import subprocess

import asset_writers
import bake_record

SHEET_NAME = "hud_sheet"
SHEET_WIDTH = 512
SHEET_HEIGHT = 128
SUPERSAMPLE = 4
GLYPH_CELL = 40
START_WIDTH = 72
LABEL_ROW_Y = 48
LABEL_WIDTH = 160
LABEL_HEIGHT = 64
INK_NAVY = "#1b2a3a"
WHITE = "#ffffff"
PAIRS = {"cross": "#8fb8ff", "circle": "#ff7a7a", "square": "#ff9ad5", "triangle": "#6fe0b0"}
FACE_ORDER = ["cross", "circle", "square", "triangle"]
LETTER_ORDER = ["L", "R"]
LABELS = ["Ghost", "Trick", "Back"]
LABEL_POINTS = 180
LABEL_OUTLINE = 14
KEY_WORD_NUDGE_Y = 8
LETTER_POINTS = 112
START_POINTS = 64


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


def label_arguments(label_font_path, word, cell_x):
    return ["(", "-size", "%dx%d" % (scaled(LABEL_WIDTH), scaled(LABEL_HEIGHT)), "xc:none", "-font", label_font_path, "-pointsize", str(LABEL_POINTS), "-gravity", "center", "-fill", INK_NAVY, "-stroke", INK_NAVY, "-strokewidth", str(LABEL_OUTLINE), "-annotate", "+0+0", word, "-fill", WHITE, "-stroke", "none", "-annotate", "+0+0", word, ")", "-gravity", "NorthWest", "-geometry", "+%d+%d" % (scaled(cell_x), scaled(LABEL_ROW_Y)), "-composite"]


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
    for index, word in enumerate(LABELS):
        arguments += label_arguments(label_font_path, word, index * LABEL_WIDTH)
    arguments += ["-filter", "Box", "-resize", "%dx%d!" % (SHEET_WIDTH, SHEET_HEIGHT), "-depth", "8", "rgba:" + output_path]
    return arguments


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
