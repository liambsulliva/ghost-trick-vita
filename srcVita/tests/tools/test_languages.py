import json
import os
import re
import shutil
import tempfile
import unittest

import bake_record
import font_file
import hud_art
import languages
import message_file
import narration
import talk_assets

SRC_VITA_FOLDER = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
ASSET_TOOLS_FOLDER = os.path.join(SRC_VITA_FOLDER, "tools", "assets")
GAME_SET = os.path.join(ASSET_TOOLS_FOLDER, "game.json")
FONT_A = "fonts/a.oft"
FONT_B = "fonts/b.oft"
ENGLISH = {"code": "en", "slot": 1, "ds": "en", "art": "en"}
FRENCH = {"code": "fr", "slot": 2, "ds": "fr", "art": "fr", "fonts": {FONT_A: FONT_B}, "text": {"pause": {"labels": ["REPRENDRE"], "widgets": {"row": {"yes": "Oui"}}}}}


def read_source(relative_path):
    with open(os.path.join(SRC_VITA_FOLDER, relative_path), "r", encoding="utf-8") as source:
        return source.read()


def c_string_list(text, name):
    match = re.search(r"%s\[[^\]]*\] = \{([^}]*)\}" % name, text)
    return re.findall(r'"([^"]*)"', match.group(1))


def strings_in(value, path=""):
    if isinstance(value, dict):
        for key, item in value.items():
            yield from strings_in(item, path + "." + key)
    elif isinstance(value, list):
        for index, item in enumerate(value):
            yield from strings_in(item, "%s[%d]" % (path, index))
    elif isinstance(value, str):
        yield path, value


class EveryLanguageIsComplete(unittest.TestCase):
    def setUp(self):
        self.languages = languages.load_languages()
        with open(GAME_SET, "r", encoding="ascii") as source:
            self.game = json.load(source)

    def test_the_app_lists_the_same_languages_in_the_same_order(self):
        keys = [language["key"] for language in self.languages]
        codes = [language["code"] for language in self.languages]
        self.assertEqual(c_string_list(read_source("src/app/pause_options.c"), "languages"), keys)
        self.assertEqual(c_string_list(read_source("src/app/asset_language.c"), "codes"), codes)
        self.assertEqual([language["key"] for language in self.game["pause"]["widgets"]["languages"]], keys)
        self.assertEqual(len(set(language["slot"] for language in self.languages)), len(self.languages))

    def test_every_language_translates_every_port_string_the_english_set_has(self):
        english_text = {"hud": {"words": self.game["hud"]["words"]}, "pause": {"labels": self.game["pause"]["labels"], "people": {"phone_word": self.game["pause"]["people"]["phone_word"]}, "widgets": {key: self.game["pause"]["widgets"][key] for key in ("chapter_format", "row", "header", "bar", "title")}}}
        wanted = set(path for path, _ in strings_in(english_text))
        for language in languages.other_languages(self.languages):
            given = dict(strings_in(language["text"]))
            self.assertEqual(set(given), wanted, language["code"])
            self.assertTrue(all(text.strip() or text == " " for text in given.values()), language["code"])
            self.assertEqual(given[".pause.widgets.chapter_format"].count("%d"), 1, language["code"])
            self.assertEqual(set(language["system_lines"]), set(self.game["talk"]["system_lines"]), language["code"])
            localized = languages.localized_set(self.game, self.languages[0], language)
            self.assertEqual(localized["pause"]["widgets"]["languages"], self.game["pause"]["widgets"]["languages"], language["code"])
            self.assertEqual(localized["hud"]["key_font"], self.game["hud"]["key_font"], language["code"])

    def test_every_language_font_is_a_remaster_font_the_set_already_names_or_maps(self):
        english_fonts = set(text for _, text in strings_in(self.game) if text.endswith(".oft.1"))
        for language in languages.other_languages(self.languages):
            self.assertLessEqual(set(language.get("fonts", {})), english_fonts, language["code"])
            for target in language.get("fonts", {}).values():
                self.assertTrue(target.startswith("natives/android/gui/font/outlinefont/"), target)


