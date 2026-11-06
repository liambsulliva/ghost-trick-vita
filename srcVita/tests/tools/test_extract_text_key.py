import contextlib
import errno
import io
import os
import random
import shutil
import signal
import stat
import struct
import subprocess
import sys
import tempfile
import unittest
import zipfile
from unittest import mock

import convert_assets
import extract_text_key
import synthetic_files

KEY = synthetic_files.FAKE_TEXT_KEY
KEY_WITH_REGULAR_EXPRESSION_CHARACTERS = bytes(range(0x20, 0x30))
KEY_WITH_ONE_REPEATED_BYTE_AT_ODD_PLACES = bytes([1, 0x77, 2, 0x77, 3, 0x77, 4, 0x77, 5, 0x77, 6, 0x77, 7, 0x77, 8, 0x77])
OTHER_MEMBER_NAME = "assets/other_file"
OTHER_MEMBER_DATA = b"OTHER MEMBER DATA THAT IS NOT THE LIBRARY"
ELF_START = b"\x7fELF"
EXIT_CODE_BASE_FOR_SIGNALS = 128
PRIVATE_FILE_MODE = 0o600
CENTRAL_HEADER_START = b"PK\x01\x02"
CENTRAL_HEADER_FLAGS_OFFSET = 8
ENCRYPTED_FLAG = 1
CUT_PACK_START = b"KPKA\x04\x00\x00\x00\x00\x00\x00"
CUT_PACK_SIZES = (4, 5, 11)
STOP_DURING_SAVE = """
import os
import sys

tools_folder, signal_number, package_path, apk_folder = sys.argv[1:]
sys.path.insert(0, tools_folder)
import extract_text_key


def stop_here(_descriptor):
    os.kill(os.getpid(), int(signal_number))


os.fsync = stop_here
sys.argv = ["extract_text_key.py", package_path, "--apk-folder", apk_folder]
sys.exit(extract_text_key.main())
"""

SMALL_ENTRIES = [("abcdefghij", ["klmn", "opqr"])]
KEY_INDEX_WITH_ONLY_LETTERS = 2
KEY_INDEX_WITH_A_TEXT_TERMINATOR = 14
FLIP_THAT_KEEPS_LETTERS = 0x20
FLIP_THAT_LEAVES_THE_NAME_CHARACTERS = 0x40
SHORT_NAME_ENTRIES = [("ab", ["a longer string of other language", "a longer string of english"])]
KEY_INDEX_WITHOUT_ANY_NAME = 7
FLIP_THAT_MAKES_A_LONE_SURROGATE = 0xD8


def spread_entries():
    return [("entry_" + "a" * count, ["other " + "b" * count, "english " + "c" * (count + 1)]) for count in range(16)]


def changed_key(key, index, flip):
    changed = bytearray(key)
    changed[index] = changed[index] ^ flip
    return bytes(changed)


def noise(size, seed=2031):
    generator = random.Random(seed)
    return bytes(generator.randrange(256) for _ in range(size))


def library_with(*windows):
    parts = [ELF_START]
    for index, window in enumerate(windows):
        parts.append(noise(1001, index))
        parts.append(window)
    parts.append(noise(1000, len(windows)))
    return b"".join(parts)


def mark_last_member_encrypted(package_path):
    with open(package_path, "rb") as source:
        data = bytearray(source.read())
    position = data.rfind(CENTRAL_HEADER_START)
    data[position + CENTRAL_HEADER_FLAGS_OFFSET] = data[position + CENTRAL_HEADER_FLAGS_OFFSET] | ENCRYPTED_FLAG
    with open(package_path, "wb") as output:
        output.write(data)


def corrupt_stored_member(package_path, member_data):
    with open(package_path, "rb") as source:
        data = bytearray(source.read())
    position = data.index(member_data)
    data[position] = data[position] ^ 0xFF
    with open(package_path, "wb") as output:
        output.write(data)


