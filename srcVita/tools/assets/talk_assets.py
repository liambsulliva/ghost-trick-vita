import os
import re
import subprocess

import asset_writers
import bake_record
import chapter_assets
import line_renderer
import message_cues
import message_file
import narration
import texture_encoder

TALK_FILE = "scripts/talk.txt"
FONT_TEXTURE = "talk_font"
PIXELS_PER_EM = 30
LINE_PITCH = 36
BOTTOM_BOX_TOP = 412
BOTTOM_BOX_HEIGHT = 132
BOTTOM_TEXT_LEFT = 40
TOP_BOX_TOP = 3
TOP_BOX_HEIGHT = 144
TOP_TEXT_LEFT = 212
ATLAS_WIDTH = 1024
ATLAS_CELL_GAP = 4
COLOR_BLACK = 0
COLOR_OF_TAG = {"c1": 1, "c2": 2}
COLOR_SYSTEM_GREEN = 3
COLOR_SHIFT = 14
IGNORED_VERBS = ("s0", "mes_se_play", "mesquake_win")
QUAKE_LEVEL_OF_TYPE = {"iQuake::LEVEL_M": 1, "iQuake::LEVEL_L": 2}
QUAKE_LEVEL_UNLESS_NAMED = 1
QUAKE_FRAMES_UNLESS_NAMED = 40
ATTRIBUTE_PATTERN = re.compile(r"(\w+)=\"([^\"]*)\"")
NEW_PAGE_CODE = 0x02
FACE_CODE = 0x08
POSE_CODE = 0x19
HEAD_CODE = 0x1b
BOX_BOTTOM = 0
BOX_TOP_ALONE = 1
BOX_TOP_WITH_HEAD = 2
TOP_WITH_HEAD_TEXT_LEFT = 282
ENDING_PRESS = 0
ENDING_PAUSE_THEN_NEXT = 1
ENDING_PAUSE_THEN_CLOSE = 2
ENDING_HOLD_THEN_CLOSE = 3
TAG_PATTERN = re.compile(r"<\s*(\w+)\s+(\w+)((?:\s+\w+=\"[^\"]*\")*)\s*/?\s*>")
ICON_TAG_PATTERN = re.compile(r"<ICON (\w+)>")
ICON_CROSS = "\ue000"
ICON_L = "\ue001"
ICON_R = "\ue002"
ICON_START = "\ue003"
CHARACTER_OF_ICON = {"TRICK": ICON_CROSS, "GHOST": ICON_L, "BACK": ICON_L, "RESTART": ICON_R, "SAVE": ICON_START}
ICON_SIZE = 26
ICON_WIDE_SIZE = 56
ICON_GAP = 4
ICON_TOP_ABOVE_BASELINE = 23
ANY_TAG_PATTERN = re.compile(r"<[^>]*>")
ORIGIN_PATTERN = re.compile(r"origin: (-?[0-9.]+),")
NO_FACE = "-"
GLYPH_ROW_ADVANCE = 6
SUPERSAMPLE = line_renderer.SUPERSAMPLE


def new_section():
    return {"reveal_is_fade": 0, "ending": ENDING_PRESS, "flashes": 0, "quake_level": 0, "quake_frames": 0, "characters": []}


def add_plain_text(section, text, color):
    for character in text.replace("\r", ""):
        section["characters"].append((character, color))


def set_quake(section, attribute_text):
    attributes = dict(ATTRIBUTE_PATTERN.findall(attribute_text))
    kind = attributes.get("type")
    if kind is not None and kind not in QUAKE_LEVEL_OF_TYPE:
        raise message_file.MessageFileError("a talk page shakes at a level this tool does not know: " + kind)
    section["quake_level"] = QUAKE_LEVEL_OF_TYPE[kind] if kind is not None else QUAKE_LEVEL_UNLESS_NAMED
    section["quake_frames"] = int(attributes.get("frame", QUAKE_FRAMES_UNLESS_NAMED))


def apply_verb(sections, verb, color, attribute_text=""):
    section = sections[-1]
    if verb in COLOR_OF_TAG:
        return COLOR_OF_TAG[verb]
    if verb == "mes_white_in":
        section["reveal_is_fade"] = 1
    elif verb == "mes_pause":
        section["ending"] = ENDING_PAUSE_THEN_NEXT
    elif verb == "auto_pf":
        sections.append(new_section())
    elif verb == "auto_end":
        section["ending"] = ENDING_PAUSE_THEN_CLOSE
    elif verb == "mesflash":
        section["flashes"] = 1
    elif verb == "mesquake":
        set_quake(section, attribute_text)
    return color