class LocalizedSets(unittest.TestCase):
    def test_fonts_art_and_text_are_swapped_and_the_ds_language_follows(self):
        base = {"font": FONT_A, "screen_textures": [{"name": "card", "source": "x/card_iml3.tex.691230823.en"}, {"name": "room", "source": "x/room.tex.691230823"}], "scripts": {"language": "en"}, "pause": {"labels": ["RESUME"], "widgets": {"row": {"yes": "Yes", "no": "No"}}}, "talk": {"system_lines": {"sys": {"file": "system/system_0000.en.msg.xml.lz"}}}}
        localized = languages.localized_set(base, ENGLISH, FRENCH)
        self.assertEqual(localized["font"], FONT_B)
        self.assertEqual(localized["screen_textures"][0]["source"], "x/card_iml3.tex.691230823.fr")
        self.assertEqual(localized["screen_textures"][1]["source"], "x/room.tex.691230823")
        self.assertEqual(localized["scripts"]["language"], "fr")
        self.assertEqual(localized["pause"]["labels"], ["REPRENDRE"])
        self.assertEqual(localized["pause"]["widgets"]["row"], {"yes": "Oui", "no": "No"})
        self.assertEqual(localized["talk"]["system_lines"]["sys"]["file"], "system/system_0000.fr.msg.xml.lz")
        self.assertEqual(base["font"], FONT_A)

    def test_the_language_list_and_the_button_keys_keep_their_fonts(self):
        base = {"hud": {"label_font": FONT_A, "key_font": FONT_A}, "pause": {"widgets": {"font": FONT_A, "languages": [{"key": "french", "text": "Fran\u00e7ais", "font": FONT_A}]}}}
        localized = languages.localized_set(base, ENGLISH, dict(FRENCH, text={}))
        self.assertEqual(localized["pause"]["widgets"]["font"], FONT_B)
        self.assertEqual(localized["hud"], {"label_font": FONT_B, "key_font": FONT_A})
        self.assertEqual(localized["pause"]["widgets"]["languages"], base["pause"]["widgets"]["languages"])

    def test_a_translation_for_text_the_set_does_not_have_is_refused(self):
        with self.assertRaises(languages.LanguageError):
            languages.merged({"row": {"yes": "Yes"}}, {"row": {"maybe": "Peut-être"}})
        with self.assertRaises(languages.LanguageError):
            languages.merged({"labels": ["A"]}, {"labels": "A"})

    def test_the_language_key_reads_its_own_slot(self):
        key = languages.language_key(bytes(16), FRENCH)
        self.assertEqual(message_file.slot_of(key), 2)
        self.assertEqual(message_file.slot_of(bytes(16)), message_file.ENGLISH_SLOT)
        self.assertEqual(bytes(key), bytes(16))


class LanguageOverlay(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, self.folder)
        self.record = bake_record.BakeRecord(self.folder)

    def test_output_the_same_as_english_stays_in_the_base_set(self):
        key = self.record.key_for("code", {}, b"same")
        self.record.store("pause/cat.txt", key, b"cat")
        overlay = languages.LanguageRecord(self.record, FRENCH)
        self.assertEqual(overlay.read_if_current("pause/cat.txt", key), b"cat")
        self.assertEqual(overlay.overlay_paths, set())

    def test_output_that_differs_goes_to_the_language_folder_and_is_reused(self):
        self.record.store("scripts/talk.txt", self.record.key_for("code", {}, b"english"), b"english")
        overlay = languages.LanguageRecord(self.record, FRENCH)
        key = overlay.key_for("code", {}, b"french")
        self.assertIsNone(overlay.read_if_current("scripts/talk.txt", key))
        overlay.store("scripts/talk.txt", key, b"french", {"rows": 1})
        self.assertEqual(overlay.overlay_paths, {"lang/fr/scripts/talk.txt"})
        with open(os.path.join(self.folder, "lang", "fr", "scripts", "talk.txt"), "rb") as source:
            self.assertEqual(source.read(), b"french")
        again = languages.LanguageRecord(self.record, FRENCH)
        self.assertEqual(again.read_if_current("scripts/talk.txt", key), b"french")
        self.assertEqual(again.details_of("scripts/talk.txt"), {"rows": 1})
        self.assertEqual(again.overlay_paths, {"lang/fr/scripts/talk.txt"})

    def test_output_with_a_new_key_but_english_bytes_is_not_copied(self):
        self.record.store("textures/pause_cat_00.gtex", self.record.key_for("code", {"labels": ["RESUME"]}, b""), b"GTEXcat")
        overlay = languages.LanguageRecord(self.record, FRENCH)
        key = overlay.key_for("code", {"labels": ["REPRENDRE"]}, b"")
        self.assertIsNone(overlay.read_if_current("textures/pause_cat_00.gtex", key))
        overlay.store("textures/pause_cat_00.gtex", key, b"GTEXcat")
        self.assertEqual(overlay.overlay_paths, set())
        self.assertEqual(overlay.same_paths, {"lang/fr/textures/pause_cat_00.gtex"})
        self.assertFalse(os.path.exists(os.path.join(self.folder, "lang", "fr", "textures", "pause_cat_00.gtex")))
        again = languages.LanguageRecord(self.record, FRENCH)
        self.assertIsNotNone(again.read_if_current("textures/pause_cat_00.gtex", key))
        self.assertEqual(again.overlay_paths, set())

    def test_language_textures_are_packed_like_base_textures(self):
        self.assertTrue(bake_record.is_texture_path("lang/fr/textures/text_a.gtex"))
        self.assertTrue(bake_record.is_texture_path("textures/text_a.gtex"))
        self.assertFalse(bake_record.is_texture_path("lang/fr/scripts/talk.txt"))
        self.assertFalse(bake_record.is_texture_path("lang/textures.gtex"))

    def test_the_names_list_and_the_sweep_keep_only_this_run_s_files(self):
        files = languages.overlay_files(FRENCH, {"scripts/talk.txt": b"fr", "pause/cat.txt": b"cat"}, {"scripts/talk.txt": b"en", "pause/cat.txt": b"cat"})
        self.assertEqual(files, {"lang/fr/scripts/talk.txt": b"fr"})
        paths = set(files) | {"lang/fr/textures/text_a.gtex"}
        self.assertEqual(languages.names_text(paths, FRENCH), "scripts/talk.txt\ntextures/text_a.gtex\n")
        for relative_path in ["lang/fr/scripts/talk.txt", "lang/fr/scripts/old.cues", "lang/ko/textures/text_b.gtex"]:
            path = os.path.join(self.folder, relative_path)
            os.makedirs(os.path.dirname(path), exist_ok=True)
            with open(path, "wb") as output:
                output.write(b"x")
        removed = languages.remove_overlay_files_not_listed(self.folder, {"lang/fr/scripts/talk.txt"})
        self.assertEqual(sorted(removed), ["lang/fr/scripts/old.cues", "lang/ko/textures/text_b.gtex"])
        self.assertFalse(os.path.exists(os.path.join(self.folder, "lang", "ko")))


