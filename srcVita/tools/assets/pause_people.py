import os
import re
import subprocess

import numpy as np

import asset_writers
import bake_record
import message_file
import narration
import frame_layout
import pause_art
import texture_encoder
import timeline_file

TABLE_PATH = "pause/people.txt"
THUMB_SHEET_NAME = "pause_people_thumbs"
SPOT_NAME = "pause_people_spot"
FACE_PREFIX = "pause_face_"
PLACE_PREFIX = "pause_place_"
NOTE_PREFIX = "pause_note_"
THUMB_SIZE = (104, 78)
FRAME_SIZE = (116, 90)
CELL_PITCH = (120, 92)
BIG_SIZE = (372, 280)
SPOT_SIZE = (440, 304)
SPOT_FLOOR = (20, 0, 0)
THUMB_COLUMNS = 8
CARD_SIZE = (448, 304)
BAND_HEIGHT = 48
TEXT_LEFT = 20
TEXT_TOP = 74
NOTE_STEP = 32
LINE_STEP = 27
TEXT_BOTTOM = 296
SMALLEST_POINTS = 15
INK = "#221e22"
BAND_EDGE = "#5a2814"
NOTE_RED = "#c41c1c"
LEVEL_PATTERN = re.compile(r"^(.+)_sub_(\d{3})$")
CAT_FACE = "cat"
CAT_HEIGHT = 336
CAT_RIGHT_GAP = 24
CAT_SHADOW_SHIFT = -44
CAT_LIT_FROM = 60
PORTRAIT_SIZE = (512, 384)
PORTRAIT_MAROON = (68, 13, 30)
PORTRAIT_SHADOW = (4, 52, 101)
PORTRAIT_EDGE_FADE = (60, 90)
PORTRAIT_SPLIT_SOFTNESS = 24.0


def label_of(name, kind, level):
    if kind == "title":
        return "%s_sub_%03d" % (name, level)
    if kind == "place":
        return "%s_sub2_%03d" % (name, level)
    return "%s_%03d" % (name, level)


def levels_by_name(entry_names, key_prefix):
    levels = {}
    for entry in entry_names:
        if not entry.startswith(key_prefix):
            continue
        match = LEVEL_PATTERN.match(entry[len(key_prefix):])
        if match:
            levels.setdefault(match.group(1), []).append(int(match.group(2)))
    return {name: sorted(set(found)) for name, found in levels.items()}


def plain(raw_text):
    return message_file.TAG_PATTERN.sub("", raw_text).replace("\r", "").strip("\n")


def note_texture_name(list_number, entry_id, level):
    return "%s%d_%02d_%03d" % (NOTE_PREFIX, list_number, entry_id, level)


def note_jobs(people, entry_names, key_prefix):
    levels = levels_by_name(entry_names, key_prefix)
    jobs = []
    for index, name in enumerate(people["names"]):
        for level in levels.get(name, []):
            jobs.append({"list": 1, "id": index + 1, "name": name, "level": level, "place": False})
    for index, name in enumerate(people["places"]):
        for level in levels.get(name, []):
            jobs.append({"list": 2, "id": index + 1, "name": name, "level": level, "place": True})
    for job in jobs:
        job["texture"] = note_texture_name(job["list"], job["id"], job["level"])
        job["keys"] = [key_prefix + label_of(job["name"], kind, job["level"]) for kind in (("title", "place", "body") if job["place"] else ("title", "body"))]
    return jobs


def magick_pixels(work_folder, arguments):
    size_text = subprocess.run(["magick"] + arguments + ["-format", "%w %h", "info:"], check=True, capture_output=True, text=True).stdout.split()
    width, height = int(size_text[0]), int(size_text[1])
    rgba_path = os.path.join(work_folder, "pause_people_text.rgba")
    subprocess.run(["magick"] + arguments + ["-depth", "8", "rgba:" + rgba_path], check=True, capture_output=True)
    with open(rgba_path, "rb") as source:
        pixels = np.frombuffer(source.read(), dtype=np.uint8).reshape(height, width, 4).copy()
    os.remove(rgba_path)
    return pixels


def text_file(work_folder, text):
    path = os.path.join(work_folder, "pause_people_text.txt")
    with open(path, "w", encoding="utf-8") as output:
        output.write(text)
    return path


