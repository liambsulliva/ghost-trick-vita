import base64
import hashlib
import os
import shutil
import subprocess
import sys
import tempfile
import unittest

TESTS_FOLDER = os.path.dirname(os.path.abspath(__file__))
REPOSITORY_TOOLS_FOLDER = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(TESTS_FOLDER))), "tools")
AUDIT_SCRIPT_PATH = os.path.join(REPOSITORY_TOOLS_FOLDER, "rom_audit.py")
AUDIT_RULES_PATH = os.path.join(REPOSITORY_TOOLS_FOLDER, "audit_rules.txt")
GIT_IGNORE_PATH = os.path.join(os.path.dirname(REPOSITORY_TOOLS_FOLDER), ".gitignore")
PADDING = bytes(16)
FAKE_KEY = bytes(range(0xC0, 0xD0))
OTHER_FAKE_KEY = bytes(range(0xD0, 0xE0))
NEAR_FAKE_KEY = bytes(range(0xC0, 0xC7)) + bytes([0xFF]) + bytes(range(0xC8, 0xCF)) + bytes([0xFF])
LOW_FAKE_KEY = bytes(range(0x01, 0x11))
KEY_WHOSE_BASE64_HAS_PLUS_AND_SLASH = bytes([0xFB, 0xEF, 0xBE, 0xFF, 0xFF, 0xFF]) + bytes(range(0x40, 0x4A))
PIECE_BELOW_THE_WINDOW = 7
SMALLEST_PIECE_TO_KEEP_OUT_OF_THE_OUTPUT = 2
MIXED_FAKE_KEY = bytes([0xC0, 0x41, 0xC2, 0x42, 0xC4, 0x43, 0xC6, 0x44, 0xC8, 0x45, 0xCA, 0x46, 0xCC, 0x47, 0xCE, 0x48])
KEY_FOLDER = os.path.join("extracted", "remaster_apk")
PLANTED_FILES = {
    "decoded_font.otf": b"plain",
    "font_in_pack.oft": b"plain",
    "messages.msg": b"plain",
    "converted.gtex": b"plain",
    "opentype.txt": b"OTTO" + PADDING,
    "packed_font.oft.1": b"FBFO" + PADDING,
    "messages.msg.22": bytes([22, 0, 0, 0]) + b"GMSG" + PADDING,
    "line_texture.txt": b"GTEX" + PADDING,
    "libruntime.so": b"plain",
    "library_copy.txt": b"\x7fELF" + PADDING,
    "track.ogg": b"plain",
    "work_file.dds": b"plain",
    "work_file.rgba": b"plain",
    "ogg_stream.txt": b"OggS" + PADDING,
    "dds_image.txt": b"DDS " + PADDING,
    "a.lmg": b"plain",
    "a.cues": b"plain",
    "a.motions": b"plain",
    "a.preload": b"plain",
    "golden_rows.txt": b"tick\tseq\tvm\tcode_base" + PADDING,
}
NAMES_GIT_WOULD_QUOTE_OR_STRIP = ["caf\u00e9", 'quo"te', "ta\tb", "new\nline", "ends with a space "]


def key_forms(key):
    pairs = ["%02x" % value for value in key]
    decimals = [str(value) for value in key]
    texts = [
        "".join(pairs),
        "".join(pairs).upper(),
        " ".join(pairs),
        ":".join(pairs),
        "bytes.fromhex('" + " ".join(pairs) + "')",
        ", ".join("0x" + pair for pair in pairs),
        ",".join("0X" + pair.upper() for pair in pairs),
        "".join("\\x" + pair for pair in pairs),
        ", ".join(decimals),
        ",".join(decimals),
        " ".join(decimals),
        "[\n    " + ",\n    ".join(decimals) + "\n]",
    ]
    return [key] + [text.encode("ascii") for text in texts]


def little_endian_words(key, word_size):
    words = []
    for start in range(0, len(key), word_size):
        words.append("0x%0*x" % (word_size * 2, int.from_bytes(key[start:start + word_size], "little")))
    return ", ".join(words)