def parse_page(raw_text):
    sections = [new_section()]
    color = COLOR_BLACK
    position = 0
    for match in ANY_TAG_PATTERN.finditer(raw_text):
        add_plain_text(sections[-1], raw_text[position:match.start()], color)
        position = match.end()
        icon = ICON_TAG_PATTERN.fullmatch(match.group(0))
        if icon is not None and icon.group(1) in CHARACTER_OF_ICON:
            sections[-1]["characters"].append((CHARACTER_OF_ICON[icon.group(1)], color))
            continue
        parts = TAG_PATTERN.fullmatch(match.group(0))
        if parts is None:
            continue
        if parts.group(1) == "TRCKEND":
            color = COLOR_BLACK
        else:
            color = apply_verb(sections, parts.group(2), color, parts.group(3))
    add_plain_text(sections[-1], raw_text[position:], color)
    return [section_lines(section) for section in sections]


def section_lines(section):
    lines = []
    current = []
    for character, color in section["characters"]:
        if character == "\n":
            lines.append(current)
            current = []
        else:
            current.append((character, color))
    lines.append(current)
    section["lines"] = [line for line in lines if "".join(character for character, _ in line).strip()]
    del section["characters"]
    return section


def measure_advance(font_path, work_folder, character):
    path = os.path.join(work_folder, "advance.txt")
    with open(path, "w", encoding="utf-8") as output:
        output.write(character)
    result = subprocess.run(["magick", "-size", "16x16", "xc:none", "-font", font_path, "-pointsize", "%g" % (PIXELS_PER_EM * SUPERSAMPLE), "-debug", "annotate", "-annotate", "+0+0", "@" + path, "null:"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    os.remove(path)
    origins = [float(value) for value in ORIGIN_PATTERN.findall(result.stderr.decode("utf-8", "replace"))]
    if result.returncode != 0 or not origins:
        raise line_renderer.LineRenderError("magick did not measure a glyph")
    return origins[-1] / SUPERSAMPLE


def icon_shape_arguments(character, font_path, size):
    middle = size // 2
    if character == ICON_CROSS:
        reach = size * 22 // 100
        return ["-fill", "white", "-stroke", "none", "-draw", "circle %d,%d %d,%d" % (middle, middle, middle, size - 2), "-fill", "none", "-stroke", "black", "-strokewidth", str(size // 9), "-draw", "line %d,%d %d,%d" % (middle - reach, middle - reach, middle + reach, middle + reach), "-draw", "line %d,%d %d,%d" % (middle - reach, middle + reach, middle + reach, middle - reach)]
    word = {ICON_L: "L", ICON_R: "R", ICON_START: "START"}[character]
    wide = ICON_WIDE_SIZE * SUPERSAMPLE if character == ICON_START else size
    return ["-fill", "white", "-stroke", "none", "-draw", "roundrectangle 0,%d %d,%d %d,%d" % (size // 10, wide - 1, size - size // 10, size // 5, size // 5), "-font", font_path, "-pointsize", str(size * 45 // 100 if character == ICON_START else size * 7 // 10), "-gravity", "center", "-fill", "black", "-annotate", "+0+0", word]


def white_pixels_with_alpha(alpha_values):
    pixels = bytearray(b"\xff\xff\xff\xff" * len(alpha_values))
    pixels[3::4] = alpha_values
    return pixels


def render_icon_glyph(font_path, work_folder, character):
    baseline_row, _ = line_renderer.line_box(PIXELS_PER_EM)
    width = ICON_WIDE_SIZE if character == ICON_START else ICON_SIZE
    gray_path = os.path.join(work_folder, "icon.gray")
    size = ICON_SIZE * SUPERSAMPLE
    arguments = ["magick", "-size", "%dx%d" % (width * SUPERSAMPLE, size), "xc:black"] + icon_shape_arguments(character, font_path, size) + ["-colorspace", "gray", "-filter", "Box", "-resize", "%dx%d!" % (width, ICON_SIZE), "-depth", "8", "gray:" + gray_path]
    line_renderer.run_magick(arguments)
    with open(gray_path, "rb") as source:
        pixels = white_pixels_with_alpha(source.read())
    os.remove(gray_path)
    return {"advance": width + ICON_GAP, "width": width, "height": ICON_SIZE, "left": 0, "top": baseline_row - ICON_TOP_ABOVE_BASELINE, "pixels": bytes(pixels)}


def render_glyph(font_path, work_folder, character):
    if character in CHARACTER_OF_ICON.values():
        return render_icon_glyph(font_path, work_folder, character)
    baseline_row, line_height = line_renderer.line_box(PIXELS_PER_EM)
    canvas_width = PIXELS_PER_EM * 4 * SUPERSAMPLE
    canvas_height = line_height * 2 * SUPERSAMPLE
    origin_x = PIXELS_PER_EM * SUPERSAMPLE
    path = os.path.join(work_folder, "glyph.txt")
    rgba_path = os.path.join(work_folder, "glyph.rgba")
    with open(path, "w", encoding="utf-8") as output:
        output.write(character)
    arguments = ["magick", "-size", "%dx%d" % (canvas_width, canvas_height), "xc:none", "-font", font_path, "-pointsize", "%g" % (PIXELS_PER_EM * SUPERSAMPLE), "-fill", "white", "-annotate", "+%d+%d" % (origin_x, baseline_row * SUPERSAMPLE), "@" + path]
    advance = measure_advance(font_path, work_folder, character)
    glyph = {"advance": int(round(advance)), "width": 0, "height": 0, "left": 0, "top": 0, "pixels": b""}
    if character.strip():
        left, top, width, height = line_renderer.read_ink_box(arguments)
        if width > 0 and height > 0:
            cell_left = left // SUPERSAMPLE
            cell_top = top // SUPERSAMPLE
            cell_width = (left + width + SUPERSAMPLE - 1) // SUPERSAMPLE - cell_left
            cell_height = (top + height + SUPERSAMPLE - 1) // SUPERSAMPLE - cell_top
            line_renderer.run_magick(arguments + ["-crop", "%dx%d+%d+%d" % (cell_width * SUPERSAMPLE, cell_height * SUPERSAMPLE, cell_left * SUPERSAMPLE, cell_top * SUPERSAMPLE), "+repage", "-filter", "Box", "-resize", "%dx%d!" % (cell_width, cell_height), "-depth", "8", "rgba:" + rgba_path])
            with open(rgba_path, "rb") as source:
                pixels = bytearray(source.read())
            os.remove(rgba_path)
            line_renderer.set_every_color(pixels, (255, 255, 255))
            glyph.update({"width": cell_width, "height": cell_height, "left": cell_left - origin_x // SUPERSAMPLE, "top": cell_top, "pixels": bytes(pixels)})
    os.remove(path)
    return glyph


def build_atlas(glyphs):
    x = ATLAS_CELL_GAP
    y = ATLAS_CELL_GAP
    row_height = 0
    placed = []
    for glyph in glyphs:
        if x + glyph["width"] + ATLAS_CELL_GAP > ATLAS_WIDTH:
            x = ATLAS_CELL_GAP
            y = y + row_height + ATLAS_CELL_GAP
            row_height = 0
        placed.append((x, y))
        x = x + ((glyph["width"] + 3) // 4 * 4) + ATLAS_CELL_GAP
        row_height = max(row_height, (glyph["height"] + 3) // 4 * 4)
    height = 4
    while height < y + row_height + ATLAS_CELL_GAP:
        height = height * 2
    pixels = bytearray(b"\xff\xff\xff\x00" * (ATLAS_WIDTH * height))
    for glyph, (cell_x, cell_y) in zip(glyphs, placed):
        for row in range(glyph["height"]):
            start = (cell_y + row) * ATLAS_WIDTH * 4 + cell_x * 4
            pixels[start:start + glyph["width"] * 4] = glyph["pixels"][row * glyph["width"] * 4:(row + 1) * glyph["width"] * 4]
    return bytes(pixels), height, placed


def bake_font_atlas(fonts, work_folder, record, code, font_path, characters):
    settings = {"font": bake_record.sha256_of_bytes(fonts.pack_bytes(font_path)), "pixels_per_em": PIXELS_PER_EM}
    relative_path = "textures/" + FONT_TEXTURE + ".gtex"
    key = record.key_for(code, settings, "".join(characters).encode("utf-8"))
    details = record.details_of(relative_path)
    current = record.read_if_current(relative_path, key) if "glyph_rows" in details else None
    if current is not None:
        return bake_record.result_of_own_size(FONT_TEXTURE, current), details["glyph_rows"]
    decoded_font = fonts.decoded_path(font_path)
    glyphs = [render_glyph(decoded_font, work_folder, character) for character in characters]
    pixels, height, placed = build_atlas(glyphs)
    blocks = texture_encoder.encode_rgba(work_folder, FONT_TEXTURE, ATLAS_WIDTH, height, pixels, "dxt5")
    gtex_bytes = asset_writers.gtex_bytes(ATLAS_WIDTH, height, asset_writers.FORMAT_DXT5, blocks)
    rows = [[cell_x, cell_y, glyph["width"], glyph["height"], glyph["left"], glyph["top"], glyph["advance"]] for glyph, (cell_x, cell_y) in zip(glyphs, placed)]
    record.store(relative_path, key, gtex_bytes, {"glyph_rows": rows})
    return bake_record.result_of_own_size(FONT_TEXTURE, gtex_bytes), rows


def layouts_by_ds_page(language_plain, offset):
    layouts = [None]
    for code, parameters in message_cues.control_codes(language_plain, offset):
        if code == FACE_CODE or code == POSE_CODE:
            layouts[-1] = {"code": code, "value": parameters[0]}
        elif code == HEAD_CODE:
            layouts[-1] = {"code": code, "value": parameters[0]}
        elif code == NEW_PAGE_CODE:
            layouts.append(None)
    return layouts


def read_ds_faces(section):
    import lmg_decode

    targets_by_plain = {}
    faces = {}
    for label, plain in section["language_files"].items():
        if plain is None:
            continue
        identity = id(plain)
        if identity not in targets_by_plain:
            targets_by_plain[identity] = message_cues.label_targets(lmg_decode.read_labels(plain, lmg_decode.read_header(plain)))
        if label in targets_by_plain[identity]:
            try:
                faces[label] = layouts_by_ds_page(plain, targets_by_plain[identity][label])
            except message_cues.CueError:
                pass
    return faces


def system_line_section(text, line):
    words = text.split(" ")
    wrapped = " ".join(words[:line["words_on_first_row"]]) + "\n" + " ".join(words[line["words_on_first_row"]:])
    section = new_section()
    section["ending"] = ENDING_HOLD_THEN_CLOSE
    section["center_x"] = line["center_x"]
    add_plain_text(section, wrapped, COLOR_SYSTEM_GREEN)
    return section_lines(section)


def centered_left(line, center_x, glyph_rows, glyph_of):
    width = sum(glyph_rows[glyph_of[character]][GLYPH_ROW_ADVANCE] for character, _ in line)
    return int(round(center_x - width / 2))


def layout_of_page(section, layouts, label, page_index):
    entries = layouts.get(label, [])
    entry = entries[page_index] if page_index < len(entries) else None
    changes_instantly = entry is not None and entry["code"] == POSE_CODE
    if entry is None and page_index > 0 and entries:
        entry = next((value for value in reversed(entries[:page_index]) if value is not None), None)
    if entry is None:
        return None, 0, BOX_BOTTOM, 0
    if entry["code"] == HEAD_CODE:
        head = section.get("head_faces", {}).get(str(entry["value"]))
        return head, 0, BOX_TOP_WITH_HEAD if head is not None else BOX_TOP_ALONE, 0
    if entry["value"] not in section["face_rows"]:
        return None, 0, BOX_BOTTOM, 0
    resource, flip = section["face_rows"][entry["value"]]
    return chapter_assets.face_texture_of(section["face_paths"], resource), flip, BOX_BOTTOM, 1 if changes_instantly else 0


def line_tops(line_count, box_top, box_height):
    baseline_row, _ = line_renderer.line_box(PIXELS_PER_EM)
    block_height = (line_count - 1) * LINE_PITCH + PIXELS_PER_EM
    first_ink_top = box_top + (box_height - block_height) / 2
    first_top = first_ink_top - (baseline_row - PIXELS_PER_EM * narration.TALLEST_INK_ABOVE_BASELINE_EM)
    return [int(round(first_top + index * LINE_PITCH)) for index in range(line_count)]


def read_all_pages(packs, section, text_key):
    entries = section["entries"]
    by_file = {}
    for label, pages in entries.items():
        for path, name in pages:
            by_file.setdefault(path, []).append(name)
    texts = {}
    for path, names in by_file.items():
        texts[path] = message_file.read_english_entries(packs.read(path), names, text_key)
    return {label: [texts[path][name] for path, name in pages] for label, pages in entries.items()}


def faces_the_set_will_use(packs, set_description, text_key):
    section = set_description.get("talk")
    if section is None or "entries" not in section:
        return []
    return parse_all_pages(packs, section, text_key)[2]


def parse_all_pages(packs, section, text_key):
    raw_pages = read_all_pages(packs, section, text_key)
    faces = read_ds_faces(section)
    parsed = {}
    characters = set()
    used_faces = []
    skipped = 0
    for label, pages in raw_pages.items():
        parsed[label] = []
        for page_index, raw_text in enumerate(pages):
            try:
                sections = parse_page(raw_text)
            except message_file.MessageFileError:
                skipped = skipped + 1
                continue
            face, flip, box_kind, changes_instantly = layout_of_page(section, faces, label, page_index)
            if face is not None and face not in used_faces:
                used_faces.append(face)
            parsed[label].append((sections, face, flip, box_kind, changes_instantly))
            for one_section in sections:
                for line in one_section["lines"]:
                    characters.update(character for character, _ in line)
    if skipped:
        print("talk pages skipped: %d" % skipped)
    for label, line in section.get("system_lines", {}).items():
        one_section = system_line_section(section["system_line_texts"][label], line)
        parsed[label] = [([one_section], None, 0, BOX_BOTTOM, 0)]
        for one_line in one_section["lines"]:
            characters.update(character for character, _ in one_line)
    return parsed, characters, used_faces


def bake_talk(packs, set_description, text_key, work_folder, record, code):
    section = set_description.get("talk")
    if section is None or "entries" not in section:
        return {}, [], [], []
    parsed, characters, used_faces = parse_all_pages(packs, section, text_key)
    ordered_characters = sorted(characters)
    fonts = narration.TextFonts(packs, work_folder)
    result, glyph_rows = bake_font_atlas(fonts, work_folder, record, code, section["font"], ordered_characters)
    glyph_of = {character: index for index, character in enumerate(ordered_characters)}
    rows = ["font " + FONT_TEXTURE]
    rows.extend("glyph " + " ".join(str(value) for value in row) for row in glyph_rows)
    for label, pages in parsed.items():
        rows.append("talk " + label)
        for sections, face, flip, box_kind, changes_instantly in pages:
            face_name = chapter_assets.face_texture_name(face) if face is not None else NO_FACE
            for one_section in sections:
                box_top = TOP_BOX_TOP if box_kind != BOX_BOTTOM else BOTTOM_BOX_TOP
                box_height = TOP_BOX_HEIGHT if box_kind != BOX_BOTTOM else BOTTOM_BOX_HEIGHT
                text_left = {BOX_BOTTOM: BOTTOM_TEXT_LEFT, BOX_TOP_ALONE: TOP_TEXT_LEFT, BOX_TOP_WITH_HEAD: TOP_WITH_HEAD_TEXT_LEFT}[box_kind]
                rows.append("page %s %d %d %d %d %d %d %d %d %d" % (face_name, flip, box_kind, one_section["reveal_is_fade"], one_section["ending"], len(one_section["lines"]), changes_instantly, one_section["flashes"], one_section["quake_level"], one_section["quake_frames"]))
                tops = line_tops(len(one_section["lines"]), box_top, box_height)
                for line, top in zip(one_section["lines"], tops):
                    codes = " ".join(str(glyph_of[character] | (color << COLOR_SHIFT)) for character, color in line)
                    line_left = centered_left(line, one_section["center_x"], glyph_rows, glyph_of) if "center_x" in one_section else text_left
                    rows.append("line %d %d %d %s" % (line_left, top, len(line), codes))
    return {TALK_FILE: ("\n".join(rows) + "\n").encode("ascii")}, [result], [], used_faces


def talk_texture_names(talk_files):
    return [FONT_TEXTURE] if talk_files else []


def print_talk_summary(talk_files):
    for text in talk_files.values():
        rows = text.decode("ascii").split("\n")
        print("talk: %d glyphs, %d messages, %d pages, %d lines" % (sum(row.startswith("glyph ") for row in rows), sum(row.startswith("talk ") for row in rows), sum(row.startswith("page ") for row in rows), sum(row.startswith("line ") for row in rows)))
