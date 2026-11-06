import os
import tempfile
import unittest

import compare_capture


class NormalizeCaptureTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.TemporaryDirectory()
        self.addCleanup(self.folder.cleanup)
        self.output_path = os.path.join(self.folder.name, "output.png")

    def make_image(self, size):
        path = os.path.join(self.folder.name, size + ".png")
        result = compare_capture.run_magick(["-size", size, "xc:red", path])
        self.assertEqual(result.returncode, 0, result.stderr)
        return path

    def test_a_vita_screen_is_cropped_to_the_compared_size(self):
        compare_capture.normalize_capture(self.make_image("960x544"), self.output_path)
        size = compare_capture.run_magick(["identify", "-format", "%wx%h", self.output_path])
        self.assertEqual(size.stdout, compare_capture.COMPARED_SIZE)

    def test_a_capture_of_the_wrong_size_is_refused_by_name(self):
        path = self.make_image("100x100")
        with self.assertRaises(RuntimeError) as caught:
            compare_capture.normalize_capture(path, self.output_path)
        self.assertIn(path, str(caught.exception))


if __name__ == "__main__":
    unittest.main()
