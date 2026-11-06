import contextlib
import ftplib
import io
import itertools
import os
import sys
import tempfile
import unittest
from unittest import mock

import vita_loop

CAPTURE_LINE_OK = "[      1.000] capture name=a result=0 took_us=5"
CAPTURE_LINE_FAILED = "[      1.000] capture name=a result=-4 took_us=5"


class FakeFtp:
    def __init__(self):
        self.files = {}

    def close(self):
        pass

    def mkd(self, path):
        pass

    def size(self, path):
        if path not in self.files:
            raise ftplib.error_perm("550 not found")
        return len(self.files[path])

    def storbinary(self, command, source):
        self.files[command[len("STOR "):]] = source.read()

    def retrbinary(self, command, write_block):
        path = command[len("RETR "):]
        if path not in self.files:
            raise ftplib.error_perm("550 not found")
        write_block(self.files[path])

    def delete(self, path):
        if path not in self.files:
            raise ftplib.error_perm("550 not found")
        del self.files[path]

    def rename(self, old_path, new_path):
        self.files[new_path] = self.files.pop(old_path)


class DeviceFreeTestCase(unittest.TestCase):
    def setUp(self):
        self.ftp = FakeFtp()
        self.folder = tempfile.TemporaryDirectory()
        self.addCleanup(self.folder.cleanup)
        self.open_ftp = self.patch(vita_loop, "open_ftp", return_value=self.ftp)
        self.send_device_command = self.patch(vita_loop, "send_device_command", return_value="ok")
        self.sleep = self.patch(vita_loop.time, "sleep")
        self.patch(vita_loop, "UPLOAD_RECORD_PATH", os.path.join(self.folder.name, "build", "uploaded_assets.json"))
        self.output = io.StringIO()
        redirect = contextlib.redirect_stdout(self.output)
        redirect.__enter__()
        self.addCleanup(redirect.__exit__, None, None, None)

    def patch(self, target, name, *arguments, **keywords):
        patcher = mock.patch.object(target, name, *arguments, **keywords)
        self.addCleanup(patcher.stop)
        return patcher.start()

    def make_time_run_out(self):
        self.patch(vita_loop.time, "time", side_effect=itertools.count(0, 1))


class FirstInstallTests(DeviceFreeTestCase):
    def install_with_reply(self, reply):
        self.patch(vita_loop, "app_is_installed", return_value=False)
        self.patch(vita_loop, "make_folders")
        self.patch(vita_loop, "stage_package")
        self.send_device_command.return_value = reply
        with contextlib.redirect_stdout(io.StringIO()):
            return vita_loop.first_install()

    def test_promote_gets_long_enough_to_answer(self):
        self.assertEqual(self.install_with_reply("Promoted."), 0)
        self.send_device_command.assert_called_once_with("promote ux0:data/ghosttrick/install", vita_loop.SECONDS_FOR_PROMOTE_REPLY)
        self.assertGreater(vita_loop.SECONDS_FOR_PROMOTE_REPLY, 30)

    def test_a_refused_promote_is_a_failure(self):
        self.assertEqual(self.install_with_reply("Error: Could not promote directory."), 1)


class SceneReadyTextTests(unittest.TestCase):
    def test_it_does_not_match_a_longer_scene_name(self):
        line = "scene ready name=check_room7_middle preload_us=1"
        self.assertNotIn(vita_loop.scene_ready_text("check_room7"), line)
        self.assertIn(vita_loop.scene_ready_text("check_room7_middle"), line)


class WaitForTextTests(DeviceFreeTestCase):
    def test_lines_to_skip_makes_it_wait_for_a_new_match(self):
        self.make_time_run_out()
        self.patch(vita_loop, "latest_run_lines", side_effect=[["x one"], ["x one", "x two"]])
        self.assertEqual(vita_loop.wait_for_text("x", 10, 1), "x two")

    def test_without_a_new_match_it_times_out(self):
        self.make_time_run_out()
        self.patch(vita_loop, "latest_run_lines", return_value=["x one"])
        self.assertIsNone(vita_loop.wait_for_text("x", 3, 1))


