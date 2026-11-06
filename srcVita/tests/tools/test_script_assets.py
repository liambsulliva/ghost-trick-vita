import contextlib
import io
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
import unittest
from unittest import mock

import asset_writers
import bake_record
import convert_assets
import script_assets
import synthetic_files
import test_end_to_end

TESTS_FOLDER = os.path.dirname(os.path.abspath(__file__))
SCRIPT_NAME_PAIRS_PATH = os.path.join(os.path.dirname(TESTS_FOLDER), "fixtures", "script_name_pairs.h")
SCRIPT_CHANGE_OP = 0x24
ACT_OP = 0x19
GOTO_OP = 0x22
END_OP = 0x0
UNKNOWN_OP = 0x7FFF
WORDS_PAST_64_KILOBYTES = 0x8000
FIRST_PLAIN = synthetic_files.script_file_bytes([END_OP] * 40, ["*", "made up words", "made up words again"])
SECOND_PLAIN = synthetic_files.script_file_bytes([END_OP] * 8, ["*"])
UNLISTED_PLAIN = synthetic_files.script_file_bytes([END_OP] * 10, ["*"])
ROM_FILES = {"first.xml.lz": synthetic_files.lz11_bytes(FIRST_PLAIN), "area/also.xml.lz": synthetic_files.lz11_bytes(UNLISTED_PLAIN), "area/second.xml.lz": synthetic_files.lz11_bytes(SECOND_PLAIN)}
BAKED_FIRST = os.path.join("scripts", "first.lmg")
BAKED_SECOND = os.path.join("scripts", "area-second.lmg")


def scripts_section(files=("first.xml", "area/second.xml")):
    return {"entry": files[0], "files": list(files), "language": "en", "stop_at_act": "made_up_stop", "never_taken": [], "timing": "ds"}


def hand_set(section=None):
    description = {"scene_name": "s", "plates": [{"name": "plate", "source": test_end_to_end.PLATE_PATH}], "steps": [["wait", 1]]}
    if section is not None:
        description["scripts"] = section
    return description


def string_offsets(strings):
    offsets = {}
    position = 0
    for text in strings:
        offsets[text] = position
        position = position + len(text) + 1
    return offsets


def script_that_changes_outside_and_inside_an_act(outside, inside, act_name, after_the_act=(), padding_words=0):
    strings = ["*", outside, inside, act_name] + list(after_the_act)
    offsets = string_offsets(strings)
    leading = [SCRIPT_CHANGE_OP, offsets[outside]]
    body = [END_OP] * padding_words + [SCRIPT_CHANGE_OP, offsets[inside]]
    trailing = [word for name in after_the_act for word in (SCRIPT_CHANGE_OP, offsets[name])]
    act_end = synthetic_files.SCRIPT_CODE_START + 2 * (len(leading) + 5 + len(body))
    act = [ACT_OP, GOTO_OP, act_end & 0xFFFF, act_end >> 16, offsets[act_name]]
    return synthetic_files.script_file_bytes(leading + act + body + trailing + [END_OP], strings)


def files_under(folder):
    found = {}
    for root, _, names in os.walk(folder):
        for name in names:
            path = os.path.join(root, name)
            with open(path, "rb") as source:
                found[os.path.relpath(path, folder)] = source.read()
    return found


class ScriptNameTests(unittest.TestCase):
    def test_script_name_flattens_folders_and_drops_xml(self):
        with open(SCRIPT_NAME_PAIRS_PATH, encoding="ascii") as source:
            pairs = re.findall(r'\{"([^"]+)", "([^"]+)"\}', source.read())
        self.assertGreater(len(pairs), 3)
        for path, name in pairs:
            self.assertEqual(script_assets.script_name(path), name)

    def test_a_name_the_asset_format_does_not_allow_is_refused_by_name(self):
        too_long = "x" * (asset_writers.NAME_MAX_LENGTH + 1) + ".xml"
        for path in ("odd.xml/inner.xml", "in ner.xml", too_long):
            with self.subTest(path):
                with self.assertRaisesRegex(script_assets.ScriptBakeError, re.escape(path)):
                    script_assets.baked_path(path)

    def test_the_longest_allowed_name_is_kept(self):
        name = "x" * asset_writers.NAME_MAX_LENGTH
        self.assertEqual(script_assets.baked_path(name + ".xml"), "scripts/" + name + ".lmg")


class ScriptBakeTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, self.folder)
        self.apk_folder = os.path.join(self.folder, "apk")
        self.output_folder = os.path.join(self.folder, "output")
        self.rom_path = os.path.join(self.folder, "game.nds")
        os.makedirs(self.apk_folder)
        synthetic_files.write_apk_folder(self.apk_folder, [(test_end_to_end.PLATE_PATH, test_end_to_end.opaque_etc2_tex(128, 64), True)])
        self.write_rom(ROM_FILES)

    def write_rom(self, files, game_code=synthetic_files.USA_GAME_CODE):
        with open(self.rom_path, "wb") as output:
            output.write(synthetic_files.mini_rom_bytes(files, game_code))

    def convert(self, set_description):
        printed = io.StringIO()
        warnings = io.StringIO()
        with contextlib.redirect_stdout(printed), contextlib.redirect_stderr(warnings):
            convert_assets.convert_set_description(set_description, self.output_folder, self.apk_folder, None, self.rom_path)
        return printed.getvalue(), warnings.getvalue()

    def output_path(self, relative_path):
        return os.path.join(self.output_folder, relative_path)

    def read_output(self, relative_path):
        with open(self.output_path(relative_path), "rb") as source:
            return source.read()

    def record_entries(self):
        with open(self.output_path(bake_record.RECORD_FILE_NAME), encoding="ascii") as source:
            return json.load(source)

    def test_bytes_come_through_unchanged(self):
        self.assertLess(len(ROM_FILES["first.xml.lz"]), len(FIRST_PLAIN))
        self.convert(hand_set(scripts_section()))
        self.assertEqual(self.read_output(BAKED_FIRST), FIRST_PLAIN)
        self.assertEqual(self.read_output(BAKED_SECOND), SECOND_PLAIN)

    def test_baked_names_stay_flat(self):
        self.convert(hand_set(scripts_section()))
        self.assertEqual(sorted(os.listdir(self.output_path("scripts"))), ["area-second.lmg", "first.lmg"])

    def test_summary_counts_scripts_apart_from_textures(self):
        printed, _ = self.convert(hand_set(scripts_section()))
        self.assertIn("textures: 1, 1 baked, 0 unchanged\n", printed)
        self.assertIn("scripts: 2 baked\n", printed)

    def test_a_set_with_no_scripts_section_needs_no_rom_and_prints_no_script_line(self):
        os.remove(self.rom_path)
        printed, warnings = self.convert(hand_set())
        self.assertEqual(warnings, "")
        self.assertNotIn("scripts", printed)
        self.assertEqual(os.listdir(self.output_path("scripts")), [])

    def test_missing_rom_warns_and_hand_set_still_bakes(self):
        os.remove(self.rom_path)
        printed, warnings = self.convert(hand_set(scripts_section()))
        self.assertEqual(warnings.count("\n"), 1)
        self.assertIn("no DS ROM", warnings)
        self.assertNotIn("scripts:", printed)
        self.assertEqual(os.listdir(self.output_path("scripts")), [])
        self.assertTrue(os.path.isfile(self.output_path(os.path.join("textures", "plate.gtex"))))
        self.assertTrue(os.path.isfile(self.output_path(os.path.join("scenes", "s.scene"))))

    def test_rom_absent_keeps_existing_scripts(self):
        self.convert(hand_set(scripts_section()))
        os.remove(self.rom_path)
        self.convert(hand_set(scripts_section()))
        self.assertEqual(self.read_output(BAKED_FIRST), FIRST_PLAIN)
        self.assertEqual(self.read_output(BAKED_SECOND), SECOND_PLAIN)
        self.assertIn("scripts/first.lmg", self.record_entries())
        self.write_rom(ROM_FILES)
        printed, _ = self.convert(hand_set(scripts_section()))
        self.assertIn("scripts: 0 baked\n", printed)

    def test_second_run_leaves_files_untouched(self):
        self.convert(hand_set(scripts_section()))
        os.utime(self.output_path(BAKED_FIRST), (1000, 1000))
        printed, _ = self.convert(hand_set(scripts_section()))
        self.assertEqual(os.path.getmtime(self.output_path(BAKED_FIRST)), 1000)
        self.assertIn("scripts: 0 baked\n", printed)

    def test_changed_rom_bytes_rebake_only_that_script(self):
        self.convert(hand_set(scripts_section()))
        changed = synthetic_files.script_file_bytes([END_OP] * 12, ["*"])
        self.write_rom({"first.xml.lz": synthetic_files.lz11_bytes(changed), "area/second.xml.lz": ROM_FILES["area/second.xml.lz"]})
        printed, _ = self.convert(hand_set(scripts_section()))
        self.assertIn("scripts: 1 baked\n", printed)
        self.assertEqual(self.read_output(BAKED_FIRST), changed)

    def test_an_edited_baked_script_is_baked_again(self):
        self.convert(hand_set(scripts_section()))
        with open(self.output_path(BAKED_FIRST), "wb") as output:
            output.write(b"edited")
        self.convert(hand_set(scripts_section()))
        self.assertEqual(self.read_output(BAKED_FIRST), FIRST_PLAIN)

    def test_changed_lz_code_rebakes_scripts(self):
        code_tree = os.path.join(self.folder, "code_tree")
        assets_folder = os.path.join(code_tree, "srcVita", "tools", "assets")
        shutil.copytree(script_assets.ASSET_TOOLS_FOLDER, assets_folder, ignore=shutil.ignore_patterns("__pycache__"))
        os.makedirs(os.path.join(code_tree, "tools"))
        lz_copy = os.path.join(code_tree, "tools", "lz.py")
        shutil.copyfile(os.path.join(script_assets.ASSET_TOOLS_FOLDER, "../../../tools/lz.py"), lz_copy)
        with mock.patch.object(bake_record, "ASSET_TOOLS_FOLDER", assets_folder):
            self.convert(hand_set(scripts_section()))
            for code_file in (lz_copy, os.path.join(assets_folder, "ds_rom.py"), os.path.join(assets_folder, "script_assets.py")):
                with self.subTest(code_file):
                    unchanged, _ = self.convert(hand_set(scripts_section()))
                    with open(code_file, "a", encoding="utf-8") as output:
                        output.write("\nCHANGED = True\n")
                    changed, _ = self.convert(hand_set(scripts_section()))
                    self.assertIn("scripts: 0 baked\n", unchanged)
                    self.assertIn("scripts: 2 baked\n", changed)

    def test_script_no_longer_listed_is_removed(self):
        self.convert(hand_set(scripts_section()))
        self.convert(hand_set(scripts_section(("first.xml",))))
        self.assertEqual(os.listdir(self.output_path("scripts")), ["first.lmg"])
        self.assertNotIn("scripts/area-second.lmg", self.record_entries())

    def test_clean_rebuild_is_byte_identical(self):
        self.convert(hand_set(scripts_section()))
        first_build = files_under(self.output_folder)
        shutil.rmtree(self.output_folder)
        self.convert(hand_set(scripts_section()))
        self.assertEqual(files_under(self.output_folder), first_build)

    def test_a_script_the_rom_does_not_hold_is_refused_by_name(self):
        with self.assertRaisesRegex(convert_assets.ds_rom.RomRefused, "absent.xml.lz"):
            self.convert(hand_set(scripts_section(("first.xml", "absent.xml"))))

    def test_a_listed_file_that_is_not_compressed_is_refused_by_name(self):
        self.write_rom({"first.xml.lz": FIRST_PLAIN})
        with self.assertRaisesRegex(script_assets.ScriptBakeError, "first.xml"):
            self.convert(hand_set(scripts_section(("first.xml",))))

    def test_a_listed_file_that_is_not_a_script_is_refused_by_name(self):
        self.write_rom({"first.xml.lz": synthetic_files.lz11_bytes(b"not a script at all")})
        with self.assertRaisesRegex(script_assets.ScriptBakeError, "^first.xml: "):
            self.convert(hand_set(scripts_section(("first.xml",))))

    def test_a_refused_script_leaves_no_baked_file_or_record_entry(self):
        self.write_rom({"first.xml.lz": synthetic_files.lz11_bytes(b"not a script at all"), "area/second.xml.lz": ROM_FILES["area/second.xml.lz"]})
        with self.assertRaises(script_assets.ScriptBakeError):
            self.convert(hand_set(scripts_section()))
        self.assertEqual(os.listdir(self.output_path("scripts")), [])
        self.assertEqual([path for path in self.record_entries() if path.startswith("scripts/")], [])

    def test_a_refused_script_change_keeps_the_last_good_bake(self):
        self.convert(hand_set(scripts_section()))
        entries_before = self.record_entries()
        unexplained = script_that_changes_outside_and_inside_an_act("made_up_unknown.xml", "made_up_after.xml", "made_up_stop")
        self.write_rom({"first.xml.lz": synthetic_files.lz11_bytes(unexplained), "area/second.xml.lz": ROM_FILES["area/second.xml.lz"]})
        with self.assertRaises(script_assets.ScriptBakeError):
            self.convert(hand_set(scripts_section()))
        self.assertEqual(self.read_output(BAKED_FIRST), FIRST_PLAIN)
        self.assertEqual(self.record_entries()["scripts/first.lmg"], entries_before["scripts/first.lmg"])

    def test_a_new_script_is_not_stored_when_another_listed_script_is_refused(self):
        self.convert(hand_set(scripts_section(("first.xml",))))
        self.write_rom({"first.xml.lz": synthetic_files.lz11_bytes(b"not a script at all"), "area/second.xml.lz": ROM_FILES["area/second.xml.lz"]})
        with self.assertRaises(script_assets.ScriptBakeError):
            self.convert(hand_set(scripts_section()))
        self.assertEqual(os.listdir(self.output_path("scripts")), ["first.lmg"])