def caption_pixels(work_folder, font_path, points, color, width, text):
    one = magick_pixels(work_folder, ["-background", "none", "-font", font_path, "-pointsize", str(points), "-size", "%dx" % width, "caption:A"])
    two = magick_pixels(work_folder, ["-background", "none", "-font", font_path, "-pointsize", str(points), "-size", "%dx" % width, "caption:A\nA"])
    spacing = LINE_STEP * points / 21.0 - (two.shape[0] - one.shape[0])
    path = text_file(work_folder, text)
    return magick_pixels(work_folder, ["-background", "none", "-fill", color, "-font", font_path, "-pointsize", str(points), "-interline-spacing", "%.2f" % spacing, "-size", "%dx" % width, "caption:@" + path])


def band_pixels(work_folder, font_path, points, text):
    path = text_file(work_folder, text)
    common = ["-background", "none", "-font", font_path, "-pointsize", str(points)]
    edge = magick_pixels(work_folder, common + ["-fill", BAND_EDGE, "-stroke", BAND_EDGE, "-strokewidth", "4", "label:@" + path])
    face = magick_pixels(work_folder, common + ["-fill", "#ffffff", "label:@" + path])
    out = edge.astype(np.float32)
    top = (edge.shape[0] - face.shape[0]) // 2
    left = (edge.shape[1] - face.shape[1]) // 2
    region = out[top:top + face.shape[0], left:left + face.shape[1]]
    weight = face[:, :, 3:4].astype(np.float32) / 255.0
    region[:, :, :3] = region[:, :, :3] * (1.0 - weight) + face[:, :, :3] * weight
    region[:, :, 3:4] = np.maximum(region[:, :, 3:4], face[:, :, 3:4])
    return out.clip(0, 255).astype(np.uint8)


def paste(card, pixels, left, top):
    height = min(pixels.shape[0], card.shape[0] - top)
    width = min(pixels.shape[1], card.shape[1] - left)
    if height <= 0 or width <= 0:
        return
    part = pixels[:height, :width].astype(np.float32)
    area = card[top:top + height, left:left + width].astype(np.float32)
    weight = part[:, :, 3:4] / 255.0
    area[:, :, :3] = area[:, :, :3] * (1.0 - weight) + part[:, :, :3] * weight
    area[:, :, 3:4] = np.maximum(area[:, :, 3:4], part[:, :, 3:4])
    card[top:top + height, left:left + width] = area.clip(0, 255).astype(np.uint8)


