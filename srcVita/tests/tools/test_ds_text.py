import struct
import unittest

import ds_text

CELL = 4
SHAPE = {"cell_width": CELL, "cell_height": CELL, "advance": 9, "line_height": 5, "glyph_bytes": CELL * CELL // 4, "two_bits": True, "kind": 1}


def control(code, *parameters):
    return [0xff00 + code] + list(parameters)


def labelled_lmg(code_words, labels):
    code = struct.pack("<%dH" % len(code_words), *code_words)
    names = b""
    entries = b""
    for name, word_index in labels:
        entries = entries + struct.pack("<II", len(names), 0x34 + 2 * word_index)
        names = names + name.encode("latin-1") + b"\0"
    label_table = struct.pack("<I", len(labels)) + entries + names
    header = bytearray(0x34)
    header[:4] = b"1LMG"
    struct.pack_into("<III", header, 8, len(code), 0, len(label_table))
    return bytes(header) + code + label_table


def glyph_bytes(rows):
    return bytes(second << 6 | first << 4 | fourth << 2 | third for first, second, third, fourth in rows)


def profile_bytes(values):
    packed = bytearray(8)
    for row, value in enumerate(values):
        packed[row // 4] |= value << (2 * (row % 4))
    return bytes(packed)


def make_font(glyph_rows_by_code, widths_by_code, profiles_by_code=None, icon_widths=None, font_index=ds_text.MAIN_FONT, code_count=0x120):
    glyphs = bytearray(SHAPE["glyph_bytes"] * code_count)
    for code, rows in glyph_rows_by_code.items():
        glyphs[code * SHAPE["glyph_bytes"]:(code + 1) * SHAPE["glyph_bytes"]] = glyph_bytes(rows)
    widths = bytearray(2 * code_count)
    for code, (bearing, width) in widths_by_code.items():
        widths[2 * code:2 * code + 2] = bytes([bearing, width])
    profiles = bytearray(16 * code_count)
    for code, (left, right) in (profiles_by_code or {}).items():
        profiles[16 * code:16 * code + 16] = profile_bytes(left) + profile_bytes(right)
    return ds_text.DsFont(font_index, dict(SHAPE), bytes(glyphs), bytes(widths), bytes(profiles), icon_widths)


BAR = [(0, 3, 0, 0)] * CELL


class MessageTests(unittest.TestCase):
    def test_controls_take_their_native_parameter_counts(self):
        words = control(0x2d, 1, 2) + [0x0a] + control(0x05, 9) + [0x0b] + [0xfffe]
        items = ds_text.message_items(struct.pack("<%dH" % len(words), *words), 0)
        self.assertEqual(items, [("control", 0x2d, (1, 2)), ("char", 0x0a), ("control", 0x05, (9,)), ("char", 0x0b)])

    def test_pages_groups_and_lines_split_where_the_native_line_drawer_stops(self):
        words = [0x0a] + control(0x01) + [0x0b] + control(0x16) + [0x0c] + control(0x02) + [0x0d] + control(0x15, 26) + [0xfffe]
        pages = ds_text.read_message_file(labelled_lmg(words, [("m01_0000", 0)]))["m01_0000"]["pages"]
        self.assertEqual([[[line["text"] for line in group] for group in page] for page in pages], [[["A", "B"], ["C"]], [["D"]]])

    def test_codes_read_as_the_cartridge_alphabet(self):
        self.assertEqual(ds_text.text_of_codes([0x16, 0x38, 0x36, 0x37, 0xf3, 0x39, 0x28, 0xff, 0xe1]), "Must've .")

    def test_unknown_codes_are_marked_not_dropped(self):
        self.assertEqual(ds_text.text_of_codes([0x11a]), ds_text.UNKNOWN_CHARACTER)

    def test_each_label_names_its_own_message(self):
        words = [0x0a, 0xfffe, 0x0b, 0xfffe]
        messages = ds_text.read_message_file(labelled_lmg(words, [("m01_0000", 0), ("m01_0010", 2)]))
        self.assertEqual({label: message["pages"][0][0][0]["text"] for label, message in messages.items()}, {"m01_0000": "A", "m01_0010": "B"})

    def test_a_message_with_no_end_is_refused(self):
        with self.assertRaises(ds_text.DsTextError):
            ds_text.read_message_file(labelled_lmg([0x0a], [("m01_0000", 0)]))

    def test_language_and_system_paths(self):
        self.assertEqual(ds_text.script_message_path("st01/st01_game000_Expand.xml", "en"), "st01/st01_game000_Expand.en.xml.lz")
        self.assertEqual(ds_text.system_message_path("fr"), "system/system_0000.fr.msg.xml.lz")


class TableTests(unittest.TestCase):
    def test_font_shape_and_icon_widths_come_from_the_arm9_tables(self):
        arm9 = bytearray(0x90000)
        struct.pack_into("<4BIII", arm9, ds_text.FONT_SHAPE_TABLE_ADDRESS - ds_text.ARM9_LOAD_ADDRESS + 16, 16, 16, 12, 12, 0x80, 0, 2)
        struct.pack_into("<HHHH", arm9, ds_text.ICON_WIDTH_TABLE_ADDRESS - ds_text.ARM9_LOAD_ADDRESS, 0x113, 0x3db, 16, 0x18)
        self.assertEqual(ds_text.read_font_shape(arm9, 1), {"cell_width": 16, "cell_height": 16, "advance": 12, "line_height": 12, "glyph_bytes": 0x80, "two_bits": False, "kind": 2})
        self.assertEqual(ds_text.read_icon_widths(arm9), {0x113: 16})


class GlyphTests(unittest.TestCase):
    def test_two_bit_pixels_expand_in_the_native_order(self):
        self.assertEqual(ds_text.expand_two_bits(bytes([0b11100100])), bytes([0x32, 0x10]))

    def test_ink_levels_move_to_the_colour_ramp(self):
        font = make_font({1: [(1, 2, 3, 0)] * CELL}, {})
        self.assertEqual(font.glyph(1, 15)[0], [13, 14, 15, 0])
        self.assertEqual(font.glyph(1, 3)[0], [1, 2, 3, 0])
        self.assertEqual(font.glyph(1, 0)[0], [0, 0, 0, 0])

    def test_the_database_font_keeps_its_own_ink(self):
        font = make_font({}, {}, font_index=ds_text.DATABASE_FONT)
        font.shape["two_bits"] = False
        font.glyphs = bytes([0x21]) * len(font.glyphs)
        self.assertEqual(font.glyph(1, 15)[0], [1, 2, 1, 2])

    def test_a_code_past_the_glyph_file_is_filled(self):
        font = make_font({}, {}, code_count=1)
        self.assertEqual(font.glyph(5, 3)[0], [0, 0, 3, 0])


class AdvanceTests(unittest.TestCase):
    def test_advance_is_the_width_byte(self):
        self.assertEqual(make_font({}, {1: (1, 5)}).advance(1), 5)

    def test_a_code_with_no_widths_takes_the_font_advance(self):
        self.assertEqual(make_font({}, {}).advance(1), 9)

    def test_space_and_oversized_widths_take_the_space_advance(self):
        font = make_font({}, {0xff: (16, 241), 2: (0, 17)})
        self.assertEqual(font.advance(0xff), 4)
        self.assertEqual(font.advance(2), 4)

    def test_descent_needs_a_bearing(self):
        font = make_font({0x2a: BAR}, {0x2a: (1, 2)})
        pixels, _, _, _ = ds_text.draw_items(font, [("char", 0x2a)], 3)
        self.assertEqual(min(y for _, y, _ in pixels), 1)
        font = make_font({0x2a: BAR}, {})
        pixels, _, _, _ = ds_text.draw_items(font, [("char", 0x2a)], 3)
        self.assertEqual(min(y for _, y, _ in pixels), 0)


class GapTests(unittest.TestCase):
    def touching(self):
        edge = [0, 3, 3, 0] + [0] * 28
        return make_font({1: BAR, 2: BAR}, {1: (1, 2), 2: (1, 2)}, {1: (edge, edge), 2: (edge, edge)})

    def test_touching_edges_get_one_pixel_gap(self):
        self.assertTrue(self.touching().needs_gap(2, 1))

    def test_edges_on_different_rows_do_not(self):
        top = [3] + [0] * 31
        bottom = [0, 0, 0, 3] + [0] * 28
        font = make_font({}, {}, {1: (top, top), 2: (bottom, bottom)})
        self.assertFalse(font.needs_gap(2, 1))

    def test_full_ink_next_to_a_neighbour_row_counts(self):
        upper = [3] + [0] * 31
        lower = [0, 3] + [0] * 30
        font = make_font({}, {}, {1: (upper, upper), 2: (lower, lower)})
        self.assertTrue(font.needs_gap(2, 1))

    def test_faint_edges_alone_do_not(self):
        faint = [0, 1, 1, 0] + [0] * 28
        font = make_font({}, {}, {1: (faint, faint), 2: (faint, faint)})
        self.assertFalse(font.needs_gap(2, 1))

    def test_after_a_space_or_a_control_there_is_no_gap(self):
        font = self.touching()
        self.assertFalse(font.needs_gap(2, 0xff))
        self.assertFalse(font.needs_gap(2, 0xff05))

    def test_codes_past_the_profiles_always_gap(self):
        self.assertTrue(make_font({}, {}).needs_gap(0x80, 1))
        self.assertTrue(make_font({}, {}).needs_gap(0x3e, 1))


class LineTests(unittest.TestCase):
    def test_glyphs_shift_left_by_their_bearing_and_step_by_their_width(self):
        font = make_font({1: BAR}, {1: (1, 2)})
        line = ds_text.render_line(font, [("char", 1), ("char", 1)], 3)
        self.assertEqual(line["advance"], 4)
        self.assertEqual(line["indices"][:line["width"]], bytes([3, 0, 3, 0]))

    def test_a_gap_moves_the_glyph_and_the_pen(self):
        line = ds_text.render_line(GapTests().touching(), [("char", 1), ("char", 2)], 3)
        self.assertEqual(line["advance"], 5)
        self.assertEqual(line["indices"][:5], bytes([3, 0, 0, 3, 0]))
        self.assertEqual(line["measured_width"], 5)

    def test_a_colour_code_recolours_the_rest_of_the_line(self):
        font = make_font({1: BAR}, {1: (1, 2)})
        line = ds_text.render_line(font, [("char", 1), ("control", 5, (9,)), ("char", 1)], 15)
        self.assertEqual(line["indices"][:4], bytes([15, 0, 9, 0]))

    def test_icons_take_their_table_width_and_are_placed_not_drawn(self):
        font = make_font({}, {}, icon_widths={0x113: 16})
        line = ds_text.render_line(font, [("char", 0x113)], 3)
        self.assertEqual((line["advance"], line["icons"], line["measured_width"]), (18, [{"code": 0x113, "x": 1}], 18))
        self.assertEqual(set(line["indices"]), {0})

    def test_double_width_counts_twice_in_the_measure(self):
        font = make_font({}, {1: (1, 2)})
        self.assertEqual(ds_text.measured_width(font, [("control", 0x17, ()), ("char", 1)]), 4)

    def test_palette_gives_rgba(self):
        font = make_font({1: BAR}, {1: (1, 2)})
        palette = ds_text.palette_of_bgr555([0] * 3 + [0x7fff] + [0] * 12)
        line = ds_text.render_line(font, [("char", 1)], 3, palette)
        self.assertEqual(line["pixels"][:8], bytes([255, 255, 255, 255, 0, 0, 0, 0]))


if __name__ == "__main__":
    unittest.main()