class ScriptChangeWalkTests(unittest.TestCase):
    def section_for(self, never_taken, stop_at_act="made_up_stop"):
        return {"files": ["listed.xml"], "never_taken": never_taken, "stop_at_act": stop_at_act}

    def test_every_0x24_target_is_listed_stopped_or_never_taken(self):
        listed = script_that_changes_outside_and_inside_an_act("listed.xml", "made_up_after.xml", "made_up_stop")
        script_assets.check_script_changes(self.section_for([]), {"listed.xml": listed})
        never = script_that_changes_outside_and_inside_an_act("made_up_never.xml", "made_up_after.xml", "made_up_stop")
        script_assets.check_script_changes(self.section_for(["made_up_never.xml"]), {"listed.xml": never})

    def test_a_target_outside_the_stop_act_with_no_row_is_refused_by_name(self):
        script = script_that_changes_outside_and_inside_an_act("made_up_unknown.xml", "made_up_after.xml", "made_up_stop")
        with self.assertRaisesRegex(script_assets.ScriptBakeError, "listed.xml can change to made_up_unknown.xml"):
            script_assets.check_script_changes(self.section_for([]), {"listed.xml": script})

    def test_a_target_inside_another_act_with_no_row_is_refused_by_name(self):
        script = script_that_changes_outside_and_inside_an_act("listed.xml", "made_up_after.xml", "made_up_other_act")
        with self.assertRaisesRegex(script_assets.ScriptBakeError, "made_up_after.xml"):
            script_assets.check_script_changes(self.section_for([]), {"listed.xml": script})

    def test_a_target_at_the_end_of_the_stop_act_with_no_row_is_refused_by_name(self):
        script = script_that_changes_outside_and_inside_an_act("listed.xml", "made_up_inside.xml", "made_up_stop", ["made_up_at_the_end.xml"])
        with self.assertRaisesRegex(script_assets.ScriptBakeError, "listed.xml can change to made_up_at_the_end.xml"):
            script_assets.check_script_changes(self.section_for([]), {"listed.xml": script})

    def test_a_target_past_the_end_of_the_stop_act_with_no_row_is_refused_by_name(self):
        script = script_that_changes_outside_and_inside_an_act("listed.xml", "made_up_inside.xml", "made_up_stop", ["listed.xml", "made_up_later.xml"])
        with self.assertRaisesRegex(script_assets.ScriptBakeError, "listed.xml can change to made_up_later.xml"):
            script_assets.check_script_changes(self.section_for([]), {"listed.xml": script})

    def test_a_target_inside_a_stop_act_that_ends_past_64_kilobytes_counts_as_stopped(self):
        script = script_that_changes_outside_and_inside_an_act("listed.xml", "made_up_inside.xml", "made_up_stop", padding_words=WORDS_PAST_64_KILOBYTES)
        script_assets.check_script_changes(self.section_for([]), {"listed.xml": script})

    def test_a_script_the_decoder_cannot_finish_is_refused_by_name(self):
        strings = ["*", "made_up_unknown.xml"]
        script = synthetic_files.script_file_bytes([UNKNOWN_OP, SCRIPT_CHANGE_OP, string_offsets(strings)["made_up_unknown.xml"], END_OP], strings)
        with self.assertRaisesRegex(script_assets.ScriptBakeError, "^listed.xml: .*unknown opcode"):
            script_assets.check_script_changes(self.section_for([]), {"listed.xml": script})

    def test_a_set_with_no_stop_act_has_no_stopped_targets(self):
        script = script_that_changes_outside_and_inside_an_act("listed.xml", "made_up_after.xml", "made_up_stop")
        with self.assertRaisesRegex(script_assets.ScriptBakeError, "made_up_after.xml"):
            script_assets.check_script_changes({"files": ["listed.xml"]}, {"listed.xml": script})


class ConverterExitTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, self.folder)
        self.apk_folder = os.path.join(self.folder, "apk")
        self.output_folder = os.path.join(self.folder, "output")
        self.rom_path = os.path.join(self.folder, "game.nds")
        os.makedirs(self.apk_folder)
        synthetic_files.write_apk_folder(self.apk_folder, [(test_end_to_end.PLATE_PATH, test_end_to_end.opaque_etc2_tex(128, 64), True)])

    def write_rom(self, game_code):
        with open(self.rom_path, "wb") as output:
            output.write(synthetic_files.mini_rom_bytes(ROM_FILES, game_code))

    def run_main(self, section=None):
        arguments = ["convert_assets.py", "--set", "tiny", "--output", self.output_folder, "--apk-folder", self.apk_folder, "--ds-rom", self.rom_path]
        printed = io.StringIO()
        errors = io.StringIO()
        real_run = subprocess.run

        def run_all_but_make(command, **keywords):
            return None if command[0] == "make" else real_run(command, **keywords)

        with mock.patch.object(sys, "argv", arguments), mock.patch.object(convert_assets.subprocess, "run", side_effect=run_all_but_make):
            with mock.patch.object(convert_assets, "load_set_description", return_value=hand_set(section or scripts_section())):
                with contextlib.redirect_stdout(printed), contextlib.redirect_stderr(errors):
                    code = convert_assets.main()
        return code, printed.getvalue(), errors.getvalue()

    def test_wrong_game_code_refused_with_plain_message(self):
        self.write_rom(synthetic_files.USA_GAME_CODE)
        code, _, errors = self.run_main()
        self.assertEqual((code, errors), (0, ""))
        with open(os.path.join(self.output_folder, BAKED_FIRST), "rb") as source:
            before = source.read()
        self.write_rom(b"XXXX")
        code, _, errors = self.run_main()
        self.assertEqual(code, 1)
        self.assertRegex(errors, "^error: .*USA")
        self.assertNotIn("Traceback", errors)
        self.assertNotIn("XXXX", errors)
        with open(os.path.join(self.output_folder, BAKED_FIRST), "rb") as source:
            self.assertEqual(source.read(), before)

    def test_another_region_on_a_fresh_output_says_nothing_was_converted(self):
        self.write_rom(b"XXXX")
        code, _, errors = self.run_main()
        self.assertEqual(code, 1)
        self.assertIn("nothing was converted", errors)
        self.assertIn("--ds-rom", errors)
        self.assertEqual(files_under(os.path.join(self.output_folder, "textures")), {})
        self.assertFalse(os.path.exists(os.path.join(self.output_folder, "manifest.txt")))

    def test_a_name_the_format_does_not_allow_stops_with_a_plain_message(self):
        self.write_rom(synthetic_files.USA_GAME_CODE)
        code, _, errors = self.run_main(scripts_section(("odd.xml/inner.xml",)))
        self.assertEqual(code, 1)
        self.assertRegex(errors, "^error: odd.xml/inner.xml: ")
        self.assertNotIn("Traceback", errors)

    def test_a_script_change_nothing_explains_stops_with_a_plain_message(self):
        unexplained = script_that_changes_outside_and_inside_an_act("made_up_unknown.xml", "made_up_after.xml", "made_up_stop")
        with open(self.rom_path, "wb") as output:
            output.write(synthetic_files.mini_rom_bytes({"first.xml.lz": synthetic_files.lz11_bytes(unexplained)}))
        code, _, errors = self.run_main(scripts_section(("first.xml",)))
        self.assertEqual(code, 1)
        self.assertRegex(errors, "^error: first.xml can change to made_up_unknown.xml")


if __name__ == "__main__":
    unittest.main()
