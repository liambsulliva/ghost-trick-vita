import os
import shutil
import subprocess
import sys
import tempfile
import unittest

import run_tests

PASSING_TEST = """
import os
import unittest


class Passing(unittest.TestCase):
    def test_it(self):
        with open(os.environ["RUN_ORDER_PATH"], "a", encoding="ascii") as output:
            output.write("{name}\\n")
"""
FAILING_TEST = """
import os
import unittest


class Failing(unittest.TestCase):
    def test_it(self):
        with open(os.environ["RUN_ORDER_PATH"], "a", encoding="ascii") as output:
            output.write("{name}\\n")
        self.fail("this one fails")
"""


class RunTestsCommandTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, self.folder)
        self.tests_folder = os.path.join(self.folder, "a", "b", "c")
        os.makedirs(self.tests_folder)
        shutil.copy(run_tests.__file__, self.tests_folder)
        self.run_order_path = os.path.join(self.folder, "order.txt")

    def add_test(self, file_name, body):
        with open(os.path.join(self.tests_folder, file_name), "w", encoding="ascii") as output:
            output.write(body.format(name=file_name))

    def run_command(self, *arguments):
        environment = dict(os.environ, RUN_ORDER_PATH=self.run_order_path)
        result = subprocess.run([sys.executable, "-B", os.path.join(self.tests_folder, "run_tests.py")] + list(arguments), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, env=environment)
        return result.returncode

    def run_order(self):
        if not os.path.exists(self.run_order_path):
            return []
        with open(self.run_order_path, encoding="ascii") as source:
            return source.read().split()

    def test_failfast_stops_at_the_first_failure_and_exits_with_a_failure(self):
        self.add_test("test_a.py", FAILING_TEST)
        self.add_test("test_b.py", PASSING_TEST)
        self.assertEqual(self.run_command("--failfast"), 1)
        self.assertEqual(self.run_order(), ["test_a.py"])

    def test_a_normal_run_keeps_going_after_a_failure(self):
        self.add_test("test_a.py", FAILING_TEST)
        self.add_test("test_b.py", PASSING_TEST)
        self.assertEqual(self.run_command(), 1)
        self.assertEqual(self.run_order(), ["test_a.py", "test_b.py"])

    def test_failfast_still_runs_every_test_when_nothing_fails(self):
        self.add_test("test_a.py", PASSING_TEST)
        self.add_test("test_b.py", PASSING_TEST)
        self.assertEqual(self.run_command("--failfast"), 0)
        self.assertEqual(self.run_order(), ["test_a.py", "test_b.py"])

    def test_a_late_failure_is_still_found_with_failfast(self):
        self.add_test("test_a.py", PASSING_TEST)
        self.add_test("test_b.py", FAILING_TEST)
        self.assertEqual(self.run_command("--failfast"), 1)
        self.assertEqual(self.run_order(), ["test_a.py", "test_b.py"])

    def test_a_test_file_that_cannot_be_imported_fails_under_failfast(self):
        self.add_test("test_a.py", "import a_module_that_does_not_exist\n")
        self.add_test("test_b.py", PASSING_TEST)
        self.assertEqual(self.run_command("--failfast"), 1)
        self.assertEqual(self.run_order(), [])

    def test_files_named_first_run_first_and_the_rest_still_run(self):
        self.add_test("test_a.py", PASSING_TEST)
        self.add_test("test_b.py", PASSING_TEST)
        self.add_test("test_c.py", PASSING_TEST)
        self.assertEqual(self.run_command("--first", "test_c.py,test_b.py"), 0)
        self.assertEqual(self.run_order(), ["test_c.py", "test_b.py", "test_a.py"])

    def test_the_pattern_still_limits_which_files_run_and_an_empty_first_is_fine(self):
        self.add_test("test_a.py", PASSING_TEST)
        self.add_test("test_b.py", PASSING_TEST)
        self.assertEqual(self.run_command("test_b*.py"), 0)
        self.assertEqual(self.run_order(), ["test_b.py"])
        os.remove(self.run_order_path)
        self.assertEqual(self.run_command("test_b*.py", "--first", "test_a.py"), 0)
        self.assertEqual(self.run_order(), ["test_b.py"])
        os.remove(self.run_order_path)
        self.assertEqual(self.run_command("--first", ""), 0)
        self.assertEqual(self.run_order(), ["test_a.py", "test_b.py"])

    def test_a_run_that_finds_no_tests_is_a_failure(self):
        self.add_test("test_a.py", PASSING_TEST)
        self.assertEqual(self.run_command("test_nothing_is_called_this*.py"), 1)
        self.assertEqual(self.run_order(), [])

    def test_a_file_named_first_runs_only_once(self):
        self.add_test("test_a.py", PASSING_TEST)
        self.add_test("test_b.py", PASSING_TEST)
        self.assertEqual(self.run_command("--first", "test_b.py"), 0)
        self.assertEqual(self.run_order(), ["test_b.py", "test_a.py"])


if __name__ == "__main__":
    unittest.main()
