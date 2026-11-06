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
    def outcome_for_source(self, source_text, tests_pass):
        with tempfile.TemporaryDirectory() as folder:
            path = os.path.join(folder, "tool.py")
            with open(path, "w", encoding="utf-8") as output:
                output.write(source_text)
            with mock.patch.object(mutation_check, "command_succeeds", return_value=tests_pass):
                return mutation_check.python_mutation_outcome(folder, path)

    def test_a_mutation_that_is_not_python_is_invalid_not_caught(self):
        self.assertEqual(self.outcome_for_source("def broken(:\n", False), "invalid")

    def test_a_mutation_that_fails_the_tests_is_caught(self):
        self.assertEqual(self.outcome_for_source("x = 1\n", False), "caught")

    def test_a_mutation_that_passes_the_tests_survives(self):
        self.assertEqual(self.outcome_for_source("x = 1\n", True), "survived")


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
