import contextlib
import io
import json
import os
import shutil
import tempfile
import unittest

import asset_writers
import bake_record
import frame_layout
import gtex_pack
import line_renderer
import narration
import synthetic_files

RUN_THE_TOOL = "run python3 -I srcVita/tools/assets/extract_text_key.py with your own copy of the Android package"
FONT_PACK_PATH = "font.oft"
FILE_PATH = "text.msg"
KEY = bytes(range(10, 18))
NARRATION_LINE_HEIGHT = 40
GLYPH_ADVANCE_IN_NARRATION_PIXELS = 18.0
GLYPH_INK_IN_NARRATION_PIXELS = 14.9
NARRATION_WIDTH_LIMIT_WITH_PADDING = 648


def line_record(line_id, top_y, natural_width=100.0):
    return {"id": line_id, "texture": "tex_%d" % line_id, "top_y": top_y, "natural_width": natural_width}


class FakePacks:
    def __init__(self, files):
        self.files = files
        self.read_paths = []

    def read(self, path):
        self.read_paths.append(path)
        return self.files[path]


class LayoutTests(unittest.TestCase):
    def test_the_first_line_follows_the_measured_rule_scaled_to_the_screen(self):
        self.assertAlmostEqual(narration.first_line_ink_top(25), 2 + 102.25 * 0.75)
        self.assertAlmostEqual(narration.first_line_ink_top(75), 2 + 290.75 * 0.75)

    def test_lines_in_a_paragraph_are_one_pitch_apart(self):
        tops = narration.ink_tops_by_paragraph([["a", "b", "c"]], 25)
        self.assertAlmostEqual(tops[0][1] - tops[0][0], 50.25 * narration.NARRATION_SCALE_FROM_CLIP_SIZE)
        self.assertAlmostEqual(tops[0][2] - tops[0][1], 50.25 * narration.NARRATION_SCALE_FROM_CLIP_SIZE)

    def test_the_next_paragraph_depends_on_the_lines_above_it(self):
        one_line = narration.ink_tops_by_paragraph([["a"], ["b"]], 25)
        two_lines = narration.ink_tops_by_paragraph([["a", "a"], ["b"]], 25)
        three_lines = narration.ink_tops_by_paragraph([["a", "a", "a"], ["b"], ["c"]], 25)
        scale = narration.NARRATION_SCALE_FROM_CLIP_SIZE
        self.assertAlmostEqual(one_line[1][0] - one_line[0][0], (79 + 67) * 0.75 * scale)
        self.assertAlmostEqual(two_lines[1][0] - two_lines[0][0], (158 + 67) * 0.75 * scale)
        self.assertAlmostEqual(three_lines[1][0] - three_lines[0][0], (237 + 67) * 0.75 * scale)
        self.assertAlmostEqual(three_lines[2][0] - three_lines[1][0], (79 + 67) * 0.75 * scale)

    def test_shrinking_keeps_a_message_centred_where_the_clip_has_it(self):
        ink_height_at_clip_size = narration.TALLEST_INK_ABOVE_BASELINE_EM * narration.CLIP_SIZED_PIXELS_PER_EM
        ink_height = narration.TALLEST_INK_ABOVE_BASELINE_EM * narration.NARRATION_PIXELS_PER_EM
        for paragraphs in ([["a"]], [["a", "b", "c"]], [["a", "b"], ["c"]]):
            clip_tops = [top for tops in narration.clip_sized_ink_tops(paragraphs, 40) for top in tops]
            tops = [top for line_tops in narration.ink_tops_by_paragraph(paragraphs, 40) for top in line_tops]
            self.assertAlmostEqual(tops[0] + tops[-1] + ink_height, clip_tops[0] + clip_tops[-1] + ink_height_at_clip_size)

    def test_a_message_with_no_lines_has_no_tops(self):
        self.assertEqual(narration.ink_tops_by_paragraph([], 40), [])

    def test_text_is_smaller_than_the_clip_and_stays_within_two_thirds_of_the_screen(self):
        self.assertEqual(narration.NARRATION_PIXELS_PER_EM, 29.8)
        self.assertEqual(narration.CLIP_SIZED_PIXELS_PER_EM, 38.5)
        self.assertEqual(narration.NARRATION_MAX_INK_WIDTH, 640)

    def test_the_texture_top_puts_the_tallest_ink_on_the_wanted_row(self):
        self.assertEqual(narration.texture_top_for_ink_top(100.0, 39, 38.5), 93)


