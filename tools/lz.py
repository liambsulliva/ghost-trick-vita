import struct


def read_u16(data, offset):
    return struct.unpack_from("<H", data, offset)[0]


def read_u32(data, offset):
    return struct.unpack_from("<I", data, offset)[0]


def lz10_decompress(data):
    size = read_u32(data, 0) >> 8
    output = bytearray()
    pos = 4
    while len(output) < size:
        if pos >= len(data):
            return None
        flags = data[pos]
        pos = pos + 1
        bit = 0
        while bit < 8 and len(output) < size:
            if flags & 0x80:
                if pos + 1 >= len(data):
                    return None
                first = data[pos]
                second = data[pos + 1]
                pos = pos + 2
                length = (first >> 4) + 3
                distance = (((first & 0x0F) << 8) | second) + 1
                if distance > len(output):
                    return None
                count = 0
                while count < length:
                    output.append(output[len(output) - distance])
                    count = count + 1
            else:
                if pos >= len(data):
                    return None
                output.append(data[pos])
                pos = pos + 1
            flags = (flags << 1) & 0xFF
            bit = bit + 1
    return bytes(output)


def lz11_decompress(data):
    size = read_u32(data, 0) >> 8
    output = bytearray()
    pos = 4
    while len(output) < size:
        if pos >= len(data):
            return None
        flags = data[pos]
        pos = pos + 1
        bit = 0
        while bit < 8 and len(output) < size:
            if flags & 0x80:
                if pos + 1 >= len(data):
                    return None
                first = data[pos]
                indicator = first >> 4
                if indicator == 0:
                    if pos + 2 >= len(data):
                        return None
                    length = (((first & 0x0F) << 4) | (data[pos + 1] >> 4)) + 0x11
                    distance = (((data[pos + 1] & 0x0F) << 8) | data[pos + 2]) + 1
                    pos = pos + 3
                elif indicator == 1:
                    if pos + 3 >= len(data):
                        return None
                    length = (((first & 0x0F) << 12) | (data[pos + 1] << 4) | (data[pos + 2] >> 4)) + 0x111
                    distance = (((data[pos + 2] & 0x0F) << 8) | data[pos + 3]) + 1
                    pos = pos + 4
                else:
                    length = indicator + 1
                    distance = (((first & 0x0F) << 8) | data[pos + 1]) + 1
                    pos = pos + 2
                if distance > len(output):
                    return None
                count = 0
                while count < length:
                    output.append(output[len(output) - distance])
                    count = count + 1
            else:
                if pos >= len(data):
                    return None
                output.append(data[pos])
                pos = pos + 1
            flags = (flags << 1) & 0xFF
            bit = bit + 1
    return bytes(output)


def try_decompress(data):
    if len(data) < 5:
        return None
    if data[0] == 0x10:
        return lz10_decompress(data)
    if data[0] == 0x11:
        return lz11_decompress(data)
    return None


def magic_text(data):
    head = data[:4]
    text = ""
    for b in head:
        if b >= 0x20 and b < 0x7F:
            text = text + chr(b)
        else:
            text = text + "."
    return text
