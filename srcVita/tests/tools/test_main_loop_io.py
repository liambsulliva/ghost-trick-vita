import os
import re
import unittest

MAIN_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "src", "main.c")
CARD_CALLS = ("file_read_whole", "file_write_whole", "file_append", "file_delete", "file_list_folder", "sceIoOpen", "sceIoDopen")
FRAME_FUNCTIONS = ("run_main_loop", "run_pending_commands", "run_one_command", "publish_frame_stats")


def body_of(source, name):
    match = re.search(r"^static [^\n]*\b" + name + r"\([^)]*\)\n\{\n", source, re.MULTILINE)
    if match is None:
        return None
    depth = 1
    at = match.end()
    while depth > 0:
        depth = depth + {"{": 1, "}": -1}.get(source[at], 0)
        at = at + 1
    return source[match.end():at]


class MainLoopCardTests(unittest.TestCase):
    def test_the_frame_loop_never_touches_the_card_itself(self):
        with open(MAIN_PATH, encoding="utf-8") as source:
            text = source.read()
        for name in FRAME_FUNCTIONS:
            body = body_of(text, name)
            self.assertIsNotNone(body, name)
            for call in CARD_CALLS:
                self.assertNotIn(call + "(", body, "%s calls %s on the frame" % (name, call))


if __name__ == "__main__":
    unittest.main()
