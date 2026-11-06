import contextlib
import io
import os
import subprocess
import tempfile
import unittest
from unittest import mock

import mutation_check

SRC_VITA_FOLDER = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


class MutationOutcomeTests(unittest.TestCase):
    def outcome_when_make_gives(self, results):
        with mock.patch.object(mutation_check, "make_succeeds", side_effect=results):
            return mutation_check.mutation_outcome("folder")

    def test_a_mutation_that_does_not_build_is_invalid_not_caught(self):
        self.assertEqual(self.outcome_when_make_gives([False]), "invalid")

    def test_a_mutation_that_fails_the_tests_is_caught(self):
        self.assertEqual(self.outcome_when_make_gives([True, False]), "caught")

    def test_a_mutation_that_passes_the_tests_survives(self):
        self.assertEqual(self.outcome_when_make_gives([True, True]), "survived")


class PythonMutationOutcomeTests(unittest.TestCase):
    def outcome_for_source(self, file_name, source_text, tests_pass):
        with tempfile.TemporaryDirectory() as folder:
            path = os.path.join(folder, file_name)
            with open(path, "w", encoding="utf-8") as output:
                output.write(source_text)
            with mock.patch.object(mutation_check, "command_succeeds", return_value=tests_pass) as command_succeeds:
                return mutation_check.python_mutation_outcome(folder, path), command_succeeds

    def test_a_mutation_that_is_not_python_is_invalid_and_runs_nothing(self):
        outcome, command_succeeds = self.outcome_for_source("tool.py", "def broken(:\n", False)
        self.assertEqual(outcome, "invalid")
        command_succeeds.assert_not_called()

    def test_a_mutation_that_fails_the_tests_is_caught(self):
        self.assertEqual(self.outcome_for_source("tool.py", "x = 1\n", False)[0], "caught")

    def test_a_mutation_that_passes_the_tests_survives(self):
        self.assertEqual(self.outcome_for_source("tool.py", "x = 1\n", True)[0], "survived")

    def test_a_data_file_is_not_compiled_as_python(self):
        self.assertEqual(self.outcome_for_source("set.json", "{\"not\": \"python (\"}\n", False)[0], "caught")

    def test_audio_tools_run_only_the_audio_tests_and_the_others_run_them_all(self):
        for file_name, command in (("audio_check.py", mutation_check.AUDIO_PYTHON_TEST_COMMAND), ("audio_file.py", mutation_check.AUDIO_PYTHON_TEST_COMMAND), ("narration.py", mutation_check.ALL_PYTHON_TEST_COMMAND + ["--first", "test_narration.py,test_convert_assets.py,test_end_to_end.py"]), ("set.json", mutation_check.ALL_PYTHON_TEST_COMMAND + ["--first", ""])):
            _, command_succeeds = self.outcome_for_source(file_name, "x = 1\n", True)
            command_succeeds.assert_called_once_with(mock.ANY, command, mutation_check.time_limit_for(command))

    def test_a_mutation_run_stops_at_the_first_failure_and_a_normal_test_run_does_not(self):
        self.assertIn("--failfast", mutation_check.ALL_PYTHON_TEST_COMMAND)
        self.assertIn("--failfast", mutation_check.AUDIO_PYTHON_TEST_COMMAND)
        self.assertIn("--failfast", mutation_check.LONGPLAY_TEST_COMMAND)
        with open(os.path.join(mutation_check.SRC_VITA_FOLDER, "tools", "assets", "Makefile"), encoding="utf-8") as source:
            self.assertNotIn("--failfast", source.read())

    def test_running_the_likely_tests_first_only_changes_the_order_so_no_test_is_left_out(self):
        command = mutation_check.python_test_command_for(os.path.join("tools", "assets", "narration.py"))
        self.assertEqual(command[:len(mutation_check.ALL_PYTHON_TEST_COMMAND)], mutation_check.ALL_PYTHON_TEST_COMMAND)
        self.assertEqual(len(command), len(mutation_check.ALL_PYTHON_TEST_COMMAND) + 2)
        self.assertEqual(command[-2], "--first")

    def test_every_test_file_named_to_run_first_exists_and_so_does_its_tool(self):
        tests_folder = os.path.join(mutation_check.SRC_VITA_FOLDER, "tests", "tools")
        mutated_files = set(os.path.basename(relative_path) for relative_path, _, _ in mutation_check.PYTHON_MUTATIONS)
        for tool_name, test_files in mutation_check.TEST_FILES_MOST_LIKELY_TO_CATCH.items():
            self.assertIn(tool_name, mutated_files)
            for test_file in test_files:
                self.assertTrue(os.path.exists(os.path.join(tests_folder, test_file)), test_file)

    def test_a_tool_with_nothing_named_first_still_runs_every_test(self):
        self.assertEqual(mutation_check.python_test_command_for(os.path.join("tools", "other.py")), mutation_check.ALL_PYTHON_TEST_COMMAND + ["--first", ""])

    def test_the_whole_tool_test_suite_gets_more_time_than_the_audio_tests(self):
        self.assertGreater(mutation_check.time_limit_for(mutation_check.ALL_PYTHON_TEST_COMMAND), mutation_check.time_limit_for(mutation_check.AUDIO_PYTHON_TEST_COMMAND))
        self.assertEqual(mutation_check.time_limit_for(mutation_check.python_test_command_for("narration.py")), mutation_check.ALL_PYTHON_TEST_TIME_LIMIT_SECONDS)

    def test_the_longplay_tests_keep_the_short_time_limit(self):
        self.assertEqual(mutation_check.time_limit_for(mutation_check.python_test_command_for("/x/tools/longplay_check.py")), mutation_check.TEST_TIME_LIMIT_SECONDS)


