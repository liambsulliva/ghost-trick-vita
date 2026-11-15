import contextlib
import io
import os
import shutil
import struct
import sys
import tempfile
import unittest
from unittest import mock

import bake_record
import chapter_assets
import convert_assets
import message_cues
import pack_files
import remaster_scripts
import script_assets
import talk_assets
import timeline_file

NO_ACTION = 77
TABLES = {
    "rooms": {"3": {"size": [256, 384], "rows": [[5, 10, 20, 0], [chapter_assets.ROOM_ROW_END, 0, 0, 0]]}},
    "core_infos": [None, [1, 2, NO_ACTION, 4], [1, 2, 9, 4]],
    "no_action_label": NO_ACTION,
    "sound_rows": [[4, 1], None],
    "effect_kinds": [12, chapter_assets.FIRST_CLASS_EFFECT_KIND + 1],
    "class_factories": [100, 200],
    "face_rows": [[0, 0, 0], [7, 0, 1], [9, 0, 0]],
    "symbols": {"iCutin::LYNNE": 40, "iFacePanel::SISSEL": 3, "iEffect::LAMP": 7},
}


class RomTripwireTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, self.folder)
        self.rom = os.path.join(self.folder, "dump.bin")
        self.other_cartridge = os.path.join(self.folder, "copy.NDS")
        self.packs = os.path.join(self.folder, "pack.bin")
        for path in (self.rom, self.other_cartridge, self.packs):
            with open(path, "wb") as output:
                output.write(b"bytes")
        self.addCleanup(convert_assets.allow_rom_reads)

    def test_while_refused_the_named_rom_and_any_cartridge_file_do_not_open(self):
        convert_assets.refuse_rom_reads([self.rom])
        with self.assertRaises(convert_assets.RomReadRefused):
            open(self.rom, "rb").close()
        with self.assertRaises(convert_assets.RomReadRefused):
            open(self.other_cartridge, "rb").close()

    def test_while_refused_the_android_files_still_read(self):
        convert_assets.refuse_rom_reads([self.rom])
        with open(self.packs, "rb") as source:
            self.assertEqual(source.read(), b"bytes")

    def test_once_allowed_the_rom_reads_again(self):
        convert_assets.refuse_rom_reads([self.rom])
        convert_assets.allow_rom_reads()
        with open(self.rom, "rb") as source:
            self.assertEqual(source.read(), b"bytes")

    def run_main(self, convert, apk_only=True):
        errors = io.StringIO()
        arguments = ["convert_assets.py", "--set", "tiny", "--output", os.path.join(self.folder, "out"), "--apk-folder", self.folder, "--ds-rom", self.rom] + (["--apk-only"] if apk_only else [])
        with mock.patch.object(convert_assets.subprocess, "run"), \
                mock.patch.object(convert_assets.narration, "load_text_key", return_value=b"k" * 16), \
                mock.patch.object(convert_assets, "convert_set_description", side_effect=convert), \
                mock.patch.object(convert_assets, "load_set_description", return_value={}), \
                mock.patch.object(sys, "argv", arguments), \
                contextlib.redirect_stderr(errors):
            code = convert_assets.main()
        return code, errors.getvalue()

    def test_an_android_only_build_that_opens_the_rom_stops_plainly(self):
        def opens_the_rom(set_description, output, apk_folder, text_key, rom_path):
            open(self.rom, "rb").close()

        code, errors = self.run_main(opens_the_rom)
        self.assertEqual(code, 1)
        self.assertIn("must read nothing from the DS cartridge", errors)
        self.assertNotIn("Traceback", errors)

    def test_an_android_only_build_is_given_no_rom_and_reads_the_android_files(self):
        given = []

        def reads_the_packs(set_description, output, apk_folder, text_key, rom_path):
            given.append(rom_path)
            with open(self.packs, "rb") as source:
                source.read()

        code, errors = self.run_main(reads_the_packs)
        self.assertEqual((code, errors, given), (0, "", [None]))

    def test_the_combined_build_still_reads_the_rom_it_is_given(self):
        given = []

        def reads_the_rom(set_description, output, apk_folder, text_key, rom_path):
            given.append(rom_path)
            with open(rom_path, "rb") as source:
                source.read()

        code, errors = self.run_main(reads_the_rom, apk_only=False)
        self.assertEqual((code, errors, given), (0, "", [self.rom]))

    def test_the_tripwire_is_lifted_after_an_android_only_build(self):
        self.run_main(lambda *arguments: None)
        with open(self.rom, "rb") as source:
            self.assertEqual(source.read(), b"bytes")


