import io
import os
import sys
import tempfile
import unittest
import zipfile
from contextlib import redirect_stdout
from unittest import mock

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools"))

import set_up_game


def fake_apk(folder, names):
    path = os.path.join(folder, "game.apk")
    with zipfile.ZipFile(path, "w") as package:
        for name in names:
            package.writestr("assets/" + name, name.encode("ascii") * 1000)
        package.writestr("assets/other.bin", b"x")
    return path


class SetUpGameTest(unittest.TestCase):
    def test_the_three_packs_are_copied_with_their_own_names(self):
        with tempfile.TemporaryDirectory() as folder:
            apk = fake_apk(folder, set_up_game.PACK_NAMES)
            target = os.path.join(folder, "remaster_apk")
            with mock.patch.object(set_up_game, "APK_FOLDER", target), redirect_stdout(io.StringIO()):
                set_up_game.copy_packs(apk)
                self.assertTrue(set_up_game.packs_already_copied())
            for name in set_up_game.PACK_NAMES:
                with open(os.path.join(target, name), "rb") as copied:
                    self.assertEqual(copied.read(), name.encode("ascii") * 1000)
            self.assertFalse(os.path.exists(os.path.join(target, "other.bin")))

    def test_a_package_missing_a_pack_is_refused_by_name(self):
        with tempfile.TemporaryDirectory() as folder:
            apk = fake_apk(folder, ("re_chunk_000.pak", "data01"))
            with self.assertRaises(set_up_game.SetupFailed) as caught:
                set_up_game.pack_members(apk)
            self.assertIn("assets/data02", str(caught.exception))

    def test_the_bar_shows_share_and_sizes(self):
        self.assertIn(" 50%", set_up_game.bar_text(512, 1024, 0))
        self.assertIn("512 B of 1.0 KB", set_up_game.bar_text(512, 1024, 0))


if __name__ == "__main__":
    unittest.main()
