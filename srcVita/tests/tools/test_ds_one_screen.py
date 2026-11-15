import os
import unittest

import ds_one_screen
import ds_text
import languages

SRC_VITA = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
ROM_PATH = os.path.join(os.path.dirname(SRC_VITA), "extracted", "ds_rom", "game.nds")
CODE = ds_text.CODE_OF_CHARACTER
BREAK = ("control", ds_text.NEW_LINE, ())
PAGE = ("control", ds_text.NEW_PAGE, ())
HIGHLIGHT = ("control", ds_text.COLOR, (6,))
PLAIN = ("control", ds_text.COLOR, (15,))
FACE = ("control", 0x1b, (1,))
SOUND = ("control", 0x0d, (0x16,))
TOP_SCREEN_WORDS = {"en": ["top screen", "upper screen"], "fr": ["écran supérieur"], "gr": ["oberen bildschirm"], "it": ["schermo superiore"], "sp": ["pantalla superior"]}


def chars(text):
    items = []
    for character in text:
        items.append(BREAK if character == "|" else ("char", CODE[character]))
    return items


def message(items):
    return {"items": items, "pages": ds_text.pages_of_items(items)}


def table(rewrites=(), dropped=(), refused=("top screen",)):
    return {"en": {"rewrites": [list(pair) for pair in rewrites], "dropped_pages": list(dropped), "refused": list(refused)}}


class RewriteTests(unittest.TestCase):
    def test_a_rewrite_keeps_the_controls_inside_the_phrase_where_the_marks_put_them(self):
        items = chars("a look at the ") + [HIGHLIGHT] + chars("top screen") + [PLAIN] + chars("?")
        one = ds_one_screen.OneScreen("en", table([("a look at the top screen?", "a look at ^the middle of|the screen^?")]))
        result = one.message("m", message(items))
        self.assertEqual(result["items"], chars("a look at ") + [HIGHLIGHT] + chars("the middle of|the screen") + [PLAIN] + chars("?"))
        self.assertEqual([line["text"] for line in result["pages"][0][0]], ["a look at the middle of", "the screen?"])

    def test_a_phrase_matches_across_a_line_break_and_leaves_other_pages_alone(self):
        items = chars("Have a|look at the top screen.") + [PAGE] + chars("The top screen, eh?")
        one = ds_one_screen.OneScreen("en", table([("look at the top screen.", "look at the right side|of the screen."), ("The top screen, eh?", "The right side, eh?")]))
        result = one.message("m", message(items))
        self.assertEqual(result["items"], chars("Have a|look at the right side|of the screen.") + [PAGE] + chars("The right side, eh?"))
        self.assertEqual(one.unused(), [])

    def test_a_phrase_never_matches_across_a_page_end(self):
        items = chars("the top") + [PAGE] + chars("screen")
        one = ds_one_screen.OneScreen("en", table([("the top|screen", "x")], refused=()))
        self.assertEqual(one.message("m", message(items))["items"], items)
        self.assertEqual(one.unused(), ["the top|screen"])

    def test_a_dropped_page_hands_its_controls_to_the_next_page(self):
        items = chars("Hurry!") + [PAGE, FACE, SOUND] + chars("(I can check the ") + [HIGHLIGHT] + chars("top screen") + [PLAIN] + chars(".)") + [PAGE] + chars("(Got it.)")
        one = ds_one_screen.OneScreen("en", table(dropped=["(I can check the top screen.)"]))
        result = one.message("m", message(items))
        self.assertEqual(result["items"], chars("Hurry!") + [PAGE, FACE, SOUND, HIGHLIGHT, PLAIN] + chars("(Got it.)"))
        self.assertEqual(len(result["pages"]), 2)

    def test_a_dropped_last_page_folds_its_controls_into_the_page_before(self):
        items = chars("Hurry!") + [PAGE, FACE] + chars("(the top screen)")
        one = ds_one_screen.OneScreen("en", table(dropped=["(the top screen)"]))
        result = one.message("m", message(items))
        self.assertEqual(result["items"], chars("Hurry!") + [FACE])
        self.assertEqual(len(result["pages"]), 1)

    def test_a_rewrite_that_loses_a_control_is_refused(self):
        items = chars("the ") + [HIGHLIGHT] + chars("top screen") + [PLAIN]
        one = ds_one_screen.OneScreen("en", table([("the top screen", "the middle of the screen")]))
        with self.assertRaises(ds_one_screen.DsOneScreenError):
            one.message("m", message(items))

    def test_a_rewrite_that_holds_its_own_source_is_refused(self):
        one = ds_one_screen.OneScreen("en", table([("screen", "the screen")], refused=()))
        with self.assertRaises(ds_one_screen.DsOneScreenError):
            one.message("m", message(chars("screen")))

    def test_a_message_that_still_names_the_top_screen_stops_the_bake(self):
        one = ds_one_screen.OneScreen("en", table())
        with self.assertRaises(ds_one_screen.DsOneScreenError):
            one.message("m", message(chars("Look at the top|screen.")))

    def test_every_cartridge_language_refuses_the_top_screen(self):
        every = ds_one_screen.load_table()
        for code in sorted({language["ds"] for language in languages.load_languages()}):
            refused = every.get(code, {}).get("refused", [])
            self.assertTrue(refused, code)