class RemasterTablesTests(unittest.TestCase):
    def setUp(self):
        self.tables = chapter_assets.RemasterTables(TABLES)

    def test_a_room_reads_its_rows_up_to_the_end_row_like_the_cartridge_table(self):
        self.assertEqual(chapter_assets.read_room(self.tables, 3), (256, 384, [(5, 160, 320)]))

    def test_a_room_the_tables_lack_stops_plainly(self):
        with self.assertRaises(chapter_assets.ChapterBakeError):
            self.tables.room(4)

    def test_a_core_is_trickless_when_its_action_is_the_no_action_label(self):
        self.assertEqual([info for info in range(4) if self.tables.is_trickless(info)], [1])

    def test_sounds_and_effects_read_from_their_rows(self):
        self.assertEqual((self.tables.sound_row(0), self.tables.sound_row(1), self.tables.sound_row(5)), ((4, 1), None, None))
        self.assertEqual((self.tables.effect_list_row(0), self.tables.effect_list_row(1)), (12, None))
        self.assertEqual(self.tables.effect_factory(chapter_assets.FIRST_CLASS_EFFECT_KIND + 1), 200)

    def test_face_rows_name_the_texture_list_path_and_keep_the_flip(self):
        with mock.patch.object(chapter_assets.resource_list, "read_list", return_value=[(7, "faces/lynne"), (9, "faces/sissel")]):
            self.assertEqual(self.tables.face_rows(None), {1: ("faces/lynne", 1), 2: ("faces/sissel", 0)})

    def test_cartridge_bytes_still_go_through_the_arm9_reader(self):
        self.assertIsInstance(chapter_assets.tables_of(b"\0" * 8), chapter_assets.Arm9Tables)
        self.assertIs(chapter_assets.tables_of(self.tables), self.tables)


class RemasterTextTests(unittest.TestCase):
    def test_a_cut_in_with_a_fade_is_the_pose_code_and_a_face_panel_is_the_head_code(self):
        symbols = TABLES["symbols"]
        self.assertEqual(talk_assets.layout_of_attributes({"CutInOrFace": "iCutin::LYNNE", "CutInFade": 0}, symbols), {"code": talk_assets.FACE_CODE, "value": 40})
        self.assertEqual(talk_assets.layout_of_attributes({"CutInOrFace": "iCutin::LYNNE", "CutInFade": 1}, symbols), {"code": talk_assets.POSE_CODE, "value": 40})
        self.assertEqual(talk_assets.layout_of_attributes({"CutInOrFace": "iFacePanel::SISSEL"}, symbols), {"code": talk_assets.HEAD_CODE, "value": 3})
        self.assertIsNone(talk_assets.layout_of_attributes({"CutInOrFace": ""}, symbols))

    def test_a_remaster_page_counts_paragraphs_and_keeps_the_last_hold_of_each(self):
        text = 'One<TRCK mes_wait_set time="10"/><TRCK mes_wait_set time="20"/><TRCK paragraph_end/>Two<TRCK mono_type_set type="MONO_TYPE_03"/><TRCK auto_pf time="90"/>'
        self.assertEqual(message_cues.page_line(message_cues.remaster_page(text)), "page 90 3 2 20 0")
        self.assertEqual(message_cues.remaster_cue_text({"m01": [text, "Plain"]}), "message m01\npage 90 3 2 20 0\npage 0 0 1 0\n")

    def test_a_tag_with_spaces_around_its_equals_sign_reads_like_any_other(self):
        self.assertEqual(message_cues.remaster_page('<TRCK auto_pf time= "120" /><TRCK mono_type_set type ="MONO_TYPE_02"/>'), {"limit": 120, "options": 2, "groups": 1, "holds": {}})

    def test_system_lines_come_from_the_languages_file_and_a_missing_one_stops_plainly(self):
        talk = {"talk": {"system_lines": {"sys_add_data": {}}}, "languages": "languages.json"}
        self.assertEqual(chapter_assets.literal_system_line_texts(talk), {"sys_add_data": "New info has been added."})
        with self.assertRaises(chapter_assets.ChapterBakeError):
            chapter_assets.literal_system_line_texts({"talk": {"system_lines": {"sys_unknown": {}}}, "languages": "languages.json"})

    def test_another_language_without_a_rom_takes_its_literal_lines_and_never_opens_one(self):
        localized = {"talk": {"system_lines": {"sys_add_data": {}}, "language_paths": {"m01": "st01/a.xml"}}, "scripts": {"language": "fr"}}
        with mock.patch.object(chapter_assets.ds_rom, "DsRom", side_effect=AssertionError("opened the ROM")):
            chapter_assets.use_language_text(localized, None, "en", {"sys_add_data": "Nouvelles informations"})
        self.assertEqual(localized["talk"]["system_line_texts"], {"sys_add_data": "Nouvelles informations"})


