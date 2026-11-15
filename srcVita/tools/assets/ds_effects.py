import struct

import ds_sprites
import lz

EFFECT_BLOCK = 3
ITEM_HEADER_SIZE = 8
COMPRESSED_FLAG = 1
SCRIPT_ITEM = 0


class EffectRefused(ValueError):
    pass


def item_offsets(effect_bytes):
    if len(effect_bytes) < 4:
        raise EffectRefused("a DS effect is shorter than its item count")
    count = struct.unpack_from("<I", effect_bytes, 0)[0]
    base = 4 + 4 * count
    if count == 0 or base > len(effect_bytes):
        raise EffectRefused("a DS effect's item table runs past its end")
    starts = [base + offset for offset in struct.unpack_from("<%dI" % count, effect_bytes, 4)]
    ends = starts[1:] + [len(effect_bytes)]
    if any(start > end for start, end in zip(starts, ends)):
        raise EffectRefused("a DS effect's items are out of order")
    return list(zip(starts, ends))


def item_count(effect_bytes):
    return len(item_offsets(effect_bytes))


def texture_item(effect_bytes, item):
    places = item_offsets(effect_bytes)
    if item == SCRIPT_ITEM or not 0 <= item < len(places):
        raise EffectRefused("a DS effect has no texture item %d" % item)
    start, end = places[item]
    if end - start < ITEM_HEADER_SIZE:
        raise EffectRefused("a DS effect's item %d is shorter than its header" % item)
    stored = effect_bytes[start + ITEM_HEADER_SIZE:end]
    if not effect_bytes[start] & COMPRESSED_FLAG:
        return stored
    plain = lz.try_decompress(stored)
    if plain is None:
        raise EffectRefused("a DS effect's texture item %d does not decompress" % item)
    return plain


class EffectPack:
    def __init__(self, sprite_pack):
        self.pack = sprite_pack

    def effect_bytes(self, effect, language=1):
        try:
            return self.pack.entry(EFFECT_BLOCK, effect, language)
        except ds_sprites.SpriteRefused as error:
            raise EffectRefused(str(error))

    def texture_bytes(self, effect, item, language=1):
        return texture_item(self.effect_bytes(effect, language), item)
