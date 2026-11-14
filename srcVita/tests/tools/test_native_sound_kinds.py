import glob
import os
import re
import sys
import unittest

SRC_VITA_FOLDER = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..")
sys.path.insert(0, os.path.join(SRC_VITA_FOLDER, "tools", "assets"))

import chapter_assets

SOUND_DEFINE = re.compile(r"^#define\s+(\w*SOUND\w*)\s+(\d+)\s*$", re.MULTILINE)
KINDS_NATIVE_PLAYS_AS_SILENCE = (0, 0x21)


def sound_kinds_the_app_plays():
    kinds = {}
    for path in glob.glob(os.path.join(SRC_VITA_FOLDER, "src", "app", "*.c")):
        with open(path, encoding="utf-8") as source:
            for name, value in SOUND_DEFINE.findall(source.read()):
                kind = int(value)
                if kind not in KINDS_NATIVE_PLAYS_AS_SILENCE and kind < chapter_assets.SOUND_ROW_COUNT:
                    kinds[name] = kind
    return kinds


class NativeSoundKindsTest(unittest.TestCase):
    def test_the_scan_finds_the_soul_sounds(self):
        kinds = sound_kinds_the_app_plays()
        self.assertEqual(kinds.get("SOUL_JUMP_SOUND_KIND"), 16)
        self.assertEqual(kinds.get("CHARACTER_SWITCH_SOUND_KIND"), 31)

    def test_every_sound_kind_the_app_plays_is_baked(self):
        missing = {name: kind for name, kind in sound_kinds_the_app_plays().items() if kind not in chapter_assets.NATIVE_SOUND_KINDS}
        self.assertEqual(missing, {})


if __name__ == "__main__":
    unittest.main()
