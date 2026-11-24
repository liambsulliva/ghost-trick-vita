import hashlib
import os
import struct
import subprocess
import sys
import tempfile
import unittest

SRC_VITA = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SCRIPT = os.path.join(SRC_VITA, "tools", "make_livearea.py")
RELEASE_ART = os.path.join(SRC_VITA, "tools", "livearea", "release")
NAMES = ("icon0.png", "livearea/contents/bg.png", "livearea/contents/startup.png")
SIZES = {"icon0.png": (128, 128), "livearea/contents/startup.png": (280, 158)}


def digests(folder):
    return [hashlib.sha256(open(os.path.join(folder, name), "rb").read()).hexdigest() for name in NAMES]


def build(folder, *extra):
    subprocess.run([sys.executable, "-I", SCRIPT, folder] + list(extra), check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    return digests(folder)


class MakeLiveAreaTest(unittest.TestCase):
    def test_the_monogram_icon_and_banner_are_the_original_release_art_at_the_right_sizes(self):
        with tempfile.TemporaryDirectory() as plain:
            build(plain, "--monogram")
            for name, size in SIZES.items():
                with open(os.path.join(plain, name), "rb") as made:
                    data = made.read()
                with open(os.path.join(RELEASE_ART, os.path.basename(name)), "rb") as release:
                    self.assertEqual(data, release.read(), name)
                self.assertEqual(struct.unpack(">II", data[16:24]), size, name)

    def test_missing_assets_give_the_same_images_as_the_monogram_option(self):
        with tempfile.TemporaryDirectory() as plain, tempfile.TemporaryDirectory() as missing, tempfile.TemporaryDirectory() as empty:
            self.assertEqual(build(plain, "--monogram"), build(missing, "--assets-folder", os.path.join(empty, "none"), "--ds-rom", os.path.join(empty, "none.nds")))

    def test_a_cartridge_alone_gives_its_own_logo_the_same_way_each_time(self):
        rom = os.path.join(SRC_VITA, "..", "extracted", "ds_rom", "game.nds")
        if not os.path.isfile(rom):
            self.skipTest("no DS cartridge dump")
        with tempfile.TemporaryDirectory() as plain, tempfile.TemporaryDirectory() as empty, tempfile.TemporaryDirectory() as first, tempfile.TemporaryDirectory() as second:
            none = os.path.join(empty, "none")
            logo = build(first, "--assets-folder", none)
            self.assertNotEqual(build(plain, "--monogram"), logo)
            self.assertEqual(build(second, "--assets-folder", none), logo)

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