class FakePacks:
    def __init__(self, paths):
        self.entries = {pack_files.path_hash_key(path): None for path in paths}


class RemasterPathsTests(unittest.TestCase):
    def test_message_files_are_found_from_the_stages_and_scenes_of_the_chain(self):
        folder = remaster_scripts.TEXT_FOLDER
        present = [folder + name + ".msg.22" for name in ("st01/st01_0000", "st01/st01_2000", "st01/st01_id", "st06/st06_demo001", "st06/st06_game002", "database/database", "st09/st09_0000")]
        found = remaster_scripts.message_paths(FakePacks(present), ["st01/st01_game000_Expand.xml", "st06/st06_game002_Expand.xml"])
        self.assertEqual(sorted(found), sorted(present[:-1]))

    def test_chara_lists_are_named_after_the_scripts(self):
        self.assertEqual(remaster_scripts.chara_list_names(["st01/st01_game000_Expand.xml", "st01/st01_game000_Expand.xml", "flags.xml"]), ["st01_game000_expand", "flags"])

    def test_a_timeline_with_no_frames_is_the_remaster_s_no_art_placeholder(self):
        root = {"kind": "root", "sheet_index": 0, "record_indices": [], "ds_frame_count": -1}
        with mock.patch.object(timeline_file, "read_objects", return_value=[None, root]):
            self.assertFalse(timeline_file.has_frames(b""))
        with mock.patch.object(timeline_file, "read_objects", return_value=[None, dict(root, record_indices=[1, 2], ds_frame_count=2)]):
            self.assertTrue(timeline_file.has_frames(b""))


class RemasterScriptBakeTests(unittest.TestCase):
    def test_compiled_scripts_and_cues_are_stored_and_kept_when_unchanged(self):
        folder = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, folder)
        os.makedirs(os.path.join(folder, script_assets.SCRIPTS_FOLDER))
        path = "st01/st01_game000_Expand.xml"
        set_description = {"scripts": {"files": [path]}, "remaster_scripts": {"plains": {path: b"LMG1"}, "cue_pages": {path: {"m01": [["file", "entry"]]}}}}
        with mock.patch.object(chapter_assets, "remaster_cue_texts", return_value={path: "message m01\npage 0 0 1 0\n"}):
            record = bake_record.BakeRecord(folder)
            paths = script_assets.bake_remaster_scripts(set_description, None, b"k" * 16, record)
            record.save()
            again = bake_record.BakeRecord(folder)
            script_assets.bake_remaster_scripts(set_description, None, b"k" * 16, again)
        self.assertEqual(paths, [script_assets.baked_path(path), script_assets.SCRIPTS_FOLDER + "/st01-st01_game000_Expand" + script_assets.CUES_EXTENSION])
        self.assertEqual(sum(again.baked_counts.values()), 0)


if __name__ == "__main__":
    unittest.main()
