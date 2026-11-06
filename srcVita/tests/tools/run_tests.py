import os
import sys
import unittest

TESTS_FOLDER = os.path.dirname(os.path.abspath(__file__))
SRC_VITA_FOLDER = os.path.dirname(os.path.dirname(TESTS_FOLDER))

sys.path.insert(0, TESTS_FOLDER)
sys.path.insert(0, os.path.join(SRC_VITA_FOLDER, "tools", "assets"))
sys.path.insert(0, os.path.join(SRC_VITA_FOLDER, "tools"))


def main():
    pattern = sys.argv[1] if len(sys.argv) > 1 else "test_*.py"
    suite = unittest.defaultTestLoader.discover(TESTS_FOLDER, pattern=pattern)
    result = unittest.TextTestRunner(verbosity=1).run(suite)
    return 0 if result.wasSuccessful() else 1


if __name__ == "__main__":
    sys.exit(main())