class EntryTests(unittest.TestCase):
    def test_every_entry_changes_something(self):
        for relative_path, old_text, new_text in mutation_check.MUTATIONS + mutation_check.PYTHON_MUTATIONS:
            self.assertNotEqual(old_text, new_text, relative_path)

    def test_python_entries_are_routed_to_the_tool_tests_and_c_entries_are_not(self):
        for relative_path, _, _ in mutation_check.PYTHON_MUTATIONS:
            self.assertTrue(relative_path.startswith(("tools/", "tests/tools/")), relative_path)
        for relative_path, _, _ in mutation_check.MUTATIONS:
            self.assertFalse(relative_path.startswith("tools/"), relative_path)

    @unittest.skipUnless(os.path.isdir(os.path.join(os.path.dirname(mutation_check.SRC_VITA_FOLDER), "tools")), "a mutation run changes the text this reads")
    def test_an_entry_for_this_tool_changes_its_code_and_not_the_entry_itself(self):
        with open(os.path.join(mutation_check.SRC_VITA_FOLDER, "tools", "mutation_check.py"), encoding="utf-8") as source:
            text = source.read()
        entries_start = text.index("PYTHON_MUTATIONS += [")
        entries = [entry for entry in mutation_check.PYTHON_MUTATIONS if entry[0] == "tools/mutation_check.py"]
        self.assertGreater(len(entries), 0)
        for _, old_text, _ in entries:
            self.assertLess(text.index(old_text), entries_start, old_text)

    @unittest.skipUnless(os.path.isdir(os.path.join(os.path.dirname(mutation_check.SRC_VITA_FOLDER), "tools")), "a mutation run changes the text this reads")
    def test_every_entry_still_finds_its_text(self):
        for relative_path, old_text, _ in mutation_check.MUTATIONS + mutation_check.PYTHON_MUTATIONS:
            with open(os.path.join(mutation_check.SRC_VITA_FOLDER, relative_path), encoding="utf-8") as source:
                self.assertIn(old_text, source.read(), relative_path)


class PythonTestChoiceTests(unittest.TestCase):
    def test_a_longplay_mutation_runs_the_longplay_tests(self):
        self.assertEqual(mutation_check.python_test_command_for("/x/tools/longplay_check.py"), mutation_check.LONGPLAY_TEST_COMMAND)

    def test_an_audio_tool_runs_the_audio_tests(self):
        self.assertEqual(mutation_check.python_test_command_for("/x/tools/audio_check.py"), mutation_check.AUDIO_PYTHON_TEST_COMMAND)

    def test_the_audio_and_longplay_routes_are_not_given_a_running_order(self):
        for file_name in ("audio_check.py", "audio_file.py", "longplay_check.py"):
            self.assertNotIn("--first", mutation_check.python_test_command_for(os.path.join("tools", file_name)))


