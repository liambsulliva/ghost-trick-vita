import contextlib
import importlib
import io
import os
import sys
import tempfile
import unittest
from unittest import mock

TESTS_FOLDER = os.path.dirname(os.path.abspath(__file__))
REPOSITORY_TOOLS_FOLDER = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(TESTS_FOLDER))), "tools")
TRACE_TOOL_PATH = os.path.join(REPOSITORY_TOOLS_FOLDER, "trace_vm.py")
NO_STUB = (None, None, None)


@unittest.skipUnless(os.path.isfile(TRACE_TOOL_PATH), "mutation_check does not copy this tool")
class TraceToolStartTests(unittest.TestCase):
    def setUp(self):
        with mock.patch.object(sys, "path", [REPOSITORY_TOOLS_FOLDER] + sys.path):
            self.trace_vm = importlib.import_module("trace_vm")
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.folder = temporary.name
        self.plain_path = os.path.join(self.folder, "plain.dec")

    def run_tool(self, out_dir):
        with mock.patch.object(self.trace_vm, "GAME_SCRIPT_PLAIN", self.plain_path), mock.patch.object(self.trace_vm.melonds, "start", return_value=NO_STUB) as start, mock.patch.object(sys, "argv", ["trace_vm.py", out_dir, "100"]), contextlib.redirect_stdout(io.StringIO()):
            try:
                self.trace_vm.main()
            except FileNotFoundError:
                return start, "missing file"
            except SystemExit:
                return start, "no stub"
        return start, "finished"

    def test_a_missing_plain_script_stops_before_melonds_starts(self):
        start, outcome = self.run_tool(os.path.join(self.folder, "out"))
        self.assertEqual(outcome, "missing file")
        start.assert_not_called()
        self.assertFalse(os.path.exists(os.path.join(self.folder, "out")))

    def test_a_relative_output_folder_reaches_melonds_as_a_full_path(self):
        with open(self.plain_path, "wb") as output:
            output.write(bytes(range(12)))
        out_dir = os.path.join(self.folder, "out")
        start, outcome = self.run_tool(os.path.relpath(out_dir))
        self.assertEqual(outcome, "no stub")
        log_path = start.call_args[0][1]
        self.assertTrue(os.path.isabs(log_path), log_path)
        self.assertEqual(os.path.dirname(os.path.realpath(log_path)), os.path.realpath(out_dir))
        self.assertTrue(os.path.isdir(out_dir))


if __name__ == "__main__":
    unittest.main()
