import argparse
import glob
import os
import sys
import unittest

TESTS_FOLDER = os.path.dirname(os.path.abspath(__file__))
SRC_VITA_FOLDER = os.path.dirname(os.path.dirname(TESTS_FOLDER))

sys.path.insert(0, TESTS_FOLDER)
sys.path.insert(0, os.path.join(SRC_VITA_FOLDER, "tools", "assets"))
sys.path.insert(0, os.path.join(SRC_VITA_FOLDER, "tools"))


def test_files_in_running_order(tests_folder, pattern, files_to_run_first):
    matching = sorted(os.path.basename(path) for path in glob.glob(os.path.join(tests_folder, pattern)))
    first = [name for name in files_to_run_first if name in matching]
    return first + [name for name in matching if name not in first]


def run_suite(tests_folder, pattern, failfast, files_to_run_first):
    suite = unittest.TestSuite()
    for file_name in test_files_in_running_order(tests_folder, pattern, files_to_run_first):
        suite.addTests(unittest.defaultTestLoader.discover(tests_folder, pattern=file_name))
    return unittest.TextTestRunner(verbosity=1, failfast=failfast).run(suite)


def main():
    parser = argparse.ArgumentParser(description="Runs the tool tests. A mutation run stops at the first failure, because one failure already proves the mutation was caught.")
    parser.add_argument("pattern", nargs="?", default="test_*.py")
    parser.add_argument("--failfast", action="store_true", help="stop at the first failure")
    parser.add_argument("--first", default="", help="comma-separated test files to run before the others")
    arguments = parser.parse_args()
    result = run_suite(TESTS_FOLDER, arguments.pattern, arguments.failfast, arguments.first.split(","))
    return 0 if result.wasSuccessful() and result.testsRun > 0 else 1


if __name__ == "__main__":
    sys.exit(main())
