import os
import struct
import zipfile
import zlib

import pack_files
import timeline_file as timeline_format

FORMAT_RGBA8 = 28
FORMAT_ASTC_6X6 = 1038
FORMAT_ETC2_RGB = 1086
TEX_VERSION = 691230823
TEX_HEADER_AND_RECORD_SIZE = 56
MIP_RECORD_SIZE = 16
TYPE_SHEET = 147581786
TYPE_RECORD = 4048237731
TYPE_ROOT_ANIMATION = 240123514
GMSG_MAGIC = b"GMSG"
GMSG_VERSION = 22
GMSG_ENTRY_COUNT_OFFSET = 16
GMSG_LANGUAGE_COUNT_OFFSET = 24
GMSG_STRING_AREA_POINTER_OFFSET = 32
GMSG_ENTRY_POINTERS_OFFSET = 72
GMSG_ENTRY_NAME_POINTER_OFFSET = 24
GMSG_ENTRY_TEXT_POINTERS_OFFSET = 40
FAKE_TEXT_KEY = bytes(range(16))


def tex_file(width, height, format_code, payload, row_pitch):
    header = b"TEX\0" + struct.pack("<IHHHBBIiIIQ", TEX_VERSION, width, height, 1, 1, 16, format_code, -1, 0, 0, 0)
    record = struct.pack("<QII", TEX_HEADER_AND_RECORD_SIZE, row_pitch, len(payload))
    return header + record + payload


