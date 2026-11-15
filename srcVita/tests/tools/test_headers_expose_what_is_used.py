import glob
import os
import re
import unittest

SRC_VITA_FOLDER = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
DECLARATION = re.compile(r"^[A-Za-z_][\w \*]*?\b(\w+)\(", re.M)
THIRD_PARTY_HEADERS = ["stb_vorbis.h"]
WORD = re.compile(r"[A-Za-z_]\w*")


def read_all(pattern):
    texts = {}
    for path in glob.glob(os.path.join(SRC_VITA_FOLDER, pattern), recursive=True):
        with open(path, encoding="utf-8", errors="replace") as source:
            texts[os.path.relpath(path, SRC_VITA_FOLDER)] = source.read()
    return texts


def defines(text, name):
    return re.search(r"^[A-Za-z_][\w \*]*?\b" + name + r"\([^;]*?\)\s*\{", text, re.M) is not None


def words_by_path(texts):
    return {path: set(WORD.findall(text)) for path, text in texts.items()}


def declared_but_used_by_one_file(headers, sources, tests):
    source_words = words_by_path(sources)
    other_words = words_by_path(tests)
    header_words = words_by_path(headers)
    found = []
    for header_path, header_text in sorted(headers.items()):
        if os.path.basename(header_path) in THIRD_PARTY_HEADERS:
            continue
        for name in DECLARATION.findall(header_text):
            is_used = any(name in words for words in other_words.values())
            is_used = is_used or any(name in words for path, words in header_words.items() if path != header_path)
            is_used = is_used or any(name in words and not defines(sources[path], name) for path, words in source_words.items())
            if not is_used:
                found.append("%s: %s" % (header_path, name))
    return found


class HeadersExposeWhatIsUsedTests(unittest.TestCase):
    def test_a_function_only_its_own_file_calls_is_found(self):
        headers = {"include/a.h": "int a_used(void);\nint a_private(void);\n"}
        sources = {"src/app/a.c": "int a_private(void)\n{\n    return 1;\n}\nint a_used(void)\n{\n    return a_private();\n}\n", "src/app/b.c": "int b(void)\n{\n    return a_used();\n}\n"}
        self.assertEqual(declared_but_used_by_one_file(headers, sources, {}), ["include/a.h: a_private"])

    def test_a_function_only_a_test_calls_is_kept(self):
        headers = {"include/a.h": "int a_seen(void);\n"}
        sources = {"src/app/a.c": "int a_seen(void)\n{\n    return 1;\n}\n"}
        self.assertEqual(declared_but_used_by_one_file(headers, sources, {"tests/unit/test_a.c": "a_seen();"}), [])

    def test_every_declared_function_has_a_caller_outside_its_file(self):
        sources = read_all("src/**/*.c")
        tests = {path: text for path, text in read_all("tests/**/*.c").items() if "/fakes/" not in path}
        self.assertEqual(declared_but_used_by_one_file(read_all("include/*.h"), sources, tests), [])


if __name__ == "__main__":
    unittest.main()