class CaptureTests(DeviceFreeTestCase):
    def capture_path(self):
        return os.path.join(self.folder.name, "a.bmp")

    def test_a_capture_name_used_twice_waits_for_its_new_acknowledgement(self):
        self.make_time_run_out()
        self.ftp.files[vita_loop.CAPTURE_FOLDER + "/a.bmp"] = b"new image"
        run_lines = self.patch(vita_loop, "latest_run_lines", side_effect=[[CAPTURE_LINE_OK], [CAPTURE_LINE_OK], [CAPTURE_LINE_OK, CAPTURE_LINE_OK]])
        self.assertEqual(vita_loop.request_capture("a", self.folder.name), 0)
        self.assertEqual(run_lines.call_count, 3)
        with open(self.capture_path(), "rb") as source:
            self.assertEqual(source.read(), b"new image")

    def test_a_failed_capture_does_not_pull_the_old_image(self):
        self.make_time_run_out()
        self.ftp.files[vita_loop.CAPTURE_FOLDER + "/a.bmp"] = b"old image"
        self.patch(vita_loop, "latest_run_lines", side_effect=itertools.chain([[]], itertools.repeat([CAPTURE_LINE_FAILED])))
        self.assertEqual(vita_loop.request_capture("a", self.folder.name, 3), 1)
        self.assertFalse(os.path.exists(self.capture_path()))


class CommandFileTests(DeviceFreeTestCase):
    def test_writing_waits_for_the_app_to_read_the_previous_command(self):
        self.ftp.files[vita_loop.COMMANDS_PATH] = b"old\n"
        self.sleep.side_effect = lambda seconds: self.ftp.files.pop(vita_loop.COMMANDS_PATH)
        vita_loop.write_app_commands("new\n")
        self.assertEqual(self.sleep.call_count, 1)
        self.assertEqual(self.ftp.files[vita_loop.COMMANDS_PATH], b"new\n")

    def test_writing_refuses_to_drop_a_command_the_app_never_read(self):
        self.make_time_run_out()
        self.ftp.files[vita_loop.COMMANDS_PATH] = b"old\n"
        with self.assertRaises(RuntimeError):
            vita_loop.write_app_commands("new\n")
        self.assertEqual(self.ftp.files[vita_loop.COMMANDS_PATH], b"old\n")


