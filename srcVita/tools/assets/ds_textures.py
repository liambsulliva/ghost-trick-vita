import struct

import ds_effects
import ds_sprites

PACK_PATH = "cpac_3d.bin"
TEXTURE_BLOCK = 0
MAGIC = b"sdt."
HEADER = "<4sIBBBxIIIIII"
HEADER_SIZE = struct.calcsize(HEADER)
FORMAT_A3I5 = 1
FORMAT_4_COLORS = 2
FORMAT_16_COLORS = 3
FORMAT_256_COLORS = 4
FORMAT_A5I3 = 6
FORMAT_DIRECT = 7
DIRECT_ALPHA_BIT = 0x8000
INDEX_BITS = {FORMAT_4_COLORS: 2, FORMAT_16_COLORS: 4, FORMAT_256_COLORS: 8}
SMALLEST_ROW = 8
FULL_ALPHA = 255
LANGUAGE_NUMBERS = {"en": 1, "fr": 2, "gr": 3, "it": 4, "sp": 5}


class TextureRefused(ValueError):
    pass


def color_of(word):
    return ((word & 0x1f) * 255 // 31, ((word >> 5) & 0x1f) * 255 // 31, ((word >> 10) & 0x1f) * 255 // 31)


def packed_indexes(data, bits):
    mask = (1 << bits) - 1
    indexes = []
    for byte in data:
        for shift in range(0, 8, bits):
            indexes.append(((byte >> shift) & mask, FULL_ALPHA))
    return indexes


def translucent_indexes(data, index_bits):
    alpha_levels = (1 << (8 - index_bits)) - 1
    mask = (1 << index_bits) - 1
    return [(byte & mask, (byte >> index_bits) * FULL_ALPHA // alpha_levels) for byte in data]


def direct_colors(data, width_code, width, height):
    row_width = SMALLEST_ROW << width_code
    if width > row_width or width == 0 or height == 0 or len(data) < row_width * height * 2:
        raise TextureRefused("a DS texture's size does not match its pixels")
    words = struct.unpack_from("<%dH" % (row_width * height), data, 0)
    rgba = bytearray()
    for row in range(height):
        for word in words[row * row_width:row * row_width + width]:
            rgba += bytes(color_of(word)) + bytes((FULL_ALPHA if word & DIRECT_ALPHA_BIT else 0,))
    return width, height, bytes(rgba)


def decode(texture_bytes, first_color_is_clear=True):
    if len(texture_bytes) < HEADER_SIZE:
        raise TextureRefused("a DS texture is shorter than its header")
    magic, _version, texture_format, width_code, _height_code, data_offset, data_size, palette_offset, palette_size, width, height = struct.unpack_from(HEADER, texture_bytes, 0)
    if magic != MAGIC:
        raise TextureRefused("a DS texture does not begin with its magic")
    if data_offset + data_size > len(texture_bytes) or palette_offset + palette_size > len(texture_bytes):
        raise TextureRefused("a DS texture's pixels or palette run past its end")
    data = texture_bytes[data_offset:data_offset + data_size]
    palette = [color_of(word) for word in struct.unpack_from("<%dH" % (palette_size // 2), texture_bytes, palette_offset)]
    if texture_format in INDEX_BITS:
        indexes = packed_indexes(data, INDEX_BITS[texture_format])
        if first_color_is_clear:
            indexes = [(index, 0 if index == 0 else alpha) for index, alpha in indexes]
    elif texture_format == FORMAT_A3I5:
        indexes = translucent_indexes(data, 5)
    elif texture_format == FORMAT_A5I3:
        indexes = translucent_indexes(data, 3)
    elif texture_format == FORMAT_DIRECT:
        return direct_colors(data, width_code, width, height)
    else:
        raise TextureRefused("DS texture format %d is not decoded" % texture_format)
    row_width = SMALLEST_ROW << width_code
    if width > row_width or width == 0 or height == 0 or len(indexes) < row_width * height:
        raise TextureRefused("a DS texture's size does not match its pixels")
    rgba = bytearray()
    for row in range(height):
        for index, alpha in indexes[row * row_width:row * row_width + width]:
            if index >= len(palette):
                raise TextureRefused("a DS texture names a color past its palette")
            rgba += bytes(palette[index]) + bytes((alpha,))
    return width, height, bytes(rgba)


class TexturePack:
    def __init__(self, pack_bytes):
        self.pack = ds_sprites.SpritePack(pack_bytes)

    def texture_bytes(self, index, language=1):
        try:
            if isinstance(index, dict):
                return ds_effects.EffectPack(self.pack).texture_bytes(index["effect"], index["item"], language)
            return self.pack.entry(TEXTURE_BLOCK, index, language)
        except (ds_sprites.SpriteRefused, ds_effects.EffectRefused) as error:
            raise TextureRefused(str(error))

    def texture(self, index, language=1, first_color_is_clear=True):
        return decode(self.texture_bytes(index, language), first_color_is_clear)
