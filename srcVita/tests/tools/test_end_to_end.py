import contextlib
import io
import os
import shutil
import struct
import tempfile
import unittest

import convert_assets
import synthetic_files

TESTS_FOLDER = os.path.dirname(os.path.abspath(__file__))
SRC_VITA_FOLDER = os.path.dirname(os.path.dirname(TESTS_FOLDER))
CHECKED_TOOL_PATH = os.path.join(SRC_VITA_FOLDER, "build", "tools", "texture_tool_checked")
TIMELINE_PATH = "natives/android/charfiles/2d/01_human/st01/a/st01_a_000/st01_a_000.user.2"
PAGE_0_PATH = "natives/android/charfiles/2d/01_human/st01/a/st01_a_000/st01_a_000_00_iml4.tex.691230823"
PAGE_1_PATH = "natives/android/charfiles/2d/01_human/st01/a/st01_a_000/st01_a_000_01_iml4.tex.691230823"
PLATE_PATH = "natives/android/charfiles/2d/02_bg/st01/plate_iml4.tex.691230823"
TEXTURE_FILE_HEADER_SIZE = 20
DXT1_BLOCK_BYTES = 8
LOGICAL_PAGES = ["charfiles/2D/01_Human/st01/A/st01_A_000/st01_A_000_00_IML4.tex", "charfiles/2D/01_Human/st01/A/st01_A_000/st01_A_000_01_IML4.tex"]


def transparent_astc_tex(width, height):
    columns = (width + 5) // 6
    rows = (height + 5) // 6
    block = synthetic_files.astc_constant_block(200, 100, 50, 128)
    return synthetic_files.tex_file(width, height, synthetic_files.FORMAT_ASTC_6X6, block * (columns * rows), columns * 16)


def opaque_etc2_tex(width, height):
    columns = width // 4
    rows = height // 4
    block = synthetic_files.etc2_constant_block(20, 10, 5)
    return synthetic_files.tex_file(width, height, synthetic_files.FORMAT_ETC2_RGB, block * (columns * rows), columns * 8)


def game_files():
    records = [
        (synthetic_files.record_shorts(10, (-6, -6), (0, 0)), (16.0, 8.0), 0),
        (synthetic_files.record_shorts(10, (-6, -6), (0, 0)), (16.0, 8.0), 1),
        (synthetic_files.record_shorts(0, (0, 0), (0, 0)), (0.0, 0.0), 2),
        (synthetic_files.record_shorts(10, (0, 0), (0, 0)), (4.0, 4.0), 2),
    ]
    timeline = synthetic_files.timeline_file(LOGICAL_PAGES, (64, 64), (32, 32), [2, 1], records, 2, (8, -4))
    return [
        (TIMELINE_PATH, timeline, True),
        (PAGE_0_PATH, transparent_astc_tex(64, 64), True),
        (PAGE_1_PATH, transparent_astc_tex(32, 32), False),
        (PLATE_PATH, opaque_etc2_tex(128, 64), True),
    ]


def tiny_set_description():
    return {
        "scene_name": "opening",
        "screen_textures": [
            {"name": "card", "source": PLATE_PATH, "width": 64, "height": 48, "list_as_background": True},
            {"name": "card_corner", "source": PLATE_PATH, "width": 8, "height": 16, "crop": [0, 0, 32, 64]},
        ],
        "plates": [{"name": "plate", "source": PLATE_PATH}],
        "animations": [
            {"name": "st01_a_000", "timeline": TIMELINE_PATH},
            {"name": "st01_a_000_f01", "timeline": TIMELINE_PATH, "ds_frames": [1]},
        ],
        "objects": {"a": {"layer": 3, "flip": 1}},
        "object_groups": {"everyone": [["a", "st01_a_000", 10, 20]]},
        "steps": [
            ["card", "card"],
            ["hide_card"],
            ["room", 1],
            ["plate", "plate", 0, 16],
            ["camera", 0, 0],
            ["object_group", "everyone"],
            ["wait", 30],
            ["object", "a", "st01_a_000_f01", 30, 40],
        ],
    }


class EndToEndTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, self.folder)
        self.apk_folder = os.path.join(self.folder, "apk")
        self.output_folder = os.path.join(self.folder, "output")
        os.makedirs(self.apk_folder)
        synthetic_files.write_apk_folder(self.apk_folder, game_files())
        tool_path = convert_assets.TEXTURE_TOOL_PATH
        convert_assets.TEXTURE_TOOL_PATH = CHECKED_TOOL_PATH
        self.addCleanup(setattr, convert_assets, "TEXTURE_TOOL_PATH", tool_path)

    def convert(self, set_description):
        with contextlib.redirect_stdout(io.StringIO()):
            convert_assets.convert_set_description(set_description, self.output_folder, self.apk_folder)

    def read_text(self, *parts):
        with open(os.path.join(self.output_folder, *parts), "r", encoding="ascii") as source:
            return source.read()

    def read_gtex_header(self, name):
        with open(os.path.join(self.output_folder, "textures", name + ".gtex"), "rb") as source:
            data = source.read()
        self.assertEqual(data[:4], b"GTEX")
        return struct.unpack_from("<IIII", data, 4), len(data)

    def first_pixel_red(self, name):
        with open(os.path.join(self.output_folder, "textures", name + ".gtex"), "rb") as source:
            data = source.read()
        return synthetic_files.decode_color_block(data[TEXTURE_FILE_HEADER_SIZE:TEXTURE_FILE_HEADER_SIZE + DXT1_BLOCK_BYTES])[0][0]

    def test_a_small_set_converts_completely(self):
        self.convert(tiny_set_description())
        self.assertEqual(self.read_text("manifest.txt"), "\n".join([
            "scene opening",
            "background plate",
            "background card",
            "animation st01_a_000",
            "animation st01_a_000_f01",
        ]) + "\n")
        self.assertEqual(self.read_text("scenes", "opening.scene"), "\n".join([
            "scene opening",
            "card card",
            "hide_card",
            "room 1",
            "plate plate 0 16 32 16",
            "camera 0 0",
            "object a st01_a_000 10 20 3 1",
            "wait 30",
            "object a st01_a_000_f01 30 40 3 1",
        ]) + "\n")
        self.assertEqual(self.read_text("animations", "st01_a_000.anim"), "\n".join([
            "animation st01_a_000",
            "loops 0",
            "frame st01_a_000_00 2 -247 -124 7.818182 8 0 0 0.488636 1",
            "frame st01_a_000_00 2 -246.818182 -124 7.818182 8 0.511364 0 0.488636 1",
            "frame - 2 0 0 0 0 0 0 0 0",
            "frame st01_a_000_01 2 -253 -128 8 8 0 0 1 1",
        ]) + "\n")
        self.assertEqual(self.read_text("animations", "st01_a_000_f01.anim").splitlines()[2], "frame - 4 0 0 0 0 0 0 0 0")
        self.assertEqual(self.read_gtex_header("st01_a_000_00"), ((44, 24, 1, 1056), 1076))
        self.assertEqual(self.read_gtex_header("st01_a_000_01"), ((24, 24, 1, 576), 596))
        self.assertEqual(self.read_gtex_header("plate"), ((92, 44, 2, 2024), 2044))
        self.assertEqual(self.read_gtex_header("card"), ((64, 48, 2, 1536), 1556))
        self.assertEqual(self.read_gtex_header("card_corner"), ((8, 16, 2, 64), 84))

    def test_stored_colors_keep_their_source_brightness(self):
        self.convert(tiny_set_description())
        self.assertAlmostEqual(self.first_pixel_red("card"), 167, delta=6)
        self.assertAlmostEqual(self.first_pixel_red("card_corner"), 167, delta=6)

    def test_a_cell_outside_its_page_is_refused(self):
        set_description = tiny_set_description()
        set_description["animations"] = [{"name": "st01_a_000", "timeline": TIMELINE_PATH}]
        files = game_files()
        files[2] = (PAGE_1_PATH, transparent_astc_tex(16, 16), False)
        synthetic_files.write_apk_folder(self.apk_folder, files)
        with self.assertRaises(ValueError):
            self.convert(set_description)

    def test_running_twice_gives_the_same_files_and_removes_stale_ones(self):
        self.convert(tiny_set_description())
        stale_path = os.path.join(self.output_folder, "textures", "old.gtex")
        with open(stale_path, "wb") as output:
            output.write(b"old")
        first_scene = self.read_text("scenes", "opening.scene")
        with open(os.path.join(self.output_folder, "textures", "plate.gtex"), "rb") as source:
            first_plate = source.read()
        self.convert(tiny_set_description())
        self.assertFalse(os.path.exists(stale_path))
        self.assertEqual(self.read_text("scenes", "opening.scene"), first_scene)
        with open(os.path.join(self.output_folder, "textures", "plate.gtex"), "rb") as source:
            self.assertEqual(source.read(), first_plate)

    def test_a_missing_pack_entry_is_reported(self):
        set_description = tiny_set_description()
        set_description["plates"] = [{"name": "plate", "source": "natives/android/not/there.tex.691230823"}]
        with self.assertRaises(KeyError):
            self.convert(set_description)

    def test_a_scene_naming_an_unknown_animation_is_refused(self):
        set_description = tiny_set_description()
        set_description["steps"].append(["object", "a", "no_such_animation", 0, 0])
        with self.assertRaises(ValueError):
            self.convert(set_description)


if __name__ == "__main__":
    unittest.main()