class ToolTestCase(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, self.folder)
        self.apk_folder = os.path.join(self.folder, "apk")
        os.makedirs(self.apk_folder)
        self.key_path = os.path.join(self.apk_folder, "text_key.bin")
        self.hidden_keys = [KEY]
        self.use_message(spread_entries())

    def use_message(self, entries, key=KEY):
        files = [(extract_text_key.CHECK_MESSAGE_PATH, synthetic_files.message_file_bytes(entries, key=key), True)]
        synthetic_files.write_apk_folder(self.apk_folder, files)

    def write_library(self, data):
        path = os.path.join(self.folder, "libruntime.so")
        with open(path, "wb") as output:
            output.write(data)
        return path

    def write_package(self, library_data, other_data=OTHER_MEMBER_DATA):
        path = os.path.join(self.folder, "copy_of_the_package.zip")
        with zipfile.ZipFile(path, "w", zipfile.ZIP_STORED) as package:
            package.writestr(OTHER_MEMBER_NAME, other_data)
            if library_data is not None:
                package.writestr(extract_text_key.LIBRARY_MEMBER, library_data)
        return path

    def run_tool(self, path):
        command = [sys.executable, "-I", extract_text_key.__file__, path, "--apk-folder", self.apk_folder]
        result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        self.assert_nothing_leaked(result)
        return result

    def assert_nothing_leaked(self, result):
        for key in self.hidden_keys:
            for form in synthetic_files.key_text_forms(key):
                self.assertNotIn(form, result.stdout)
                self.assertNotIn(form, result.stderr)
        self.assertNotIn(b"Traceback", result.stderr)

    def saved_key(self):
        with open(self.key_path, "rb") as source:
            return source.read()

    def assert_saved(self, result, key):
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(result.stdout.decode("utf-8"), "saved the text key to " + self.key_path + "\n")
        self.assertEqual(result.stderr, b"")
        self.assertEqual(self.saved_key(), key)
        self.assertEqual(stat.S_IMODE(os.stat(self.key_path).st_mode), PRIVATE_FILE_MODE)
        self.assertEqual(self.temporary_files(), [])

    def temporary_files(self):
        return [name for name in os.listdir(self.apk_folder) if name.startswith("text_key") and name != "text_key.bin"]

    def assert_refused(self, result, wanted_words):
        self.assertEqual(result.returncode, 1)
        self.assertEqual(result.stdout, b"")
        self.assertIn(wanted_words, result.stderr.decode("utf-8"))
        self.assertFalse(os.path.exists(self.key_path))


class FindingTheKeyTests(ToolTestCase):
    def test_a_library_given_directly(self):
        self.assert_saved(self.run_tool(self.write_library(library_with(KEY))), KEY)

    def test_the_library_inside_a_package(self):
        self.assert_saved(self.run_tool(self.write_package(library_with(KEY))), KEY)

    def test_a_compressed_library_inside_a_package(self):
        path = os.path.join(self.folder, "compressed.zip")
        with zipfile.ZipFile(path, "w", zipfile.ZIP_DEFLATED) as package:
            package.writestr(extract_text_key.LIBRARY_MEMBER, library_with(KEY))
        self.assert_saved(self.run_tool(path), KEY)

    def test_a_key_at_odd_places_is_found(self):
        for size_before in (0, 1, 3, 1001, 4103):
            library = ELF_START + noise(size_before) + KEY + noise(33)
            self.assert_saved(self.run_tool(self.write_library(library)), KEY)

    def test_a_key_at_the_very_end_of_the_library_is_found(self):
        self.assert_saved(self.run_tool(self.write_library(ELF_START + noise(100) + KEY)), KEY)

    def test_a_key_holding_regular_expression_characters_is_found(self):
        self.hidden_keys.append(KEY_WITH_REGULAR_EXPRESSION_CHARACTERS)
        self.use_message(spread_entries(), KEY_WITH_REGULAR_EXPRESSION_CHARACTERS)
        self.assert_saved(self.run_tool(self.write_library(library_with(KEY_WITH_REGULAR_EXPRESSION_CHARACTERS))), KEY_WITH_REGULAR_EXPRESSION_CHARACTERS)

    def test_a_filter_match_that_overlaps_the_key_does_not_hide_it(self):
        key = KEY_WITH_ONE_REPEATED_BYTE_AT_ODD_PLACES
        self.hidden_keys.append(key)
        self.use_message(spread_entries(), key)
        library = ELF_START + noise(100) + bytes([0x55, 0x77]) + key + noise(100)
        self.assert_saved(self.run_tool(self.write_library(library)), key)

    def test_the_same_key_twice_counts_once(self):
        self.assert_saved(self.run_tool(self.write_library(library_with(KEY, KEY))), KEY)

    def test_an_old_key_file_is_replaced(self):
        synthetic_files.write_text_key(self.apk_folder, bytes(16))
        self.assert_saved(self.run_tool(self.write_library(library_with(KEY))), KEY)

    def test_the_default_folder_is_the_one_the_converter_uses(self):
        self.assertEqual(extract_text_key.DEFAULT_APK_FOLDER, convert_assets.DEFAULT_APK_FOLDER)


