import contextlib
import io
import os
import shutil
import struct
import tempfile
import unittest
from unittest import mock

import bake_record
import convert_assets
import synthetic_files
import test_audio_file
import texture_encoder

TESTS_FOLDER = os.path.dirname(os.path.abspath(__file__))
SRC_VITA_FOLDER = os.path.dirname(os.path.dirname(TESTS_FOLDER))
CHECKED_TOOL_PATH = os.path.join(SRC_VITA_FOLDER, "build", "tools", "texture_tool_checked")
TIMELINE_PATH = "natives/android/charfiles/2d/01_human/st01/a/st01_a_000/st01_a_000.user.2"
PAGE_0_PATH = "natives/android/charfiles/2d/01_human/st01/a/st01_a_000/st01_a_000_00_iml4.tex.691230823"
PAGE_1_PATH = "natives/android/charfiles/2d/01_human/st01/a/st01_a_000/st01_a_000_01_iml4.tex.691230823"
PLATE_PATH = "natives/android/charfiles/2d/02_bg/st01/plate_iml4.tex.691230823"
TEXTURE_FILE_HEADER_SIZE = 20
DXT1_BLOCK_BYTES = 8
SONG_PATH = "natives/android/streaming/sound/bgm_new/song.asrc.31"
LOOPED_SONG_PATH = "natives/android/streaming/sound/bgm_new/looped_song.asrc.31"
PING_PATH = "natives/android/streaming/sound/se_resource/sys/ping.asrc.31"
ODD_RATE_PING_PATH = "natives/android/streaming/sound/se_resource/sys/odd.asrc.31"
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
        (SONG_PATH, test_audio_file.wrapped(), False),
        (LOOPED_SONG_PATH, test_audio_file.wrapped(loop_flag=1, loop_start=4, loop_last=9), True),
        (PING_PATH, test_audio_file.wrapped(channels=1), False),
        (ODD_RATE_PING_PATH, test_audio_file.wrapped(channels=1, rate=18000), False),
    ]


