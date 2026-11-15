import hashlib
import struct

import asset_writers
import bake_record
import chapter_assets
import ds_rom
import ds_sprites
import ds_text
import lz
import narration
import talk_assets

TEXTURE_FOLDER = "textures"
CODE_FILES = ["ds_set_text.py", "ds_text.py"]
DS_STEP = 16
SCREEN_STEP = 45
CAMERA_LEFT_ON_SCREEN = 120
CAMERA_TOP_ON_SCREEN = 2
SCREEN_CENTER_X = 480
RGBA_BYTES = 4
PACK_PATH = "cpac_2d.bin"
TEXT_PALETTE_BLOCK = 0
TEXT_PALETTE_ENTRY = 5
PALETTE_COLORS = 16
NARRATION_COLOR = 3
TALK_COLOR = 15
COLOR_GROUP_SIZE = 3
WINDOW_TABLE_ADDRESS = 0x02084a8c
WINDOW_ROW_SIZE = 18
NARRATION_ROW = 3
BOTTOM_ROW = 0
TOP_ROW = 4
NARRATION_WIDTH = 0x100
WINDOW_LIFT_OFFSET = -1
SMALL_KIND_LIFT = -1
LARGE_KIND_LIFT = -7
SMALL_KIND_LIMIT = 2
POS_Y_CODE = 0x20
MONO_TYPE_CODE = 0x22
FLASH_CODE = 0x0f
SHAKE_CODE = 0x10
PAUSE_CODE = 0x21
AUTO_END_CODE = 0x23
WHITE_IN_CODE = 0x24
TINT_OF_DS_COLOR = {15: 0, 6: 1, 9: 2, 12: 3, 3: 0}
SYSTEM_LINE_TINT = talk_assets.COLOR_SYSTEM_GREEN
FULL_ALPHA = 255
WHITE = b"\xff\xff\xff"
CLEAR = b"\x00\x00\x00\x00"
GLYPH_LIMIT = 512


class DsSetTextError(ValueError):
    pass