def forms_in_other_orders_and_encodings(key):
    pairs = ["%02x" % value for value in key]
    back_pairs = ["%02x" % value for value in key[::-1]]
    half_pairs = pairs[:8]
    texts = [
        "".join(back_pairs),
        ", ".join("0x" + pair for pair in back_pairs),
        ", ".join(str(value) for value in key[::-1]),
        hashlib.md5(key).hexdigest(),
        hashlib.sha1(key).hexdigest(),
        hashlib.sha256(key).hexdigest(),
        hashlib.sha256(key).hexdigest().upper(),
        hashlib.sha512(key).hexdigest(),
        "".join(half_pairs),
        "".join(pairs[8:]),
        ", ".join("0x" + pair for pair in half_pairs),
        ", ".join(str(value) for value in key[:8]),
        little_endian_words(key, 2),
        little_endian_words(key, 4),
        little_endian_words(key, 8),
        str(int.from_bytes(key, "big")),
        str(int.from_bytes(key, "little")),
    ]
    data = [
        key[::-1],
        key[:8],
        key[8:],
        key[:15],
        hashlib.md5(key).digest(),
        hashlib.sha256(key).digest(),
        base64.b64encode(key),
        base64.b64encode(key).rstrip(b"="),
        base64.urlsafe_b64encode(key),
        base64.b32encode(key),
        base64.b32encode(key).lower(),
        base64.b85encode(key),
        base64.a85encode(key),
    ]
    return data + [text.encode("ascii") for text in texts]


def forms_with_other_marks_between_the_bytes(key):
    pairs = ["%02x" % value for value in key]
    decimals = [str(value) for value in key]
    texts = [
        "\\\n".join(pairs),
        "/".join(pairs),
        "|".join(pairs),
        ", /* one byte */ ".join("0x" + pair for pair in pairs),
        ", ".join("(byte)0x" + pair for pair in pairs),
        ", ".join("(unsigned char)0x" + pair for pair in pairs),
        ",".join("{0x" + pair + "}" for pair in pairs),
        "".join("<" + pair + ">" for pair in pairs),
        "".join("[" + pair + "]" for pair in pairs),
        ", ".join(pair + "h" for pair in pairs),
        " ".join("$" + pair for pair in pairs),
        "".join("\\u00" + pair for pair in pairs),
        "".join("\\%03o" % value for value in key),
        ", ".join("%03d" % value for value in key),
        "".join("[" + decimal + "]" for decimal in decimals),
    ]
    data = [text.encode("ascii") for text in texts]
    data.append("".join(pairs).encode("utf-16-le"))
    data.append("".join(pairs).encode("utf-16-be"))
    data.append(", ".join(decimals).encode("utf-16-le"))
    return data


def forms_of_every_piece(key):
    forms = []
    for size in range(SMALLEST_PIECE_TO_KEEP_OUT_OF_THE_OUTPUT, len(key) + 1):
        for start in range(len(key) - size + 1):
            piece = key[start:start + size]
            forms.append(piece)
            forms.append(piece.hex().encode("ascii"))
            forms.append(piece.hex().upper().encode("ascii"))
            forms.append(", ".join("0x%02x" % value for value in piece).encode("ascii"))
            forms.append("".join("\\x%02x" % value for value in piece).encode("ascii"))
            forms.append(", ".join(str(value) for value in piece).encode("ascii"))
    return forms


