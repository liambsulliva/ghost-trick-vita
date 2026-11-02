import hashlib
import tempfile
import unittest
import xml.etree.ElementTree as ET
from pathlib import Path
import sys

TOOLS_DIR = Path(__file__).resolve().parents[1] / "tools"
sys.path.insert(0, str(TOOLS_DIR))

import progress_treemap


def function(address, size, matched=False, module="main"):
    return {"module": module, "name": "function_" + str(address), "address": address, "size": size, "mode": "thumb", "matched": matched}


class ProgressTests(unittest.TestCase):
    def test_shared_code_is_counted_once_within_each_module(self):
        rows = [function(0x1000, 16), function(0x1002, 14), function(0x1000, 8, module="itcm")]
        self.assertEqual(progress_treemap.code_bytes(rows), 24)

    def test_layout_preserves_area_without_overlapping_bounds(self):
        rows = [function(i * 8, i + 1) for i in range(10)]
        boxes = progress_treemap.rectangles(rows, 0, 0, 100, 80)
        self.assertAlmostEqual(sum(w * h for row, x, y, w, h in boxes), 8000)
        for i, (row, x, y, w, h) in enumerate(boxes):
            self.assertGreater(w, 0)
            self.assertGreater(h, 0)
            for other, a, b, c, d in boxes[i + 1:]:
                self.assertTrue(x + w <= a + 1e-8 or a + c <= x + 1e-8 or y + h <= b + 1e-8 or b + d <= y + 1e-8)

    def test_svg_has_one_tile_per_function_and_honest_zero_progress(self):
        rows = [function(0x1000, 2), function(0x1002, 14)]
        svg = progress_treemap.make_svg(rows)
        root = ET.fromstring(svg)
        tiles = [element for element in root.iter() if "data-function" in element.attrib]
        self.assertEqual(len(tiles), 2)
        self.assertTrue(all(element.attrib["fill"] == "#535e6d" for element in tiles))
        self.assertIn("Functions matched: 0 / 2 (0.00%)", svg)

    def test_changed_source_and_unknown_match_cannot_inflate_progress(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            (root / "test.c").write_text("void test(void) {}\n")
            rows = [function(0x1000, 2)]
            match = {"module": "main", "address": "0x1000", "size": 2, "source": "test.c", "source_sha256": "0" * 64, "verification_sha256": "1" * 64}
            with self.assertRaises(ValueError):
                progress_treemap.apply_matches(rows, {"matched": [match]}, root)
            match["source_sha256"] = hashlib.sha256((root / "test.c").read_bytes()).hexdigest()
            progress_treemap.apply_matches(rows, {"matched": [match]}, root)
            self.assertTrue(rows[0]["matched"])
            match["address"] = "0x2000"
            with self.assertRaises(ValueError):
                progress_treemap.apply_matches(rows, {"matched": [match]}, root)

    def test_verified_tile_links_to_its_source_and_unmatched_tile_has_no_link(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            source = root / "src/one & two.cpp"
            source.parent.mkdir()
            source.write_text("void test(void) {}\n")
            rows = [function(0x1000, 2), function(0x1002, 14)]
            match = {"module": "main", "address": "0x1000", "size": 2, "source": "src/one & two.cpp", "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(), "verification_sha256": "1" * 64}
            progress_treemap.apply_matches(rows, {"matched": [match]}, root)
            svg = ET.fromstring(progress_treemap.make_svg(rows))
            namespace = {"svg": "http://www.w3.org/2000/svg"}
            links = svg.findall(".//svg:a", namespace)
            self.assertEqual(len(links), 1)
            self.assertEqual(links[0].attrib["href"], "../src/one%20%26%20two.cpp")
            self.assertEqual(links[0].find("svg:rect", namespace).attrib["data-function"], "main:function_4096")
            relocated = ET.fromstring(progress_treemap.make_svg(rows, "../../checkout"))
            self.assertEqual(relocated.find(".//svg:a", namespace).attrib["href"], "../../checkout/src/one%20%26%20two.cpp")
            hosted = ET.fromstring(progress_treemap.make_svg(rows, "https://github.com/owner/repo/blob/revision/"))
            link = hosted.find(".//svg:a", namespace)
            self.assertEqual(link.attrib["href"], "https://github.com/owner/repo/blob/revision/src/one%20%26%20two.cpp")
            self.assertEqual(link.attrib["target"], "_top")

    def test_static_preview_keeps_every_rectangle_without_claiming_links(self):
        rows = [function(0x1000, 2), function(0x1002, 14)]
        text = progress_treemap.make_svg(rows, interactive=False)
        svg = ET.fromstring(text)
        namespace = {"svg": "http://www.w3.org/2000/svg"}
        self.assertEqual(svg.findall(".//svg:a", namespace), [])
        self.assertEqual(sum(path.attrib["d"].count("M") for path in svg.findall("svg:path", namespace)), 2)
        self.assertIn("Static preview", text)
        self.assertNotIn("click to open source", text)

    def test_hosted_svg_scales_to_its_frame_while_static_preview_keeps_its_size(self):
        rows = [function(0x1000, 2), function(0x1002, 14)]
        namespace = {"svg": "http://www.w3.org/2000/svg"}
        hosted = ET.fromstring(progress_treemap.make_svg(rows, "https://github.com/owner/repo/blob/revision/"))
        self.assertEqual(hosted.attrib["viewBox"], "0 0 1600 1092")
        self.assertEqual(hosted.attrib["preserveAspectRatio"], "xMinYMid meet")
        self.assertNotIn("width", hosted.attrib)
        self.assertNotIn("height", hosted.attrib)
        static = ET.fromstring(progress_treemap.make_svg(rows, interactive=False))
        self.assertEqual(static.attrib["width"], "1600")
        self.assertEqual(static.attrib["height"], "1092")
        self.assertEqual(static.find("svg:title", namespace).text, "Ghost Trick: Phantom Detective Decompilation Progress")

    def test_portrait_map_keeps_every_tile_inside_its_taller_canvas(self):
        rows = [function(i * 8, i + 1) for i in range(40)]
        width, height = progress_treemap.PORTRAIT_WIDTH, progress_treemap.PORTRAIT_HEIGHT
        svg = ET.fromstring(progress_treemap.make_svg(rows, "https://github.com/owner/repo/blob/revision/", canvas_width=width, canvas_height=height))
        self.assertEqual(svg.attrib["viewBox"], f"0 0 {width} {height}")
        tiles = [element for element in svg.iter() if "data-function" in element.attrib]
        self.assertEqual(len(tiles), 40)
        for element in tiles:
            x, y, width, height = (float(element.attrib[name]) for name in ("x", "y", "width", "height"))
            self.assertGreaterEqual(x, 0)
            self.assertGreaterEqual(y, 0)
            self.assertLessEqual(x + width, progress_treemap.PORTRAIT_WIDTH)
            self.assertLessEqual(y + height, progress_treemap.PORTRAIT_HEIGHT)

    def test_hosted_viewer_embeds_svg_as_a_document(self):
        viewer = progress_treemap.make_viewer()
        self.assertIn('<iframe class="landscape" src="treemap.svg"', viewer)
        self.assertIn('<iframe class="portrait" src="treemap-portrait.svg"', viewer)
        self.assertIn(f"calc(100dvh * {progress_treemap.WIDE_WIDTH} / {progress_treemap.WIDE_HEIGHT})", viewer)
        self.assertIn(f"aspect-ratio: {progress_treemap.PORTRAIT_WIDTH} / {progress_treemap.PORTRAIT_HEIGHT}", viewer)
        self.assertNotIn('<img', viewer)
        self.assertIn('history.scrollRestoration = "manual"', viewer)
        self.assertIn('<title>Ghost Trick Decomp Progress</title>', viewer)
        self.assertNotIn('<p>', viewer)


if __name__ == "__main__":
    unittest.main()
