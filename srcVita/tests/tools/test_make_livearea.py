import hashlib
import os
import subprocess
import sys
import tempfile
import unittest

SRC_VITA = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SCRIPT = os.path.join(SRC_VITA, "tools", "make_livearea.py")
NAMES = ("icon0.png", "livearea/contents/bg.png", "livearea/contents/startup.png")


def digests(folder):
    return [hashlib.sha256(open(os.path.join(folder, name), "rb").read()).hexdigest() for name in NAMES]


def build(folder, *extra):
    subprocess.run([sys.executable, "-I", SCRIPT, folder] + list(extra), check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    return digests(folder)


class MakeLiveAreaTest(unittest.TestCase):
    def test_missing_assets_give_the_same_images_as_the_monogram_option(self):
        with tempfile.TemporaryDirectory() as plain, tempfile.TemporaryDirectory() as missing, tempfile.TemporaryDirectory() as empty:
            self.assertEqual(build(plain, "--monogram"), build(missing, "--assets-folder", os.path.join(empty, "none")))

    def test_present_assets_give_the_logo_the_same_way_each_time(self):
        assets = os.path.join(SRC_VITA, "..", "extracted", "remaster_apk")
        if not os.path.isdir(assets):
            self.skipTest("no extracted assets")
        with tempfile.TemporaryDirectory() as plain, tempfile.TemporaryDirectory() as first, tempfile.TemporaryDirectory() as second:
            logo = build(first)
            self.assertNotEqual(build(plain, "--monogram"), logo)
            self.assertEqual(build(second), logo)


if __name__ == "__main__":
    unittest.main()