class QuickFilterTests(unittest.TestCase):
    def test_entry_names_fix_the_key_bytes_that_fall_on_high_bytes(self):
        layout = extract_text_key.read_string_layout(synthetic_files.message_file_bytes(spread_entries()))
        wanted = [KEY[index] if index % 2 == 1 else None for index in range(16)]
        self.assertEqual(extract_text_key.key_bytes_from_entry_names(layout), wanted)

    def test_a_few_stray_high_bytes_do_not_change_the_vote(self):
        entries = spread_entries() + [("unusual\u0151name", ["other", "english"])]
        layout = extract_text_key.read_string_layout(synthetic_files.message_file_bytes(entries))
        wanted = [KEY[index] if index % 2 == 1 else None for index in range(16)]
        self.assertEqual(extract_text_key.key_bytes_from_entry_names(layout), wanted)


class FullCheckTests(ToolTestCase):
    def test_every_even_key_byte_is_checked_when_terminators_reach_it(self):
        for index in range(0, 16, 2):
            decoy = changed_key(KEY, index, 0x20)
            self.assert_saved(self.run_tool(self.write_library(library_with(decoy, KEY))), KEY)
            os.remove(self.key_path)

    def test_a_decoy_alone_is_not_a_key(self):
        result = self.run_tool(self.write_library(library_with(changed_key(KEY, 4, 0x20))))
        self.assert_refused(result, "no key found")

    def test_a_terminator_that_runs_into_the_next_string_fails(self):
        self.use_message(SMALL_ENTRIES)
        decoy = changed_key(KEY, KEY_INDEX_WITH_A_TEXT_TERMINATOR, FLIP_THAT_KEEPS_LETTERS)
        self.assert_saved(self.run_tool(self.write_library(library_with(decoy, KEY))), KEY)

    def test_a_name_with_other_characters_fails(self):
        self.use_message(SMALL_ENTRIES)
        decoy = changed_key(KEY, KEY_INDEX_WITH_ONLY_LETTERS, FLIP_THAT_LEAVES_THE_NAME_CHARACTERS)
        self.assert_saved(self.run_tool(self.write_library(library_with(decoy, KEY))), KEY)

    def test_text_that_is_not_utf16_fails(self):
        self.use_message(SHORT_NAME_ENTRIES)
        decoy = changed_key(KEY, KEY_INDEX_WITHOUT_ANY_NAME, FLIP_THAT_MAKES_A_LONE_SURROGATE)
        self.assert_saved(self.run_tool(self.write_library(library_with(decoy, KEY))), KEY)

    def test_two_different_valid_keys_save_nothing(self):
        self.use_message(SMALL_ENTRIES)
        other_key = changed_key(KEY, KEY_INDEX_WITH_ONLY_LETTERS, FLIP_THAT_KEEPS_LETTERS)
        self.hidden_keys.append(other_key)
        result = self.run_tool(self.write_library(library_with(KEY, other_key)))
        self.assert_refused(result, "several different keys found")

    def test_two_different_valid_keys_leave_an_old_key_file_alone(self):
        self.use_message(SMALL_ENTRIES)
        other_key = changed_key(KEY, KEY_INDEX_WITH_ONLY_LETTERS, FLIP_THAT_KEEPS_LETTERS)
        synthetic_files.write_text_key(self.apk_folder, bytes(16))
        result = self.run_tool(self.write_library(library_with(KEY, other_key)))
        self.assertEqual(result.returncode, 1)
        self.assertEqual(self.saved_key(), bytes(16))


