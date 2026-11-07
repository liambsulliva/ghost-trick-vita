import struct

GTEX_MAGIC = b"GTEX"
GTEX_HEADER_SIZE = 20
GTEX_FORMAT_OFFSET = 12
FORMAT_RGBA8888 = 0
FORMAT_DXT5 = 1
FORMAT_DXT1 = 2
PACKED_FLAG = 0x100
BYTES_PER_UNIT = {FORMAT_RGBA8888: 4, FORMAT_DXT5: 16, FORMAT_DXT1: 8}
MATCH_FLAG = 0x80000000
LONGEST_RUN_UNITS = 0x7fffffff


def packed_body(units, unit_size):
    table = {}
    parts = []
    index = 0
    literal_start = 0
    count = len(units)
    while index < count:
        source = table.get(units[index])
        table[units[index]] = index
        if source is None:
            index += 1
            continue
        length = 1
        while index + length < count and units[source + length] == units[index + length]:
            length += 1
        if literal_start < index:
            parts.append(struct.pack("<I", index - literal_start))
            parts.append(b"".join(units[literal_start:index]))
        parts.append(struct.pack("<II", MATCH_FLAG | length, source))
        for later in range(index + 1, index + length):
            table[units[later]] = later
        index += length
        literal_start = index
    if literal_start < count:
        parts.append(struct.pack("<I", count - literal_start))
        parts.append(b"".join(units[literal_start:count]))
    return b"".join(parts)


def pack_gtex(gtex_bytes):
    if len(gtex_bytes) < GTEX_HEADER_SIZE or gtex_bytes[:4] != GTEX_MAGIC:
        return gtex_bytes
    texture_format = struct.unpack_from("<I", gtex_bytes, GTEX_FORMAT_OFFSET)[0]
    unit_size = BYTES_PER_UNIT.get(texture_format)
    if unit_size is None:
        return gtex_bytes
    pixels = gtex_bytes[GTEX_HEADER_SIZE:]
    if len(pixels) == 0 or len(pixels) % unit_size != 0:
        return gtex_bytes
    units = [pixels[offset:offset + unit_size] for offset in range(0, len(pixels), unit_size)]
    body = packed_body(units, unit_size)
    if len(body) >= len(pixels):
        return gtex_bytes
    header = bytearray(gtex_bytes[:GTEX_HEADER_SIZE])
    struct.pack_into("<I", header, GTEX_FORMAT_OFFSET, texture_format | PACKED_FLAG)
    return bytes(header) + body


def unpacked_size(gtex_bytes):
    return GTEX_HEADER_SIZE + struct.unpack_from("<I", gtex_bytes, GTEX_HEADER_SIZE - 4)[0]


def unpack_gtex(packed):
    texture_format = struct.unpack_from("<I", packed, GTEX_FORMAT_OFFSET)[0]
    if not texture_format & PACKED_FLAG:
        return packed
    texture_format = texture_format & ~PACKED_FLAG
    unit_size = BYTES_PER_UNIT[texture_format]
    expected = struct.unpack_from("<I", packed, GTEX_HEADER_SIZE - 4)[0]
    out = bytearray()
    offset = GTEX_HEADER_SIZE
    while offset < len(packed):
        token = struct.unpack_from("<I", packed, offset)[0]
        offset += 4
        if token & MATCH_FLAG:
            source = struct.unpack_from("<I", packed, offset)[0]
            offset += 4
            for unit in range(token & ~MATCH_FLAG):
                start = (source + unit) * unit_size
                out += out[start:start + unit_size]
        else:
            size = token * unit_size
            out += packed[offset:offset + size]
            offset += size
    if len(out) != expected:
        raise ValueError("packed texture has the wrong size")
    header = bytearray(packed[:GTEX_HEADER_SIZE])
    struct.pack_into("<I", header, GTEX_FORMAT_OFFSET, texture_format)
    return bytes(header) + bytes(out)
