import struct
import unittest

import message_cues
import talk_assets

CODE_OF_CAPITAL_A = 10
CODE_OF_SMALL_A = 36
CODE_OF_UNKNOWN_SYMBOL = 282


def message_words(*words):
    return struct.pack("<%dH" % len(words), *words)


class CharacterCodeTests(unittest.TestCase):
    def test_control_words_and_their_parameters_are_not_characters(self):
        hold_with_one_parameter = message_cues.CONTROL_CODE_BASE + 0xc
        data = message_words(CODE_OF_CAPITAL_A, hold_with_one_parameter, CODE_OF_SMALL_A, CODE_OF_SMALL_A + 1, message_cues.END_OF_MESSAGE)
        self.assertEqual(message_cues.character_codes(data, 0), [CODE_OF_CAPITAL_A, CODE_OF_SMALL_A + 1])

    def test_a_message_without_an_end_is_refused(self):
        with self.assertRaises(message_cues.CueError):
            message_cues.character_codes(message_words(CODE_OF_SMALL_A), 0)

    def test_letters_digits_spaces_and_periods_decode(self):
        codes = [CODE_OF_CAPITAL_A, CODE_OF_SMALL_A + 1, message_cues.DS_SPACE_CODE, 7, message_cues.DS_PERIOD_CODE]
        self.assertEqual(message_cues.text_of_character_codes(codes), "Ab 7.")

    def test_an_unknown_leading_symbol_leaves_no_space_behind(self):
        codes = [CODE_OF_UNKNOWN_SYMBOL, message_cues.DS_SPACE_CODE, CODE_OF_CAPITAL_A]
        self.assertEqual(message_cues.text_of_character_codes(codes), "A")


class SystemLineSectionTests(unittest.TestCase):
    def setUp(self):
        self.section = talk_assets.system_line_section("one two three four", {"words_on_first_row": 3, "center_x": 464.6})

    def test_the_text_wraps_after_the_named_word(self):
        rows = ["".join(character for character, _ in line) for line in self.section["lines"]]
        self.assertEqual(rows, ["one two three", "four"])

    def test_every_character_is_the_system_green(self):
        colors = {color for line in self.section["lines"] for _, color in line}
        self.assertEqual(colors, {talk_assets.COLOR_SYSTEM_GREEN})

    def test_the_line_closes_itself_after_its_hold(self):
        self.assertEqual(self.section["ending"], talk_assets.ENDING_HOLD_THEN_CLOSE)

    def test_a_line_is_centred_on_its_advance_width(self):
        glyph_rows = [[0, 0, 0, 0, 0, 0, 10], [0, 0, 0, 0, 0, 0, 30]]
        glyph_of = {"a": 0, "b": 1}
        line = [("a", 3), ("b", 3)]
        self.assertEqual(talk_assets.centered_left(line, 100.0, glyph_rows, glyph_of), 80)


if __name__ == "__main__":
    unittest.main()
