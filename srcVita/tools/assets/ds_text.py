import struct

import chapter_assets
import lmg_decode
import lz
import message_cues
import script_assets

ARM9_LOAD_ADDRESS = 0x02000000
FONT_SHAPE_TABLE_ADDRESS = 0x02083c78
FONT_SHAPE_ROW_SIZE = 16
ICON_WIDTH_TABLE_ADDRESS = 0x02084ae8
ICON_WIDTH_ROW_SIZE = 8
MAIN_FONT = 0
BALLOON_FONT = 1
DATABASE_FONT = 2
FONT_FILES = {
    MAIN_FONT: ("fonts2bpp_en.imb", "CharWidth.bin", "PixLine.bin"),
    BALLOON_FONT: ("balloon_font_en.imb", "CharWidthBln.bin", "PixLineBln.bin"),
    DATABASE_FONT: ("font_db.imb", "CharWidthDB.bin", "PixLineDB.bin"),
}
TWO_BIT_PACKING = 1
EXPANDED_KIND = 1
MISSING_GLYPH_FILL = 3
SPACE_BASE = 3
LETTER_GAP = 1
SPACE_CODE = 0xff
WIDE_WIDTH_LIMIT = 0x10
PROFILE_ROWS = 32
PROFILE_CODE_LIMIT = 0x72
TWO_BIT_PROFILE_SIZE = 8
FOUR_BIT_PROFILE_SIZE = 16
PAIR_LIMIT = 0xe0
ALWAYS_GAP_CODES = (0x3e, 0x3f, 0x73, 0x74)
NO_CODE = 0xfffe
CONTROL_CODE_BASE = 0xff00
END_OF_MESSAGE = 0xfffe
NEW_LINE = 0x01
NEW_PAGE = 0x02
COLOR = 0x05
AUTO_PAGE = 0x15
NEW_GROUP = 0x16
DOUBLE_WIDTH_TOGGLE = 0x17
OTHER_LINE_ENDS = (0x23, 0x26)
PAGE_ENDS = (NEW_PAGE, AUTO_PAGE)
DESCENT_BY_WIDTH_MODE = {
    0: {0x2a: 1, 0x2d: 1, 0x33: 1, 0x34: 1, 0xef: 2, 0xf3: 2, 0x101: 2, 0xe9: 3, 0xea: 3},
    1: {0x33: 1, 0x3c: 1, 0xe1: 1, 0xef: 1, 0x2a: 2, 0x2d: 2},
    2: {0xef: 1, 0x2a: 2, 0x2d: 2, 0x33: 2, 0x34: 2, 0x3c: 2},
}
SYSTEM_MESSAGE_PATH = "system/system_0000.%s.msg.xml.lz"
DS_ALPHABET = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!?"
CHARACTER_OF_CODE = dict(enumerate(DS_ALPHABET))
CHARACTER_OF_CODE.update({0x60: "é", 0x66: "ï", 0xe1: ".", 0xe5: "(", 0xe6: ")", 0xea: "\"", 0xed: ":", 0xef: ",", 0xf2: "*", 0xf3: "'", 0xf4: "-", 0xf8: "…", 0xfe: "♪", 0xff: " ", 0x10c: ";"})
UNKNOWN_CHARACTER = "�"
RGBA_BYTES = 4


class DsTextError(ValueError):
    pass


def u16(data, offset):
    try:
        return struct.unpack_from("<H", data, offset)[0]
    except struct.error:
        raise DsTextError("a message runs past the end of its file") from None


def parameter_count(control):
    return message_cues.PARAMETER_COUNTS[control] if control < len(message_cues.PARAMETER_COUNTS) else 0


def message_items(data, offset):
    items = []
    position = offset
    while True:
        word = u16(data, position)
        if word == END_OF_MESSAGE:
            return items
        position = position + 2
        if word < CONTROL_CODE_BASE:
            items.append(("char", word))
            continue
        control = word - CONTROL_CODE_BASE
        count = parameter_count(control)
        parameters = tuple(u16(data, position + 2 * index) for index in range(count))
        position = position + 2 * count
        items.append(("control", control, parameters))


def is_line_end(item):
    return item[0] == "control" and (item[1] in (NEW_LINE, NEW_GROUP) + PAGE_ENDS + OTHER_LINE_ENDS)


def text_of_codes(codes):
    return "".join(CHARACTER_OF_CODE.get(code, UNKNOWN_CHARACTER) for code in codes)


def new_line():
    return {"items": [], "codes": [], "text": ""}


def finish_line(line):
    line["text"] = text_of_codes(line["codes"])
    return line


def pages_of_items(items):
    pages = [[[new_line()]]]
    for item in items:
        if not is_line_end(item):
            pages[-1][-1][-1]["items"].append(item)
            if item[0] == "char":
                pages[-1][-1][-1]["codes"].append(item[1])
            continue
        control = item[1]
        if control in PAGE_ENDS:
            pages.append([[new_line()]])
        elif control == NEW_GROUP:
            pages[-1].append([new_line()])
        else:
            pages[-1][-1].append(new_line())
    for page in pages:
        for paragraph in page:
            for line in paragraph:
                finish_line(line)
    while len(pages) > 1 and all(not line["items"] for paragraph in pages[-1] for line in paragraph):
        pages.pop()
    return pages


