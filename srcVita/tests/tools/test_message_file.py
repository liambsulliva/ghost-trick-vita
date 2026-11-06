import random
import struct
import unittest

import message_file
import synthetic_files

CORRUPTED_FILE_COUNT = 3000
WILD_POINTERS = (0, 1, 7, 0xFFFFFFFF, 0x7FFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF)


def read_entries(data, wanted_names):
    return message_file.read_english_entries(data, wanted_names, synthetic_files.FAKE_TEXT_KEY)


def english_file(entries):
    return synthetic_files.message_file_bytes([(name, ["other language", text]) for name, text in entries])


def with_u64(data, offset, value):
    changed = bytearray(data)
    struct.pack_into("<Q", changed, offset, value)
    return bytes(changed)


def corrupted_copy(data, generator):
    changed = bytearray(data)
    for _ in range(generator.randint(1, 8)):
        changed[generator.randrange(len(changed))] = generator.randrange(256)
    if generator.random() < 0.5:
        offset = generator.randrange(0, len(changed) - 8)
        struct.pack_into("<Q", changed, offset, generator.choice(WILD_POINTERS))
    return bytes(changed[:generator.randint(0, len(changed))]) if generator.random() < 0.25 else bytes(changed)


class ReadingEntriesTests(unittest.TestCase):
    def test_the_english_slot_of_each_wanted_entry_is_read(self):
        data = english_file([("first", "alpha beta"), ("second", "gamma"), ("third", "delta")])
        self.assertEqual(read_entries(data, ["first", "third"]), {"first": "alpha beta", "third": "delta"})

    def test_a_string_after_a_long_one_still_decodes(self):
        data = english_file([("first", "x" * 1000), ("second", "after the long string")])
        self.assertEqual(read_entries(data, ["second"])["second"], "after the long string")

    def test_a_missing_entry_is_reported_by_name(self):
        with self.assertRaisesRegex(message_file.MessageFileError, "no such entry: absent"):
            read_entries(english_file([("first", "alpha")]), ["absent"])
        with self.assertRaises(message_file.MissingEntryError):
            read_entries(english_file([("first", "alpha")]), ["absent"])

    def test_entry_names_that_are_not_plain_text_do_not_count_as_a_missing_entry(self):
        with self.assertRaises(message_file.MessageFileError) as caught:
            read_entries(english_file([("n\u00e4me", "beta"), ("first", "alpha")]), ["absent"])
        self.assertNotIsInstance(caught.exception, message_file.MissingEntryError)
        self.assertEqual(str(caught.exception), "entry names are not plain text")

    def test_a_wanted_entry_is_still_found_beside_a_name_that_is_not_plain_text(self):
        data = english_file([("first", "alpha"), ("n\u00e4me", "beta")])
        self.assertEqual(read_entries(data, ["first"])["first"], "alpha")

    def test_other_files_are_refused(self):
        data = english_file([("first", "alpha")])
        with self.assertRaises(message_file.MessageFileError):
            read_entries(b"XXXX" + data[4:8] + data[8:], ["first"])
        with self.assertRaises(message_file.MessageFileError):
            read_entries(data[:4] + b"XXXX" + data[8:], ["first"])
        with self.assertRaises(message_file.MessageFileError):
            read_entries(synthetic_files.message_file_bytes([("first", ["a", "b"])], version=21), ["first"])

    def test_a_file_without_an_english_slot_is_refused(self):
        with self.assertRaises(message_file.MessageFileError):
            read_entries(synthetic_files.message_file_bytes([("first", ["a"])], language_count=1), ["first"])


class MalformedFileTests(unittest.TestCase):
    def setUp(self):
        self.data = english_file([("first", "alpha"), ("last", "the final words")])
        self.first_entry_text_pointer = synthetic_files.gmsg_entry_start(2) + synthetic_files.GMSG_ENTRY_TEXT_POINTERS_OFFSET + 8

    def test_a_string_cut_before_its_terminator_is_refused(self):
        with self.assertRaisesRegex(message_file.MessageFileError, "entry last: .*terminator"):
            read_entries(self.data[:-2], ["last"])

    def test_every_cut_of_the_file_is_refused_or_gives_the_whole_text(self):
        for length in range(len(self.data)):
            try:
                found = read_entries(self.data[:length], ["last"])
            except message_file.MessageFileError:
                continue
            self.assertEqual(found["last"], "the final words", length)

    def test_a_wild_entry_count_is_refused(self):
        changed = bytearray(self.data)
        struct.pack_into("<I", changed, synthetic_files.GMSG_ENTRY_COUNT_OFFSET, 0xFFFFFFFF)
        with self.assertRaises(message_file.MessageFileError):
            read_entries(bytes(changed), ["last"])

    def test_a_wild_text_pointer_is_refused_naming_the_entry(self):
        for pointer in WILD_POINTERS:
            changed = with_u64(self.data, self.first_entry_text_pointer, pointer)
            with self.assertRaisesRegex(message_file.MessageFileError, "entry first: "):
                read_entries(changed, ["first"])

    def test_a_wild_entry_pointer_is_refused_naming_its_number(self):
        for pointer in WILD_POINTERS:
            changed = with_u64(self.data, synthetic_files.GMSG_ENTRY_POINTERS_OFFSET + 8, pointer)
            with self.assertRaisesRegex(message_file.MessageFileError, "entry number 1: "):
                read_entries(changed, ["first"])

    def test_a_wild_name_pointer_is_refused_naming_its_number(self):
        for pointer in WILD_POINTERS:
            changed = with_u64(self.data, synthetic_files.gmsg_entry_start(2) + synthetic_files.GMSG_ENTRY_NAME_POINTER_OFFSET, pointer)
            with self.assertRaisesRegex(message_file.MessageFileError, "entry number 0: "):
                read_entries(changed, ["first"])

    def test_a_file_cut_inside_its_header_is_refused(self):
        for length in (0, 4, 8, 12, 20, 28, 36):
            with self.assertRaises(message_file.MessageFileError):
                read_entries(self.data[:length], ["first"])

    def test_corrupted_files_only_ever_raise_message_file_errors(self):
        generator = random.Random(2029)
        for _ in range(CORRUPTED_FILE_COUNT):
            try:
                read_entries(corrupted_copy(self.data, generator), ["first", "last"])
            except message_file.MessageFileError:
                pass


