import contextlib
import io
import os
import shutil
import signal
import sys
import tempfile
import time
import unittest
from unittest import mock

import convert_assets
import narration
import synthetic_files

DEFAULT_DS_ROM_PATH = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))), "extracted", "ds_rom", "game.nds")
AN_HOUR_IN_SECONDS = 3600
JOB_COUNT = 20
SLOW_JOB_SECONDS = 0.2


class ArtSetTests(unittest.TestCase):
    def test_the_manifest_declares_each_bundled_art_set(self):
        plates = [{"name": "st01-00"}]
        self.assertEqual(convert_assets.art_sets_of({"plates": plates}), ["remastered"])
        self.assertEqual(convert_assets.art_sets_of({"ds_art": ["st01-00"]}), ["ds"])
        self.assertEqual(convert_assets.art_sets_of({"plates": plates, "ds_art": ["st01-00"]}), ["remastered", "ds"])
        self.assertEqual(convert_assets.manifest_bytes({"plates": plates}, {}).decode("ascii").splitlines()[0], "art remastered")


class TextureNameTests(unittest.TestCase):
    def test_names_that_differ_pass(self):
        convert_assets.check_texture_names_differ(["a", "b", "c"])

    def test_a_repeated_name_fails_and_is_named(self):
        with self.assertRaises(convert_assets.ConversionFailed) as caught:
            convert_assets.check_texture_names_differ(["a", "b", "a"])
        self.assertEqual(caught.exception.failures, [("a", "another texture has this name")])

    def test_names_that_differ_only_by_case_fail_because_the_files_would_clash(self):
        with self.assertRaises(convert_assets.ConversionFailed):
            convert_assets.check_texture_names_differ(["art", "ART"])
        with self.assertRaises(convert_assets.ConversionFailed):
            convert_assets.check_texture_names_differ(["ART", "art"])


class OldWorkFolderTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.folder = temporary.name
        patcher = mock.patch.object(tempfile, "tempdir", self.folder)
        patcher.start()
        self.addCleanup(patcher.stop)

    def make_folder(self, name, age_seconds):
        path = os.path.join(self.folder, name)
        os.makedirs(path)
        with open(os.path.join(path, "font_0.otf"), "wb") as output:
            output.write(b"font")
        stamp = time.time() - age_seconds
        os.utime(path, (stamp, stamp))
        return path

    def test_only_old_folders_of_the_converter_are_removed(self):
        old = self.make_folder(convert_assets.WORK_FOLDER_PREFIX + "old", AN_HOUR_IN_SECONDS + 60)
        new = self.make_folder(convert_assets.WORK_FOLDER_PREFIX + "new", AN_HOUR_IN_SECONDS - 60)
        other = self.make_folder("other_tool_old", AN_HOUR_IN_SECONDS + 60)
        convert_assets.remove_old_work_folders()
        self.assertFalse(os.path.exists(old))
        self.assertTrue(os.path.exists(new))
        self.assertTrue(os.path.exists(other))

    def test_a_file_with_the_same_prefix_is_left_alone(self):
        path = os.path.join(self.folder, convert_assets.WORK_FOLDER_PREFIX + "file")
        with open(path, "wb") as output:
            output.write(b"x")
        stamp = time.time() - AN_HOUR_IN_SECONDS - 60
        os.utime(path, (stamp, stamp))
        convert_assets.remove_old_work_folders()
        self.assertTrue(os.path.exists(path))

    def test_a_folder_that_vanishes_while_it_is_checked_is_not_old(self):
        self.assertFalse(convert_assets.work_folder_is_old(os.path.join(self.folder, "absent"), time.time()))
        with mock.patch.object(convert_assets.os.path, "getmtime", side_effect=OSError):
            path = self.make_folder(convert_assets.WORK_FOLDER_PREFIX + "racing", 0)
            self.assertFalse(convert_assets.work_folder_is_old(path, time.time()))


class WorkFolderDeletionTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.work_folder = os.path.join(temporary.name, "work")
        os.makedirs(self.work_folder)

    def test_a_stop_during_the_deletion_still_deletes_the_folder_and_still_stops(self):
        real_rmtree = shutil.rmtree
        calls = []

        def stop_on_the_first_call(path, ignore_errors=False):
            calls.append(path)
            if len(calls) == 1:
                raise SystemExit(143)
            real_rmtree(path, ignore_errors=ignore_errors)

        with mock.patch.object(convert_assets.shutil, "rmtree", side_effect=stop_on_the_first_call):
            with self.assertRaises(SystemExit):
                convert_assets.delete_work_folder(self.work_folder)
        self.assertEqual(len(calls), 2)
        self.assertFalse(os.path.exists(self.work_folder))

    def test_a_plain_deletion_deletes_the_folder_once(self):
        convert_assets.delete_work_folder(self.work_folder)
        self.assertFalse(os.path.exists(self.work_folder))


class StopSignalTests(unittest.TestCase):
    def setUp(self):
        for stop_signal in convert_assets.STOP_SIGNALS:
            self.addCleanup(signal.signal, stop_signal, signal.getsignal(stop_signal))
        self.addCleanup(setattr, convert_assets, "stop_signal_number", None)

    def test_the_first_stop_ignores_every_later_stop_and_exits_with_the_signal_code(self):
        with self.assertRaises(SystemExit) as caught:
            convert_assets.exit_so_the_work_folder_is_deleted(signal.SIGTERM, None)
        self.assertEqual(caught.exception.code, 128 + signal.SIGTERM)
        for stop_signal in (signal.SIGINT, signal.SIGTERM, signal.SIGHUP):
            self.assertEqual(signal.getsignal(stop_signal), signal.SIG_IGN)

    def run_main_with_a_conversion_that(self, conversion):
        arguments = ["convert_assets.py", "--set", "s", "--output", "o"]
        with mock.patch.object(sys, "argv", arguments):
            with mock.patch.object(convert_assets, "load_set_description", return_value={}):
                with mock.patch.object(convert_assets.subprocess, "run"):
                    with mock.patch.object(convert_assets, "convert_set_description", side_effect=conversion):
                        return convert_assets.main()

    def test_a_stop_python_swallowed_still_sets_the_exit_code(self):
        def stop_inside_a_destructor(*_arguments):
            try:
                convert_assets.exit_so_the_work_folder_is_deleted(signal.SIGTERM, None)
            except SystemExit:
                pass

        self.assertEqual(self.run_main_with_a_conversion_that(stop_inside_a_destructor), 128 + signal.SIGTERM)

    def test_a_run_nothing_stopped_exits_with_zero(self):
        self.assertEqual(self.run_main_with_a_conversion_that(lambda *_arguments: None), 0)

    def test_main_catches_all_three_stop_signals(self):
        self.run_main_with_a_conversion_that(lambda *_arguments: None)
        for stop_signal in (signal.SIGINT, signal.SIGTERM, signal.SIGHUP):
            self.assertEqual(signal.getsignal(stop_signal), convert_assets.exit_so_the_work_folder_is_deleted)


class TextureJobTests(unittest.TestCase):
    def run_jobs(self, job_body):
        jobs = {"job_%d" % index: {"name": "job_%d" % index} for index in range(JOB_COUNT)}
        with mock.patch.object(convert_assets.os, "cpu_count", return_value=1):
            with mock.patch.object(convert_assets, "convert_one_texture", side_effect=job_body):
                with mock.patch.object(convert_assets, "texture_code_fingerprint", return_value="code"):
                    return convert_assets.convert_all_textures(None, jobs, "work", None)

    def test_an_interrupted_run_cancels_the_jobs_still_waiting(self):
        started = []

        def interrupt_first_and_run_the_rest_slowly(packs, work_folder, job, record, code):
            started.append(job["name"])
            if job["name"] == "job_0":
                raise SystemExit(143)
            time.sleep(SLOW_JOB_SECONDS)

        with self.assertRaises(SystemExit):
            self.run_jobs(interrupt_first_and_run_the_rest_slowly)
        self.assertLess(len(started), JOB_COUNT)

    def test_a_failed_job_lets_the_others_finish_and_is_reported_by_name(self):
        started = []

        def fail_first(packs, work_folder, job, record, code):
            started.append(job["name"])
            if job["name"] == "job_0":
                raise ValueError("bad texture")
            return {"name": job["name"]}

        results, failures = self.run_jobs(fail_first)
        self.assertEqual(len(started), JOB_COUNT)
        self.assertEqual(len(results), JOB_COUNT - 1)
        self.assertEqual(failures, [("job_0", "bad texture")])