class FailureTests(ToolTestCase):
    def test_no_key_in_the_library(self):
        result = self.run_tool(self.write_library(library_with()))
        self.assert_refused(result, "no key found: this copy may be a different build")

    def test_an_encrypted_library_is_reported(self):
        path = self.write_package(library_with(KEY))
        mark_last_member_encrypted(path)
        self.assert_refused(self.run_tool(path), "could not read lib/arm64-v8a/libruntime.so")

    def test_a_pack_cut_inside_its_header_is_reported(self):
        library_path = self.write_library(library_with(KEY))
        for size in CUT_PACK_SIZES:
            with self.subTest(size=size):
                with open(os.path.join(self.apk_folder, "re_chunk_000.pak"), "wb") as output:
                    output.write(CUT_PACK_START[:size])
                self.assert_refused(self.run_tool(library_path), "the packs are missing or damaged in ")

    def test_a_missing_package(self):
        result = self.run_tool(os.path.join(self.folder, "absent.zip"))
        self.assert_refused(result, "package not found: ")

    def test_a_folder_is_not_a_package(self):
        self.assert_refused(self.run_tool(self.folder), "package not found: ")

    def test_a_file_that_is_neither_a_package_nor_a_library(self):
        self.assert_refused(self.run_tool(self.write_library(b"plain text")), "not a package or a library: ")

    def test_a_package_without_the_library(self):
        result = self.run_tool(self.write_package(None))
        self.assert_refused(result, "the package has no lib/arm64-v8a/libruntime.so")

    def test_only_the_library_member_is_searched(self):
        result = self.run_tool(self.write_package(library_with(), library_with(KEY)))
        self.assert_refused(result, "no key found")

    def test_a_bad_checksum_on_another_member_is_ignored(self):
        path = self.write_package(library_with(KEY))
        corrupt_stored_member(path, OTHER_MEMBER_DATA)
        with zipfile.ZipFile(path) as package:
            self.assertEqual(package.testzip(), OTHER_MEMBER_NAME)
        self.assert_saved(self.run_tool(path), KEY)

    def test_a_bad_checksum_on_the_library_is_reported(self):
        library = library_with(KEY)
        path = self.write_package(library)
        corrupt_stored_member(path, library[:100])
        self.assert_refused(self.run_tool(path), "could not read lib/arm64-v8a/libruntime.so")

    def test_missing_packs(self):
        library_path = self.write_library(library_with(KEY))
        shutil.rmtree(self.apk_folder)
        os.makedirs(self.apk_folder)
        self.assert_refused(self.run_tool(library_path), "the packs are missing or damaged in ")

    def test_a_missing_folder(self):
        library_path = self.write_library(library_with(KEY))
        shutil.rmtree(self.apk_folder)
        self.assert_refused(self.run_tool(library_path), "the packs are missing or damaged in ")

    def test_a_damaged_pack(self):
        library_path = self.write_library(library_with(KEY))
        with open(os.path.join(self.apk_folder, "data01"), "wb") as output:
            output.write(b"not a zip file")
        self.assert_refused(self.run_tool(library_path), "the packs are missing or damaged in ")

    def test_packs_without_the_message_file(self):
        library_path = self.write_library(library_with(KEY))
        synthetic_files.write_apk_folder(self.apk_folder, [])
        self.assert_refused(self.run_tool(library_path), "this copy may be a different build")

    def test_a_message_file_of_another_kind(self):
        library_path = self.write_library(library_with(KEY))
        synthetic_files.write_apk_folder(self.apk_folder, [(extract_text_key.CHECK_MESSAGE_PATH, b"not a message file" * 8, True)])
        self.assert_refused(self.run_tool(library_path), "could not read the layout")

    def test_a_string_pointer_outside_the_area(self):
        library_path = self.write_library(library_with(KEY))
        data = bytearray(synthetic_files.message_file_bytes(SMALL_ENTRIES))
        struct.pack_into("<Q", data, synthetic_files.gmsg_entry_start(1) + synthetic_files.GMSG_ENTRY_TEXT_POINTERS_OFFSET, 1)
        synthetic_files.write_apk_folder(self.apk_folder, [(extract_text_key.CHECK_MESSAGE_PATH, bytes(data), True)])
        self.assert_refused(self.run_tool(library_path), "could not read the layout")

    def test_a_message_file_with_no_entries_finds_no_key(self):
        self.use_message([])
        self.assert_refused(self.run_tool(self.write_library(library_with(KEY))), "no key found")