class KeyTests(unittest.TestCase):
    def test_no_key_is_built_in(self):
        self.assertFalse(hasattr(message_file, "STRING_AREA_KEY"))
        self.assertFalse(hasattr(synthetic_files, "GMSG_STRING_AREA_KEY"))

    def test_a_file_reads_back_with_the_key_it_was_written_with(self):
        key = bytes(range(100, 116))
        data = synthetic_files.message_file_bytes([("first", ["other language", "alpha"])], key=key)
        self.assertEqual(message_file.read_english_entries(data, ["first"], key), {"first": "alpha"})

    def test_the_wrong_key_does_not_give_the_text(self):
        data = synthetic_files.message_file_bytes([("first", ["other language", "alpha"])], key=bytes(range(100, 116)))
        with self.assertRaises(message_file.MessageFileError):
            read_entries(data, ["first"])

    def test_decoding_repeats_all_sixteen_key_bytes(self):
        key = bytes(range(1, 17))
        self.assertEqual(message_file.decode_string_area(bytes(18), key), key + key[:2])

    def test_decoding_chains_on_the_stored_bytes(self):
        self.assertEqual(message_file.decode_string_area(bytes([5, 3, 9]), bytes(16)), bytes([5, 5 ^ 3, 3 ^ 9]))


class ParsingMessageTests(unittest.TestCase):
    def test_a_message_without_tags_is_one_line(self):
        message = message_file.parse_message("just one line")
        self.assertEqual(message["paragraphs"], [["just one line"]])
        self.assertIsNone(message["pos_y"])
        self.assertIsNone(message["auto_pf"])
        self.assertIsNone(message["mono_type"])
        self.assertEqual(message["waits"], [])

    def test_every_verb_of_the_opening_is_read(self):
        raw = '<TRCK mes_wait_set time="190" /><TRCK mono_type_set type="MONO_TYPE_03" /><TRCK mono_pos_set posY="25"/>first<TRCK mes_wait_set time="96" /><TRCK auto_pf time="26" />'
        message = message_file.parse_message(raw)
        self.assertEqual(message["pos_y"], 25)
        self.assertEqual(message["mono_type"], 3)
        self.assertEqual(message["auto_pf"], 26)
        self.assertEqual(message["waits"], [190, 96])
        self.assertEqual(message["paragraphs"], [["first"]])

    def test_line_breaks_split_lines_and_paragraph_ends_split_paragraphs(self):
        raw = 'one\r\ntwo<TRCK paragraph_end />\r\nthree<TRCK paragraph_end />\r\n<TRCK mes_wait_set time="96" />four\r\n<TRCK mes_wait_set time="96" />five'
        self.assertEqual(message_file.parse_message(raw)["paragraphs"], [["one", "two"], ["three"], ["four", "five"]])

    def test_a_paragraph_end_without_a_line_break_still_starts_a_new_paragraph(self):
        self.assertEqual(message_file.parse_message("one<TRCK paragraph_end />two")["paragraphs"], [["one"], ["two"]])

    def test_unknown_tags_are_refused_without_quoting_the_text(self):
        with self.assertRaises(message_file.MessageFileError) as caught:
            message_file.parse_message('private words<TRCK made_up_verb time="1" />')
        self.assertIn("made_up_verb", str(caught.exception))
        self.assertNotIn("private", str(caught.exception))
        with self.assertRaises(message_file.MessageFileError) as caught:
            message_file.parse_message("private words<FONT size=3>")
        self.assertNotIn("private", str(caught.exception))

    def test_a_tag_missing_its_attribute_is_refused_naming_the_verb(self):
        for verb in ("mes_wait_set", "mono_pos_set", "auto_pf", "mono_type_set"):
            with self.assertRaisesRegex(message_file.MessageFileError, verb):
                message_file.parse_message("private words<TRCK %s />" % verb)

    def test_an_attribute_that_is_not_a_number_is_refused_without_quoting_it(self):
        tags = ('mes_wait_set time="private"', 'mono_pos_set posY="private"', 'auto_pf time="private"', 'mono_type_set type="private"', 'mono_type_set type="MONO_TYPE_private"', 'mono_type_set type="PRIVATEXYZ3"')
        for tag in tags:
            with self.assertRaises(message_file.MessageFileError) as caught:
                message_file.parse_message("words<TRCK %s />" % tag)
            self.assertNotIn("private", str(caught.exception))

    def test_the_paragraph_mark_is_the_paragraph_separator_written_as_an_escape(self):
        self.assertEqual(message_file.PARAGRAPH_MARK, chr(0x2029))
        self.assertEqual(message_file.parse_message("one" + chr(0x2029) + "two")["paragraphs"], [["one"], ["two"]])
        with open(message_file.__file__, "rb") as source:
            source.read().decode("ascii")


if __name__ == "__main__":
    unittest.main()
