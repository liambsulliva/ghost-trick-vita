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
ATLAS_WIDTHS = (1024, 2048, 4096)
ATLAS_HEIGHT_LIMIT = 2048
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
FACE_ATTRIBUTE = "CutInOrFace"
FADE_ATTRIBUTE = "CutInFade"
CUTIN_SYMBOL_PREFIX = "iCutin::"
HEAD_SYMBOL_PREFIX = "iFacePanel::"
BOX_BOTTOM = 0
BOX_TOP_ALONE = 1
BOX_TOP_WITH_HEAD = 2
TOP_WITH_HEAD_TEXT_LEFT = 282
BOX_LEFT = {BOX_BOTTOM: 0.0, BOX_TOP_ALONE: 188.0, BOX_TOP_WITH_HEAD: 255.5}
BOX_TOP_EDGE = {BOX_BOTTOM: BOTTOM_BOX_TOP, BOX_TOP_ALONE: TOP_BOX_TOP, BOX_TOP_WITH_HEAD: TOP_BOX_TOP}
BOX_HEIGHT_OF = {BOX_BOTTOM: BOTTOM_BOX_HEIGHT, BOX_TOP_ALONE: TOP_BOX_HEIGHT, BOX_TOP_WITH_HEAD: TOP_BOX_HEIGHT}
TEXT_RIGHT = 960 - BOTTOM_TEXT_LEFT
ENDING_PRESS = 0
ENDING_PAUSE_THEN_NEXT = 1
ENDING_PAUSE_THEN_CLOSE = 2
ENDING_HOLD_THEN_CLOSE = 3
TAG_PATTERN = re.compile(r"<\s*(\w+)\s+(\w+)((?:\s+\w+=\"[^\"]*\")*)\s*/?\s*>")
ICON_TAG_PATTERN = re.compile(r"<ICON (\w+)>")
ICON_CROSS = "\ue000"
ICON_L = "\ue001"
ICON_R = "\ue002"
ICON_SELECT = "\ue003"
BUTTON_OF_ICON = {"TRICK": "CROSS", "GHOST": "L", "BACK": "L", "RESTART": "R", "SAVE": "SELECT"}
CHARACTER_OF_BUTTON = {"CROSS": ICON_CROSS, "L": ICON_L, "R": ICON_R, "SELECT": ICON_SELECT}
CHARACTER_OF_ICON = {icon: CHARACTER_OF_BUTTON[button] for icon, button in BUTTON_OF_ICON.items()}
ICON_SIZE = 26
ICON_WIDE_SIZE = 66
ICON_GAP = 4
ICON_TOP_ABOVE_BASELINE = 23
ANY_TAG_PATTERN = re.compile(r"<[^>]*>")
ORIGIN_PATTERN = re.compile(r"origin: (-?[0-9.]+),")
NO_FACE = "-"
GLYPH_ROW_ADVANCE = 6
SUPERSAMPLE = line_renderer.SUPERSAMPLE


def new_section():
    return {"reveal_is_fade": 0, "ending": ENDING_PRESS, "flashes": 0, "quake_level": 0, "quake_frames": 0, "pauses_before": 0, "pauses_so_far": 0, "characters": []}


def has_text(section):
    return any(not character.isspace() for character, _ in section["characters"])


def note_text_after_pauses(section):
    if not has_text(section) and section["pauses_so_far"] > 0:
        section["pauses_before"] = section["pauses_so_far"]
        section["ending"] = ENDING_PRESS