class CommandLineTests(ToolTestCase):
    def run_main(self, side_effect):
        output = io.StringIO()
        errors = io.StringIO()
        with mock.patch.object(sys, "argv", ["extract_text_key.py", "package", "--apk-folder", self.apk_folder]):
            with mock.patch.object(extract_text_key, "extract_key", side_effect=side_effect):
                with contextlib.redirect_stdout(output), contextlib.redirect_stderr(errors):
                    code = extract_text_key.main()
        return code, output.getvalue(), errors.getvalue()

    def test_an_unexpected_error_prints_only_its_type(self):
        code, output, errors = self.run_main(ValueError("words that must not be printed"))
        self.assertEqual(code, 1)
        self.assertEqual(output, "")
        self.assertEqual(errors, "error: stopped by ValueError\n")

    def test_an_unexpected_file_error_prints_only_its_type(self):
        code, output, errors = self.run_main(PermissionError(13, "words that must not be printed", "some_file"))
        self.assertEqual(code, 1)
        self.assertEqual(output, "")
        self.assertEqual(errors, "error: stopped by PermissionError\n")


class FileErrorTests(ToolTestCase):
    def test_a_package_that_cannot_be_read_is_named_with_the_reason(self):
        with mock.patch.object(extract_text_key, "read_library", side_effect=PermissionError(errno.EACCES, "the reason", "some_file")):
            with self.assertRaises(extract_text_key.ExtractionError) as caught:
                extract_text_key.extract_key("the_package", self.apk_folder)
        self.assertEqual(str(caught.exception), "could not read the_package: the reason")

    def test_a_full_disk_names_the_folder_and_the_reason(self):
        library_path = self.write_library(library_with(KEY))
        with mock.patch.object(extract_text_key.os, "fsync", side_effect=OSError(errno.ENOSPC, "No space left on device")):
            with self.assertRaises(extract_text_key.ExtractionError) as caught:
                extract_text_key.extract_key(library_path, self.apk_folder)
        self.assertEqual(str(caught.exception), "could not save the key in " + self.apk_folder + ": No space left on device")
        self.assertEqual(self.temporary_files(), [])

    def test_an_error_without_a_reason_names_its_type(self):
        library_path = self.write_library(library_with(KEY))
        with mock.patch.object(extract_text_key.os, "fsync", side_effect=OSError("failed")):
            with self.assertRaises(extract_text_key.ExtractionError) as caught:
                extract_text_key.extract_key(library_path, self.apk_folder)
        self.assertEqual(str(caught.exception), "could not save the key in " + self.apk_folder + ": OSError")

    def test_a_folder_at_the_key_path_names_the_folder_not_a_temporary_file(self):
        library_path = self.write_library(library_with(KEY))
        os.makedirs(self.key_path)
        result = self.run_tool(library_path)
        self.assertEqual(result.returncode, 1)
        errors = result.stderr.decode("utf-8")
        self.assertIn("could not save the key in " + self.apk_folder + ": ", errors)
        self.assertNotIn("text_key_", errors)
        self.assertEqual(self.temporary_files(), [])

    @unittest.skipIf(os.geteuid() == 0, "root can write to a read-only folder")
    def test_a_read_only_folder_names_the_folder_not_a_temporary_file(self):
        library_path = self.write_library(library_with(KEY))
        os.chmod(self.apk_folder, 0o555)
        self.addCleanup(os.chmod, self.apk_folder, 0o755)
        result = self.run_tool(library_path)
        self.assertEqual(result.returncode, 1)
        errors = result.stderr.decode("utf-8")
        self.assertIn("could not save the key in " + self.apk_folder + ": ", errors)
        self.assertNotIn("text_key_", errors)

    @unittest.skipIf(os.geteuid() == 0, "root can read any file")
    def test_an_unreadable_package_is_named_with_the_reason(self):
        library_path = self.write_library(library_with(KEY))
        os.chmod(library_path, 0)
        self.assert_refused(self.run_tool(library_path), "could not read " + library_path + ": ")


