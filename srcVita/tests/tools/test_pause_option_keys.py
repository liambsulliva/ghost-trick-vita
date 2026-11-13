import json
import os
import re
import unittest

TESTS_FOLDER = os.path.dirname(os.path.abspath(__file__))
OPTIONS_SOURCE = os.path.join(TESTS_FOLDER, "..", "..", "src", "app", "pause_options.c")
GAME_SET = os.path.join(TESTS_FOLDER, "..", "..", "tools", "assets", "game.json")
ROW = re.compile(r'\{ (?:NULL|"(\w+)"), "(\w+)", ROW_\w+, (JOB_\w+), \d, (?:NULL|"(\w+)") \}')
VALUE_NAME = re.compile(r'static const char \*switch_value_name\(.*?\n\}', re.S)
QUOTED = re.compile(r'"(\w+)"')
ONLY_SUFFIX = "_only"
JOBS_WITH_VALUE_BARS = ("JOB_FPS_CAP", "JOB_VISUAL_FILTER")
PINNED_PREFIX = "filter_"
SOURCE_FOLDER = os.path.join(TESTS_FOLDER, "..", "..", "src", "app")
LITERAL_KEY = re.compile(r'"(bar|row|header|language)\.(\w+)"')


def options_source():
    with open(OPTIONS_SOURCE, encoding="utf-8") as source:
        return source.read()


def widget_strings():
    with open(GAME_SET, encoding="utf-8") as source:
        return json.load(source)["pause"]["widgets"]


class OptionKeyTests(unittest.TestCase):
    def test_every_row_header_value_and_bar_the_options_ask_for_has_a_string(self):
        source = options_source()
        widgets = widget_strings()
        rows = ROW.findall(source)
        self.assertGreater(len(rows), 10)
        value_names = [name for name in QUOTED.findall(VALUE_NAME.search(source).group(0)) if name not in ("on", "off")]
        for header, name, job, fixed in rows:
            self.assertIn(name, widgets["row"], name)
            if header:
                self.assertIn(header, widgets["header"], header)
            if fixed:
                self.assertIn(fixed, widgets["row"], fixed)
            if job not in JOBS_WITH_VALUE_BARS:
                self.assertIn(name, widgets["bar"], name)
        for value in value_names:
            self.assertIn(value, widgets["row"], value)
            self.assertIn(value, widgets["bar"], value)
            if value.startswith(PINNED_PREFIX):
                self.assertIn(value + ONLY_SUFFIX, widgets["bar"], value + ONLY_SUFFIX)

    def test_every_text_key_the_pause_code_names_has_a_string(self):
        widgets = widget_strings()
        languages = set(language["key"] for language in widgets["languages"])
        named = []
        for file_name in sorted(os.listdir(SOURCE_FOLDER)):
            if file_name.startswith("pause_") and file_name.endswith(".c"):
                with open(os.path.join(SOURCE_FOLDER, file_name), encoding="utf-8") as source:
                    named.extend((file_name, style, key) for style, key in LITERAL_KEY.findall(source.read()))
        self.assertGreater(len(named), 10)
        for file_name, style, key in named:
            known = languages if style == "language" else widgets[style]
            self.assertIn(key, known, file_name + ": " + style + "." + key)

    def test_no_string_is_left_for_a_removed_row(self):
        rows = set(name for _, name, _, _ in ROW.findall(options_source()))
        for removed in ("background", "resolution", "screen_mode"):
            self.assertNotIn(removed, rows)
            self.assertNotIn(removed, widget_strings()["bar"])

    def test_two_line_bar_strings_keep_each_line_inside_the_bar(self):
        for key, text in widget_strings()["bar"].items():
            lines = text.split("\n")
            self.assertLessEqual(len(lines), 2, key)
            if len(lines) == 2:
                for line in lines:
                    self.assertLessEqual(len(line), 64, key)


if __name__ == "__main__":
    unittest.main()