def read_message_file(plain):
    try:
        labels = lmg_decode.read_labels(plain, lmg_decode.read_header(plain))
    except ValueError as error:
        raise DsTextError("not a 1LMG message file: " + str(error)) from None
    messages = {}
    for label, target in message_cues.label_targets(labels).items():
        items = message_items(plain, target)
        messages[label] = {"items": items, "pages": pages_of_items(items)}
    return messages


def read_rom_message_file(rom, path):
    plain = lz.try_decompress(rom.read_file(path))
    if plain is None:
        raise DsTextError(path + " in the DS ROM could not be decompressed")
    return read_message_file(plain)


def script_message_path(script_path, language):
    return script_assets.language_path_of(script_path, language)


def system_message_path(language):
    return SYSTEM_MESSAGE_PATH % language


def read_script_messages(rom, script_paths, language):
    by_label = {}
    for script_path in script_paths:
        path = script_message_path(script_path, language)
        if rom.file_id_of(path) is None:
            continue
        for label, message in read_rom_message_file(rom, path).items():
            by_label.setdefault(label, dict(message, file=path))
    return by_label


def read_font_shape(arm9, font_index):
    offset = FONT_SHAPE_TABLE_ADDRESS - ARM9_LOAD_ADDRESS + FONT_SHAPE_ROW_SIZE * font_index
    cell_width, cell_height, advance, line_height, glyph_bytes, packing, kind = struct.unpack_from("<4BIII", arm9, offset)
    return {"cell_width": cell_width, "cell_height": cell_height, "advance": advance, "line_height": line_height, "glyph_bytes": glyph_bytes, "two_bits": packing == TWO_BIT_PACKING, "kind": kind}


def read_icon_widths(arm9):
    widths = {}
    offset = ICON_WIDTH_TABLE_ADDRESS - ARM9_LOAD_ADDRESS
    while True:
        code, _, width = struct.unpack_from("<HHH", arm9, offset)
        if code == 0:
            return widths
        widths[code] = width
        offset = offset + ICON_WIDTH_ROW_SIZE


def expand_two_bits(source):
    expanded = bytearray()
    for value in source:
        expanded.append(((value & 0xc0) >> 2) | ((value & 0x30) >> 4))
        expanded.append(((value & 0x0c) << 2) | (value & 0x03))
    return bytes(expanded)


def colored_nibble(nibble, color):
    if nibble == 0:
        return 0
    return (nibble + color - 3) & 0xf if color != 0 else 0


