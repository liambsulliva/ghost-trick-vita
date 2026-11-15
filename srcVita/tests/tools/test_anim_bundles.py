import os
import shutil
import tempfile
import unittest
import zlib

import bake_record
import convert_assets
import packed_file
import read_ahead

OLD_FRAME = "frame ds_sprite_9 4 -256 -112 416 240 0 0 0.418511 0.03963\n"
NEW_FRAME = "frame ds_sprite_9 4 -256 -112 416 240 0 0 0.4994 0.124546\n"


def unpacked(path):
    with open(path, "rb") as source:
        data = source.read()
    return zlib.decompress(data[packed_file.PACKED_FILE_HEADER_SIZE:], packed_file.RAW_DEFLATE_WINDOW_BITS).decode("ascii")


class ConvertedBundlesFollowTheirAnimations(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.mkdtemp()
        os.makedirs(os.path.join(self.folder, "scripts"))
        self.bundle_path = os.path.join(self.folder, "scripts", "scene" + read_ahead.BUNDLE_EXTENSION)
        with open(self.bundle_path, "wb") as output:
            output.write(packed_file.packed(("animation t_crane\nloops 0\n" + OLD_FRAME).encode("ascii")))

    def tearDown(self):
        shutil.rmtree(self.folder)

    def convert(self, frame):
        record = bake_record.BakeRecord(self.folder)
        files = {"animations/t_crane.anim": ("animation t_crane\nloops 0\n" + frame).encode("ascii"),
                 "scripts/scene.motions": b"motion 10 t_crane 1\n", "scripts/chain.txt": b"scene\n"}
        convert_assets.write_outputs(self.folder, record, [], files, b"art ds\n", [])

    def test_a_conversion_rewrites_a_bundle_the_read_ahead_step_left_behind(self):
        self.convert(NEW_FRAME)
        self.assertEqual(unpacked(self.bundle_path), "animation t_crane\nloops 0\n" + NEW_FRAME)

    def test_every_script_with_motions_gets_a_bundle_matching_its_animations(self):
        self.convert(NEW_FRAME)
        scripts = os.path.join(self.folder, "scripts")
        animations = os.path.join(self.folder, "animations")
        for name in os.listdir(scripts):
            if name.endswith(read_ahead.MOTIONS_EXTENSION):
                script = name[:-len(read_ahead.MOTIONS_EXTENSION)]
                self.assertEqual(unpacked(os.path.join(scripts, script + read_ahead.BUNDLE_EXTENSION)), read_ahead.bundle_text(scripts, animations, script))

    def test_the_script_pack_carries_the_fresh_bundle(self):
        self.convert(NEW_FRAME)
        with open(os.path.join(self.folder, "scripts", "scripts.pack"), "rb") as source:
            pack = source.read()
        with open(self.bundle_path, "rb") as source:
            self.assertIn(source.read(), pack)


if __name__ == "__main__":
    unittest.main()