class StepExpansionTests(unittest.TestCase):
    def setUp(self):
        self.lines = {"m": {"paragraphs": [[line_record(1, 70), line_record(2, 120)], [line_record(3, 240)]]}}

    def expand(self, steps, sizes=None):
        return narration.expand_text_steps(steps, self.lines, sizes or {})

    def test_showing_a_paragraph_shows_each_of_its_lines_centred(self):
        self.assertEqual(self.expand([["narration_show", "m", 0, 60]]), [
            ["text_show", 1, "tex_1", 480, 70, 60],
            ["text_show", 2, "tex_2", 480, 120, 60],
        ])
        self.assertEqual(self.expand([["narration_show", "m", 1, 30]]), [["text_show", 3, "tex_3", 480, 240, 30]])

    def test_hiding_a_message_hides_every_line_of_it(self):
        self.assertEqual(self.expand([["narration_hide", "m", 60]]), [["text_hide", 1, 60], ["text_hide", 2, 60], ["text_hide", 3, 60]])

    def test_an_overlay_gets_its_size_from_the_texture_at_screen_scale(self):
        step = self.expand([["overlay", 7, "art", 10, 20, 6]], {"art": (90, 180)})[0]
        self.assertEqual(step[:5], ["overlay", 7, "art", 10, 20])
        self.assertAlmostEqual(step[5], 90 / frame_layout.SCREEN_PIXELS_PER_DS_PIXEL)
        self.assertAlmostEqual(step[6], 180 / frame_layout.SCREEN_PIXELS_PER_DS_PIXEL)
        self.assertEqual(step[7], 6)

    def test_other_steps_pass_through_in_order(self):
        self.assertEqual(self.expand([["wait", 5], ["text_hide_all"], ["overlay_hide", 2]]), [["wait", 5], ["text_hide_all"], ["overlay_hide", 2]])

    def test_unknown_names_and_paragraphs_are_refused(self):
        with self.assertRaises(narration.NarrationError):
            self.expand([["narration_show", "absent", 0, 60]])
        with self.assertRaises(narration.NarrationError):
            self.expand([["narration_hide", "absent", 60]])
        with self.assertRaises(narration.NarrationError):
            self.expand([["narration_show", "m", 2, 60]])
        with self.assertRaises(narration.NarrationError):
            self.expand([["narration_show", "m", -1, 60]])
        with self.assertRaises(narration.NarrationError):
            self.expand([["overlay", 1, "absent", 0, 0, 0]])


