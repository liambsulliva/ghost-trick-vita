import os
import struct
import sys
import tempfile
import unittest
from unittest import mock

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools", "assets"))

import read_ahead
import script_pack


def write(folder, name, data):
    with open(os.path.join(folder, name), "wb") as output:
        output.write(data)


def unpack(data):
    assert data[:4] == script_pack.MAGIC
    version, count = struct.unpack("<II", data[4:12])
    at = 12
    files = {}
    for _ in range(count):
        (name_size,) = struct.unpack("<H", data[at:at + 2])
        name = data[at + 2:at + 2 + name_size].decode("ascii")
        offset, size = struct.unpack("<II", data[at + 2 + name_size:at + 10 + name_size])
        files[name] = data[offset:offset + size]
        at = at + 10 + name_size
    return version, files


class ScriptPackTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.TemporaryDirectory()
        self.addCleanup(self.folder.cleanup)
        self.scripts = os.path.join(self.folder.name, "scripts")
        os.makedirs(self.scripts)
        write(self.scripts, "chain.txt", b"file a\n")

    def pack_path(self):
        return os.path.join(self.scripts, script_pack.PACK_FILE_NAME)

    def read_pack(self):
        with open(self.pack_path(), "rb") as source:
            return unpack(source.read())

    def test_every_file_the_chain_reads_is_packed_byte_for_byte(self):
        write(self.scripts, "chain.txt", b"file a\n")
        write(self.scripts, "a.lmg", b"\x00\x01script")
        write(self.scripts, "a.anims", b"bundle")
        self.assertTrue(script_pack.write_pack(self.scripts))
        version, files = self.read_pack()
        self.assertEqual(version, script_pack.VERSION)
        self.assertEqual(files, {"a.anims": b"bundle", "a.lmg": b"\x00\x01script", "chain.txt": b"file a\n"})

    def test_the_pack_leaves_out_itself_half_written_files_and_the_tile_table(self):
        write(self.scripts, "a.lmg", b"script")
        write(self.scripts, "a.ahead.new", b"half")
        write(self.scripts, "tiles.txt", b"tiles")
        script_pack.write_pack(self.scripts)
        script_pack.write_pack(self.scripts)
        self.assertEqual(sorted(self.read_pack()[1]), ["a.lmg", "chain.txt"])

    def test_a_folder_with_no_chain_has_no_pack(self):
        write(self.scripts, "a.lmg", b"script")
        script_pack.write_pack(self.scripts)
        os.remove(os.path.join(self.scripts, "chain.txt"))
        self.assertFalse(script_pack.write_pack(self.scripts))
        self.assertFalse(os.path.exists(self.pack_path()))

    def test_an_unchanged_folder_leaves_the_pack_alone(self):
        write(self.scripts, "a.lmg", b"script")
        self.assertTrue(script_pack.write_pack(self.scripts))
        self.assertFalse(script_pack.write_pack(self.scripts))
        write(self.scripts, "a.lmg", b"changed")
        self.assertTrue(script_pack.write_pack(self.scripts))
        self.assertEqual(self.read_pack()[1]["a.lmg"], b"changed")

    def test_the_read_ahead_step_packs_what_it_wrote(self):
        animations = os.path.join(self.folder.name, "animations")
        os.makedirs(animations)
        write(self.scripts, "scene.motions", b"")
        write(self.folder.name, "cues.tsv", b"0\tmark script_begun scene\n")
        arguments = ["read_ahead.py", os.path.join(self.folder.name, "cues.tsv"), self.scripts, animations]
        with mock.patch.object(sys, "argv", arguments), mock.patch("builtins.print"):
            self.assertEqual(read_ahead.main(), 0)
        files = self.read_pack()[1]
        self.assertIn("scene" + read_ahead.AHEAD_EXTENSION, files)
        self.assertIn("scene" + read_ahead.BUNDLE_EXTENSION, files)


if __name__ == "__main__":
    unittest.main()
