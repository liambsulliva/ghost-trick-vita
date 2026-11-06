import contextlib
import io
import os
import tempfile
import unittest
from unittest import mock

import mutation_check


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
        for file_name, command in (("audio_check.py", mutation_check.AUDIO_PYTHON_TEST_COMMAND), ("audio_file.py", mutation_check.AUDIO_PYTHON_TEST_COMMAND), ("narration.py", mutation_check.ALL_PYTHON_TEST_COMMAND), ("set.json", mutation_check.ALL_PYTHON_TEST_COMMAND)):
            _, command_succeeds = self.outcome_for_source(file_name, "x = 1\n", True)
            command_succeeds.assert_called_once_with(mock.ANY, command, mutation_check.time_limit_for(command))

    def test_the_whole_tool_test_suite_gets_more_time_than_the_audio_tests(self):
        self.assertGreater(mutation_check.time_limit_for(mutation_check.ALL_PYTHON_TEST_COMMAND), mutation_check.time_limit_for(mutation_check.AUDIO_PYTHON_TEST_COMMAND))


class EntryTests(unittest.TestCase):
    def test_every_entry_changes_something(self):
        for relative_path, old_text, new_text in mutation_check.MUTATIONS + mutation_check.PYTHON_MUTATIONS:
            self.assertNotEqual(old_text, new_text, relative_path)

    def test_python_entries_are_routed_to_the_tool_tests_and_c_entries_are_not(self):
        for relative_path, _, _ in mutation_check.PYTHON_MUTATIONS:
            self.assertTrue(relative_path.startswith("tools/"), relative_path)
        for relative_path, _, _ in mutation_check.MUTATIONS:
            self.assertFalse(relative_path.startswith("tools/"), relative_path)

    @unittest.skipUnless(os.path.isdir(os.path.join(os.path.dirname(mutation_check.SRC_VITA_FOLDER), "tools")), "a mutation run changes the text this reads")
    def test_every_entry_still_finds_its_text(self):
        for relative_path, old_text, _ in mutation_check.MUTATIONS + mutation_check.PYTHON_MUTATIONS:
            with open(os.path.join(mutation_check.SRC_VITA_FOLDER, relative_path), encoding="utf-8") as source:
                self.assertIn(old_text, source.read(), relative_path)


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