class TextureToolBuildTests(unittest.TestCase):
    def test_main_brings_the_texture_tool_up_to_date_first(self):
        calls = []
        with mock.patch.object(convert_assets.subprocess, "run", side_effect=lambda command, check: calls.append((command, check))):
            with mock.patch.object(convert_assets, "convert_set_description") as convert:
                with mock.patch.object(convert_assets, "load_set_description", return_value={}):
                    with mock.patch.object(sys, "argv", ["convert_assets.py", "--set", "tiny", "--output", "out"]):
                        self.assertEqual(convert_assets.main(), 0)
        self.assertEqual(calls, [(["make", "-C", convert_assets.ASSET_TOOLS_FOLDER], True)])
        convert.assert_called_once()


class TextKeyTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, self.folder)
        self.calls = []

    def run_main(self, set_description):
        errors = io.StringIO()
        with mock.patch.object(convert_assets.subprocess, "run", side_effect=lambda command, check: self.calls.append("make")):
            with mock.patch.object(convert_assets, "convert_set_description", side_effect=lambda *arguments: self.calls.append(arguments)):
                with mock.patch.object(convert_assets, "load_set_description", return_value=set_description):
                    with mock.patch.object(sys, "argv", ["convert_assets.py", "--set", "tiny", "--output", "out", "--apk-folder", self.folder]):
                        with contextlib.redirect_stderr(errors):
                            code = convert_assets.main()
        return code, errors.getvalue()

    def test_a_set_with_text_and_no_key_file_stops_before_any_other_work(self):
        for set_description in ({"narration": [{}]}, {"bubble_text": [{}]}):
            code, errors = self.run_main(set_description)
            self.assertEqual(code, 1)
            self.assertEqual(self.calls, [])
            self.assertIn("no text key at " + os.path.join(self.folder, "text_key.bin"), errors)
            self.assertIn("extract_text_key.py with your own copy of the Android package", errors)

    def test_a_key_file_of_the_wrong_size_stops_before_any_other_work(self):
        synthetic_files.write_text_key(self.folder, bytes(15))
        code, errors = self.run_main({"narration": [{}]})
        self.assertEqual(code, 1)
        self.assertEqual(self.calls, [])
        self.assertIn("is the wrong size", errors)

    def test_the_key_reaches_the_conversion_after_the_tool_is_built(self):
        synthetic_files.write_text_key(self.folder)
        set_description = {"narration": [{}]}
        code, errors = self.run_main(set_description)
        self.assertEqual(code, 0)
        self.assertEqual(errors, "")
        self.assertEqual(self.calls, ["make", (set_description, "out", self.folder, synthetic_files.FAKE_TEXT_KEY, DEFAULT_DS_ROM_PATH)])

    def test_a_set_without_text_runs_with_no_key_file(self):
        code, errors = self.run_main({})
        self.assertEqual(code, 0)
        self.assertEqual(errors, "")
        self.assertEqual(self.calls, ["make", ({}, "out", self.folder, None, DEFAULT_DS_ROM_PATH)])

    def test_a_key_file_that_cannot_be_read_stops_before_any_other_work(self):
        os.makedirs(os.path.join(self.folder, "text_key.bin"))
        code, errors = self.run_main({"narration": [{}]})
        self.assertEqual(code, 1)
        self.assertEqual(self.calls, [])
        self.assertIn("could not read " + os.path.join(self.folder, "text_key.bin"), errors)
        self.assertNotIn("Traceback", errors)

    def test_a_decoding_failure_during_the_conversion_is_a_plain_message(self):
        synthetic_files.write_text_key(self.folder)
        with mock.patch.object(convert_assets.subprocess, "run"):
            with mock.patch.object(convert_assets, "convert_set_description", side_effect=narration.NarrationError("the plain words")):
                with mock.patch.object(convert_assets, "load_set_description", return_value={"narration": [{}]}):
                    with mock.patch.object(sys, "argv", ["convert_assets.py", "--set", "tiny", "--output", "out", "--apk-folder", self.folder]):
                        errors = io.StringIO()
                        with contextlib.redirect_stderr(errors):
                            self.assertEqual(convert_assets.main(), 1)
        self.assertEqual(errors.getvalue(), "error: the plain words\n")


if __name__ == "__main__":
    unittest.main()