class SavingTests(ToolTestCase):
    def test_a_failed_save_keeps_the_old_file_and_leaves_nothing_else(self):
        synthetic_files.write_text_key(self.apk_folder, bytes(16))
        names_before = sorted(os.listdir(self.apk_folder))
        with mock.patch.object(extract_text_key.os, "replace", side_effect=OSError("failed")):
            with self.assertRaises(OSError):
                extract_text_key.save_key(self.apk_folder, KEY)
        self.assertEqual(self.saved_key(), bytes(16))
        self.assertEqual(sorted(os.listdir(self.apk_folder)), names_before)

    def test_the_file_appears_whole_under_its_final_name(self):
        seen = []
        real_replace = os.replace

        def look_then_replace(source, target):
            with open(source, "rb") as whole_file:
                seen.append(whole_file.read())
            self.assertFalse(os.path.exists(target))
            self.assertEqual(os.path.dirname(source), os.path.dirname(target))
            real_replace(source, target)

        with mock.patch.object(extract_text_key.os, "replace", side_effect=look_then_replace):
            extract_text_key.save_key(self.apk_folder, KEY)
        self.assertEqual(seen, [KEY])
        self.assertEqual(self.saved_key(), KEY)

    def test_the_temporary_file_has_a_name_git_ignores(self):
        seen = []
        real_replace = os.replace

        def note_the_name_then_replace(source, target):
            seen.append(os.path.basename(source))
            real_replace(source, target)

        with mock.patch.object(extract_text_key.os, "replace", side_effect=note_the_name_then_replace):
            extract_text_key.save_key(self.apk_folder, KEY)
        self.assertEqual(len(seen), 1)
        self.assertTrue(seen[0].startswith("text_key_"), seen[0])
        self.assertTrue(seen[0].endswith(".bin"), seen[0])

    def stop_during_save(self, signal_number):
        tools_folder = os.path.dirname(os.path.abspath(extract_text_key.__file__))
        package_path = self.write_library(library_with(KEY))
        command = [sys.executable, "-I", "-c", STOP_DURING_SAVE, tools_folder, str(int(signal_number)), package_path, self.apk_folder]
        return subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    def test_a_terminated_tool_keeps_the_old_file_and_leaves_nothing_else(self):
        synthetic_files.write_text_key(self.apk_folder, bytes(16))
        for signal_number in (signal.SIGTERM, signal.SIGHUP):
            with self.subTest(signal_number=signal_number):
                result = self.stop_during_save(signal_number)
                self.assertEqual(result.returncode, EXIT_CODE_BASE_FOR_SIGNALS + signal_number)
                self.assert_nothing_leaked(result)
                self.assertEqual(self.saved_key(), bytes(16))
                self.assertEqual(self.temporary_files(), [])

    def test_a_killed_tool_leaves_only_a_name_git_ignores(self):
        result = self.stop_during_save(signal.SIGKILL)
        self.assertEqual(result.returncode, -signal.SIGKILL)
        self.assertFalse(os.path.exists(self.key_path))
        leftovers = self.temporary_files()
        self.assertEqual(len(leftovers), 1)
        self.assertTrue(leftovers[0].endswith(".bin"), leftovers[0])
        self.assertEqual(stat.S_IMODE(os.stat(os.path.join(self.apk_folder, leftovers[0])).st_mode), PRIVATE_FILE_MODE)


if __name__ == "__main__":
    unittest.main()
