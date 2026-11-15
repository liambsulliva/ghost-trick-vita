import os
import unittest
from unittest import mock

import smoke_device
import smoke_test
import vita_loop
from test_vita_loop import DeviceFreeTestCase

DATA = vita_loop.DATA_FOLDER
LAUNCH = vita_loop.LAUNCH_MARKER


def flow_of(name="pause", install="remastered", lines=("press 10", "capture shot"), relaunches=False):
    flow = smoke_test.Flow(name)
    flow.installs = [install]
    flow.frames_most = 600
    flow.launches = [{"lines": list(lines), "relaunches": relaunches}]
    return flow


class SmokeDeviceTestCase(DeviceFreeTestCase):
    def setUp(self):
        super().setUp()
        self.patch(smoke_device.time, "sleep")
        self.ftp.files[vita_loop.ASSETS_FOLDER + "/manifest.txt"] = b"art remastered\n"
        self.states = os.path.join(self.folder.name, "states")
        os.makedirs(self.states)
        self.target = smoke_device.VitaTarget({"remastered": "unused"}, {"remastered": self.states}, os.path.join(self.folder.name, "out"))

    def app_writes(self, text):
        def launch_app(command):
            if command.startswith("launch "):
                self.ftp.files[vita_loop.LOG_PATH] = (LAUNCH + "\n").encode()
            return "ok"
        self.send_device_command.side_effect = launch_app
        store = self.ftp.storbinary

        def store_and_run(command, source):
            store(command, source)
            if command.endswith("commands.txt.new"):
                self.ftp.files[vita_loop.LOG_PATH] += text.encode()
                self.ftp.files[vita_loop.CAPTURE_FOLDER + "/shot.bmp"] = b"BMshot"
        self.ftp.storbinary = store_and_run


class InstallTests(SmokeDeviceTestCase):
    def test_flows_for_another_install_are_skipped_not_run(self):
        kept, left_out = self.target.flows_it_can_run([flow_of("a"), flow_of("b", "ds"), flow_of("c", "both")])
        self.assertEqual([flow.name for flow in kept], ["a"])
        self.assertEqual([(result.name, result.is_skipped) for result in left_out], [("flow b@ds", True), ("flow c@both", True)])

    def test_both_sets_on_the_card_are_the_both_install(self):
        self.ftp.files[vita_loop.DS_ASSETS_FOLDER + "/manifest.txt"] = b"art ds\n"
        kept, _ = self.target.flows_it_can_run([flow_of("a"), flow_of("c", "both")])
        self.assertEqual([flow.name for flow in kept], ["c"])


class PlayerFileTests(SmokeDeviceTestCase):
    def test_the_players_files_come_back_and_the_flows_files_go(self):
        self.ftp.files[DATA + "/save_auto.txt"] = b"the player's save"
        self.ftp.files[DATA + "/settings.txt"] = b"language 4\n"
        self.target.flows_it_can_run([flow_of()])
        flow = flow_of()
        flow.settings = {"language": "2"}
        with open(os.path.join(self.states, "st01.txt"), "w") as output:
            output.write("script st01\n")
        flow.start_state = "st01"
        self.target.prepare(flow)
        self.assertEqual(self.ftp.files[DATA + "/save_auto.txt"], b"script st01\n")
        self.assertIn(b"language 2\n", self.ftp.files[DATA + "/settings.txt"])
        self.ftp.files[DATA + "/save_manual.txt"] = b"left by a flow"
        self.target.finish()
        self.assertEqual(self.ftp.files[DATA + "/save_auto.txt"], b"the player's save")
        self.assertEqual(self.ftp.files[DATA + "/settings.txt"], b"language 4\n")
        self.assertNotIn(DATA + "/save_manual.txt", self.ftp.files)

    def test_a_flow_starts_from_no_saves(self):
        self.ftp.files[DATA + "/chapter_03.txt"] = b"earlier"
        self.target.flows_it_can_run([flow_of()])
        self.target.prepare(flow_of())
        self.assertNotIn(DATA + "/chapter_03.txt", self.ftp.files)

    def test_nothing_is_touched_when_no_flow_fits_the_install(self):
        self.ftp.files[DATA + "/save_auto.txt"] = b"the player's save"
        self.target.flows_it_can_run([flow_of("b", "ds")])
        self.target.finish()
        self.assertEqual(self.send_device_command.call_count, 0)
        self.assertEqual(self.ftp.files[DATA + "/save_auto.txt"], b"the player's save")


