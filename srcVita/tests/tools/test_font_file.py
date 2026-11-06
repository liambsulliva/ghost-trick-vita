import struct
import unittest

import font_file
import synthetic_files

KEY = bytes([0x12, 0x9A, 0x00, 0xFF, 0x7E, 0x31, 0xC4, 0x05])


def font_with_table_count(table_count, magic):
    tables = {b"t%03d" % index: bytes([index & 255]) * (index + 1) for index in range(table_count)}
    return synthetic_files.sfnt_bytes(tables, magic)


class DecodingTests(unittest.TestCase):
    def test_the_key_is_found_from_the_header_for_any_table_count(self):
        for table_count in (1, 2, 3, 4, 15, 16, 17, 40):
            for magic in font_file.SFNT_MAGICS:
                font = font_with_table_count(table_count, magic)
                self.assertEqual(font_file.decode_font(synthetic_files.fbfo_font_file(font, KEY)), font)

    def test_a_real_font_shape_decodes_exactly(self):
        font = synthetic_files.box_font_bytes()
        self.assertEqual(font_file.decode_font(synthetic_files.fbfo_font_file(font, KEY)), font)

    def test_a_file_without_the_header_is_refused(self):
        font = font_with_table_count(3, b"OTTO")
        with self.assertRaisesRegex(font_file.FontFileError, "FBFO"):
            font_file.decode_font(b"XXXX" + synthetic_files.fbfo_font_file(font, KEY)[4:])

    def test_a_body_that_is_not_a_font_is_refused(self):
        with self.assertRaises(font_file.FontFileError):
            font_file.decode_font(b"FBFO" + bytes(range(256)))

    def test_a_table_past_the_end_of_the_file_is_refused(self):
        font = bytearray(font_with_table_count(3, b"OTTO"))
        struct.pack_into(">I", font, font_file.SFNT_DIRECTORY_START + 12, len(font) + 1)
        with self.assertRaises(font_file.FontFileError):
            font_file.decode_font(synthetic_files.fbfo_font_file(bytes(font), KEY))

    def test_the_table_count_fields_follow_the_sfnt_rule(self):
        self.assertEqual(font_file.sfnt_table_count_fields(15), struct.pack(">HHHH", 15, 128, 3, 112))
        self.assertEqual(font_file.sfnt_table_count_fields(16), struct.pack(">HHHH", 16, 256, 4, 0))


if __name__ == "__main__":
    unittest.main()
