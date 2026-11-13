import os
import re
import subprocess

import numpy as np

import asset_writers
import bake_record
import message_file
import narration
import pause_art
import pause_people

CLOCK_SHEET_NAME = "pause_clock"
CLOCK_TABLE_PATH = "pause/clock.txt"
MINUTES_PER_HOUR = 60
HOURS_PER_DAY = 24
NOON = 12
TIME_PATTERN = re.compile(r"^(\d{1,2}):(\d{2}) ([AP]M)$")
PLACE_LEVEL_PATTERN = re.compile(r"^(.+)_sub2_(\d{3})$")
UNKNOWN_PLACE = "???"
GLYPHS = [("d%d" % digit, str(digit)) for digit in range(10)] + [("colon", ":"), ("am", "AM"), ("pm", "PM")]


def minutes_of(text, evening_starts):
    match = TIME_PATTERN.match(text)
    if match is None:
        raise ValueError("clock time %r is not like 7:02 PM" % text)
    hour = int(match.group(1)) % NOON + (NOON if match.group(3) == "PM" else 0)
    minutes = hour * MINUTES_PER_HOUR + int(match.group(2))
    if minutes < evening_starts * MINUTES_PER_HOUR:
        minutes += HOURS_PER_DAY * MINUTES_PER_HOUR
    return minutes


def scene_minutes(clock):
    evening = clock["evening_starts_hour"]
    starts = {int(chapter): minutes_of(text, evening) for chapter, text in clock["chapter_times"].items()}
    night_ends = minutes_of(clock["night_ends"], evening)
    lines = []
    for chapter_text, scenes in sorted(clock["scenes"].items(), key=lambda item: int(item[0])):
        chapter = int(chapter_text)
        start = starts.get(chapter)
        if start is None:
            continue
        end = starts.get(chapter + 1, night_ends)
        for index, scene in enumerate(scenes):
            lines.append("scene %d %s %d" % (chapter, scene, start + (end - start) * index // len(scenes)))
    return lines


def place_names(packs, people, text_key):
    data = packs.read(people["message_file"])
    prefix = people["key_prefix"]
    entry_names = message_file.read_entry_names(data, text_key)
    wanted = {}
    for entry in entry_names:
        match = PLACE_LEVEL_PATTERN.match(entry[len(prefix):]) if entry.startswith(prefix) else None
        if match and match.group(1) in people["places"]:
            wanted.setdefault(match.group(1), []).append(entry)
    texts = message_file.read_english_entries(data, sorted(key for keys in wanted.values() for key in keys), text_key)
    names = {}
    for place, keys in wanted.items():
        for key in sorted(keys, reverse=True):
            text = pause_people.plain(texts.get(key, ""))
            if text and text != UNKNOWN_PLACE:
                names[place] = text
                break
    return names


def outlined_text_pixels(work_folder, font_path, points, text):
    outline = pause_art.LABEL_OUTLINE
    size = subprocess.run(["magick", "-background", "none", "-font", font_path, "-pointsize", str(points), "label:" + text, "-format", "%w %h", "info:"], check=True, capture_output=True, text=True).stdout.split()
    width, height = int(size[0]) + 2 * outline, int(size[1]) + 2 * outline
    centre = "+%d+%d" % (outline, outline)
    rgba_path = os.path.join(work_folder, "pause_clock.rgba")
    subprocess.run(["magick", "-size", "%dx%d" % (width, height), "xc:none", "-gravity", "NorthWest", "-font", font_path, "-pointsize", str(points), "-fill", pause_art.NAVY, "-stroke", pause_art.NAVY, "-strokewidth", str(2 * outline), "-annotate", centre, text, "-fill", pause_art.CREAM, "-stroke", "none", "-annotate", centre, text, "-depth", "8", "rgba:" + rgba_path], check=True, capture_output=True)
    with open(rgba_path, "rb") as source:
        pixels = np.frombuffer(source.read(), dtype=np.uint8).reshape(height, width, 4).copy()
    os.remove(rgba_path)
    return pixels


def clock_cells(work_folder, font_path, clock, names):
    cells = [("glyph", name, outlined_text_pixels(work_folder, font_path, clock["time_points"], text)) for name, text in GLYPHS]
    for place in sorted(names):
        cells.append(("place", place, outlined_text_pixels(work_folder, font_path, clock["place_points"], names[place])))
    return cells


def bake_clock(packs, work_folder, record, code, clock, label_font, people, text_key):
    fonts = narration.TextFonts(packs, work_folder)
    names = place_names(packs, people, text_key)
    scene_lines = scene_minutes(clock)
    key = record.key_for(code, {"clock": clock, "names": names, "outline": pause_art.LABEL_OUTLINE}, fonts.pack_bytes(label_font))
    relative_path = "textures/" + CLOCK_SHEET_NAME + ".gtex"
    gtex = record.read_if_current(relative_path, key)
    table = record.read_if_current(CLOCK_TABLE_PATH, key)
    if gtex is None or table is None:
        sheet, lines = pause_art.pack_widget_cells(clock_cells(work_folder, fonts.decoded_path(label_font), clock, names))
        table = ("\n".join(lines + scene_lines) + "\n").encode("utf-8")
        gtex = asset_writers.gtex_bytes(sheet.shape[1], sheet.shape[0], asset_writers.FORMAT_RGBA8888, sheet.tobytes())
        record.store(relative_path, key, gtex)
        record.store(CLOCK_TABLE_PATH, key, table)
    return table, bake_record.result_of_own_size(CLOCK_SHEET_NAME, gtex)