def tiny_set_description():
    return {
        "scene_name": "opening",
        "screen_textures": [
            {"name": "card", "source": PLATE_PATH, "width": 64, "height": 48, "list_as_background": True},
            {"name": "card_corner", "source": PLATE_PATH, "width": 8, "height": 16, "crop": [0, 0, 32, 64]},
        ],
        "plates": [{"name": "plate", "source": PLATE_PATH}],
        "music": [{"name": "song", "source": SONG_PATH}, {"name": "looped_song", "source": LOOPED_SONG_PATH}],
        "sounds": [{"name": "ping", "source": PING_PATH}],
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
            ["music", "song", 1],
            ["sound", "ping"],
            ["music_stop", 4],
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
            "music song 1",
            "sound ping",
            "music_stop 4",
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

    def read_bytes(self, *parts):
        with open(os.path.join(self.output_folder, *parts), "rb") as source:
            return source.read()

    def test_audio_is_written_without_its_wrapper_and_only_looped_tracks_get_a_loop_file(self):
        self.convert(tiny_set_description())
        self.assertEqual(self.read_bytes("music", "song.ogg"), test_audio_file.OGG_BYTES)
        self.assertEqual(self.read_bytes("music", "looped_song.ogg"), test_audio_file.OGG_BYTES)
        self.assertEqual(self.read_bytes("sounds", "ping.ogg"), test_audio_file.OGG_BYTES)
        self.assertEqual(self.read_text("music", "looped_song.loop"), "4 9\n")
        self.assertFalse(os.path.exists(os.path.join(self.output_folder, "music", "song.loop")))
        self.assertFalse(os.path.exists(os.path.join(self.output_folder, "sounds", "ping.loop")))

    def test_a_set_without_audio_still_converts(self):
        set_description = tiny_set_description()
        del set_description["music"]
        del set_description["sounds"]
        set_description["steps"] = [step for step in set_description["steps"] if step[0] not in ("music", "sound", "music_stop")]
        self.convert(set_description)
        self.assertEqual(os.listdir(os.path.join(self.output_folder, "music")), [])

    def test_an_effect_at_another_rate_is_refused(self):
        set_description = tiny_set_description()
        set_description["sounds"] = [{"name": "ping", "source": ODD_RATE_PING_PATH}]
        with self.assertRaisesRegex(ValueError, "18000 Hz"):
            self.convert(set_description)

    def test_a_music_step_naming_an_undeclared_track_is_refused(self):
        set_description = tiny_set_description()
        set_description["steps"].append(["music", "no_such_track", 0])
        with self.assertRaises(ValueError):
            self.convert(set_description)

    def test_stale_audio_is_removed(self):
        self.convert(tiny_set_description())
        stale_path = os.path.join(self.output_folder, "sounds", "old.ogg")
        with open(stale_path, "wb") as output:
            output.write(b"old")
        self.convert(tiny_set_description())
        self.assertFalse(os.path.exists(stale_path))

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
        with self.assertRaisesRegex(convert_assets.ConversionFailed, "plate"):
            self.convert(set_description)

    def output_tree(self):
        tree = {}
        for folder, _, names in os.walk(self.output_folder):
            for name in names:
                if name != bake_record.RECORD_FILE_NAME:
                    path = os.path.join(folder, name)
                    with open(path, "rb") as source:
                        tree[os.path.relpath(path, self.output_folder)] = source.read()
        return tree

    def textures_baked_by(self, set_description):
        with mock.patch.object(texture_encoder, "convert_tex_to_gtex", wraps=texture_encoder.convert_tex_to_gtex) as convert:
            self.convert(set_description)
        return sorted(call.args[2] for call in convert.call_args_list)

    def test_a_second_run_bakes_nothing_and_leaves_every_file_untouched(self):
        self.convert(tiny_set_description())
        before = {}
        for relative_path in self.output_tree():
            path = os.path.join(self.output_folder, relative_path)
            os.utime(path, (1000, 1000))
            before[relative_path] = 1000
        self.assertEqual(self.textures_baked_by(tiny_set_description()), [])
        for relative_path, moment in before.items():
            self.assertEqual(os.path.getmtime(os.path.join(self.output_folder, relative_path)), moment, relative_path)

    def test_a_changed_source_bakes_only_the_textures_made_from_it(self):
        self.convert(tiny_set_description())
        files = game_files()
        files[1] = (PAGE_0_PATH, transparent_astc_tex(64, 64) + b"\0", True)
        synthetic_files.write_apk_folder(self.apk_folder, files)
        self.assertEqual(self.textures_baked_by(tiny_set_description()), ["st01_a_000_00"])

    def test_changed_settings_bake_only_that_texture(self):
        self.convert(tiny_set_description())
        set_description = tiny_set_description()
        set_description["screen_textures"][0]["width"] = 32
        self.assertEqual(self.textures_baked_by(set_description), ["card"])
        self.assertEqual(self.read_gtex_header("card")[0][0], 32)

    def test_changed_encoder_code_bakes_everything_again(self):
        self.convert(tiny_set_description())
        with mock.patch.object(convert_assets, "texture_code_fingerprint", return_value="other code"):
            self.assertEqual(len(self.textures_baked_by(tiny_set_description())), 5)

    def test_a_deleted_or_edited_output_is_baked_again(self):
        self.convert(tiny_set_description())
        os.remove(os.path.join(self.output_folder, "textures", "plate.gtex"))
        with open(os.path.join(self.output_folder, "textures", "card.gtex"), "wb") as output:
            output.write(b"edited")
        self.assertEqual(self.textures_baked_by(tiny_set_description()), ["card", "plate"])

    def test_building_over_an_old_set_gives_the_same_files_as_a_clean_build(self):
        old_set = tiny_set_description()
        old_set["screen_textures"][0]["width"] = 32
        old_set["animations"] = old_set["animations"][:1]
        old_set["steps"] = [step for step in old_set["steps"] if step[0] != "object"]
        self.convert(old_set)
        self.convert(tiny_set_description())
        incremental = self.output_tree()
        shutil.rmtree(self.output_folder)
        self.convert(tiny_set_description())
        self.assertEqual(incremental, self.output_tree())

    def test_a_texture_no_longer_in_the_set_is_removed_and_forgotten(self):
        self.convert(tiny_set_description())
        smaller = tiny_set_description()
        del smaller["screen_textures"][1]
        self.convert(smaller)
        self.assertFalse(os.path.exists(os.path.join(self.output_folder, "textures", "card_corner.gtex")))
        self.assertEqual(self.textures_baked_by(tiny_set_description()), ["card_corner"])

    def test_one_bad_texture_does_not_lose_the_others(self):
        set_description = tiny_set_description()
        set_description["screen_textures"].append({"name": "bad", "source": PLATE_PATH, "width": 8, "height": 8, "crop": [0, 0, 9999, 8]})
        with self.assertRaisesRegex(convert_assets.ConversionFailed, "bad"):
            self.convert(set_description)
        self.assertEqual(self.textures_baked_by(tiny_set_description()), [])

    def test_a_failed_run_leaves_the_last_good_manifest_and_files(self):
        self.convert(tiny_set_description())
        before = self.output_tree()
        set_description = tiny_set_description()
        set_description["screen_textures"].append({"name": "bad", "source": PLATE_PATH, "width": 8, "height": 8, "crop": [0, 0, 9999, 8]})
        with self.assertRaises(convert_assets.ConversionFailed):
            self.convert(set_description)
        self.assertEqual(self.output_tree(), before)

    def test_too_many_frames_are_refused_before_any_texture_is_baked(self):
        set_description = tiny_set_description()
        set_description["animations"].append({"name": "long", "timeline": TIMELINE_PATH, "ds_frames": [0] * 1025})
        with mock.patch.object(texture_encoder, "convert_tex_to_gtex") as convert:
            with self.assertRaises(ValueError):
                self.convert(set_description)
        convert.assert_not_called()

    def test_a_set_with_no_scene_lists_only_its_art(self):
        self.convert(tiny_set_description())
        set_description = {"animations": [{"name": "st01_a_000", "timeline": TIMELINE_PATH}], "plates": [{"name": "plate", "source": PLATE_PATH}]}
        self.convert(set_description)
        self.assertEqual(self.read_text("manifest.txt"), "background plate\nanimation st01_a_000\n")
        self.assertEqual(os.listdir(os.path.join(self.output_folder, "scenes")), [])

    def test_a_set_description_can_be_named_by_its_path(self):
        path = os.path.join(self.folder, "my_set.json")
        with open(path, "w", encoding="ascii") as output:
            output.write('{"scene_name": "x"}')
        self.assertEqual(convert_assets.load_set_description(path), {"scene_name": "x"})

    def test_a_scene_naming_an_unknown_animation_is_refused(self):
        set_description = tiny_set_description()
        set_description["steps"].append(["object", "a", "no_such_animation", 0, 0])
        with self.assertRaises(ValueError):
            self.convert(set_description)


if __name__ == "__main__":
    unittest.main()
