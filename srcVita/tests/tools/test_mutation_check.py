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
