import os
import shutil
import tempfile
import unittest

import line_renderer
import synthetic_files

WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
PIXELS_PER_EM = 40


def expected_ink_width(character_count):
    return ((character_count - 1) * synthetic_files.GLYPH_ADVANCE_EM + synthetic_files.GLYPH_INK_RIGHT_EM - synthetic_files.GLYPH_INK_LEFT_EM) * PIXELS_PER_EM


def alpha_at(line, x, y):
    return line["pixels"][(y * line["width"] + x) * 4 + 3]


def columns_with_ink(line):
    return [x for x in range(line["width"]) if any(alpha_at(line, x, y) > 127 for y in range(line["height"]))]


def rows_with_ink(line):
    return [y for y in range(line["height"]) if any(alpha_at(line, x, y) > 127 for x in range(line["width"]))]


class LineBoxTests(unittest.TestCase):
    def test_the_box_is_one_em_above_the_baseline_and_three_tenths_below_in_steps_of_four(self):
        self.assertEqual(line_renderer.line_box(100), (100, 132))
        self.assertEqual(line_renderer.line_box(38.5), (39, 52))
        self.assertEqual(line_renderer.line_box(34), (34, 48))

    def test_magick_draws_at_four_times_the_final_size(self):
        arguments = line_renderer.annotate_arguments("font.otf", "line.txt", 40, 800, 200, 45)
        self.assertEqual(arguments[arguments.index("-pointsize") + 1], "160")
        self.assertEqual(arguments[arguments.index("-annotate") + 1], "+200+180")


class RenderLineTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, self.folder)
        self.font_path = os.path.join(self.folder, "box.ttf")
        with open(self.font_path, "wb") as output:
            output.write(synthetic_files.box_font_bytes())

    def render(self, text, max_ink_width=840, color=WHITE, font_path=None):
        return line_renderer.render_line(self.folder, "line", font_path or self.font_path, text, PIXELS_PER_EM, max_ink_width, color)

    def test_the_line_is_as_wide_as_its_glyphs_and_centred(self):
        line = self.render("abcdefgh")
        self.assertAlmostEqual(line["natural_width"], expected_ink_width(8), delta=1)
        columns = columns_with_ink(line)
        self.assertAlmostEqual(columns[-1] - columns[0] + 1, expected_ink_width(8), delta=1)
        left_margin = columns[0]
        right_margin = line["width"] - 1 - columns[-1]
        self.assertLessEqual(abs(left_margin - right_margin), 1)

    def test_sides_are_multiples_of_four_with_room_to_spare(self):
        for text in ("a", "ab", "abc", "abcd", "abcdefghijklmnop"):
            line = self.render(text)
            self.assertEqual(line["width"] % 4, 0)
            self.assertEqual(line["height"] % 4, 0)
            columns = columns_with_ink(line)
            self.assertGreaterEqual(columns[0], 3)
            self.assertLessEqual(columns[-1], line["width"] - 4)

    def test_every_line_shares_one_baseline_and_one_height(self):
        short = self.render("a")
        long = self.render("abcdefghijkl")
        self.assertEqual((short["height"], short["baseline_row"]), (long["height"], long["baseline_row"]))
        rows = rows_with_ink(long)
        self.assertEqual(rows[-1] + 1, long["baseline_row"])
        self.assertEqual(rows[0], long["baseline_row"] - int(synthetic_files.GLYPH_INK_TOP_EM * PIXELS_PER_EM))

    def test_every_pixel_has_the_asked_color_even_where_transparent(self):
        for color in (WHITE, BLACK):
            line = self.render("ab", color=color)
            self.assertEqual(set(line["pixels"][0::4]), {color[0]})
            self.assertEqual(set(line["pixels"][1::4]), {color[1]})
            self.assertEqual(set(line["pixels"][2::4]), {color[2]})
            self.assertEqual(set(line["pixels"][3::4]) >= {0, 255}, True)

    def test_a_line_within_the_limit_is_not_squeezed(self):
        limit = expected_ink_width(8) + 2
        line = self.render("abcdefgh", max_ink_width=limit)
        columns = columns_with_ink(line)
        self.assertAlmostEqual(columns[-1] - columns[0] + 1, expected_ink_width(8), delta=1)
        self.assertEqual(line["drawn_ink_width"], line["natural_width"])

    def test_a_line_over_the_limit_is_squeezed_to_exactly_the_limit_keeping_its_height(self):
        natural = self.render("abcdefghijkl")
        limit = natural["natural_width"] * 0.8
        line = self.render("abcdefghijkl", max_ink_width=limit)
        self.assertAlmostEqual(line["natural_width"], natural["natural_width"], delta=0.5)
        self.assertEqual(line["drawn_ink_width"], limit)
        columns = columns_with_ink(line)
        self.assertAlmostEqual(columns[-1] - columns[0] + 1, limit, delta=2)
        self.assertEqual(rows_with_ink(line), rows_with_ink(natural))
        self.assertLess(line["width"], natural["width"])

    def test_percent_and_backslash_are_drawn_as_plain_characters(self):
        self.assertAlmostEqual(self.render("50%w")["natural_width"], expected_ink_width(4), delta=1)
        self.assertAlmostEqual(self.render("a\\nb")["natural_width"], expected_ink_width(4), delta=1)

    def test_an_unreadable_font_is_refused_without_quoting_the_text(self):
        bad_font_path = os.path.join(self.folder, "bad.otf")
        with open(bad_font_path, "wb") as output:
            output.write(b"not a font at all")
        with self.assertRaises(line_renderer.LineRenderError) as caught:
            self.render("private words", font_path=bad_font_path)
        self.assertNotIn("private", str(caught.exception))

    def test_ink_taller_than_the_line_box_is_refused(self):
        with self.assertRaises(line_renderer.LineRenderError):
            self.render(synthetic_files.TALL_CHARACTER)

    def test_no_work_files_are_left_behind(self):
        self.render("ab")
        self.assertEqual(sorted(os.listdir(self.folder)), ["box.ttf"])


if __name__ == "__main__":
    unittest.main()