def add_plain_text(section, text, color):
    if text.strip():
        note_text_after_pauses(section)
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
        section["pauses_so_far"] += 1
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
            note_text_after_pauses(sections[-1])
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
    del section["pauses_so_far"]
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
    word = {ICON_L: "L", ICON_R: "R", ICON_SELECT: "SELECT"}[character]
    wide = ICON_WIDE_SIZE * SUPERSAMPLE if character == ICON_SELECT else size
    return ["-fill", "white", "-stroke", "none", "-draw", "roundrectangle 0,%d %d,%d %d,%d" % (size // 10, wide - 1, size - size // 10, size // 5, size // 5), "-font", font_path, "-pointsize", str(size * 45 // 100 if character == ICON_SELECT else size * 7 // 10), "-gravity", "center", "-fill", "black", "-annotate", "+0+0", word]


def white_pixels_with_alpha(alpha_values):
    pixels = bytearray(b"\xff\xff\xff\xff" * len(alpha_values))
    pixels[3::4] = alpha_values
    return pixels


def render_icon_glyph(font_path, work_folder, character):
    baseline_row, _ = line_renderer.line_box(PIXELS_PER_EM)
    width = ICON_WIDE_SIZE if character == ICON_SELECT else ICON_SIZE
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


def packed_places(glyphs, width):
    x = ATLAS_CELL_GAP
    y = ATLAS_CELL_GAP
    row_height = 0
    placed = []
    for glyph in glyphs:
        if x + glyph["width"] + ATLAS_CELL_GAP > width:
            x = ATLAS_CELL_GAP
            y = y + row_height + ATLAS_CELL_GAP
            row_height = 0
        placed.append((x, y))
        x = x + ((glyph["width"] + 3) // 4 * 4) + ATLAS_CELL_GAP
        row_height = max(row_height, (glyph["height"] + 3) // 4 * 4)
    height = 4
    while height < y + row_height + ATLAS_CELL_GAP:
        height = height * 2
    return placed, height


def atlas_width_for(glyphs):
    for width in ATLAS_WIDTHS:
        if packed_places(glyphs, width)[1] <= ATLAS_HEIGHT_LIMIT:
            return width
    return ATLAS_WIDTHS[-1]


def build_atlas(glyphs):
    width = atlas_width_for(glyphs)
    placed, height = packed_places(glyphs, width)
    pixels = bytearray(b"\xff\xff\xff\x00" * (width * height))
    for glyph, (cell_x, cell_y) in zip(glyphs, placed):
        for row in range(glyph["height"]):
            start = (cell_y + row) * width * 4 + cell_x * 4
            pixels[start:start + glyph["width"] * 4] = glyph["pixels"][row * glyph["width"] * 4:(row + 1) * glyph["width"] * 4]
    return bytes(pixels), width, height, placed


def bake_font_atlas(fonts, work_folder, record, code, font_path, characters, font_texture=FONT_TEXTURE):
    settings = {"font": bake_record.sha256_of_bytes(fonts.pack_bytes(font_path)), "pixels_per_em": PIXELS_PER_EM}
    if fonts.fallbacks.get(font_path):
        settings["fallbacks"] = [bake_record.sha256_of_bytes(fonts.pack_bytes(path)) for path in fonts.fallbacks[font_path]]
    relative_path = "textures/" + font_texture + ".gtex"
    key = record.key_for(code, settings, "".join(characters).encode("utf-8"))
    details = record.details_of(relative_path)
    current = record.read_if_current(relative_path, key) if "glyph_rows" in details else None
    if current is not None:
        return bake_record.result_of_own_size(font_texture, current), details["glyph_rows"]
    fonts.require(font_path, "".join(character for character in characters if character not in CHARACTER_OF_ICON.values()), may_fall_back=True)
    decoded_font = fonts.decoded_path(font_path)
    glyphs = [render_glyph(decoded_font if character in CHARACTER_OF_ICON.values() else fonts.decoded_path(fonts.font_for(font_path, character)), work_folder, character) for character in characters]
    pixels, width, height, placed = build_atlas(glyphs)
    blocks = texture_encoder.encode_rgba(work_folder, font_texture, width, height, pixels, "dxt5")
    gtex_bytes = asset_writers.gtex_bytes(width, height, asset_writers.FORMAT_DXT5, blocks)
    rows = [[cell_x, cell_y, glyph["width"], glyph["height"], glyph["left"], glyph["top"], glyph["advance"]] for glyph, (cell_x, cell_y) in zip(glyphs, placed)]
    record.store(relative_path, key, gtex_bytes, {"glyph_rows": rows})
    return bake_record.result_of_own_size(font_texture, gtex_bytes), rows


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


def layout_of_attributes(attributes, symbols):
    value = attributes.get(FACE_ATTRIBUTE, "")
    if value.startswith(CUTIN_SYMBOL_PREFIX) and value in symbols:
        return {"code": POSE_CODE if attributes.get(FADE_ATTRIBUTE) else FACE_CODE, "value": symbols[value]}
    if value.startswith(HEAD_SYMBOL_PREFIX) and value in symbols:
        return {"code": HEAD_CODE, "value": symbols[value]}
    return None


def read_remaster_faces(packs, section, text_key):
    attributes = {}
    for path in sorted({path for pages in section["entries"].values() for path, _ in pages}):
        attributes.update(message_file.read_attributes(packs.read(path), text_key))
    return {label: [layout_of_attributes(attributes.get(name, {}), section["face_symbols"]) for _, name in pages] for label, pages in section["entries"].items()}


def faces_of(packs, section, text_key):
    return read_remaster_faces(packs, section, text_key) if "face_symbols" in section else read_ds_faces(section)


def system_line_section(text, line):
    words = text.split(" ")
    wrapped = text if "\n" in text else " ".join(words[:line["words_on_first_row"]]) + "\n" + " ".join(words[line["words_on_first_row"]:])
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
    path, flip = section["face_rows"][entry["value"]]
    return path, flip, BOX_BOTTOM, 1 if changes_instantly else 0


def glyph_shapes_of_rows(rows):
    return [tuple(int(value) for value in row.split()[1:]) for row in rows if row.startswith("glyph ")]


def ink_of_line(fields, shapes):
    x, y = int(fields[1]), int(fields[2])
    inked = [shapes[int(code) & ((1 << COLOR_SHIFT) - 1)] for code in fields[4:]]
    inked = [shape for shape in inked if shape[2] * shape[3] > 0]
    if not inked:
        return None
    return x + inked[0][4], y + min(shape[5] for shape in inked), y + max(shape[5] + shape[3] for shape in inked)


def pages_of_rows(rows, fixed_talks):
    pages = []
    label = None
    for index, row in enumerate(rows):
        fields = row.split()
        if not fields:
            continue
        if fields[0] == "talk":
            label = fields[1]
        elif fields[0] == "page":
            pages.append({"box": int(fields[3]), "fixed": label in fixed_talks, "lines": []})
        elif fields[0] == "line" and pages:
            pages[-1]["lines"].append(index)
    return pages


def even_left_margins(rows, fixed_talks=()):
    shapes = glyph_shapes_of_rows(rows)
    pages = pages_of_rows(rows, set(fixed_talks))
    for page in pages:
        inks = [ink_of_line(rows[index].split(), shapes) for index in page["lines"]]
        inks = [ink for ink in inks if ink is not None]
        page["ink"] = (min(ink[0] for ink in inks), min(ink[1] for ink in inks), max(ink[2] for ink in inks)) if inks else None
    margins = {}
    for box in BOX_LEFT:
        movable = [page for page in pages if page["box"] == box and not page["fixed"] and page["ink"] is not None and page["ink"][1] >= BOX_TOP_EDGE[box] and page["ink"][2] <= BOX_TOP_EDGE[box] + BOX_HEIGHT_OF[box]]
        if not movable:
            continue
        most = max(len(page["lines"]) for page in movable)
        full = [page for page in movable if len(page["lines"]) == most]
        gaps = [((page["ink"][1] - BOX_TOP_EDGE[box]) + (BOX_TOP_EDGE[box] + BOX_HEIGHT_OF[box] - page["ink"][2])) / 2.0 for page in full]
        margins[box] = sorted(gaps)[len(gaps) // 2]
    result = list(rows)
    for page in pages:
        if page["fixed"] or page["ink"] is None or page["box"] not in margins:
            continue
        shift = int(round(BOX_LEFT[page["box"]] + margins[page["box"]] - page["ink"][0]))
        for index in page["lines"]:
            fields = result[index].split(" ")
            fields[1] = str(int(fields[1]) + shift)
            result[index] = " ".join(fields)
    return result, margins


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
        texts[path] = message_file.read_entries(packs.read(path), names, text_key)
    return {label: [texts[path][name] for path, name in pages] for label, pages in entries.items()}


def faces_the_set_will_use(packs, set_description, text_key):
    section = set_description.get("talk")
    if section is None or "entries" not in section:
        return []
    return parse_all_pages(packs, section, text_key)[2]


def parse_all_pages(packs, section, text_key):
    raw_pages = read_all_pages(packs, section, text_key)
    faces = faces_of(packs, section, text_key)
    left_out = set(section.get("pages_left_out", []))
    every_name = {name for pages in section["entries"].values() for _, name in pages}
    if left_out - every_name:
        raise message_file.MissingEntryError("pages_left_out names no talk page: " + ", ".join(sorted(left_out - every_name)))
    parsed = {}
    characters = set()
    used_faces = []
    skipped = 0
    for label, pages in raw_pages.items():
        parsed[label] = []
        for page_index, raw_text in enumerate(pages):
            if section["entries"][label][page_index][1] in left_out:
                continue
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


def bake_talk(packs, set_description, text_key, work_folder, record, code, font_texture=FONT_TEXTURE):
    section = set_description.get("talk")
    if section is None or "entries" not in section:
        return {}, [], [], []
    parsed, characters, used_faces = parse_all_pages(packs, section, text_key)
    ordered_characters = sorted(characters)
    fonts = narration.TextFonts(packs, work_folder, set_description.get("font_fallbacks"))
    result, glyph_rows = bake_font_atlas(fonts, work_folder, record, code, section["font"], ordered_characters, font_texture)
    glyph_of = {character: index for index, character in enumerate(ordered_characters)}
    failures = []
    rows = ["font " + font_texture]
    rows.extend("glyph " + " ".join(str(value) for value in row) for row in glyph_rows)
    for label, pages in parsed.items():
        rows.append("talk " + label)
        for sections, face, flip, box_kind, changes_instantly in pages:
            face_name = chapter_assets.face_texture_name(face) if face is not None else NO_FACE
            for one_section in sections:
                box_top = TOP_BOX_TOP if box_kind != BOX_BOTTOM else BOTTOM_BOX_TOP
                box_height = TOP_BOX_HEIGHT if box_kind != BOX_BOTTOM else BOTTOM_BOX_HEIGHT
                text_left = {BOX_BOTTOM: BOTTOM_TEXT_LEFT, BOX_TOP_ALONE: TOP_TEXT_LEFT, BOX_TOP_WITH_HEAD: TOP_WITH_HEAD_TEXT_LEFT}[box_kind]
                rows.append("page %s %d %d %d %d %d %d %d %d %d %d" % (face_name, flip, box_kind, one_section["reveal_is_fade"], one_section["ending"], len(one_section["lines"]), changes_instantly, one_section["flashes"], one_section["quake_level"], one_section["quake_frames"], one_section["pauses_before"]))
                tops = line_tops(len(one_section["lines"]), box_top, box_height)
                for line, top in zip(one_section["lines"], tops):
                    codes = " ".join(str(glyph_of[character] | (color << COLOR_SHIFT)) for character, color in line)
                    line_left = centered_left(line, one_section["center_x"], glyph_rows, glyph_of) if "center_x" in one_section else text_left
                    line_width = sum(glyph_rows[glyph_of[character]][GLYPH_ROW_ADVANCE] for character, _ in line)
                    if line_left + line_width > TEXT_RIGHT:
                        failures.append((label, "a line is %d px wider than its box" % (line_left + line_width - TEXT_RIGHT)))
                    rows.append("line %d %d %d %s" % (line_left, top, len(line), codes))
    rows, _ = even_left_margins(rows, list(section.get("system_lines", {})))
    return {TALK_FILE: ("\n".join(rows) + "\n").encode("ascii")}, [result], failures, used_faces


def talk_texture_names(talk_files):
    names = []
    for text in talk_files.values():
        first_row = text.split(b"\n", 1)[0].decode("ascii").split()
        names.append(first_row[1] if len(first_row) == 2 and first_row[0] == "font" else FONT_TEXTURE)
    return names


def print_talk_summary(talk_files):
    for text in talk_files.values():
        rows = text.decode("ascii").split("\n")
        print("talk: %d glyphs, %d messages, %d pages, %d lines" % (sum(row.startswith("glyph ") for row in rows), sum(row.startswith("talk ") for row in rows), sum(row.startswith("page ") for row in rows), sum(row.startswith("line ") for row in rows)))
