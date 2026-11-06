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
