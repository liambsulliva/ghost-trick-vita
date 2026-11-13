import glob
import os
import unittest

SRC_VITA_FOLDER = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
CHECKED_PATTERNS = ["src/**/*.c", "include/*.h", "tests/**/*.c", "tests/**/*.h", "tests/**/*.py", "tools/**/*.py", "Makefile", "tools/assets/Makefile"]
FORBIDDEN_WORDS = ["/private/tmp/", "/scratchpad/", "/Users/"]


class NoSessionPathTests(unittest.TestCase):
    def test_no_source_reads_a_machine_or_session_folder(self):
        this_file = os.path.abspath(__file__)
        found = []
        for pattern in CHECKED_PATTERNS:
            for path in glob.glob(os.path.join(SRC_VITA_FOLDER, pattern), recursive=True):
                if os.path.abspath(path) == this_file or "__pycache__" in path:
                    continue
                with open(path, encoding="utf-8", errors="replace") as source:
                    text = source.read()
                found.extend("%s: %s" % (os.path.relpath(path, SRC_VITA_FOLDER), word) for word in FORBIDDEN_WORDS if word in text)
        self.assertEqual(found, [])


if __name__ == "__main__":
    unittest.main()
