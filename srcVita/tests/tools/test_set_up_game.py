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

    def run_stages_with(self, folder, apk):
        calls = []
        rom = os.path.join(folder, "game.nds")
        with open(rom, "wb") as output:
            output.write(b"rom")
        arguments = mock.Mock(apk=apk, ds_rom=rom)
        with mock.patch.object(set_up_game, "APK_FOLDER", os.path.join(folder, "remaster_apk")), \
                mock.patch.object(set_up_game, "TEXT_KEY_PATH", os.path.join(folder, "remaster_apk", "text_key.bin")), \
                mock.patch.object(set_up_game, "check_tools"), \
                mock.patch.object(set_up_game, "copy_packs", side_effect=lambda path: calls.append("copy")), \
                mock.patch.object(set_up_game, "find_text_key", side_effect=lambda path: calls.append("key")), \
                mock.patch.object(set_up_game, "convert", side_effect=lambda path: calls.append("remastered")), \
                mock.patch.object(set_up_game, "convert_ds_set", side_effect=lambda path: calls.append("ds")), \
                mock.patch.object(set_up_game, "run_with_progress", side_effect=lambda command, label, environment=None: calls.append(next((word for word in command if word.startswith("PLAN_SET=")), command[-1])) or []), \
                mock.patch.object(set_up_game, "vitasdk_folder", return_value=None), \
                redirect_stdout(io.StringIO()) as shown:
            set_up_game.run_stages(arguments)
        return calls, shown.getvalue()

    def test_with_only_the_cartridge_only_the_ds_set_is_built_and_planned(self):
        with tempfile.TemporaryDirectory() as folder:
            calls, shown = self.run_stages_with(folder, None)
        self.assertEqual(calls, ["srcVita/tools/assets", "ds", "PLAN_SET=game_ds"])
        self.assertIn("ux0:data/ghosttrick/assets_ds/", shown)
        self.assertNotIn("ux0:data/ghosttrick/assets/\n", shown)

    def test_with_the_android_package_both_sets_are_built_and_planned(self):
        with tempfile.TemporaryDirectory() as folder:
            calls, shown = self.run_stages_with(folder, os.path.join(folder, "game.apk"))
        self.assertEqual(calls, ["copy", "key", "srcVita/tools/assets", "ds", "remastered", "PLAN_SET=game_ds", "PLAN_SET=game"])
        self.assertIn("ux0:data/ghosttrick/assets_ds/", shown)
        self.assertIn("ux0:data/ghosttrick/assets/", shown)

    def test_the_bar_shows_share_and_sizes(self):
        self.assertIn(" 50%", set_up_game.bar_text(512, 1024, 0))
        self.assertIn("512 B of 1.0 KB", set_up_game.bar_text(512, 1024, 0))


if __name__ == "__main__":
    unittest.main()