class TextThatEveryLanguageNeeds(unittest.TestCase):
    def test_a_font_without_a_glyph_for_the_text_is_named(self):
        mapped = set(ord(character) for character in "ABC ")
        self.assertEqual(font_file.missing_characters(b"", "AB C\n", mapped), [])
        self.assertEqual(font_file.missing_characters(b"", "ABÉ枪", mapped), ["É", "枪"])

    def test_a_character_the_first_font_lacks_comes_from_its_fallback(self):
        fonts = narration.TextFonts(None, None, {FONT_A: [FONT_B]})
        fonts.characters_by_path = {FONT_A: set(map(ord, "中文")), FONT_B: set(map(ord, "♪A"))}
        self.assertEqual(fonts.font_for(FONT_A, "中"), FONT_A)
        self.assertEqual(fonts.font_for(FONT_A, "♪"), FONT_B)
        self.assertEqual(fonts.font_for(FONT_A, "枪"), FONT_A)
        fonts.require(FONT_A, "中文♪", may_fall_back=True)
        with self.assertRaises(narration.NarrationError):
            fonts.require(FONT_A, "中文♪")
        with self.assertRaises(narration.NarrationError):
            fonts.require(FONT_A, "枪", may_fall_back=True)

    def test_bubbles_wrap_cjk_text_between_characters_and_latin_text_between_words(self):
        self.assertEqual(narration.wrapped_bubble_lines(["one two three four five six seven eight"]), ["one two three four five six", "seven eight"])
        lines = narration.wrapped_bubble_lines(["あいうえおかきくけこさしすせそたちつてと"])
        self.assertEqual(lines, ["あいうえおかきくけこさしすせそ", "たちつてと"])
        self.assertTrue(all(narration.width_in_columns(line) <= narration.BUBBLE_CHARACTERS_PER_LINE for line in lines))
        self.assertEqual(narration.wrapped_bubble_lines(["「はい」と言った。"]), ["「はい」と言った。"])

    def test_the_talk_atlas_widens_rather_than_growing_past_its_height_limit(self):
        glyph = {"width": 28, "height": 30}
        self.assertEqual(talk_assets.atlas_width_for([glyph] * 100), 1024)
        self.assertEqual(talk_assets.atlas_width_for([glyph] * 2500), 2048)
        placed, height = talk_assets.packed_places([glyph] * 2500, talk_assets.atlas_width_for([glyph] * 2500))
        self.assertLessEqual(height, talk_assets.ATLAS_HEIGHT_LIMIT)
        self.assertEqual(len(placed), 2500)

    def test_a_system_line_with_its_own_break_keeps_it(self):
        section = talk_assets.system_line_section("新しい情報が\n追加されました。", {"words_on_first_row": 3, "center_x": 464.6})
        self.assertEqual(["".join(character for character, _ in line) for line in section["lines"]], ["新しい情報が", "追加されました。"])
        section = talk_assets.system_line_section("New info has been added.", {"words_on_first_row": 3, "center_x": 464.6})
        self.assertEqual(["".join(character for character, _ in line) for line in section["lines"]], ["New info has", "been added."])

    def test_the_hud_words_default_to_english_and_cover_every_cell(self):
        self.assertEqual(list(hud_art.ENGLISH_WORDS), hud_art.WORD_KEYS)
        with open(GAME_SET, "r", encoding="ascii") as source:
            self.assertEqual(json.load(source)["hud"]["words"], hud_art.ENGLISH_WORDS)


if __name__ == "__main__":
    unittest.main()