def card_pixels(work_folder, font_path, people, texts):
    card = np.zeros((CARD_SIZE[1], CARD_SIZE[0], 4), dtype=np.uint8)
    band = band_pixels(work_folder, font_path, people["band_points"], texts[1] if len(texts) == 3 else texts[0])
    paste(card, band, max(0, (CARD_SIZE[0] - band.shape[1]) // 2), max(0, (BAND_HEIGHT - band.shape[0]) // 2))
    top = TEXT_TOP
    if len(texts) == 3:
        note = magick_pixels(work_folder, ["-background", "none", "-fill", NOTE_RED, "-font", font_path, "-pointsize", str(people["text_points"]), "label:@" + text_file(work_folder, people["phone_word"] + " " + texts[0])])
        paste(card, note, TEXT_LEFT, top)
        top += NOTE_STEP
    width = CARD_SIZE[0] - 2 * TEXT_LEFT
    points = people["text_points"]
    body = caption_pixels(work_folder, font_path, points, INK, width, texts[-1])
    while body.shape[0] > TEXT_BOTTOM - top and points > SMALLEST_POINTS:
        points -= 1
        body = caption_pixels(work_folder, font_path, points, INK, width, texts[-1])
    paste(card, body, TEXT_LEFT, top)
    return card


def resized(work_folder, pixels, size, opaque_on=None):
    rgba_path = os.path.join(work_folder, "pause_people_art.rgba")
    out_path = os.path.join(work_folder, "pause_people_art_out.rgba")
    with open(rgba_path, "wb") as output:
        output.write(pixels.tobytes())
    arguments = ["magick", "-size", "%dx%d" % (pixels.shape[1], pixels.shape[0]), "-depth", "8", "rgba:" + rgba_path, "-filter", "Lanczos", "-resize", "%dx%d!" % size]
    if opaque_on is not None:
        arguments += ["-background", "rgb(%d,%d,%d)" % opaque_on, "-flatten"]
    subprocess.run(arguments + ["-depth", "8", "rgba:" + out_path], check=True, capture_output=True)
    with open(out_path, "rb") as source:
        out = np.frombuffer(source.read(), dtype=np.uint8).reshape(size[1], size[0], 4).copy()
    os.remove(rgba_path)
    os.remove(out_path)
    return out


def whole_art(packs, tool_path, work_folder, path):
    width, height = texture_encoder.read_tex_size(packs.read(path))
    return pause_art.remaster_page(packs, tool_path, work_folder, path, (0, 0, width, height))


def encoded(work_folder, name, pixels, compression):
    texture_format = asset_writers.FORMAT_DXT1 if compression == "dxt1" else asset_writers.FORMAT_DXT5
    blocks = texture_encoder.encode_rgba(work_folder, name, pixels.shape[1], pixels.shape[0], pixels.tobytes(), compression)
    return asset_writers.gtex_bytes(pixels.shape[1], pixels.shape[0], texture_format, blocks)


def face_name(face):
    return FACE_PREFIX + (face if face == CAT_FACE else "%02d" % face)


def reveal_portraits(people):
    return [face for reveal in people.get("reveals", []) for face in reveal["faces"].values() if face != CAT_FACE]


def art_sources(people):
    faces = [(FACE_PREFIX + "%02d" % index, people["portrait"] % index) for index in sorted(set(list(people["portraits"].values()) + reveal_portraits(people)))]
    places = [(PLACE_PREFIX + "%02d" % index, people["place_picture"] % index) for index in sorted(set(people["place_pictures"].values()))]
    return faces + places


def bake_art(packs, tool_path, work_folder, record, code, people):
    results = []
    thumbs = []
    for name, path in art_sources(people):
        data = packs.read(path)
        key = record.key_for(code, {"big": BIG_SIZE, "thumb": THUMB_SIZE}, data)
        relative_path = "textures/" + name + ".gtex"
        gtex = record.read_if_current(relative_path, key)
        art = None
        if gtex is None:
            art = whole_art(packs, tool_path, work_folder, path)
            gtex = encoded(work_folder, name, resized(work_folder, art, BIG_SIZE, (0, 0, 0)), "dxt1")
            record.store(relative_path, key, gtex)
        results.append(bake_record.result_of_own_size(name, gtex))
        thumbs.append((name, path))
    thumbs.append(("unknown", people["unknown"]))
    thumbs.append(("frame", people["frame"]))
    return results, thumbs


def cat_timeline_path(people, set_animations):
    wanted = people.get("cat_portrait")
    if wanted is None:
        return None
    for animation in set_animations:
        if animation["name"] == wanted["animation"]:
            return animation["timeline"]
    raise ValueError("Record cat portrait animation not in the set: " + wanted["animation"])


def cat_cell(packs, tool_path, work_folder, timeline_path, frame_index):
    timeline = timeline_file.decode_timeline(packs.read(timeline_path))
    crops = {}
    frame_layout.add_used_cells_to_crops(crops, timeline, None)
    frame = frame_layout.frames_for_animation(timeline, None, crops)[frame_index]
    paths = {frame_layout.texture_name_for(path): path for path in timeline["pages"]}
    page = pause_art.remaster_page(packs, tool_path, work_folder, frame_layout.pack_path_for(paths[frame["texture"]]), crops[frame["texture"]])
    return np.ascontiguousarray(pause_art.cell_pixels(page, frame, True))


def portrait_backdrop(lit_from):
    width, height = PORTRAIT_SIZE
    ys, xs = np.mgrid[0:height, 0:width].astype(np.float32)
    edge = np.minimum.reduce([ys / PORTRAIT_EDGE_FADE[0], (height - 1 - ys) / PORTRAIT_EDGE_FADE[0], (width - 1 - xs) / PORTRAIT_EDGE_FADE[1], np.ones_like(ys)])
    lit = np.clip((xs - lit_from) / PORTRAIT_SPLIT_SOFTNESS, 0.0, 1.0) * np.clip(edge, 0.0, 1.0) ** 0.8
    return np.array(PORTRAIT_MAROON, np.float32) * lit[..., None]


def lay_over(canvas, left, top, alpha, color):
    region = canvas[top:top + alpha.shape[0], left:left + alpha.shape[1]]
    region += (color - region) * alpha[:, :, None]


def cat_portrait(work_folder, cell):
    width = round(cell.shape[1] * CAT_HEIGHT / cell.shape[0])
    cat = resized(work_folder, cell, (width, CAT_HEIGHT))
    left = PORTRAIT_SIZE[0] - width - CAT_RIGHT_GAP
    top = PORTRAIT_SIZE[1] - CAT_HEIGHT
    canvas = portrait_backdrop(left + CAT_LIT_FROM)
    alpha = cat[:, :, 3].astype(np.float32) / 255.0
    lay_over(canvas, left + CAT_SHADOW_SHIFT, top, alpha, np.array(PORTRAIT_SHADOW, np.float32))
    lay_over(canvas, left, top, alpha, cat[:, :, :3].astype(np.float32))
    out = np.full((PORTRAIT_SIZE[1], PORTRAIT_SIZE[0], 4), 255, dtype=np.uint8)
    out[:, :, :3] = np.clip(canvas + 0.5, 0, 255).astype(np.uint8)
    return out


def bake_cat_face(packs, tool_path, work_folder, record, code, people, set_animations):
    timeline_path = cat_timeline_path(people, set_animations)
    if timeline_path is None:
        return [], {}
    name = face_name(CAT_FACE)
    settings = {"cat": people["cat_portrait"], "height": CAT_HEIGHT, "gap": CAT_RIGHT_GAP, "shadow": CAT_SHADOW_SHIFT, "lit": CAT_LIT_FROM, "colors": [PORTRAIT_MAROON, PORTRAIT_SHADOW], "fade": PORTRAIT_EDGE_FADE, "big": BIG_SIZE}
    timeline_bytes = packs.read(timeline_path)
    page_bytes = b"".join(packs.read(frame_layout.pack_path_for(path)) for path in timeline_file.decode_timeline(timeline_bytes)["pages"])
    key = record.key_for(code, settings, timeline_bytes + page_bytes)
    relative_path = "textures/" + name + ".gtex"
    gtex = record.read_if_current(relative_path, key)

    def art():
        return cat_portrait(work_folder, cat_cell(packs, tool_path, work_folder, timeline_path, people["cat_portrait"]["frame"]))
    if gtex is None:
        gtex = encoded(work_folder, name, resized(work_folder, art(), BIG_SIZE, (0, 0, 0)), "dxt1")
        record.store(relative_path, key, gtex)
    return [bake_record.result_of_own_size(name, gtex)], {name: (key.encode("utf-8") if isinstance(key, str) else bytes(key), art)}


def thumb_places(thumbs):
    places = []
    for index, (name, _) in enumerate(thumbs):
        size = FRAME_SIZE if name == "frame" else THUMB_SIZE
        places.append((name, (index % THUMB_COLUMNS) * CELL_PITCH[0], (index // THUMB_COLUMNS) * CELL_PITCH[1], size[0], size[1]))
    return places


def thumb_source(packs, tool_path, work_folder, composed, name, path):
    return composed[name][1]() if name in composed else whole_art(packs, tool_path, work_folder, path)


def bake_thumbs(packs, tool_path, work_folder, record, code, thumbs, composed):
    sources = b"".join(composed[name][0] if name in composed else packs.read(path) for name, path in thumbs)
    places = thumb_places(thumbs)
    key = record.key_for(code, {"places": places}, sources)
    relative_path = "textures/" + THUMB_SHEET_NAME + ".gtex"
    gtex = record.read_if_current(relative_path, key)
    if gtex is None:
        rows = (len(thumbs) + THUMB_COLUMNS - 1) // THUMB_COLUMNS
        sheet = np.zeros((rows * CELL_PITCH[1], THUMB_COLUMNS * CELL_PITCH[0], 4), dtype=np.uint8)
        for (name, path), (_, x, y, w, h) in zip(thumbs, places):
            art = thumb_source(packs, tool_path, work_folder, composed, name, path)
            sheet[y:y + h, x:x + w] = resized(work_folder, art, (w, h), None if name == "frame" else (0, 0, 0))
        gtex = encoded(work_folder, THUMB_SHEET_NAME, sheet, "dxt5")
        record.store(relative_path, key, gtex)
    return bake_record.result_of_own_size(THUMB_SHEET_NAME, gtex), ["thumb %s %d %d %d %d" % place for place in places]


def bake_spot(packs, tool_path, work_folder, record, code, people):
    data = packs.read(people["spotlight"])
    key = record.key_for(code, {"size": SPOT_SIZE, "floor": SPOT_FLOOR}, data)
    relative_path = "textures/" + SPOT_NAME + ".gtex"
    gtex = record.read_if_current(relative_path, key)
    if gtex is None:
        art = whole_art(packs, tool_path, work_folder, people["spotlight"])
        gtex = encoded(work_folder, SPOT_NAME, resized(work_folder, art, SPOT_SIZE, SPOT_FLOOR), "dxt1")
        record.store(relative_path, key, gtex)
    return bake_record.result_of_own_size(SPOT_NAME, gtex)


def bake_notes(packs, work_folder, record, code, people, font_path, font_bytes, text_key):
    data = packs.read(people["message_file"])
    entry_names = message_file.read_entry_names(data, text_key)
    jobs = note_jobs(people, entry_names, people["key_prefix"])
    wanted = sorted(set(key for job in jobs for key in job["keys"]))
    texts = message_file.read_english_entries(data, wanted, text_key)
    settings = {key: people[key] for key in ("band_points", "text_points", "phone_word")}
    settings.update({"card": CARD_SIZE, "font": bake_record.sha256_of_bytes(font_bytes)})
    results = []
    for job in jobs:
        parts = [plain(texts.get(key, "")) for key in job["keys"]]
        relative_path = "textures/" + job["texture"] + ".gtex"
        key = record.key_for(code, settings, "\u0000".join(parts).encode("utf-8"))
        gtex = record.read_if_current(relative_path, key)
        if gtex is None:
            gtex = encoded(work_folder, job["texture"], card_pixels(work_folder, font_path, people, parts), "dxt5")
            record.store(relative_path, key, gtex)
        results.append(bake_record.result_of_own_size(job["texture"], gtex))
    return results, ["note %d %d %d %s" % (job["list"], job["id"], job["level"], job["texture"]) for job in jobs]


def table_lines(people):
    names = people["names"]
    places = people["places"]
    lines = ["order " + " ".join(str(value) for value in people["order"])]
    lines.append("skip_people " + " ".join(str(value) for value in people["skip_people"]))
    lines.append("skip_places " + " ".join(str(value) for value in people["skip_places"]))
    lines.append("first_people " + " ".join(str(value) for value in people["first_people"]))
    lines.append("first_places " + " ".join(str(value) for value in people["first_places"]))
    lines.append("place_count %d" % len(places))
    for name, index in sorted(people["portraits"].items(), key=lambda item: names.index(item[0])):
        lines.append("face %d %s%02d" % (names.index(name) + 1, FACE_PREFIX, index))
    for name, index in sorted(people["place_pictures"].items(), key=lambda item: places.index(item[0])):
        lines.append("place %d %s%02d" % (places.index(name) + 1, PLACE_PREFIX, index))
    return lines + reveal_lines(people)


def reveal_lines(people):
    names = people["names"]
    lines = []
    for stage, reveal in enumerate(people.get("reveals", []), start=1):
        if "note" in reveal:
            lines.append("reveal %d note 1 %d %d" % (stage, names.index(reveal["note"][0]) + 1, reveal["note"][1]))
        else:
            lines.append("reveal %d talk %s" % (stage, reveal["talk"]))
        for name, face in sorted(reveal["faces"].items()):
            lines.append("reveal_face %d %d %s" % (stage, names.index(name) + 1, face_name(face)))
    return lines


def bake_people(packs, tool_path, work_folder, record, code, people, widget_font, text_key, set_animations=()):
    fonts = narration.TextFonts(packs, work_folder)
    font_path = fonts.decoded_path(widget_font)
    font_bytes = fonts.pack_bytes(widget_font)
    art_results, thumbs = bake_art(packs, tool_path, work_folder, record, code, people)
    cat_results, composed = bake_cat_face(packs, tool_path, work_folder, record, code, people, set_animations)
    thumbs = thumbs[:-2] + [(name, None) for name in composed] + thumbs[-2:]
    art_results = art_results + cat_results
    thumb_result, thumb_lines = bake_thumbs(packs, tool_path, work_folder, record, code, thumbs, composed)
    spot_result = bake_spot(packs, tool_path, work_folder, record, code, people)
    note_results, note_lines = bake_notes(packs, work_folder, record, code, people, font_path, font_bytes, text_key)
    table = ("\n".join(table_lines(people) + thumb_lines + note_lines) + "\n").encode("ascii")
    return table, art_results + [thumb_result, spot_result] + note_results