def tex_file_with_small_mip_stored_first(width, height, format_code, payload, row_pitch, small_mip_payload):
    header = b"TEX\0" + struct.pack("<IHHHBBIiIIQ", TEX_VERSION, width, height, 1, 1, 32, format_code, -1, 0, 0, 0)
    small_mip_offset = len(header) + 2 * MIP_RECORD_SIZE
    top_record = struct.pack("<QII", small_mip_offset + len(small_mip_payload), row_pitch, len(payload))
    small_record = struct.pack("<QII", small_mip_offset, row_pitch // 2, len(small_mip_payload))
    return header + top_record + small_record + small_mip_payload + payload


def astc_constant_block(red, green, blue, alpha):
    block = bytes([0xFC, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF])
    return block + struct.pack("<HHHH", red * 257, green * 257, blue * 257, alpha * 257)


def etc2_constant_block(red_5bit, green_5bit, blue_5bit):
    return bytes([red_5bit << 3, green_5bit << 3, blue_5bit << 3, 0x02, 0, 0, 0, 0])


def expand_5_bits(value):
    return (value << 3) | (value >> 2)


def rgb565_to_rgb(value):
    red = (value >> 11) & 31
    green = (value >> 5) & 63
    blue = value & 31
    return ((red << 3) | (red >> 2), (green << 2) | (green >> 4), (blue << 3) | (blue >> 2))


def color_table(color0_value, color1_value):
    color0 = rgb565_to_rgb(color0_value)
    color1 = rgb565_to_rgb(color1_value)
    table = [color0 + (255,), color1 + (255,)]
    if color0_value > color1_value:
        table.append(tuple((2 * a + b) // 3 for a, b in zip(color0, color1)) + (255,))
        table.append(tuple((a + 2 * b) // 3 for a, b in zip(color0, color1)) + (255,))
    else:
        table.append(tuple((a + b) // 2 for a, b in zip(color0, color1)) + (255,))
        table.append((0, 0, 0, 0))
    return table


def decode_color_block(block):
    color0_value, color1_value, indices = struct.unpack("<HHI", block)
    table = color_table(color0_value, color1_value)
    return [table[(indices >> (2 * position)) & 3] for position in range(16)]


class ByteWriter:
    def __init__(self):
        self.data = bytearray()

    def align(self, size):
        while len(self.data) % size != 0:
            self.data.append(0)

    def i32(self, value):
        self.align(4)
        self.data.extend(struct.pack("<i", value))

    def i16(self, value):
        self.align(2)
        self.data.extend(struct.pack("<h", value))

    def u8(self, value):
        self.data.append(value)

    def string(self, text):
        encoded = (text + "\0").encode("utf-16-le")
        self.i32(len(encoded) // 2)
        self.data.extend(encoded)

    def vector(self, x, y):
        self.align(16)
        self.data.extend(struct.pack("<4f", x, y, 0.0, 0.0))


def write_sheet(writer, pages, atlas_size, cell_size, cells_per_page):
    writer.i32(len(pages))
    for page in pages:
        writer.string(page)
    writer.i32(atlas_size[0])
    writer.i32(atlas_size[1])
    writer.i32(cell_size[0])
    writer.i32(cell_size[1])
    writer.i32(len(cells_per_page))
    for count in cells_per_page:
        writer.i32(count)
    writer.vector(atlas_size[0] // cell_size[0], atlas_size[1] // cell_size[1])
    writer.i32(sum(cells_per_page))
    writer.u8(0)
    writer.u8(0)


def record_shorts(first, anchor, point):
    values = [0] * timeline_format.RECORD_SHORT_COUNT
    values[timeline_format.SHORT_BLANK_MARKER] = first
    values[timeline_format.SHORT_ANCHOR_X] = anchor[0]
    values[timeline_format.SHORT_ANCHOR_Y] = anchor[1]
    values[timeline_format.SHORT_ORIGIN_X] = 1024
    values[timeline_format.SHORT_ORIGIN_Y] = 512
    values[timeline_format.SHORT_POINT_X] = point[0]
    values[timeline_format.SHORT_POINT_Y] = point[1]
    return values


def write_record(writer, shorts, position, cell_index, adjust):
    writer.i32(len(shorts))
    for value in shorts:
        writer.i16(value)
    writer.i32(1)
    writer.vector(position[0], position[1])
    writer.i32(cell_index)
    writer.i32(adjust[0])
    writer.i32(adjust[1])
    writer.i32(0)


def timeline_file(pages, atlas_size, cell_size, cells_per_page, records, ds_frame_count, placement_shift=(0, 0)):
    rsz_offset = 64
    instance_count = 1 + 1 + len(records) + 1
    instance_table_offset = 48
    data_offset = instance_table_offset + 8 * instance_count
    data_offset = data_offset + (16 - data_offset % 16) % 16
    writer = ByteWriter()
    writer.data.extend(b"USR\0" + struct.pack("<III", 0, 0, 0))
    writer.data.extend(struct.pack("<QQQ", 0, 0, rsz_offset))
    writer.align(16)
    while len(writer.data) < rsz_offset:
        writer.data.append(0)
    writer.data.extend(b"RSZ\0" + struct.pack("<IIII", 16, 1, instance_count, 0))
    writer.align(8)
    writer.data.extend(struct.pack("<QQ", instance_table_offset, data_offset))
    types = [0, TYPE_SHEET] + [TYPE_RECORD] * len(records) + [TYPE_ROOT_ANIMATION]
    while len(writer.data) < rsz_offset + instance_table_offset:
        writer.data.append(0)
    for type_id in types:
        writer.data.extend(struct.pack("<II", type_id, 0))
    while len(writer.data) < rsz_offset + data_offset:
        writer.data.append(0)
    write_sheet(writer, pages, atlas_size, cell_size, cells_per_page)
    for record in records:
        adjust = (0, 0)
        if len(record) > 3:
            adjust = record[3]
        write_record(writer, record[0], record[1], record[2], adjust)
    writer.i32(1)
    writer.i32(len(records))
    for index in range(len(records)):
        writer.i32(2 + index)
    for value in (1, ds_frame_count, 0, placement_shift[0], placement_shift[1]):
        writer.i32(value)
    return bytes(writer.data)


def pack_bytes(files):
    table = bytearray()
    payload = bytearray()
    table_size = len(files) * pack_files.PACK_ENTRY_SIZE
    data_start = pack_files.PACK_HEADER_SIZE + table_size
    for path, data, use_deflate in files:
        lower_hash, upper_hash = pack_files.path_hash_key(path)
        stored = data
        method = pack_files.METHOD_STORED
        if use_deflate:
            compressor = zlib.compressobj(wbits=-15)
            stored = compressor.compress(data) + compressor.flush()
            method = pack_files.METHOD_DEFLATE
        table.extend(struct.pack("<IIQQQQQ", lower_hash, upper_hash, data_start + len(payload), len(stored), len(data), method, 0))
        payload.extend(stored)
    header = b"KPKA" + struct.pack("<IIII", 4, len(files), 0, 0)
    return header[:pack_files.PACK_HEADER_SIZE] + bytes(table) + bytes(payload)


def write_apk_folder(folder, files):
    with open(os.path.join(folder, "re_chunk_000.pak"), "wb") as output:
        output.write(pack_bytes(files))
    for file_name, member_name in pack_files.PACK_SOURCES[1:]:
        with zipfile.ZipFile(os.path.join(folder, file_name), "w", zipfile.ZIP_STORED) as archive:
            archive.writestr(member_name, pack_bytes([]))


def key_text_forms(key):
    pairs = ["%02x" % value for value in key]
    decimals = [str(value) for value in key]
    texts = [
        "".join(pairs),
        "".join(pairs).upper(),
        " ".join(pairs),
        " ".join(pairs).upper(),
        ":".join(pairs),
        ", ".join("0x" + pair for pair in pairs),
        "".join("\\x" + pair for pair in pairs),
        ", ".join(decimals),
        " ".join(decimals),
        ",".join(decimals),
        repr(key),
    ]
    return [key, key[:8], key[8:], key.hex()[:16].encode("ascii")] + [text.encode("ascii") for text in texts]


def write_text_key(folder, key=FAKE_TEXT_KEY):
    with open(os.path.join(folder, "text_key.bin"), "wb") as output:
        output.write(key)


def gmsg_entry_size(language_count):
    return GMSG_ENTRY_TEXT_POINTERS_OFFSET + 8 * language_count


def gmsg_entry_start(entry_count):
    return GMSG_ENTRY_POINTERS_OFFSET + 8 * entry_count


def message_file_bytes(entries, language_count=2, version=GMSG_VERSION, key=FAKE_TEXT_KEY):
    entry_size = gmsg_entry_size(language_count)
    entry_start = gmsg_entry_start(len(entries))
    string_area_start = entry_start + entry_size * len(entries)
    plain_strings = bytearray()

    def add_string(text):
        pointer = string_area_start + len(plain_strings)
        plain_strings.extend(text.encode("utf-16-le") + b"\0\0")
        return pointer

    header = bytearray(GMSG_ENTRY_POINTERS_OFFSET)
    struct.pack_into("<I4s", header, 0, version, GMSG_MAGIC)
    struct.pack_into("<I", header, GMSG_ENTRY_COUNT_OFFSET, len(entries))
    struct.pack_into("<I", header, GMSG_LANGUAGE_COUNT_OFFSET, language_count)
    struct.pack_into("<Q", header, GMSG_STRING_AREA_POINTER_OFFSET, string_area_start)
    pointers = bytearray()
    entry_area = bytearray()
    for index, (name, texts) in enumerate(entries):
        pointers.extend(struct.pack("<Q", entry_start + entry_size * index))
        entry = bytearray(entry_size)
        struct.pack_into("<Q", entry, GMSG_ENTRY_NAME_POINTER_OFFSET, add_string(name))
        for language, text in enumerate(texts):
            struct.pack_into("<Q", entry, GMSG_ENTRY_TEXT_POINTERS_OFFSET + 8 * language, add_string(text))
        entry_area.extend(entry)
    encoded = bytearray(len(plain_strings))
    previous = 0
    for index, value in enumerate(plain_strings):
        encoded[index] = value ^ previous ^ key[index & 15]
        previous = encoded[index]
    return bytes(header) + bytes(pointers) + bytes(entry_area) + bytes(encoded)


LZ11_MINIMUM_MATCH = 3
LZ11_MAXIMUM_MATCH = 16
LZ11_WINDOW = 4096
ROM_HEADER_SIZE = 0x200
ROM_GAME_CODE_OFFSET = 12
ROM_TABLES_OFFSET = 0x40
USA_GAME_CODE = b"BGTE"
SCRIPT_CODE_START = 0x34
SCRIPT_ID = b"1LMG"


def longest_earlier_match(plain, position):
    best_length = 0
    best_distance = 0
    for distance in range(1, min(position, LZ11_WINDOW) + 1):
        length = 0
        while length < LZ11_MAXIMUM_MATCH and position + length < len(plain) and plain[position + length] == plain[position + length - distance]:
            length = length + 1
        if length > best_length:
            best_length = length
            best_distance = distance
    return best_length, best_distance


def lz11_bytes(plain):
    output = bytearray([0x11]) + bytearray(struct.pack("<I", len(plain))[:3])
    position = 0
    while position < len(plain):
        flags_position = len(output)
        output.append(0)
        for bit in range(8):
            if position >= len(plain):
                break
            length, distance = longest_earlier_match(plain, position)
            if length >= LZ11_MINIMUM_MATCH:
                output[flags_position] |= 0x80 >> bit
                output.append(((length - 1) << 4) | ((distance - 1) >> 8))
                output.append((distance - 1) & 0xFF)
                position = position + length
            else:
                output.append(plain[position])
                position = position + 1
    return bytes(output)


def folder_tree_of(paths):
    folders = {"": []}
    for path in paths:
        parts = path.split("/")
        parent = ""
        for part in parts[:-1]:
            folder = parent + "/" + part if parent else part
            if folder not in folders:
                folders[folder] = []
                folders[parent].append((part, None, folder))
            parent = folder
        folders[parent].append((parts[-1], path, None))
    return folders


def file_name_table(folders, file_ids):
    folder_ids = {folder: 0xF000 + index for index, folder in enumerate(folders)}
    main_table = bytearray()
    sub_tables = bytearray()
    next_file_id = 0
    for folder, entries in folders.items():
        parent_or_count = len(folders) if folder == "" else folder_ids[folder.rpartition("/")[0]]
        main_table.extend(struct.pack("<IHH", 8 * len(folders) + len(sub_tables), next_file_id, parent_or_count))
        for name, path, child in sorted(entries, key=lambda entry: entry[0]):
            if child is None:
                file_ids[path] = next_file_id
                next_file_id = next_file_id + 1
                sub_tables.extend(bytes([len(name)]) + name.encode("latin-1"))
            else:
                sub_tables.extend(bytes([0x80 | len(name)]) + name.encode("latin-1") + struct.pack("<H", folder_ids[child]))
        sub_tables.append(0)
    return bytes(main_table + sub_tables)


def mini_rom_bytes(files, game_code=USA_GAME_CODE):
    file_ids = {}
    names = file_name_table(folder_tree_of(list(files)), file_ids)
    names_offset = ROM_HEADER_SIZE
    allocation_offset = names_offset + len(names) + (4 - len(names) % 4) % 4
    allocation_size = 8 * len(files)
    payload = bytearray()
    payload_offset = allocation_offset + allocation_size
    entries = [b""] * len(files)
    for path, data in files.items():
        entries[file_ids[path]] = struct.pack("<II", payload_offset + len(payload), payload_offset + len(payload) + len(data))
        payload.extend(data)
    header = bytearray(ROM_HEADER_SIZE)
    header[ROM_GAME_CODE_OFFSET:ROM_GAME_CODE_OFFSET + len(game_code)] = game_code
    struct.pack_into("<IIII", header, ROM_TABLES_OFFSET, names_offset, len(names), allocation_offset, allocation_size)
    return bytes(header) + names + bytes(allocation_offset - names_offset - len(names)) + b"".join(entries) + bytes(payload)


def script_file_bytes(code_words, strings):
    string_table = b"".join(text.encode("latin-1") + b"\0" for text in strings)
    code = struct.pack("<%dH" % len(code_words), *code_words)
    header = bytearray(SCRIPT_CODE_START)
    header[:len(SCRIPT_ID)] = SCRIPT_ID
    struct.pack_into("<III", header, 8, len(code), len(string_table), 0)
    return bytes(header) + code + string_table


GLYPH_ADVANCE_EM = 0.6
GLYPH_INK_LEFT_EM = 0.05
GLYPH_INK_RIGHT_EM = 0.55
GLYPH_INK_TOP_EM = 0.7
TALL_GLYPH_INK_TOP_EM = 2.0
TALL_CHARACTER = "|"
FONT_UNITS_PER_EM = 1000
FIRST_CHARACTER_IN_FONT = 32
CHARACTER_COUNT_IN_FONT = 95


def glyph_box(top_em):
    left = int(GLYPH_INK_LEFT_EM * FONT_UNITS_PER_EM)
    right = int(GLYPH_INK_RIGHT_EM * FONT_UNITS_PER_EM)
    top = int(top_em * FONT_UNITS_PER_EM)
    outline = struct.pack(">hhhhh", 1, left, 0, right, top)
    outline = outline + struct.pack(">HH", 3, 0) + bytes([0x01, 0x01, 0x01, 0x01])
    outline = outline + struct.pack(">hhhh", left, 0, right - left, 0)
    outline = outline + struct.pack(">hhhh", 0, top, 0, -top)
    return outline + bytes((4 - len(outline) % 4) % 4)


def sfnt_bytes(tables, magic):
    table_count = len(tables)
    entry_selector = 0
    while (2 << entry_selector) <= table_count:
        entry_selector = entry_selector + 1
    search_range = 16 << entry_selector
    header = magic + struct.pack(">HHHH", table_count, search_range, entry_selector, table_count * 16 - search_range)
    offset = len(header) + 16 * table_count
    directory = bytearray()
    body = bytearray()
    for tag in sorted(tables):
        data = tables[tag]
        directory.extend(tag + struct.pack(">III", 0, offset + len(body), len(data)))
        body.extend(data + bytes((4 - len(data) % 4) % 4))
    return header + bytes(directory) + bytes(body)


def box_font_bytes():
    glyphs = [b"", glyph_box(GLYPH_INK_TOP_EM), glyph_box(TALL_GLYPH_INK_TOP_EM)]
    glyph_ids = [1] * CHARACTER_COUNT_IN_FONT
    glyph_ids[0] = 0
    glyph_ids[ord(TALL_CHARACTER) - FIRST_CHARACTER_IN_FONT] = 2
    advance = int(GLYPH_ADVANCE_EM * FONT_UNITS_PER_EM)
    locations = [0]
    for glyph in glyphs:
        locations.append(locations[-1] + len(glyph))
    head = struct.pack(">IIIIHHQQhhhhHHhhh", 0x00010000, 0x00010000, 0, 0x5F0F3CF5, 0, FONT_UNITS_PER_EM, 0, 0, 0, 0, advance, 2000, 0, 8, 2, 1, 0)
    hhea = struct.pack(">IhhhHhhhhhhhhhhhH", 0x00010000, 800, -200, 0, advance, 0, 0, advance, 1, 0, 0, 0, 0, 0, 0, 0, len(glyphs))
    maxp = struct.pack(">IHHHHHHHHHHHHHH", 0x00010000, len(glyphs), 4, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0)
    hmtx = b"".join(struct.pack(">Hh", advance, 0) for _ in glyphs)
    character_map = struct.pack(">HHHHH", 6, 10 + 2 * CHARACTER_COUNT_IN_FONT, 0, FIRST_CHARACTER_IN_FONT, CHARACTER_COUNT_IN_FONT)
    character_map = character_map + b"".join(struct.pack(">H", glyph_id) for glyph_id in glyph_ids)
    cmap = struct.pack(">HHHHI", 0, 1, 3, 1, 12) + character_map
    tables = {
        b"head": head,
        b"hhea": hhea,
        b"maxp": maxp,
        b"hmtx": hmtx,
        b"cmap": cmap,
        b"loca": b"".join(struct.pack(">I", location) for location in locations),
        b"glyf": b"".join(glyphs),
    }
    return sfnt_bytes(tables, b"\x00\x01\x00\x00")


def fbfo_font_file(font_bytes, key):
    repeat_count = len(font_bytes) // len(key) + 1
    encoded = bytes(a ^ b for a, b in zip(font_bytes, key * repeat_count))
    return b"FBFO" + encoded
