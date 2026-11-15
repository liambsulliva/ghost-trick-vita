import struct

FONT_FILE_MAGIC = b"FBFO"
FONT_FILE_HEADER_SIZE = 4
KEY_SIZE = 8
SFNT_MAGICS = (b"OTTO", b"\x00\x01\x00\x00")
SFNT_TABLE_COUNT_MAX = 64
SFNT_DIRECTORY_START = 12
SFNT_DIRECTORY_ENTRY_SIZE = 16


class FontFileError(ValueError):
    pass


def xor_bytes(first, second):
    return bytes(a ^ b for a, b in zip(first, second))


def sfnt_table_count_fields(table_count):
    entry_selector = 0
    while (2 << entry_selector) <= table_count:
        entry_selector = entry_selector + 1
    search_range = 16 << entry_selector
    range_shift = table_count * 16 - search_range
    return struct.pack(">HHHH", table_count, search_range, entry_selector, range_shift)


def apply_repeating_key(data, key):
    repeat_count = len(data) // KEY_SIZE + 1
    key_stream = (key * repeat_count)[:len(data)]
    return (int.from_bytes(data, "little") ^ int.from_bytes(key_stream, "little")).to_bytes(len(data), "little")


def tables_fit_inside(font_bytes):
    table_count = struct.unpack_from(">H", font_bytes, 4)[0]
    directory_end = SFNT_DIRECTORY_START + table_count * SFNT_DIRECTORY_ENTRY_SIZE
    if directory_end > len(font_bytes):
        return False
    for index in range(table_count):
        _, _, offset, length = struct.unpack_from(">4sIII", font_bytes, SFNT_DIRECTORY_START + index * SFNT_DIRECTORY_ENTRY_SIZE)
        if offset + length > len(font_bytes):
            return False
    return True


def key_candidates(body):
    for magic in SFNT_MAGICS:
        key_start = xor_bytes(body[:4], magic)
        for table_count in range(1, SFNT_TABLE_COUNT_MAX + 1):
            fields = sfnt_table_count_fields(table_count)
            if xor_bytes(body[8:12], key_start) == fields[4:]:
                yield key_start + xor_bytes(body[4:8], fields[:4])


def decode_font(file_bytes):
    if file_bytes[:FONT_FILE_HEADER_SIZE] != FONT_FILE_MAGIC:
        raise FontFileError("not an FBFO font file")
    body = file_bytes[FONT_FILE_HEADER_SIZE:]
    for key in key_candidates(body):
        font_bytes = apply_repeating_key(body, key)
        if tables_fit_inside(font_bytes):
            return font_bytes
    raise FontFileError("no key gives a valid font header")


def table_of(font_bytes, tag):
    table_count = struct.unpack_from(">H", font_bytes, 4)[0]
    for index in range(table_count):
        entry_tag, _, offset, length = struct.unpack_from(">4sIII", font_bytes, SFNT_DIRECTORY_START + index * SFNT_DIRECTORY_ENTRY_SIZE)
        if entry_tag == tag:
            return font_bytes[offset:offset + length]
    raise FontFileError("the font has no %s table" % tag.decode("ascii"))


def format_4_characters(table, offset):
    segment_count = struct.unpack_from(">H", table, offset + 6)[0] // 2
    ends_at = offset + 14
    starts_at = ends_at + 2 * segment_count + 2
    deltas_at = starts_at + 2 * segment_count
    ranges_at = deltas_at + 2 * segment_count
    characters = set()
    for segment in range(segment_count):
        end = struct.unpack_from(">H", table, ends_at + 2 * segment)[0]
        start = struct.unpack_from(">H", table, starts_at + 2 * segment)[0]
        delta = struct.unpack_from(">H", table, deltas_at + 2 * segment)[0]
        range_offset = struct.unpack_from(">H", table, ranges_at + 2 * segment)[0]
        for code in range(start, min(end, 0xfffe) + 1):
            if range_offset == 0:
                glyph = (code + delta) & 0xffff
            else:
                glyph_at = ranges_at + 2 * segment + range_offset + 2 * (code - start)
                glyph = struct.unpack_from(">H", table, glyph_at)[0] if glyph_at + 2 <= len(table) else 0
                glyph = (glyph + delta) & 0xffff if glyph else 0
            if glyph:
                characters.add(code)
    return characters


def format_6_characters(table, offset):
    first_code, entry_count = struct.unpack_from(">HH", table, offset + 6)
    glyphs = struct.unpack_from(">%dH" % entry_count, table, offset + 10)
    return set(first_code + index for index, glyph in enumerate(glyphs) if glyph)


def format_12_characters(table, offset):
    group_count = struct.unpack_from(">I", table, offset + 12)[0]
    characters = set()
    for group in range(group_count):
        start, end, first_glyph = struct.unpack_from(">III", table, offset + 16 + 12 * group)
        characters.update(code for code in range(start, end + 1) if first_glyph + code - start)
    return characters


def mapped_characters(font_bytes):
    table = table_of(font_bytes, b"cmap")
    subtable_count = struct.unpack_from(">H", table, 2)[0]
    characters = set()
    for index in range(subtable_count):
        platform, encoding, offset = struct.unpack_from(">HHI", table, 4 + 8 * index)
        if (platform, encoding) not in ((0, 3), (0, 4), (3, 1), (3, 10)):
            continue
        subtable_format = struct.unpack_from(">H", table, offset)[0]
        if subtable_format == 4:
            characters |= format_4_characters(table, offset)
        elif subtable_format == 6:
            characters |= format_6_characters(table, offset)
        elif subtable_format == 12:
            characters |= format_12_characters(table, offset)
    return characters


def missing_characters(font_bytes, text, characters_of_font=None):
    mapped = characters_of_font if characters_of_font is not None else mapped_characters(font_bytes)
    return sorted(set(character for character in text if ord(character) not in mapped and not character.isspace() and ord(character) >= 0x20))