def rom_paths(rom, directory=0, prefix=""):
    for name, _, child in rom.directory_entries(directory):
        if child is not None:
            yield from rom_paths(rom, child, prefix + name + "/")
        else:
            yield prefix + name


@unittest.skipUnless(os.path.isfile(ROM_PATH), "needs the cartridge")
class CartridgeTests(unittest.TestCase):
    def test_no_baked_message_of_any_chapter_or_language_names_the_top_screen(self):
        import chapter_assets
        import ds_rom
        import ds_set_text

        self.assertEqual(sorted(TOP_SCREEN_WORDS), sorted({language["ds"] for language in languages.load_languages()}))
        with ds_rom.DsRom(ROM_PATH) as rom:
            arm9 = chapter_assets.read_arm9(rom)
            font = ds_text.font_from_rom(rom, ds_text.MAIN_FONT, arm9)
            window = ds_set_text.read_window_rows(arm9)[ds_set_text.BOTTOM_ROW]
            paths = list(rom_paths(rom))
            for code, words in TOP_SCREEN_WORDS.items():
                ending = ds_text.script_message_path("", code)
                scripts = {path[:-len(ending)] + ".xml": None for path in paths if path.startswith("st") and path.endswith(ending)}
                self.assertGreater(len(scripts), 100, code)
                book = ds_set_text.MessageBook(rom, scripts, code)
                baked = [(path, label, found) for path, messages in book.by_script.items() for label, found in messages.items()]
                baked += [(ds_text.system_message_path(code), label, found) for label, found in ds_set_text.read_system_messages(rom, {label: {"file": ds_text.system_message_path(code)} for label in ds_text.read_rom_message_file(rom, ds_text.system_message_path(code))}, code).items()]
                cartridge = {}
                rewritten = 0
                for path, label, found in baked:
                    text = ds_one_screen.plain_text(found["items"]).lower()
                    for word in words:
                        self.assertNotIn(word, text, "%s %s" % (path, label))
                    if path not in cartridge:
                        cartridge[path] = ds_text.read_rom_message_file(rom, ds_text.script_message_path(path, code) if path in scripts else path)
                    if found["items"] == cartridge[path][label]["items"]:
                        continue
                    rewritten = rewritten + 1
                    for page in found["pages"]:
                        for paragraph in page:
                            for line in paragraph:
                                self.assertLessEqual(ds_text.measured_width(font, line["items"]), window["width"] - 2 * window["left"], "%s %s %r" % (path, label, line["text"]))
                self.assertEqual(book.one_screen.unused(), [], code)
                self.assertGreater(rewritten, 0, code)


if __name__ == "__main__":
    unittest.main()