class UploadAssetsTests(DeviceFreeTestCase):
    def setUp(self):
        super().setUp()
        self.set_folder = os.path.join(self.folder.name, "set")
        for folder_name in ("textures", "animations", "scenes"):
            os.makedirs(os.path.join(self.set_folder, folder_name))
        self.write_file("manifest.txt", b"scene s\n")
        self.write_file("textures/a.gtex", b"texture")
        self.write_file("textures/.DS_Store", b"finder")
        self.write_file("animations/a.anim", b"animation")
        self.write_file("scenes/s.scene", b"scene")
        self.write_file("stray.txt", b"stray")

    def write_file(self, relative_path, data):
        with open(os.path.join(self.set_folder, relative_path), "wb") as output:
            output.write(data)

    def test_only_asset_files_are_listed(self):
        relative_paths = [relative_path for _, relative_path in vita_loop.list_local_files(self.set_folder)]
        self.assertEqual(relative_paths, ["manifest.txt", "textures/a.gtex", "animations/a.anim", "scenes/s.scene"])

    def test_files_the_device_already_has_are_skipped(self):
        self.assertEqual(vita_loop.upload_assets(self.set_folder), 0)
        self.assertIn("uploaded 4 skipped 0", self.output.getvalue())
        self.assertEqual(vita_loop.upload_assets(self.set_folder), 0)
        self.assertIn("uploaded 0 skipped 4", self.output.getvalue())

    def test_a_file_missing_from_the_device_is_uploaded_again(self):
        vita_loop.upload_assets(self.set_folder)
        del self.ftp.files[vita_loop.ASSETS_FOLDER + "/textures/a.gtex"]
        self.assertEqual(vita_loop.upload_assets(self.set_folder), 0)
        self.assertEqual(self.ftp.files[vita_loop.ASSETS_FOLDER + "/textures/a.gtex"], b"texture")

    def test_a_file_of_the_wrong_size_on_the_device_is_uploaded_again(self):
        vita_loop.upload_assets(self.set_folder)
        self.ftp.files[vita_loop.ASSETS_FOLDER + "/textures/a.gtex"] = b"cut"
        vita_loop.upload_assets(self.set_folder)
        self.assertEqual(self.ftp.files[vita_loop.ASSETS_FOLDER + "/textures/a.gtex"], b"texture")

    def test_an_upload_that_fails_is_not_remembered_as_uploaded(self):
        vita_loop.upload_assets(self.set_folder)
        self.write_file("textures/a.gtex", b"changed")
        with mock.patch.object(vita_loop, "upload_verified", side_effect=RuntimeError):
            with self.assertRaises(RuntimeError):
                vita_loop.upload_assets(self.set_folder)
        self.assertNotIn("textures/a.gtex", vita_loop.load_upload_record())

    def test_a_folder_without_a_manifest_fails_before_using_the_device(self):
        empty_folder = os.path.join(self.folder.name, "empty")
        os.makedirs(empty_folder)
        self.assertEqual(vita_loop.upload_assets(empty_folder), 1)
        self.assertIn("no manifest.txt", self.output.getvalue())
        self.open_ftp.assert_not_called()

    def test_an_interrupted_record_write_keeps_the_old_record(self):
        vita_loop.save_upload_record({"a": "1"})

        def write_part_then_fail(record, output, **keywords):
            output.write("{")
            raise OSError

        with mock.patch.object(vita_loop.json, "dump", side_effect=write_part_then_fail):
            with self.assertRaises(OSError):
                vita_loop.save_upload_record({"b": "2"})
        self.assertEqual(vita_loop.load_upload_record(), {"a": "1"})


class ActionTests(DeviceFreeTestCase):
    def run_main(self, *arguments):
        with mock.patch.object(sys, "argv", ["vita_loop.py"] + list(arguments)):
            return vita_loop.main()

    def test_quit_waits_for_the_app_to_tear_down(self):
        self.assertEqual(self.run_main("quit"), 0)
        self.send_device_command.assert_called_once_with("quit " + vita_loop.TITLE_ID)
        self.sleep.assert_called_once_with(vita_loop.SECONDS_FOR_APP_TEARDOWN)

    def test_seconds_reach_launch(self):
        with mock.patch.object(vita_loop, "launch", return_value=0) as launch:
            self.run_main("launch", "--seconds", "5")
        launch.assert_called_once_with(5)

    def test_seconds_reach_capture(self):
        with mock.patch.object(vita_loop, "request_capture", return_value=0) as request_capture:
            self.run_main("capture", "a", "--seconds", "7", "--output", "out")
        request_capture.assert_called_once_with("a", "out", 7)

    def test_app_command_goes_to_the_commands_file(self):
        self.assertEqual(self.run_main("app-command", "hud on"), 0)
        self.assertEqual(self.ftp.files[vita_loop.COMMANDS_PATH], b"hud on\n")
        self.send_device_command.assert_not_called()

    def test_device_command_goes_to_the_command_port(self):
        self.assertEqual(self.run_main("device-command", "status"), 0)
        self.send_device_command.assert_called_once_with("status")
        self.assertEqual(self.ftp.files, {})

    def test_old_action_names_are_refused(self):
        with contextlib.redirect_stderr(io.StringIO()):
            for action in ("install", "deploy", "command", "tell-app"):
                with self.assertRaises(SystemExit):
                    self.run_main(action)

    def test_actions_that_need_an_argument_refuse_to_run_without_it(self):
        with contextlib.redirect_stderr(io.StringIO()):
            for action in ("capture", "pull", "device-command", "app-command", "wait", "upload-assets"):
                with self.assertRaises(SystemExit):
                    self.run_main(action)


if __name__ == "__main__":
    unittest.main()