@unittest.skipUnless(os.path.isfile(AUDIT_SCRIPT_PATH), "mutation_check copies only srcVita, without the repository's tools folder")
class AuditRuleTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, self.folder)
        tools_folder = os.path.join(self.folder, "tools")
        os.makedirs(tools_folder)
        shutil.copy(AUDIT_SCRIPT_PATH, tools_folder)
        shutil.copy(AUDIT_RULES_PATH, tools_folder)
        subprocess.run(["git", "init", "--quiet", self.folder], check=True)
        self.audit_script = os.path.join(tools_folder, "rom_audit.py")

    def plant(self, name, data):
        with open(os.path.join(self.folder, name), "wb") as output:
            output.write(data)

    def run_audit(self, environment=None):
        return subprocess.run([sys.executable, "-I", self.audit_script], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, env=environment)

    def plant_each(self, forms):
        for index, form in enumerate(forms):
            self.plant("holder_%d.txt" % index, b"words before " + form + b" words after\n")

    def assert_each_holder_is_caught(self, forms):
        result = self.run_audit()
        self.assertNotEqual(result.returncode, 0)
        for index in range(len(forms)):
            self.assertIn("FAIL holder_%d.txt: holds the text key" % index, result.stdout)

    def test_fonts_message_files_and_converted_textures_cannot_be_committed(self):
        for name, data in PLANTED_FILES.items():
            self.plant(name, data)
        result = self.run_audit()
        self.assertNotEqual(result.returncode, 0)
        for name in PLANTED_FILES:
            self.assertIn("FAIL " + name + ":", result.stdout)

    def test_git_ignores_a_copy_of_the_library_and_the_key_files(self):
        with open(GIT_IGNORE_PATH, encoding="ascii") as source:
            lines = source.read().splitlines()
        self.assertIn("*.so", lines)
        self.assertIn("*.bin", lines)

    def test_an_ordinary_file_still_passes(self):
        self.plant("notes.txt", b"plain text\n")
        self.assertEqual(self.run_audit().returncode, 0)

    def plant_local_key(self, key):
        self.plant(".gitignore", b"extracted/\n")
        os.makedirs(os.path.join(self.folder, KEY_FOLDER))
        self.plant(os.path.join(KEY_FOLDER, "text_key.bin"), key)

    def git(self, *arguments):
        subprocess.run(["git", "-C", self.folder] + list(arguments), check=True)

    def test_a_file_named_text_key_cannot_be_committed(self):
        self.plant("text_key.bin", b"x")
        result = self.run_audit()
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("FAIL text_key.bin: forbidden file name", result.stdout)

    def test_a_key_file_added_past_the_ignore_rule_is_caught(self):
        self.plant_local_key(FAKE_KEY)
        self.assertEqual(self.run_audit().returncode, 0)
        self.git("add", "--force", os.path.join(KEY_FOLDER, "text_key.bin"))
        result = self.run_audit()
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("FAIL extracted/remaster_apk/text_key.bin: forbidden file name", result.stdout)

    def test_every_form_of_the_local_key_is_caught_and_never_printed(self):
        self.plant_local_key(FAKE_KEY)
        forms = key_forms(FAKE_KEY)
        for index, form in enumerate(forms):
            self.plant("holder_%d.txt" % index, b"words before " + form + b" words after\n")
        result = self.run_audit()
        self.assertNotEqual(result.returncode, 0)
        for index in range(len(forms)):
            self.assertIn("FAIL holder_%d.txt: holds the text key" % index, result.stdout)
        for form in forms[1:]:
            self.assertNotIn(form.decode("ascii"), result.stdout)

    def test_other_orders_and_encodings_of_the_key_are_caught(self):
        self.plant_local_key(FAKE_KEY)
        forms = forms_in_other_orders_and_encodings(FAKE_KEY)
        self.plant_each(forms)
        self.assert_each_holder_is_caught(forms)

    def test_other_marks_between_the_bytes_of_the_key_are_caught(self):
        self.plant_local_key(FAKE_KEY)
        forms = forms_with_other_marks_between_the_bytes(FAKE_KEY)
        self.plant_each(forms)
        self.assert_each_holder_is_caught(forms)

    def test_hex_numbers_written_without_a_leading_zero_are_caught(self):
        self.plant_local_key(LOW_FAKE_KEY)
        forms = [", ".join(hex(value) for value in LOW_FAKE_KEY).encode("ascii")]
        self.plant_each(forms)
        self.assert_each_holder_is_caught(forms)

    def test_decimal_numbers_written_with_leading_zeros_are_caught(self):
        self.plant_local_key(LOW_FAKE_KEY)
        forms = [", ".join("%03d" % value for value in LOW_FAKE_KEY).encode("ascii")]
        self.plant_each(forms)
        self.assert_each_holder_is_caught(forms)

    def test_hex_text_stuck_to_other_letters_is_caught(self):
        self.plant_local_key(FAKE_KEY)
        forms = [b"word" + FAKE_KEY.hex().encode("ascii") + b"word"]
        self.plant_each(forms)
        self.assert_each_holder_is_caught(forms)

    def test_both_kinds_of_base64_are_caught_with_or_without_padding(self):
        key = KEY_WHOSE_BASE64_HAS_PLUS_AND_SLASH
        self.assertNotEqual(base64.b64encode(key), base64.urlsafe_b64encode(key))
        self.plant_local_key(key)
        forms = [
            base64.b64encode(key).rstrip(b"="),
            base64.urlsafe_b64encode(key).rstrip(b"="),
        ]
        self.plant_each(forms)
        self.assert_each_holder_is_caught(forms)

    def test_every_window_of_eight_bytes_is_caught(self):
        self.plant_local_key(FAKE_KEY)
        forms = []
        for start in range(len(FAKE_KEY) - 8 + 1):
            forms.append(FAKE_KEY[start:start + 8])
            forms.append(FAKE_KEY[start:start + 8].hex().encode("ascii"))
        self.plant_each(forms)
        self.assert_each_holder_is_caught(forms)

    def test_a_key_sharing_only_shorter_pieces_is_left_alone(self):
        self.plant_local_key(FAKE_KEY)
        self.plant_each(key_forms(NEAR_FAKE_KEY) + forms_in_other_orders_and_encodings(NEAR_FAKE_KEY) + forms_with_other_marks_between_the_bytes(NEAR_FAKE_KEY))
        self.assertEqual(self.run_audit().returncode, 0)

    def test_a_piece_shorter_than_the_window_is_not_searched_for(self):
        self.plant_local_key(FAKE_KEY)
        piece = FAKE_KEY[:PIECE_BELOW_THE_WINDOW]
        self.plant("notes.txt", piece.hex().encode("ascii") + b" " + piece)
        self.assertEqual(self.run_audit().returncode, 0)

    def test_no_piece_of_the_key_is_printed(self):
        self.plant_local_key(FAKE_KEY)
        self.plant_each(key_forms(FAKE_KEY) + forms_in_other_orders_and_encodings(FAKE_KEY))
        result = subprocess.run([sys.executable, "-I", self.audit_script], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        self.assertNotEqual(result.returncode, 0)
        for form in forms_of_every_piece(FAKE_KEY):
            self.assertNotIn(form, result.stdout)
            self.assertNotIn(form, result.stderr)

    def test_a_key_in_a_file_with_an_unusual_name_is_caught(self):
        self.plant_local_key(FAKE_KEY)
        for name in NAMES_GIT_WOULD_QUOTE_OR_STRIP:
            self.plant(name + ".txt", FAKE_KEY.hex().encode("ascii"))
            self.plant(name + " ", FAKE_KEY)
        result = self.run_audit()
        self.assertNotEqual(result.returncode, 0)
        self.assertEqual(result.stdout.count(": holds the text key"), 2 * len(NAMES_GIT_WOULD_QUOTE_OR_STRIP))

    def test_an_unusual_name_can_be_printed_on_any_terminal(self):
        self.plant("caf\u00e9.nds", b"x")
        result = self.run_audit(dict(os.environ, PYTHONIOENCODING="ascii"))
        self.assertIn("FAIL caf\\xe9.nds: forbidden extension .nds", result.stdout)
        self.assertEqual(result.stderr, "")

    def test_a_forbidden_file_with_an_unusual_name_is_caught(self):
        for name in NAMES_GIT_WOULD_QUOTE_OR_STRIP:
            self.plant(name + ".nds", b"x")
        result = self.run_audit()
        self.assertNotEqual(result.returncode, 0)
        self.assertEqual(result.stdout.count(": forbidden extension .nds"), len(NAMES_GIT_WOULD_QUOTE_OR_STRIP))

    def test_a_tracked_file_holding_the_key_is_caught(self):
        self.plant_local_key(FAKE_KEY)
        self.plant("tracked.txt", FAKE_KEY.hex().encode("ascii"))
        self.git("add", "tracked.txt")
        self.assertIn("FAIL tracked.txt: holds the text key", self.run_audit().stdout)

    def plant_staged_then_change(self, name, staged, working):
        self.plant(name, staged)
        self.git("add", name)
        if working is None:
            os.remove(os.path.join(self.folder, name))
        else:
            self.plant(name, working)

    def test_a_key_staged_and_then_cleaned_from_the_working_copy_is_caught(self):
        self.plant_local_key(FAKE_KEY)
        self.plant_staged_then_change("notes.txt", FAKE_KEY.hex().encode("ascii"), b"clean now\n")
        result = self.run_audit()
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("FAIL notes.txt: the staged copy holds the text key", result.stdout)
        self.assertNotIn(FAKE_KEY.hex(), result.stdout)

    def test_a_key_staged_and_then_deleted_from_the_working_copy_is_caught(self):
        self.plant_local_key(FAKE_KEY)
        self.plant_staged_then_change("notes.txt", FAKE_KEY, None)
        self.assertIn("FAIL notes.txt: the staged copy holds the text key", self.run_audit().stdout)

    def test_a_key_saved_as_text_in_utf16_is_caught_in_a_staged_copy(self):
        self.plant_local_key(FAKE_KEY)
        self.plant_staged_then_change("notes.txt", FAKE_KEY.hex().encode("utf-16-le"), b"clean now\n")
        self.assertIn("FAIL notes.txt: the staged copy holds the text key", self.run_audit().stdout)

    def test_game_data_staged_and_then_cleaned_from_the_working_copy_is_caught(self):
        self.plant_staged_then_change("notes.txt", b"OggS" + PADDING, b"clean now\n")
        result = self.run_audit()
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("FAIL notes.txt: the staged copy looks like Ogg audio", result.stdout)

    def test_a_staged_copy_matching_the_working_copy_is_reported_once(self):
        self.plant_local_key(FAKE_KEY)
        self.plant_staged_then_change("notes.txt", FAKE_KEY, FAKE_KEY)
        self.assertEqual(self.run_audit().stdout.count("notes.txt"), 1)

    def test_every_staged_copy_is_read_in_turn(self):
        self.plant_local_key(FAKE_KEY)
        self.plant_staged_then_change("first.txt", b"one\n", b"changed\n")
        self.plant_staged_then_change("second.txt", b"two\n", b"changed\n")
        self.plant_staged_then_change("third.txt", FAKE_KEY, b"changed\n")
        result = self.run_audit()
        self.assertEqual(result.stdout.count("FAIL"), 1, result.stdout)
        self.assertIn("FAIL third.txt: the staged copy holds the text key", result.stdout)

    def test_a_clean_file_staged_and_changed_passes(self):
        self.plant_local_key(FAKE_KEY)
        self.plant_staged_then_change("notes.txt", b"one\n", b"two\n")
        self.assertEqual(self.run_audit().returncode, 0)

    def test_the_rules_file_may_stage_its_own_first_bytes_but_not_the_key(self):
        self.plant_local_key(FAKE_KEY)
        rules_path = os.path.join("tools", "audit_rules.txt")
        with open(os.path.join(self.folder, rules_path), "rb") as source:
            rules = source.read()
        self.plant_staged_then_change(rules_path, b"OggS" + rules, rules)
        self.assertEqual(self.run_audit().returncode, 0)
        self.plant_staged_then_change(rules_path, FAKE_KEY + rules, rules)
        self.assertIn("FAIL tools/audit_rules.txt: the staged copy holds the text key", self.run_audit().stdout)

    def test_a_staged_submodule_entry_is_not_read_as_a_file(self):
        self.git("update-index", "--add", "--cacheinfo", "160000,%s,submodule" % ("1" * 40))
        result = self.run_audit()
        self.assertEqual(result.returncode, 0, result.stdout)

    def test_a_staged_copy_git_cannot_read_fails_the_audit(self):
        self.git("update-index", "--add", "--cacheinfo", "100644,%s,lost.txt" % ("2" * 40))
        result = self.run_audit()
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("FAIL lost.txt: could not read the staged copy", result.stdout)

    def test_the_rules_file_is_checked_for_the_key_too(self):
        self.plant_local_key(FAKE_KEY)
        with open(os.path.join(self.folder, "tools", "audit_rules.txt"), "a", encoding="ascii") as output:
            output.write("# " + FAKE_KEY.hex() + "\n")
        self.assertIn("FAIL tools/audit_rules.txt: holds the text key", self.run_audit().stdout)

    def test_an_ignored_file_and_another_key_are_left_alone(self):
        self.plant_local_key(FAKE_KEY)
        self.plant(os.path.join("extracted", "ignored_copy.txt"), FAKE_KEY.hex().encode("ascii"))
        for index, form in enumerate(key_forms(OTHER_FAKE_KEY)):
            self.plant("other_%d.txt" % index, form)
        self.assertEqual(self.run_audit().returncode, 0)

    def test_without_a_local_key_nothing_is_searched_for_and_it_says_so(self):
        self.plant("notes.txt", FAKE_KEY.hex().encode("ascii"))
        result = self.run_audit()
        self.assertEqual(result.returncode, 0)
        self.assertIn("key search skipped: no extracted/remaster_apk/text_key.bin", result.stdout)

    def test_with_a_local_key_the_search_is_not_called_skipped(self):
        self.plant_local_key(FAKE_KEY)
        self.assertNotIn("key search skipped", self.run_audit().stdout)

    def test_outside_a_git_repository_the_audit_fails(self):
        shutil.rmtree(os.path.join(self.folder, ".git"))
        environment = dict(os.environ, GIT_CEILING_DIRECTORIES=os.path.dirname(self.folder))
        result = self.run_audit(environment)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("FAIL could not list the files with git", result.stdout)
        self.assertNotIn("audit passed", result.stdout)

    def test_without_git_the_audit_fails(self):
        result = self.run_audit(dict(os.environ, PATH=""))
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("FAIL could not list the files with git", result.stdout)

    def test_a_decimal_form_needs_whole_numbers(self):
        self.plant_local_key(FAKE_KEY)
        decimals = [str(value) for value in FAKE_KEY]
        decimals[7] = "1" + decimals[7]
        decimals[15] = "1" + decimals[15]
        self.plant("notes.txt", ", ".join(decimals).encode("ascii"))
        self.assertEqual(self.run_audit().returncode, 0)

    def test_a_local_key_of_the_wrong_size_fails_without_flagging_other_files(self):
        self.plant_local_key(b"")
        self.plant("notes.txt", b"plain text\n")
        result = self.run_audit()
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("FAIL extracted/remaster_apk/text_key.bin: wrong size", result.stdout)
        self.assertNotIn("notes.txt", result.stdout)

    def test_the_python_form_of_a_key_with_printable_bytes_is_caught(self):
        self.plant_local_key(MIXED_FAKE_KEY)
        self.plant("notes.txt", repr(MIXED_FAKE_KEY).encode("ascii"))
        self.assertIn("FAIL notes.txt: holds the text key", self.run_audit().stdout)

    def test_the_self_test_plants_a_fake_key(self):
        result = subprocess.run([sys.executable, "-I", self.audit_script, "--self-test"], stdout=subprocess.PIPE, text=True)
        self.assertEqual(result.returncode, 0)
        self.assertIn("55 planted key forms caught", result.stdout)


if __name__ == "__main__":
    unittest.main()