class LaunchTests(SmokeDeviceTestCase):
    def run_flow(self, flow, text):
        self.app_writes(text)
        self.target.flows_it_can_run([flow])
        sandbox = self.target.prepare(flow)
        code, _ = self.target.run_launch(flow, sandbox, flow.launches[0]["lines"])
        return code, sandbox

    def test_the_flow_is_sent_as_one_commands_file_and_its_end_is_read(self):
        code, sandbox = self.run_flow(flow_of(), "[      2.000] smoke ok name=pause frames=40\n")
        self.assertEqual(code, smoke_test.EXIT_FLOW_OVER)
        self.assertEqual(self.ftp.files[vita_loop.COMMANDS_PATH], b"smoke pause\npress 10\ncapture shot\n")
        with open(os.path.join(sandbox, "run.log")) as source:
            self.assertIn("smoke ok name=pause", source.read())
        with open(os.path.join(sandbox, "captures", "shot.bmp"), "rb") as source:
            self.assertEqual(source.read(), b"BMshot")

    def test_a_failed_flow_is_over_too_and_the_log_says_why(self):
        code, sandbox = self.run_flow(flow_of(), "[      2.000] smoke fail name=pause line=2 reason=expect frames=4 text=expect pause\n")
        self.assertEqual(code, smoke_test.EXIT_FLOW_OVER)

    def test_another_flows_end_is_not_this_flows(self):
        self.make_time_run_out()
        code, _ = self.run_flow(flow_of(), "[      2.000] smoke ok name=pause_and_more frames=40\n")
        self.assertEqual(code, smoke_test.EXIT_OUT_OF_FRAMES)

    def test_a_silent_app_runs_out_of_time(self):
        self.make_time_run_out()
        code, _ = self.run_flow(flow_of(), "")
        self.assertEqual(code, smoke_test.EXIT_OUT_OF_FRAMES)

    def test_a_new_launch_marker_is_a_relaunch_and_the_app_is_kept(self):
        code, sandbox = self.run_flow(flow_of(relaunches=True), LAUNCH + "\n")
        self.assertEqual(code, smoke_test.EXIT_RELAUNCH)
        launches_before = len([call for call in self.send_device_command.call_args_list if call.args[0].startswith("launch ")])
        self.ftp.files.pop(vita_loop.COMMANDS_PATH, None)
        self.target.run_launch(flow_of(), sandbox, ["expect title"])
        launches_after = len([call for call in self.send_device_command.call_args_list if call.args[0].startswith("launch ")])
        self.assertEqual(launches_after, launches_before)

    def test_each_launch_adds_its_lines_to_the_flows_log(self):
        code, sandbox = self.run_flow(flow_of(), "[      2.000] smoke ok name=pause frames=40\n")
        self.ftp.files.pop(vita_loop.COMMANDS_PATH, None)
        self.target.run_launch(flow_of(), sandbox, ["expect title"])
        with open(os.path.join(sandbox, "run.log")) as source:
            self.assertEqual(source.read().count("smoke ok name=pause"), 2)

    def test_an_app_that_never_starts_fails_the_launch(self):
        self.make_time_run_out()
        self.target.flows_it_can_run([flow_of()])
        sandbox = self.target.prepare(flow_of())
        code, reason = self.target.run_launch(flow_of(), sandbox, ["wait 1"])
        self.assertEqual((code, reason), (smoke_test.EXIT_OUT_OF_FRAMES, "the app did not start"))


class ActionTests(SmokeDeviceTestCase):
    def test_the_loop_action_runs_the_quick_tier_unless_told(self):
        with mock.patch.object(smoke_device, "run", return_value=0) as run, mock.patch("sys.argv", ["vita_loop.py", "smoke", "--only", "pause*"]):
            self.assertEqual(vita_loop.main(), 0)
        arguments = run.call_args.args[0]
        self.assertEqual((arguments.quick, arguments.only), (True, ["pause*"]))

    def test_failures_give_a_failing_exit(self):
        with mock.patch.object(smoke_device, "run", return_value=2), mock.patch("sys.argv", ["vita_loop.py", "smoke", "--full"]):
            self.assertEqual(vita_loop.main(), 1)


if __name__ == "__main__":
    unittest.main()