class DsFont:
    def __init__(self, font_index, shape, glyphs, widths, profiles, icon_widths=None):
        self.font_index = font_index
        self.shape = shape
        self.glyphs = glyphs
        self.widths = widths
        self.profiles = profiles
        self.icon_widths = icon_widths or {}
        self.width_mode = font_index

    def glyph(self, code, color):
        size = self.shape["glyph_bytes"]
        start = code * size
        source = self.glyphs[start:start + size] if start + size <= len(self.glyphs) else bytes([MISSING_GLYPH_FILL]) * size
        packed = expand_two_bits(source) if self.shape["two_bits"] and self.shape["kind"] == EXPANDED_KIND else source
        if self.font_index != DATABASE_FONT:
            packed = bytes((colored_nibble(value >> 4, color) << 4) | colored_nibble(value & 0xf, color) for value in packed)
        width = self.shape["cell_width"]
        rows = []
        for row in range(self.shape["cell_height"]):
            pixels = []
            for value in packed[row * width // 2:(row + 1) * width // 2]:
                pixels.extend((value & 0xf, value >> 4))
            rows.append(pixels)
        return rows

    def width_pair(self, code):
        if 2 * code + 2 > len(self.widths):
            return 0, 0
        return self.widths[2 * code], self.widths[2 * code + 1]

    def advance(self, code):
        bearing, width = self.width_pair(code)
        if bearing == 0 and width == 0:
            return self.shape["advance"]
        if width > WIDE_WIDTH_LIMIT or code == SPACE_CODE:
            return SPACE_BASE + LETTER_GAP
        return width

    def raise_of(self, code):
        return -1 if self.font_index == DATABASE_FONT and code < PROFILE_CODE_LIMIT else 0

    def descent(self, code):
        return DESCENT_BY_WIDTH_MODE.get(self.width_mode, {}).get(code, 0)

    def profile_value(self, code, side, row, shift):
        index = row - shift
        if index < 0 or index >= PROFILE_ROWS:
            return 0
        if self.width_mode == DATABASE_FONT:
            start = code * 2 * FOUR_BIT_PROFILE_SIZE + side * FOUR_BIT_PROFILE_SIZE
            value = self.profiles[start + index // 2]
            return value & 0xf if index % 2 == 0 else value >> 4
        start = code * 2 * TWO_BIT_PROFILE_SIZE + side * TWO_BIT_PROFILE_SIZE
        return (self.profiles[start + index // 4] >> (2 * (index % 4))) & 3

    def pair_always_gaps(self, code, previous):
        if code > PAIR_LIMIT or previous > PAIR_LIMIT or code in ALWAYS_GAP_CODES:
            return previous != SPACE_CODE
        return False

    def needs_gap(self, code, previous):
        if code > CONTROL_CODE_BASE or previous > CONTROL_CODE_BASE:
            return False
        if self.pair_always_gaps(code, previous):
            return True
        if previous == SPACE_CODE:
            return False
        if code > PROFILE_CODE_LIMIT or previous > PROFILE_CODE_LIMIT:
            return True
        wanted = 0 if self.width_mode == DATABASE_FONT else 3
        own_shift = self.raise_of(code) + self.descent(code)
        previous_shift = self.raise_of(previous) + self.descent(previous)
        for row in range(PROFILE_ROWS):
            left = self.profile_value(code, 0, row, own_shift)
            right = self.profile_value(previous, 1, row, previous_shift)
            if left != 0 and right != 0:
                if self.width_mode == DATABASE_FONT or wanted in (left, right):
                    return True
            if wanted == left and (wanted == self.profile_value(previous, 1, row + 1, previous_shift) or wanted == self.profile_value(previous, 1, row - 1, previous_shift)):
                return True
        return False


def font_from_rom(rom, font_index, arm9=None):
    if arm9 is None:
        arm9 = chapter_assets.read_arm9(rom)
    glyph_file, width_file, profile_file = FONT_FILES[font_index]
    return DsFont(font_index, read_font_shape(arm9, font_index), rom.read_file(glyph_file), rom.read_file(width_file), rom.read_file(profile_file), read_icon_widths(arm9))


def measured_width(font, items):
    total = 0
    previous = NO_CODE
    doubled = False
    for item in items:
        if item[0] == "control":
            if item[1] == DOUBLE_WIDTH_TOGGLE:
                doubled = not doubled
            continue
        code = item[1]
        if code in font.icon_widths:
            total = total + font.icon_widths[code] + 2 * LETTER_GAP
        else:
            advance = font.advance(code)
            total = total + (2 * advance if doubled else advance)
            if font.needs_gap(code, previous):
                total = total + LETTER_GAP
        previous = code
    return total


def draw_items(font, items, color, row_index=0, line_spacing=0):
    cursor = 0
    previous = NO_CODE
    top = (font.shape["line_height"] + line_spacing) * row_index
    pixels = []
    icons = []
    for item in items:
        if item[0] == "control":
            if item[1] == COLOR and item[2]:
                color = item[2][0]
            previous = CONTROL_CODE_BASE + item[1]
            continue
        code = item[1]
        if code in font.icon_widths:
            icons.append({"code": code, "x": cursor + LETTER_GAP})
            cursor = cursor + font.icon_widths[code] + 2 * LETTER_GAP
            previous = code
            continue
        x = cursor
        advance = font.advance(code)
        if font.needs_gap(code, previous):
            x = x + LETTER_GAP
            advance = advance + LETTER_GAP
        cursor = cursor + advance
        bearing, _ = font.width_pair(code)
        shift_x = -bearing if bearing != 0 else 0
        shift_y = font.raise_of(code) + (font.descent(code) if bearing != 0 else 0)
        for row, values in enumerate(font.glyph(code, color)):
            for column, value in enumerate(values):
                if value != 0:
                    pixels.append((x + shift_x + column, top + shift_y + row, value))
        previous = code
    return pixels, cursor, icons, color


def render_line(font, items, color, palette=None):
    pixels, cursor, icons, _ = draw_items(font, items, color)
    left = min([0] + [x for x, _, _ in pixels])
    right = max([cursor] + [x + 1 for x, _, _ in pixels])
    top = min([0] + [y for _, y, _ in pixels])
    bottom = max([font.shape["cell_height"]] + [y + 1 for _, y, _ in pixels])
    width = right - left
    height = bottom - top
    indices = bytearray(width * height)
    for x, y, value in pixels:
        indices[(y - top) * width + (x - left)] = value
    line = {"width": width, "height": height, "indices": bytes(indices), "pen_x": -left, "cell_top": -top, "advance": cursor, "measured_width": measured_width(font, items), "icons": [dict(icon, x=icon["x"] - left) for icon in icons]}
    if palette is not None:
        line["pixels"] = rgba_of_indices(line["indices"], palette)
    return line


def rgba_of_indices(indices, palette):
    pixels = bytearray(len(indices) * RGBA_BYTES)
    for position, index in enumerate(indices):
        if index != 0:
            pixels[position * RGBA_BYTES:(position + 1) * RGBA_BYTES] = bytes(palette[index])
    return bytes(pixels)


def palette_of_bgr555(words):
    colors = []
    for word in words:
        red, green, blue = word & 0x1f, (word >> 5) & 0x1f, (word >> 10) & 0x1f
        colors.append((red << 3 | red >> 2, green << 3 | green >> 2, blue << 3 | blue >> 2, 255))
    return colors
