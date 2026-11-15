import unittest
from unittest import mock

import convert_assets
import ds_languages
import ds_textures
import languages

ENGLISH = {"code": "en", "slot": 1, "ds": "en", "art": "en"}
JAPANESE = {"code": "ja", "slot": 0, "ds": "en", "art": "ja"}
FRENCH = {"code": "fr", "slot": 2, "ds": "fr", "art": "fr"}
GERMAN = {"code": "de", "slot": 4, "ds": "gr", "art": "de"}
SCRIPT = "st01/st01_game000_Expand.lmg.xml"


class FakeRom:
    def __init__(self, paths):
        self.paths = set(paths)

    def __enter__(self):
        return self

    def __exit__(self, *_details):
        return False

    def file_id_of(self, path):
        return 1 if path in self.paths else None


def rom_with(*codes):
    return FakeRom(["st01/st01_game000_Expand.lmg.%s.xml.lz" % code for code in codes])


class CarriedLanguageTests(unittest.TestCase):
    def carried(self, every_language, *codes):
        with mock.patch.object(ds_languages.ds_rom, "DsRom", lambda path: rom_with(*codes)):
            return [language["code"] for language in ds_languages.carried_languages(every_language, "game.nds", [SCRIPT])]

    def test_a_set_carries_english_and_each_language_the_cartridge_has_text_for(self):
        self.assertEqual(self.carried([ENGLISH, JAPANESE, FRENCH, GERMAN], "en", "fr", "gr"), ["en", "fr", "de"])

    def test_a_language_whose_text_the_cartridge_lacks_is_not_carried(self):
        self.assertEqual(self.carried([ENGLISH, FRENCH, GERMAN], "en", "fr"), ["en", "fr"])

    def test_a_language_that_borrows_english_cartridge_text_is_not_carried(self):
        self.assertEqual(self.carried([ENGLISH, JAPANESE], "en"), ["en"])

    def test_every_language_with_its_own_cartridge_text_names_a_texture_group(self):
        for language in languages.load_languages():
            if language["ds"] != languages.load_languages()[0]["ds"]:
                self.assertIn(language["ds"], ds_textures.LANGUAGE_NUMBERS, language["code"])


class ListTests(unittest.TestCase):
    def overlays(self, carried, baked):
        files = languages.list_files(carried)
        for language in baked:
            files[languages.names_path(language)] = b""
        return files

    def test_the_list_names_exactly_the_baked_languages_in_order(self):
        self.assertEqual(languages.list_files([ENGLISH, FRENCH, GERMAN]), {languages.LIST_PATH: b"en\nfr\nde\n"})

    def test_a_listed_language_with_no_overlay_is_refused(self):
        with self.assertRaises(ds_languages.DsLanguageError):
            ds_languages.check_listed_languages(self.overlays([ENGLISH, FRENCH, GERMAN], [FRENCH]), [ENGLISH, FRENCH, GERMAN])
        ds_languages.check_listed_languages(self.overlays([ENGLISH, FRENCH, GERMAN], [FRENCH, GERMAN]), [ENGLISH, FRENCH, GERMAN])

    def test_a_list_that_differs_from_what_was_baked_is_refused(self):
        files = self.overlays([ENGLISH, FRENCH], [FRENCH, GERMAN])
        with self.assertRaises(ds_languages.DsLanguageError):
            ds_languages.check_listed_languages(files, [ENGLISH, FRENCH, GERMAN])

    def test_bake_overlays_lists_each_language_it_baked_an_overlay_for(self):
        def overlay(set_description, base_language, language, *rest):
            path = languages.names_path(language)
            return {path: b""}, {path}

        with mock.patch.object(ds_languages, "carried_languages", lambda every, rom, paths: [ENGLISH, FRENCH, GERMAN]), mock.patch.object(ds_languages, "bake_overlay", overlay):
            files, paths = ds_languages.bake_overlays({}, {SCRIPT: None}, None, None, "game.nds", "out", None, None, {})
        self.assertEqual(files[languages.LIST_PATH], b"en\nfr\nde\n")
        self.assertEqual(paths, {languages.LIST_PATH, "lang/fr/names.txt", "lang/de/names.txt"})

    def test_the_remastered_set_lists_every_language_it_baked(self):
        every_language = languages.load_languages()

        def baked(packs, set_description, base_language, language, *rest):
            return {languages.names_path(language): b""}, {languages.names_path(language)}, []

        with mock.patch.object(convert_assets, "convert_language", baked), mock.patch.object(convert_assets.narration, "set_uses_text", lambda description: True):
            files, paths = convert_assets.convert_other_languages(None, {"languages": "languages.json"}, b"key", None, "out", None, {}, {})
        self.assertEqual(files[languages.LIST_PATH].decode("ascii").split(), [language["code"] for language in every_language])
        self.assertIn(languages.LIST_PATH, paths)

    def test_a_set_with_no_translations_lists_english_alone(self):
        files, paths = convert_assets.convert_other_languages(None, {}, None, None, "out", None, {}, {})
        self.assertEqual(files, {languages.LIST_PATH: b"en\n"})
        self.assertEqual(paths, {languages.LIST_PATH})


if __name__ == "__main__":
    unittest.main()
