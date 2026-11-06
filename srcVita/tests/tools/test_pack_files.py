import shutil
import tempfile
import unittest

import pack_files
import synthetic_files


class PackFileTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, self.folder)

    def test_stored_and_deflated_entries_read_back(self):
        synthetic_files.write_apk_folder(self.folder, [("natives/android/a.bin", b"stored bytes", False), ("natives/android/b.bin", b"deflated bytes" * 20, True)])
        packs = pack_files.PackSet(self.folder)
        self.assertEqual(packs.read("natives/android/a.bin"), b"stored bytes")
        self.assertEqual(packs.read("natives/android/b.bin"), b"deflated bytes" * 20)

    def test_a_missing_path_raises(self):
        synthetic_files.write_apk_folder(self.folder, [])
        with self.assertRaises(KeyError):
            pack_files.PackSet(self.folder).read("natives/android/missing.bin")

    def test_the_key_pairs_the_lower_case_hash_then_the_upper_case_hash(self):
        self.assertEqual(pack_files.path_hash_key("Ab"), (pack_files.murmur3_hash("ab".encode("utf-16-le")), pack_files.murmur3_hash("AB".encode("utf-16-le"))))

    def test_the_hash_matches_a_published_vector(self):
        self.assertEqual(pack_files.murmur3_hash(b""), 0x81F16F39)


if __name__ == "__main__":
    unittest.main()