class ReverseOrderCommandTests(unittest.TestCase):
    def test_it_names_every_test_file_backwards_and_nothing_else(self):
        with tempfile.TemporaryDirectory() as folder:
            tests_folder = os.path.join(folder, "tests", "tools")
            os.makedirs(tests_folder)
            for file_name in ("test_a.py", "test_b.py", "test_c.py", "run_tests.py"):
                with open(os.path.join(tests_folder, file_name), "w", encoding="utf-8"):
                    pass
            self.assertEqual(mutation_check.python_tests_in_reverse_order_command(folder), mutation_check.ALL_PYTHON_TEST_COMMAND + ["--first", "test_c.py,test_b.py,test_a.py"])


class PythonBaselineTests(unittest.TestCase):
    def main_when_this_command_fails(self, failing_command):
        run_mutations = mock.Mock()
        with mock.patch.object(mutation_check.shutil, "copytree"), \
                mock.patch.object(mutation_check, "remove_werror_from_makefile"), \
                mock.patch.object(mutation_check, "make_succeeds", return_value=True), \
                mock.patch.object(mutation_check, "command_succeeds", side_effect=lambda folder, command, time_limit=None: command != failing_command), \
                mock.patch.object(mutation_check, "run_mutations", run_mutations), \
                contextlib.redirect_stdout(io.StringIO()) as output:
            result = mutation_check.main()
        return result, output.getvalue(), run_mutations

    def test_broken_tool_tests_stop_the_run_before_any_python_mutation(self):
        result, output, run_mutations = self.main_when_this_command_fails(mutation_check.ALL_PYTHON_TEST_COMMAND)
        self.assertEqual(result, 1)
        self.assertIn("baseline python tests fail", output)
        self.assertEqual([call[0][1] for call in run_mutations.call_args_list], [mutation_check.MUTATIONS])

    def test_tests_that_pass_in_only_one_order_stop_the_run_before_any_python_mutation(self):
        result, output, run_mutations = self.main_when_this_command_fails(mutation_check.ALL_PYTHON_TEST_COMMAND + ["--first", ""])
        self.assertEqual(result, 1)
        self.assertIn("baseline python tests fail", output)
        self.assertEqual([call[0][1] for call in run_mutations.call_args_list], [mutation_check.MUTATIONS])

    def test_a_passing_baseline_runs_the_python_mutations(self):
        result, _, run_mutations = self.main_when_this_command_fails(None)
        self.assertEqual(result, 0)
        self.assertEqual([call[0][1] for call in run_mutations.call_args_list], [mutation_check.MUTATIONS, mutation_check.PYTHON_MUTATIONS])


class MakefileHeaderTests(unittest.TestCase):
    def test_every_fake_header_makes_the_tests_rebuild(self):
        with tempfile.TemporaryDirectory() as folder:
            printer_path = os.path.join(folder, "print.mk")
            with open(printer_path, "w", encoding="utf-8") as output:
                output.write("print-test-headers:\n\t@echo $(TEST_HEADERS)\n")
            result = subprocess.run(["make", "-s", "-f", "Makefile", "-f", printer_path, "print-test-headers"], cwd=SRC_VITA_FOLDER, check=True, stdout=subprocess.PIPE, universal_newlines=True)
        listed = result.stdout.split()
        fake_headers = []
        for folder, _, names in os.walk(os.path.join(SRC_VITA_FOLDER, "tests", "fakes")):
            for name in names:
                if name.endswith(".h"):
                    fake_headers.append(os.path.relpath(os.path.join(folder, name), SRC_VITA_FOLDER))
        self.assertIn("tests/fakes/psp2/kernel/processmgr.h", fake_headers)
        self.assertEqual([header for header in fake_headers if header not in listed], [])


class SingleFileRebuildTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.folder = temporary.name
        self.source_path = os.path.join(self.folder, "src", "app", "thing.c")
        self.object_path = os.path.join(self.folder, "build", "test_objects", "app", "thing.o")
        os.makedirs(os.path.dirname(self.source_path))
        os.makedirs(os.path.dirname(self.object_path))
        with open(self.source_path, "w", encoding="utf-8") as output:
            output.write("int a = 1;\n")
        self.write_object()

    def write_object(self):
        with open(self.object_path, "w", encoding="utf-8") as output:
            output.write("stale")

    def run_one_mutation(self, outcome_of):
        with contextlib.redirect_stdout(io.StringIO()):
            mutation_check.run_mutations(self.folder, [("src/app/thing.c", "a = 1", "a = 2")], outcome_of, [], [], lambda relative_path: mutation_check.forget_test_object_of(self.folder, relative_path))

    def test_the_object_is_gone_before_the_mutated_source_is_built(self):
        seen = []

        def outcome_of(path):
            seen.append(os.path.exists(self.object_path))
            self.write_object()
            return "caught"

        self.run_one_mutation(outcome_of)
        self.assertEqual(seen, [False])

    def test_the_test_binaries_are_gone_before_the_mutated_source_is_built(self):
        binary_path = os.path.join(self.folder, "build", "tests", "unit", "test_thing")
        seen = []

        def outcome_of(path):
            seen.append(os.path.exists(binary_path))
            return "caught"

        os.makedirs(os.path.dirname(binary_path))
        with open(binary_path, "w", encoding="utf-8") as output:
            output.write("linked a second ago")
        self.run_one_mutation(outcome_of)
        self.assertEqual(seen, [False])

    def test_the_mutated_object_is_gone_once_the_source_is_restored(self):
        def outcome_of(path):
            self.write_object()
            return "caught"

        self.run_one_mutation(outcome_of)
        self.assertFalse(os.path.exists(self.object_path))
        with open(self.source_path, encoding="utf-8") as source:
            self.assertEqual(source.read(), "int a = 1;\n")

    def write_stale_file(self, *path_parts):
        path = os.path.join(self.folder, "build", *path_parts)
        os.makedirs(os.path.dirname(path), exist_ok=True)
        with open(path, "w", encoding="utf-8") as output:
            output.write("stale")
        return path

    def test_a_test_that_includes_the_source_is_forgotten_only_with_that_source(self):
        test_object_path = self.write_stale_file("test_objects", "tests", "integration", "test_main_loop.o")
        mutation_check.forget_test_object_of(self.folder, "src/app/thing.c")
        self.assertTrue(os.path.exists(test_object_path))
        mutation_check.forget_test_object_of(self.folder, "src/main.c")
        self.assertFalse(os.path.exists(test_object_path))

    def test_every_test_binary_is_forgotten_because_make_skips_a_relink_within_the_same_second(self):
        binary_paths = [self.write_stale_file("tests", "unit", "test_a"), self.write_stale_file("tests", "integration", "test_b")]
        mutation_check.forget_test_object_of(self.folder, "src/app/thing.c")
        self.assertEqual([os.path.exists(path) for path in binary_paths], [False, False])

    def test_a_missing_object_is_not_an_error(self):
        os.remove(self.object_path)
        mutation_check.forget_test_object_of(self.folder, "src/app/thing.c")
        self.assertFalse(os.path.exists(self.object_path))


class PrivateTempFolderTests(unittest.TestCase):
    def test_the_tests_of_a_mutation_run_cannot_reach_the_real_temp_folder(self):
        with tempfile.TemporaryDirectory() as temporary_root:
            with mock.patch.dict(os.environ):
                mutation_check.use_private_temp_folder(temporary_root)
                self.assertEqual(os.path.dirname(os.environ["TMPDIR"]), temporary_root)
                self.assertTrue(os.path.isdir(os.environ["TMPDIR"]))

    def test_the_run_turns_the_private_folder_on_before_it_copies_or_tests_anything(self):
        calls = []
        with mock.patch.object(mutation_check, "use_private_temp_folder", side_effect=lambda root: calls.append("private")):
            with mock.patch.object(mutation_check.shutil, "copytree", side_effect=lambda *arguments, **keywords: calls.append("copy") or (_ for _ in ()).throw(KeyError)):
                with self.assertRaises(KeyError):
                    mutation_check.main()
        self.assertEqual(calls, ["private", "copy"])


class MakefileCopyTests(unittest.TestCase):
    def test_the_copy_of_the_makefile_loses_werror(self):
        with tempfile.TemporaryDirectory() as folder:
            path = os.path.join(folder, "Makefile")
            with open(path, "w", encoding="utf-8") as output:
                output.write("FLAGS := -Wall -Werror -g\n")
            mutation_check.remove_werror_from_makefile(folder)
            with open(path, encoding="utf-8") as source:
                self.assertEqual(source.read(), "FLAGS := -Wall -g\n")


if __name__ == "__main__":
    unittest.main()