class ConversionTests(unittest.TestCase):
    def setUp(self):
        self.work_folder = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, self.work_folder)

    def packs_with(self, entries):
        message_data = synthetic_files.message_file_bytes([(name, ["ignored", text]) for name, text in entries])
        return FakePacks({FILE_PATH: message_data, FONT_PACK_PATH: synthetic_files.fbfo_font_file(synthetic_files.box_font_bytes(), KEY)})

    def convert_set(self, packs, set_description, text_key=synthetic_files.FAKE_TEXT_KEY):
        messages = narration.read_text_messages(packs, set_description, text_key)
        output_folder = os.path.join(self.work_folder, "output")
        os.makedirs(os.path.join(output_folder, "textures"), exist_ok=True)
        lines, results, failures = narration.bake_text_textures(packs, set_description, messages, self.work_folder, bake_record.BakeRecord(output_folder), "code")
        self.assertEqual(failures, [])
        return lines, results

    def convert(self, packs, requests):
        return self.convert_set(packs, {"narration_font": FONT_PACK_PATH, "narration": requests})

    def read_gtex(self, name):
        with open(os.path.join(self.work_folder, "output", "textures", name + ".gtex"), "rb") as source:
            return gtex_pack.unpack_gtex(source.read())

    def test_each_line_gets_a_texture_and_a_place(self):
        raw = '<TRCK mono_pos_set posY="25"/>one\r\ntwo<TRCK paragraph_end />\r\nthree'
        lines, results = self.convert(self.packs_with([("entry", raw)]), [{"name": "m", "file": FILE_PATH, "entry": "entry"}])
        self.assertEqual([result["name"] for result in results], ["text_m_0", "text_m_1", "text_m_2"])
        paragraphs = lines["m"]["paragraphs"]
        self.assertEqual([[line["id"] for line in paragraph] for paragraph in paragraphs], [[1, 2], [3]])
        self.assertEqual(paragraphs[0][1]["top_y"] - paragraphs[0][0]["top_y"], 39)
        self.assertEqual(lines["m"]["pos_y"], 25)
        for result in results:
            gtex_bytes = self.read_gtex(result["name"])
            self.assertEqual(gtex_bytes[:4], b"GTEX")
            self.assertEqual(result["byte_count"], len(gtex_bytes))
            self.assertEqual(result["format"], asset_writers.FORMAT_DXT5)
            self.assertEqual(result["width"] % 4, 0)

    def test_a_blank_line_keeps_its_row_but_gets_no_texture(self):
        raw = '<TRCK mono_pos_set posY="25"/>one\r\n\r\nthree'
        lines, results = self.convert(self.packs_with([("entry", raw)]), [{"name": "m", "file": FILE_PATH, "entry": "entry"}])
        self.assertEqual(len(results), 2)
        first, second = lines["m"]["paragraphs"][0]
        self.assertAlmostEqual(second["top_y"] - first["top_y"], 2 * 50.25 * narration.NARRATION_SCALE_FROM_CLIP_SIZE, delta=1)

    def test_narration_without_a_position_is_refused(self):
        with self.assertRaises(narration.NarrationError):
            self.convert(self.packs_with([("entry", "one")]), [{"name": "m", "file": FILE_PATH, "entry": "entry"}])

    def test_a_font_is_decoded_once_for_many_messages(self):
        entries = [("a", '<TRCK mono_pos_set posY="25"/>one'), ("b", '<TRCK mono_pos_set posY="25"/>two')]
        packs = self.packs_with(entries)
        self.convert(packs, [{"name": "a", "file": FILE_PATH, "entry": "a"}, {"name": "b", "file": FILE_PATH, "entry": "b"}])
        self.assertEqual(packs.read_paths.count(FONT_PACK_PATH), 1)
        self.assertEqual(packs.read_paths.count(FILE_PATH), 1)

    def test_bubble_text_must_be_one_line_and_uses_its_own_size_and_color(self):
        packs = self.packs_with([("one", "abc"), ("two", "a\r\nb")])
        request = {"name": "bubble", "file": FILE_PATH, "entry": "one", "font": FONT_PACK_PATH, "pixels_per_em": 20, "color": [0, 0, 0]}
        _, results = self.convert_set(packs, {"bubble_text": [request]})
        self.assertEqual(results[0]["name"], "bubble")
        self.assertEqual(results[0]["height"], line_renderer.line_box(20)[1])
        request = dict(request, entry="two")
        with self.assertRaises(narration.NarrationError):
            self.convert_set(packs, {"bubble_text": [request]})

    def test_narration_is_drawn_at_its_measured_size(self):
        lines, results = self.convert(self.packs_with([("entry", '<TRCK mono_pos_set posY="25"/>abcdefgh')]), [{"name": "m", "file": FILE_PATH, "entry": "entry"}])
        self.assertEqual(results[0]["height"], NARRATION_LINE_HEIGHT)
        expected_width = 7 * GLYPH_ADVANCE_IN_NARRATION_PIXELS + GLYPH_INK_IN_NARRATION_PIXELS
        self.assertAlmostEqual(lines["m"]["paragraphs"][0][0]["natural_width"], expected_width, delta=1)

    def test_a_line_wider_than_the_narration_limit_is_squeezed_to_it(self):
        lines, results = self.convert(self.packs_with([("entry", '<TRCK mono_pos_set posY="25"/>' + "a" * 40)]), [{"name": "m", "file": FILE_PATH, "entry": "entry"}])
        expected_natural_width = 39 * GLYPH_ADVANCE_IN_NARRATION_PIXELS + GLYPH_INK_IN_NARRATION_PIXELS
        self.assertAlmostEqual(lines["m"]["paragraphs"][0][0]["natural_width"], expected_natural_width, delta=1)
        self.assertEqual(results[0]["width"], NARRATION_WIDTH_LIMIT_WITH_PADDING)

    def test_a_malformed_tag_names_the_message_without_quoting_the_text(self):
        packs = self.packs_with([("entry", "private words<TRCK mes_wait_set />")])
        with self.assertRaisesRegex(narration.NarrationError, "^m: mes_wait_set") as caught:
            self.convert(packs, [{"name": "m", "file": FILE_PATH, "entry": "entry"}])
        self.assertNotIn("private", str(caught.exception))
        request = {"name": "bubble", "file": FILE_PATH, "entry": "entry", "font": FONT_PACK_PATH, "pixels_per_em": 20, "color": [0, 0, 0]}
        with self.assertRaisesRegex(narration.NarrationError, "^bubble: mes_wait_set"):
            self.convert_set(packs, {"bubble_text": [request]})

    def test_a_tag_this_reader_does_not_know_is_reported_by_the_message_name(self):
        packs = self.packs_with([("entry", 'private words<TRCK made_up_verb />')])
        with self.assertRaisesRegex(narration.NarrationError, "^m: ") as caught:
            self.convert(packs, [{"name": "m", "file": FILE_PATH, "entry": "entry"}])
        self.assertNotIn("private", str(caught.exception))

    def test_a_wrong_entry_name_is_reported_without_blaming_the_key(self):
        packs = self.packs_with([("entry", '<TRCK mono_pos_set posY="25"/>private words')])
        with self.assertRaises(narration.NarrationError) as caught:
            self.convert(packs, [{"name": "m", "file": FILE_PATH, "entry": "typo"}])
        self.assertEqual(str(caught.exception), FILE_PATH + ": no such entry: typo")
        self.assertNotIn(RUN_THE_TOOL, str(caught.exception))

    def test_a_line_whose_record_lost_its_width_is_baked_again_and_gets_it_back(self):
        packs = self.packs_with([("entry", '<TRCK mono_pos_set posY="25"/>one')])
        set_description = {"narration_font": FONT_PACK_PATH, "narration": [{"name": "m", "file": FILE_PATH, "entry": "entry"}]}
        messages = narration.read_text_messages(packs, set_description, synthetic_files.FAKE_TEXT_KEY)
        output_folder = os.path.join(self.work_folder, "output")
        os.makedirs(os.path.join(output_folder, "textures"))
        record = bake_record.BakeRecord(output_folder)
        first_lines, _, _ = narration.bake_text_textures(packs, set_description, messages, self.work_folder, record, "code")
        record.save()
        with open(record.path, encoding="ascii") as source:
            saved = json.load(source)
        del saved["textures/text_m_0.gtex"]["details"]
        with open(record.path, "w", encoding="ascii") as output:
            json.dump(saved, output)
        record = bake_record.BakeRecord(output_folder)
        lines, _, failures = narration.bake_text_textures(packs, set_description, messages, self.work_folder, record, "code")
        self.assertEqual(failures, [])
        self.assertEqual(record.baked_in("textures"), 1)
        self.assertEqual(record.reused_in("textures"), 0)
        self.assertEqual(lines["m"]["paragraphs"][0][0]["natural_width"], first_lines["m"]["paragraphs"][0][0]["natural_width"])

    def test_text_texture_names_match_what_is_baked(self):
        raw = '<TRCK mono_pos_set posY="25"/>one\r\n\r\nthree<TRCK paragraph_end />\r\nfour'
        packs = self.packs_with([("entry", raw), ("bubble", "abc")])
        set_description = {
            "narration_font": FONT_PACK_PATH,
            "narration": [{"name": "m", "file": FILE_PATH, "entry": "entry"}],
            "bubble_text": [{"name": "bubble", "file": FILE_PATH, "entry": "bubble", "font": FONT_PACK_PATH, "pixels_per_em": 20, "color": [0, 0, 0]}],
        }
        _, results = self.convert_set(packs, set_description)
        messages = narration.read_text_messages(packs, set_description, synthetic_files.FAKE_TEXT_KEY)
        self.assertEqual(sorted(narration.text_texture_names(set_description, messages)), sorted(result["name"] for result in results))

    def test_a_set_without_text_reads_nothing(self):
        packs = FakePacks({})
        self.assertEqual(self.convert_set(packs, {}, None), ({}, []))
        self.assertEqual(packs.read_paths, [])

    def test_the_wrong_key_is_reported_plainly_without_any_decoded_bytes(self):
        packs = self.packs_with([("entry", '<TRCK mono_pos_set posY="25"/>private words')])
        with self.assertRaises(narration.NarrationError) as caught:
            narration.read_text_messages(packs, {"narration_font": FONT_PACK_PATH, "narration": [{"name": "m", "file": FILE_PATH, "entry": "entry"}]}, bytes(range(16, 32)))
        self.assertEqual(str(caught.exception), "the saved text key did not decode text.msg: " + RUN_THE_TOOL)


class LoadingTheKeyTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, self.folder)
        self.key_path = os.path.join(self.folder, "text_key.bin")

    def test_a_set_without_text_never_looks_for_the_file(self):
        self.assertFalse(narration.set_uses_text({"narration": [], "bubble_text": []}))
        self.assertIsNone(narration.load_text_key({"narration": [], "bubble_text": []}, self.folder))
        self.assertIsNone(narration.load_text_key({}, os.path.join(self.folder, "absent")))

    def test_narration_or_bubble_text_alone_needs_the_key(self):
        synthetic_files.write_text_key(self.folder)
        for set_description in ({"narration": [{}]}, {"bubble_text": [{}]}):
            self.assertEqual(narration.load_text_key(set_description, self.folder), synthetic_files.FAKE_TEXT_KEY)

    def test_a_missing_file_says_to_run_the_tool(self):
        with self.assertRaises(narration.NarrationError) as caught:
            narration.load_text_key({"narration": [{}]}, self.folder)
        self.assertEqual(str(caught.exception), "no text key at " + self.key_path + ": " + RUN_THE_TOOL)

    def test_a_file_that_cannot_be_read_says_why(self):
        os.makedirs(self.key_path)
        with self.assertRaises(narration.NarrationError) as caught:
            narration.load_text_key({"narration": [{}]}, self.folder)
        self.assertEqual(str(caught.exception), "could not read " + self.key_path + ": Is a directory: " + RUN_THE_TOOL)

    def test_loading_the_key_prints_nothing(self):
        synthetic_files.write_text_key(self.folder)
        output = io.StringIO()
        errors = io.StringIO()
        with contextlib.redirect_stdout(output), contextlib.redirect_stderr(errors):
            narration.load_text_key({"narration": [{}]}, self.folder)
        self.assertEqual(output.getvalue(), "")
        self.assertEqual(errors.getvalue(), "")

    def test_a_file_of_the_wrong_size_says_so(self):
        for size in (0, 15, 17, 32):
            synthetic_files.write_text_key(self.folder, bytes(size))
            with self.assertRaises(narration.NarrationError) as caught:
                narration.load_text_key({"narration": [{}]}, self.folder)
            self.assertEqual(str(caught.exception), self.key_path + " is the wrong size: " + RUN_THE_TOOL)


class SummaryTests(unittest.TestCase):
    def summary(self, lines):
        output = io.StringIO()
        with contextlib.redirect_stdout(output):
            narration.print_narration_summary(lines)
        return output.getvalue()

    def test_a_line_over_the_limit_is_named(self):
        lines = {"m": {"paragraphs": [[line_record(1, 0, 640.0), line_record(2, 0, 640.5)]]}}
        text = self.summary(lines)
        self.assertIn("1 messages, 2 lines, widest 640.5 px", text)
        self.assertIn("squeezed: tex_2 640.5 px", text)
        self.assertNotIn("tex_1", text)

    def test_no_narration_prints_nothing(self):
        self.assertEqual(self.summary({}), "")


if __name__ == "__main__":
    unittest.main()