def screen_of(ds_pixel):
    return -((-ds_pixel * SCREEN_STEP) // DS_STEP)


def ds_of(screen_pixel):
    return (screen_pixel * DS_STEP) // SCREEN_STEP


def screen_x_of(ds_x):
    return CAMERA_LEFT_ON_SCREEN + screen_of(ds_x)


def screen_y_of(ds_y):
    return CAMERA_TOP_ON_SCREEN + screen_of(ds_y)


def read_window_rows(arm9):
    rows = {}
    for row in (BOTTOM_ROW, NARRATION_ROW, TOP_ROW):
        x, y, width, height, lift, left, *tops = struct.unpack_from("<9h", arm9, WINDOW_TABLE_ADDRESS - ds_text.ARM9_LOAD_ADDRESS + WINDOW_ROW_SIZE * row)
        rows[row] = {"x": x, "y": y, "width": width, "height": height, "lift": lift, "left": left, "tops": tops}
    return rows


def top_of_kind(window, kind):
    if kind == 1:
        return window["tops"][0]
    if kind == 2:
        return window["tops"][1]
    return window["tops"][2]


def lift_of_kind(kind):
    return SMALL_KIND_LIFT if kind <= SMALL_KIND_LIMIT else LARGE_KIND_LIFT


def text_palette(palette_bytes):
    words = struct.unpack_from("<%dH" % PALETTE_COLORS, palette_bytes)
    return [bytes(color) for color in ds_text.palette_of_bgr555(words)]


def read_text_palette(pack_bytes):
    return text_palette(ds_sprites.SpritePack(pack_bytes).entry(TEXT_PALETTE_BLOCK, TEXT_PALETTE_ENTRY))


def ink_alphas(palette, color):
    alphas = {}
    for nibble in range(1, COLOR_GROUP_SIZE + 1):
        red, green, blue, _ = palette[nibble + color - COLOR_GROUP_SIZE]
        alphas[nibble] = FULL_ALPHA - (red + green + blue + 1) // 3
    return alphas


def first_parameter(items, code):
    for item in items:
        if item[0] == "control" and item[1] == code and item[2]:
            return item[2][0]
    return None


def has_glyphs(line):
    return any(item[0] == "char" for item in line["items"])


def font_digest(font):
    digest = hashlib.sha256()
    for part in (font.glyphs, font.widths, font.profiles, repr(font.shape).encode("ascii"), repr(sorted(font.icon_widths.items())).encode("ascii")):
        digest.update(part)
    return digest.hexdigest()


def scaled_rgba(ds_pixels, left, top, width, height):
    rows = {}
    for x, y, color in ds_pixels:
        rows.setdefault(y, {})[x] = color
    column_sources = [ds_of(left + column - CAMERA_LEFT_ON_SCREEN) for column in range(width)]
    rgba = bytearray()
    for row in range(height):
        source = rows.get(ds_of(top + row - CAMERA_TOP_ON_SCREEN), {})
        rgba.extend(b"".join(source.get(column, CLEAR) for column in column_sources))
    return bytes(rgba)


def narration_line_image(font, palette, items, color, window, ds_top):
    pixels, _, _, color = ds_text.draw_items(font, items, color)
    width = ds_text.measured_width(font, items)
    ds_left = window["x"] + max((NARRATION_WIDTH - width) // 2, 0)
    if not pixels:
        return None, color
    placed = [(ds_left + x, ds_top + y, palette[index]) for x, y, index in pixels]
    left = screen_x_of(min(x for x, _, _ in placed))
    right = screen_x_of(max(x for x, _, _ in placed) + 1)
    top = screen_y_of(min(y for _, y, _ in placed))
    bottom = screen_y_of(max(y for _, y, _ in placed) + 1)
    half = max(SCREEN_CENTER_X - left, right - SCREEN_CENTER_X)
    texture_left = SCREEN_CENTER_X - half
    image = {"width": 2 * half, "height": bottom - top, "top": top, "ink_width": right - left}
    image["rgba"] = scaled_rgba(placed, texture_left, top, image["width"], image["height"])
    return image, color


def narration_pitch(font, window):
    return font.shape["line_height"] + window["lift"] + WINDOW_LIFT_OFFSET


def narration_group_gap(font, window):
    return 2 * narration_pitch(font, window) - font.shape["line_height"]


def narration_tops(font, page, pos_y, window):
    pitch = narration_pitch(font, window)
    group_top = pos_y
    tops = []
    for group_index, paragraph in enumerate(page):
        if group_index > 0:
            group_top = group_top + font.shape["line_height"] + (len(page[group_index - 1]) - 1) * pitch + narration_group_gap(font, window)
        tops.append([group_top + row * pitch for row in range(len(paragraph))])
    return tops


class Baker:
    def __init__(self, record, font, palette, windows):
        self.record = record
        self.windows = windows
        self.font = font
        self.palette = palette
        self.code = bake_record.code_fingerprint(CODE_FILES)
        self.settings = {"font": font_digest(font), "palette": b"".join(palette).hex()}
        self.texture_paths = []
        self.texture_sizes = {}

    def keep(self, name, relative_path, gtex_bytes):
        width, height, _ = asset_writers.read_gtex_header(gtex_bytes)
        self.texture_paths.append(relative_path)
        self.texture_sizes[name] = (width, height)

    def narration_line(self, name, items, color, ds_top):
        relative_path = TEXTURE_FOLDER + "/" + name + ".gtex"
        key = self.record.key_for(self.code, dict(self.settings, top=ds_top, color=color), repr(items).encode("ascii"))
        details = self.record.details_of(relative_path)
        current = self.record.read_if_current(relative_path, key) if "top_y" in details else None
        if current is not None:
            self.keep(name, relative_path, current)
            return details, details["end_color"]
        image, end_color = narration_line_image(self.font, self.palette, items, color, self.windows[NARRATION_ROW], ds_top)
        if image is None:
            return None, end_color
        gtex_bytes = asset_writers.gtex_bytes(image["width"], image["height"], asset_writers.FORMAT_RGBA8888, image["rgba"])
        details = {"top_y": image["top"], "natural_width": image["ink_width"], "end_color": end_color}
        self.record.store(relative_path, key, gtex_bytes, details)
        self.keep(name, relative_path, gtex_bytes)
        return details, end_color

    def narration_message(self, label, message):
        pos_y = first_parameter(message["items"], POS_Y_CODE)
        pos_y = narration.DEFAULT_POS_Y if pos_y is None else pos_y
        color = NARRATION_COLOR
        paragraphs = []
        line_index = 0
        for page in message["pages"]:
            for paragraph, tops in zip(page, narration_tops(self.font, page, pos_y, self.windows[NARRATION_ROW])):
                lines = []
                for line, ds_top in zip(paragraph, tops):
                    if not has_glyphs(line):
                        continue
                    name = narration.line_texture_name(label, line_index)
                    details, color = self.narration_line(name, line["items"], color, ds_top)
                    if details is None:
                        continue
                    line_index = line_index + 1
                    lines.append({"id": line_index, "texture": name, "top_y": details["top_y"], "natural_width": details["natural_width"]})
                paragraphs.append(lines)
        ends = [item[2][0] for item in message["items"] if item[0] == "control" and item[1] == ds_text.AUTO_PAGE and item[2]]
        return {"paragraphs": paragraphs, "pos_y": pos_y, "mono_type": first_parameter(message["items"], MONO_TYPE_CODE), "auto_pf": ends[0] if ends else None}


class GlyphShapes:
    def __init__(self, font):
        self.font = font
        self.gaps = {}
        self.shapes = {}
        self.shape = font.shape

    def needs_gap(self, code, previous):
        if (code, previous) not in self.gaps:
            self.gaps[(code, previous)] = self.font.needs_gap(code, previous)
        return self.gaps[(code, previous)]

    def glyph_of(self, code):
        if code not in self.shapes:
            bearing, _ = self.font.width_pair(code)
            shift_x = -bearing if bearing != 0 else 0
            shift_y = self.font.raise_of(code) + (self.font.descent(code) if bearing != 0 else 0)
            pixels = []
            for row, values in enumerate(self.font.glyph(code, COLOR_GROUP_SIZE)):
                for column, value in enumerate(values):
                    if value != 0:
                        pixels.append((shift_x + column, shift_y + row, value))
            self.shapes[code] = (self.font.advance(code), tuple(pixels))
        return self.shapes[code]


def shapes_of(font):
    return font if isinstance(font, GlyphShapes) else GlyphShapes(font)


def placed_glyphs(font, items, color):
    shapes = shapes_of(font)
    cursor = 0
    previous = ds_text.NO_CODE
    placed = []
    for item in items:
        if item[0] == "control":
            if item[1] == ds_text.COLOR and item[2]:
                color = item[2][0]
            previous = ds_text.CONTROL_CODE_BASE + item[1]
            continue
        code = item[1]
        if code in shapes.font.icon_widths:
            placed.append({"x": cursor, "pixels": (), "color": color})
            cursor = cursor + shapes.font.icon_widths[code] + 2 * ds_text.LETTER_GAP
            previous = code
            continue
        advance, pixels = shapes.glyph_of(code)
        x = cursor
        if shapes.needs_gap(code, previous):
            x = x + ds_text.LETTER_GAP
            advance = advance + ds_text.LETTER_GAP
        placed.append({"x": x, "pixels": pixels, "color": color})
        cursor = cursor + advance
        previous = code
    for glyph, following in zip(placed, placed[1:] + [{"x": cursor}]):
        glyph["advance"] = following["x"] - glyph["x"]
    return placed, color


def glyph_cell(pixels, phase_y, alphas):
    screen_pixels = []
    for x, y, nibble in pixels:
        for column in range(screen_of(x), screen_of(x + 1)):
            for row in range(screen_of(phase_y + y), screen_of(phase_y + y + 1)):
                screen_pixels.append((column, row - screen_of(phase_y), nibble))
    if not screen_pixels:
        return {"width": 0, "height": 0, "left": 0, "top": 0, "pixels": b""}
    left = min(column for column, _, _ in screen_pixels)
    top = min(row for _, row, _ in screen_pixels)
    width = max(column for column, _, _ in screen_pixels) + 1 - left
    height = max(row for _, row, _ in screen_pixels) + 1 - top
    rgba = bytearray(WHITE + b"\x00") * (width * height)
    for column, row, nibble in screen_pixels:
        rgba[((row - top) * width + column - left) * RGBA_BYTES + 3] = alphas[nibble]
    return {"width": width, "height": height, "left": left, "top": top, "pixels": bytes(rgba)}


class TalkGlyphs:
    def __init__(self, alphas, keeps_row_phase=True):
        self.alphas = alphas
        self.keeps_row_phase = keeps_row_phase
        self.keys = {}

    def id_of(self, glyph, ds_x, ds_y):
        phase_y = ds_y % DS_STEP if glyph["pixels"] and self.keeps_row_phase else 0
        key = (glyph["pixels"], phase_y, screen_of(ds_x + glyph["advance"]) - screen_of(ds_x))
        if key not in self.keys:
            self.keys[key] = len(self.keys)
        return self.keys[key]

    def cells(self):
        cells = []
        for pixels, phase_y, advance in self.keys:
            cell = glyph_cell(pixels, phase_y, self.alphas)
            cell["advance"] = advance
            cells.append(cell)
        return cells


def page_line_count(items):
    return 1 + sum(1 for item in items if item[0] == "control" and item[1] == ds_text.NEW_LINE)


def talk_layout(items):
    entry = None
    for item in items:
        if item[0] == "control" and item[1] in (talk_assets.FACE_CODE, talk_assets.POSE_CODE, talk_assets.HEAD_CODE):
            entry = item[1]
    return entry


def page_items(items):
    pages = [[]]
    for item in items:
        if item[0] == "control" and item[1] in ds_text.PAGE_ENDS:
            pages.append([])
        else:
            pages[-1].append(item)
    return pages


def is_visible_char(item):
    return item[0] == "char" and item[1] != ds_text.SPACE_CODE


def talk_page_fields(page):
    fields = {"reveal_is_fade": 0, "ending": talk_assets.ENDING_PRESS, "flashes": 0, "quake_level": 0, "quake_frames": 0, "pauses_before": 0}
    pauses = 0
    has_text = False
    for item in page:
        if is_visible_char(item):
            if not has_text and pauses > 0:
                fields["pauses_before"] = pauses
                fields["ending"] = talk_assets.ENDING_PRESS
            has_text = True
            continue
        if item[0] != "control":
            continue
        if item[1] == WHITE_IN_CODE:
            fields["reveal_is_fade"] = 1
        elif item[1] == PAUSE_CODE:
            fields["ending"] = talk_assets.ENDING_PAUSE_THEN_NEXT
            pauses = pauses + 1
        elif item[1] == AUTO_END_CODE:
            fields["ending"] = talk_assets.ENDING_PAUSE_THEN_CLOSE
        elif item[1] == FLASH_CODE:
            fields["flashes"] = 1
        elif item[1] == SHAKE_CODE and len(item[2]) == 2:
            fields["quake_level"], fields["quake_frames"] = item[2]
    return fields


def talk_page_row(page, layout, line_count):
    box_kind = talk_assets.BOX_TOP_WITH_HEAD if layout == talk_assets.HEAD_CODE else talk_assets.BOX_BOTTOM
    fields = talk_page_fields(page)
    changes_instantly = 1 if layout == talk_assets.POSE_CODE else 0
    return "page %s 0 %d %d %d %d %d %d %d %d %d" % (talk_assets.NO_FACE, box_kind, fields["reveal_is_fade"], fields["ending"], line_count, changes_instantly, fields["flashes"], fields["quake_level"], fields["quake_frames"], fields["pauses_before"])


def talk_line_rows(font, glyphs, lines, window, kind, color, center_x=None, tint=None):
    rows = []
    pitch = font.shape["line_height"] + lift_of_kind(kind)
    for row_index, line in enumerate(lines):
        placed, color = placed_glyphs(font, line["items"], color)
        if not any(item[0] == "char" for item in line["items"]):
            continue
        ds_x = window["x"] + window["left"]
        if center_x is not None:
            ds_x = int(round((center_x - CAMERA_LEFT_ON_SCREEN) * DS_STEP / SCREEN_STEP - ds_text.measured_width(shapes_of(font).font, line["items"]) / 2))
        ds_y = window["y"] + top_of_kind(window, kind) + row_index * pitch
        codes = []
        for glyph in placed:
            glyph_tint = tint if tint is not None else TINT_OF_DS_COLOR.get(glyph["color"], 0)
            codes.append(str(glyphs.id_of(glyph, ds_x + glyph["x"], ds_y) | (glyph_tint << talk_assets.COLOR_SHIFT)))
        rows.append("line %d %d %d %s" % (screen_x_of(ds_x + placed[0]["x"]), screen_y_of(ds_y), len(codes), " ".join(codes)))
    return rows, color


def talk_message_rows(font, glyphs, windows, label, message):
    rows = ["talk " + label]
    color = TALK_COLOR
    layout = None
    raw_pages = page_items(message["items"])
    for page_index, page in enumerate(message["pages"]):
        raw = raw_pages[page_index] if page_index < len(raw_pages) else []
        layout = talk_layout(raw) or layout
        window = windows[TOP_ROW if layout == talk_assets.HEAD_CODE else BOTTOM_ROW]
        lines = [line for paragraph in page for line in paragraph]
        line_rows, color = talk_line_rows(font, glyphs, lines, window, page_line_count(raw), color)
        rows.append(talk_page_row(raw, layout, len(line_rows)))
        rows.extend(line_rows)
    return rows


def system_line_rows(font, glyphs, windows, label, message, line):
    lines = [one for page in message["pages"] for paragraph in page for one in paragraph if has_glyphs(one)]
    line_rows, _ = talk_line_rows(font, glyphs, lines, windows[BOTTOM_ROW], max(len(lines), 1), TALK_COLOR, line.get("center_x"), SYSTEM_LINE_TINT)
    page = "page %s 0 %d 0 %d %d 0 0 0 0 0" % (talk_assets.NO_FACE, talk_assets.BOX_BOTTOM, talk_assets.ENDING_HOLD_THEN_CLOSE, len(line_rows))
    return ["talk " + label, page] + line_rows


def talk_rows_with(glyphs, font, windows, talk_messages, system_lines, system_messages):
    rows = []
    for label, message in talk_messages:
        rows.extend(talk_message_rows(font, glyphs, windows, label, message))
    for label, line in system_lines.items():
        if label in system_messages:
            rows.extend(system_line_rows(font, glyphs, windows, label, system_messages[label], line))
    return rows


def talk_rows(font, palette, windows, talk_messages, system_lines, system_messages):
    for keeps_row_phase in (True, False):
        glyphs = TalkGlyphs(ink_alphas(palette, TALK_COLOR), keeps_row_phase)
        rows = talk_rows_with(glyphs, font, windows, talk_messages, system_lines, system_messages)
        if len(glyphs.keys) <= GLYPH_LIMIT:
            return glyphs, rows
    raise DsSetTextError("the DS talk text needs %d glyph cells, more than the %d the talk book holds" % (len(glyphs.keys), GLYPH_LIMIT))


def bake_talk_atlas(baker, glyphs):
    cells = glyphs.cells()
    pixels, width, height, placed = talk_assets.build_atlas(cells)
    relative_path = TEXTURE_FOLDER + "/" + talk_assets.FONT_TEXTURE + ".gtex"
    key = baker.record.key_for(baker.code, baker.settings, pixels)
    gtex_bytes = baker.record.read_if_current(relative_path, key)
    if gtex_bytes is None:
        gtex_bytes = asset_writers.gtex_bytes(width, height, asset_writers.FORMAT_RGBA8888, pixels)
        baker.record.store(relative_path, key, gtex_bytes)
    baker.keep(talk_assets.FONT_TEXTURE, relative_path, gtex_bytes)
    return ["glyph %d %d %d %d %d %d %d" % (cell_x, cell_y, cell["width"], cell["height"], cell["left"], cell["top"], cell["advance"]) for cell, (cell_x, cell_y) in zip(cells, placed)]


def labels_of_scripts(scripts):
    narration_labels = []
    talk_labels = []
    for path, script in scripts.items():
        for label in script.strings_of_ops((chapter_assets.NARRATION_OP,)):
            narration_labels.append((label, path))
        for label in script.strings_of_ops(chapter_assets.TALK_OPS) + script.talk_labels_of_choice_panels() + script.strings_of_ops((chapter_assets.SOUL_BALLOON_TALK_OP,)):
            talk_labels.append((label, path))
    return list(dict.fromkeys(narration_labels)), list(dict.fromkeys(talk_labels))


class MessageBook:
    def __init__(self, rom, scripts, language):
        self.by_script = {}
        self.anywhere = {}
        for path in scripts:
            message_path = ds_text.script_message_path(path, language)
            if rom.file_id_of(message_path) is None:
                continue
            self.by_script[path] = ds_text.read_rom_message_file(rom, message_path)
            for label, message in self.by_script[path].items():
                self.anywhere.setdefault(label, message)

    def find(self, label, path):
        return self.by_script.get(path, {}).get(label, self.anywhere.get(label))


def read_system_messages(rom, system_lines):
    messages = {}
    for label, line in system_lines.items():
        plain = lz.try_decompress(rom.read_file(line["file"]))
        if plain is None:
            raise DsSetTextError(line["file"] + " in the DS ROM could not be decompressed")
        found = ds_text.read_message_file(plain).get(label)
        if found is not None:
            messages[label] = found
    return messages


def bake_text(set_description, scripts, rom_path, record, work_folder):
    language = set_description.get("scripts", {}).get("language", "en")
    system_lines = set_description.get("talk", {}).get("system_lines", {})
    with ds_rom.DsRom(rom_path) as rom:
        arm9 = chapter_assets.read_arm9(rom)
        font = ds_text.font_from_rom(rom, ds_text.MAIN_FONT, arm9)
        palette = read_text_palette(rom.read_file(PACK_PATH))
        book = MessageBook(rom, scripts, language)
        system_messages = read_system_messages(rom, system_lines)
    return bake_from_messages(record, font, palette, read_window_rows(arm9), book, scripts, system_lines, system_messages)


def bake_from_messages(record, font, palette, windows, book, scripts, system_lines=None, system_messages=None):
    baker = Baker(record, font, palette, windows)
    narration_labels, talk_labels = labels_of_scripts(scripts)
    narration_lines = {}
    missing = []
    for label, path in narration_labels:
        message = book.find(label, path)
        if message is None:
            missing.append(label)
        elif label not in narration_lines:
            narration_lines[label] = baker.narration_message(label, message)
    talk_messages = []
    for label, path in talk_labels:
        message = book.find(label, path)
        if message is None:
            missing.append(label)
        elif all(label != known for known, _ in talk_messages):
            talk_messages.append((label, message))
    glyphs, message_rows = talk_rows(GlyphShapes(font), palette, windows, talk_messages, system_lines or {}, system_messages or {})
    talk_files = {}
    if message_rows:
        glyph_rows = bake_talk_atlas(baker, glyphs)
        talk_files[talk_assets.TALK_FILE] = ("\n".join(["font " + talk_assets.FONT_TEXTURE] + glyph_rows + message_rows) + "\n").encode("ascii")
    if missing:
        print("ds text labels with no DS message: %d (%s)" % (len(set(missing)), ", ".join(sorted(set(missing))[:12])))
    return {"narration_lines": narration_lines, "talk_files": talk_files, "talk_textures": talk_assets.talk_texture_names(talk_files), "bubble_rows": [],
            "texture_paths": baker.texture_paths, "texture_sizes": baker.texture_sizes}
