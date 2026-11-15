import json
import os
import struct
import unittest

import ds_effects
import ds_textures
import ui_clips

SRC_VITA = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
GAME_DESCRIPTION = os.path.join(SRC_VITA, "tools", "assets", "game.json")
ROM_PATH = os.path.join(os.path.dirname(SRC_VITA), "extracted", "ds_rom", "game.nds")
ASPECT_TOLERANCE = 0.02
LANGUAGES = range(1, 6)
COMPRESSED_HEADER = bytes([ds_effects.COMPRESSED_FLAG]) + bytes(7)
RAW_HEADER = bytes([0x80]) + bytes(7)


def stored_lz10(plain):
    stream = bytearray(struct.pack("<I", 0x10 | (len(plain) << 8)))
    for start in range(0, len(plain), 8):
        stream.append(0)
        stream += plain[start:start + 8]
    return bytes(stream)


def effect_file(items):
    offsets = []
    body = b""
    for item in items:
        offsets.append(len(body))
        body += item
    return struct.pack("<%dI" % (len(items) + 1), len(items), *offsets) + body


def ds_entries():
    with open(GAME_DESCRIPTION, encoding="utf-8") as source:
        return json.load(source)["ds_textures"]


def effect_parts():
    return [(entry, part) for entry in ds_entries() for part in entry["parts"] if isinstance(part["texture"], dict)]


class EffectFileTests(unittest.TestCase):
    def test_a_texture_item_is_an_eight_byte_header_then_lz10_when_its_first_bit_is_set(self):
        plain = b"sdt." + bytes(range(20))
        effect = effect_file([b"bws.script", COMPRESSED_HEADER + stored_lz10(plain), RAW_HEADER + b"sdt.raw"])
        self.assertEqual(ds_effects.item_count(effect), 3)
        self.assertEqual(ds_effects.texture_item(effect, 1), plain)
        self.assertEqual(ds_effects.texture_item(effect, 2), b"sdt.raw")

    def test_the_script_item_and_missing_items_are_refused(self):
        effect = effect_file([b"bws.script", COMPRESSED_HEADER + stored_lz10(b"sdt.")])
        for item in (0, 2, -1):
            with self.assertRaises(ds_effects.EffectRefused):
                ds_effects.texture_item(effect, item)

    def test_a_table_past_the_end_is_refused(self):
        with self.assertRaises(ds_effects.EffectRefused):
            ds_effects.item_offsets(struct.pack("<I", 50))


class EffectTextureEntryTests(unittest.TestCase):
    def test_effect_references_name_an_effect_and_a_texture_item(self):
        parts = effect_parts()
        self.assertTrue(parts)
        for entry, part in parts:
            self.assertEqual(sorted(part["texture"]), ["effect", "item"], entry["name"])
            self.assertGreater(part["texture"]["item"], ds_effects.SCRIPT_ITEM, entry["name"])

    def test_every_banner_keeps_the_shape_the_app_draws_it_at(self):
        entries = {entry["name"]: entry for entry in ds_entries()}
        for kind in ui_clips.BANNER_KINDS.values():
            entry = entries[kind["texture"]]
            width, height = kind["size"]
            self.assertAlmostEqual(entry["width"] / entry["height"], width / height, delta=width / height * ASPECT_TOLERANCE, msg=kind["texture"])


@unittest.skipUnless(os.path.isfile(ROM_PATH), "needs the cartridge")
class CartridgeEffectTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        import ds_rom
        with ds_rom.DsRom(ROM_PATH) as rom:
            cls.textures = ds_textures.TexturePack(rom.read_file(ds_textures.PACK_PATH))

    def test_every_effect_texture_the_set_names_decodes_in_every_language(self):
        for entry, part in effect_parts():
            for language in LANGUAGES:
                width, height, _ = self.textures.texture(part["texture"], language)
                if "crop" in part:
                    left, top, crop_width, crop_height = part["crop"]
                    self.assertLessEqual((left + crop_width, top + crop_height), (width, height), entry["name"])

    def test_every_texture_item_of_every_effect_decodes(self):
        effects = ds_effects.EffectPack(self.textures.pack)
        for index in sorted(self.textures.pack.entries(ds_effects.EFFECT_BLOCK)):
            effect = effects.effect_bytes(index)
            for item in range(1, ds_effects.item_count(effect)):
                plain = ds_effects.texture_item(effect, item)
                if plain[:4] == ds_textures.MAGIC:
                    ds_textures.decode(plain)


if __name__ == "__main__":
    unittest.main()
