import contextlib
import hashlib
import io
import json
import os
import shutil
import signal
import struct
import subprocess
import sys
import tempfile
import time
import unittest
from unittest import mock

import bake_record
import convert_assets
import font_file
import gtex_pack
import line_renderer
import narration
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
MESSAGE_PATH = "natives/android/gamedesign/text/st01/made_up.msg.22"
FONT_PATH = "natives/android/gui/font/outlinefont/made_up.oft.1"
FONT_KEY = bytes([9, 8, 7, 6, 5, 4, 3, 2])
OTHER_FONT_KEY = bytes([1, 2, 3, 4, 5, 6, 7, 8])
BUBBLE_FONT_PATH = "natives/android/gui/font/outlinefont/made_up_bubble.oft.1"
SECOND_TEXT_KEY = bytes(range(32, 48))
STORY_LINE_PREFIX = "SECRETWORDS"
TEXTURE_FILE_HEADER_SIZE = 20
DXT1_BLOCK_BYTES = 8
SONG_PATH = "natives/android/streaming/sound/bgm_new/song.asrc.31"
LOOPED_SONG_PATH = "natives/android/streaming/sound/bgm_new/looped_song.asrc.31"
PING_PATH = "natives/android/streaming/sound/se_resource/sys/ping.asrc.31"
ODD_RATE_PING_PATH = "natives/android/streaming/sound/se_resource/sys/odd.asrc.31"
ODD_RATE = 18000
DXT5_BLOCK_BYTES = 16
DXT5_COLOR_ENDPOINTS_OFFSET = 8
WHITE_RGB565 = 0xFFFF
BLACK_RGB565 = 0
LINE_COUNT_THAT_TAKES_LONGER_THAN_THE_TEST = 200
CONVERTER_WAIT_SECONDS = 30
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
        (ODD_RATE_PING_PATH, test_audio_file.wrapped(ogg_bytes=tone_ogg(ODD_RATE), rate=ODD_RATE), False),
    ]


def tone_ogg(rate):
    return subprocess.run(["ffmpeg", "-v", "error", "-f", "lavfi", "-i", "sine=frequency=440:sample_rate=%d:duration=0.2" % rate, "-ac", "2", "-c:a", "vorbis", "-strict", "-2", "-fflags", "+bitexact", "-flags:a", "+bitexact", "-f", "ogg", "-"], capture_output=True, check=True).stdout


def sample_rate_of(path):
    return int(subprocess.run(["ffprobe", "-v", "error", "-show_entries", "stream=sample_rate", "-of", "csv=p=0", path], capture_output=True, text=True, check=True).stdout.strip())


def tiny_set_description():
    return {
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
    }


def story_text(pos_y=25, last_line="three"):
    return '<TRCK mono_pos_set posY="%d"/>' % pos_y + STORY_LINE_PREFIX + 'one\r\n' + STORY_LINE_PREFIX + 'two<TRCK paragraph_end />\r\n' + STORY_LINE_PREFIX + last_line


def text_game_files(story=None, bubble="bubble", narration_font_key=FONT_KEY, bubble_font_key=FONT_KEY, text_key=synthetic_files.FAKE_TEXT_KEY):
    entries = [("entry_story", ["other language", story or story_text()]), ("entry_bubble", ["other language", STORY_LINE_PREFIX + bubble])]
    message_data = synthetic_files.message_file_bytes(entries, key=text_key)
    narration_font = synthetic_files.fbfo_font_file(synthetic_files.box_font_bytes(), narration_font_key)
    bubble_font = synthetic_files.fbfo_font_file(synthetic_files.box_font_bytes(), bubble_font_key)
    return game_files() + [(MESSAGE_PATH, message_data, True), (FONT_PATH, narration_font, True), (BUBBLE_FONT_PATH, bubble_font, True)]


def long_text_game_files():
    lines = [STORY_LINE_PREFIX + str(index) for index in range(LINE_COUNT_THAT_TAKES_LONGER_THAN_THE_TEST)]
    story = '<TRCK mono_pos_set posY="25"/>' + "\r\n".join(lines)
    message_data = synthetic_files.message_file_bytes([("entry_story", ["other language", story]), ("entry_bubble", ["other language", STORY_LINE_PREFIX + "bubble"])])
    font_file_data = synthetic_files.fbfo_font_file(synthetic_files.box_font_bytes(), FONT_KEY)
    return game_files() + [(MESSAGE_PATH, message_data, True), (FONT_PATH, font_file_data, True)]


def text_set_description():
    set_description = tiny_set_description()
    set_description["narration_font"] = FONT_PATH
    set_description["narration"] = [{"name": "m01", "file": MESSAGE_PATH, "entry": "entry_story"}]
    set_description["bubble_text"] = [{"name": "bubble_words", "file": MESSAGE_PATH, "entry": "entry_bubble", "font": FONT_PATH, "pixels_per_em": 20, "color": [0, 0, 0]}]
    set_description["composed_textures"] = [{
        "name": "bubble_body",
        "source": PAGE_1_PATH,
        "width": 16,
        "height": 8,
        "parts": [{"crop": [0, 0, 32, 32], "x": 0, "y": 0, "width": 16, "height": 8}],
    }]
    return set_description


def all_files_under(folder):
    paths = []
    for parent, _, names in os.walk(folder):
        for name in names:
            paths.append(os.path.join(parent, name))
    return paths


class EndToEndTests(unittest.TestCase):
    def setUp(self):
        self.folder = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, self.folder)
        self.apk_folder = os.path.join(self.folder, "apk")
        self.output_folder = os.path.join(self.folder, "output")
        os.makedirs(self.apk_folder)
        synthetic_files.write_apk_folder(self.apk_folder, game_files())
        synthetic_files.write_text_key(self.apk_folder)
        tool_path = convert_assets.TEXTURE_TOOL_PATH
        convert_assets.TEXTURE_TOOL_PATH = CHECKED_TOOL_PATH
        self.addCleanup(setattr, convert_assets, "TEXTURE_TOOL_PATH", tool_path)

    def convert(self, set_description):
        text_key = narration.load_text_key(set_description, self.apk_folder)
        with contextlib.redirect_stdout(io.StringIO()):
            convert_assets.convert_set_description(set_description, self.output_folder, self.apk_folder, text_key)

    def read_text(self, *parts):
        with open(os.path.join(self.output_folder, *parts), "r", encoding="ascii") as source:
            return source.read()

    def read_gtex(self, name):
        with open(os.path.join(self.output_folder, "textures", name + ".gtex"), "rb") as source:
            return gtex_pack.unpack_gtex(source.read())

    def read_gtex_header(self, name):
        data = self.read_gtex(name)
        self.assertEqual(data[:4], b"GTEX")
        return struct.unpack_from("<IIII", data, 4), len(data)

    def first_pixel_red(self, name):
        data = self.read_gtex(name)
        return synthetic_files.decode_color_block(data[TEXTURE_FILE_HEADER_SIZE:TEXTURE_FILE_HEADER_SIZE + DXT1_BLOCK_BYTES])[0][0]

    def visible_block_color_endpoints(self, name):
        data = self.read_gtex(name)
        endpoints = set()
        for offset in range(TEXTURE_FILE_HEADER_SIZE, len(data), DXT5_BLOCK_BYTES):
            if data[offset] != 0 or data[offset + 1] != 0:
                endpoints.add(struct.unpack_from("<HH", data, offset + DXT5_COLOR_ENDPOINTS_OFFSET))
        return endpoints

    def run_converter_on_the_text_set(self, set_description=None, files=None):
        synthetic_files.write_apk_folder(self.apk_folder, files or text_game_files())
        set_path = os.path.join(self.folder, "text_set")
        with open(set_path + ".json", "w", encoding="ascii") as output:
            json.dump(set_description or text_set_description(), output)
        command = [sys.executable, "-I", convert_assets.__file__, "--set", set_path, "--output", self.output_folder, "--apk-folder", self.apk_folder]
        return subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

    def assert_converter_stopped_plainly(self, result, wanted_words):
        self.assertEqual(result.returncode, 1)
        self.assertIn(wanted_words, result.stderr)
        self.assertIn("extract_text_key.py with your own copy of the Android package", result.stderr)
        self.assertNotIn("Traceback", result.stderr)
        self.assertNotIn(STORY_LINE_PREFIX, result.stdout + result.stderr)
        self.assertFalse(os.path.exists(self.output_folder))

    def test_a_text_set_with_no_key_file_stops_before_writing_anything(self):
        os.remove(os.path.join(self.apk_folder, "text_key.bin"))
        self.assert_converter_stopped_plainly(self.run_converter_on_the_text_set(), "no text key at")

    def test_a_key_file_of_the_wrong_size_stops_before_writing_anything(self):
        synthetic_files.write_text_key(self.apk_folder, bytes(17))
        self.assert_converter_stopped_plainly(self.run_converter_on_the_text_set(), "is the wrong size")

    def test_the_wrong_key_stops_before_writing_anything(self):
        synthetic_files.write_text_key(self.apk_folder, bytes(range(16, 32)))
        self.assert_converter_stopped_plainly(self.run_converter_on_the_text_set(), "did not decode " + MESSAGE_PATH)

    def test_the_right_key_gets_through_the_command_line(self):
        self.assertEqual(self.run_converter_on_the_text_set().returncode, 0)
        self.assertTrue(os.path.isfile(os.path.join(self.output_folder, "textures", "text_m01_0.gtex")))

    def assert_key_not_printed(self, result, key):
        printed = (result.stdout + result.stderr).encode("utf-8")
        for form in synthetic_files.key_text_forms(key):
            self.assertNotIn(form, printed)

    def test_a_finished_conversion_prints_no_form_of_the_key(self):
        result = self.run_converter_on_the_text_set()
        self.assertEqual(result.returncode, 0)
        self.assert_key_not_printed(result, synthetic_files.FAKE_TEXT_KEY)

    def test_a_key_file_of_the_wrong_size_is_not_printed(self):
        synthetic_files.write_text_key(self.apk_folder, synthetic_files.FAKE_TEXT_KEY + bytes(1))
        result = self.run_converter_on_the_text_set()
        self.assertEqual(result.returncode, 1)
        self.assert_key_not_printed(result, synthetic_files.FAKE_TEXT_KEY)

    def test_the_wrong_key_is_not_printed(self):
        wrong_key = bytes(range(16, 32))
        synthetic_files.write_text_key(self.apk_folder, wrong_key)
        result = self.run_converter_on_the_text_set()
        self.assertEqual(result.returncode, 1)
        self.assert_key_not_printed(result, wrong_key)

    def test_a_traceback_from_a_failed_conversion_does_not_print_the_key(self):
        set_description = text_set_description()
        set_description["screen_textures"].append({"name": "card_corner", "source": PLATE_PATH, "width": 8, "height": 16})
        result = self.run_converter_on_the_text_set(set_description)
        self.assertEqual(result.returncode, 1)
        self.assertIn("card_corner", result.stderr)
        self.assert_key_not_printed(result, synthetic_files.FAKE_TEXT_KEY)

    def test_a_wrong_entry_name_stops_plainly_without_blaming_the_key(self):
        set_description = text_set_description()
        set_description["narration"][0]["entry"] = "entry_typo"
        result = self.run_converter_on_the_text_set(set_description)
        self.assertEqual(result.returncode, 1)
        self.assertIn("error: " + MESSAGE_PATH + ": no such entry: entry_typo", result.stderr)
        self.assertNotIn("extract_text_key", result.stderr)
        self.assertNotIn("Traceback", result.stderr)
        self.assertFalse(os.path.exists(self.output_folder))

    def test_a_tag_nothing_knows_stops_plainly_without_quoting_the_story(self):
        story = '<TRCK mono_pos_set posY="25"/>' + STORY_LINE_PREFIX + '<TRCK made_up_verb />'
        result = self.run_converter_on_the_text_set(files=text_game_files(story=story))
        self.assertEqual(result.returncode, 1)
        self.assertIn("error: m01: ", result.stderr)
        self.assertNotIn("Traceback", result.stderr)
        self.assertNotIn(STORY_LINE_PREFIX, result.stdout + result.stderr)
        self.assertFalse(os.path.exists(self.output_folder))

    def test_two_textures_of_one_name_stop_before_anything_is_written(self):
        clashes = {
            "bubble text and composed": lambda set_description: set_description["bubble_text"][0].update(name="bubble_body"),
            "narration line and composed": lambda set_description: set_description["composed_textures"][0].update(name="text_m01_1"),
            "plate and screen texture": lambda set_description: set_description["plates"][0].update(name="card"),
            "atlas page and plate": lambda set_description: set_description["plates"][0].update(name="st01_a_000_00"),
        }
        for label, make_clash in clashes.items():
            with self.subTest(label):
                set_description = text_set_description()
                make_clash(set_description)
                result = self.run_converter_on_the_text_set(set_description)
                self.assertEqual(result.returncode, 1)
                self.assertIn("another texture has this name", result.stderr)
                self.assertNotIn("Traceback", result.stderr)
                self.assertFalse(os.path.exists(self.output_folder))

    def test_a_stray_manifest_temporary_file_goes_on_a_run_that_writes_nothing_new(self):
        self.convert(tiny_set_description())
        stray_path = os.path.join(self.output_folder, "manifest.txt.new")
        with open(stray_path, "wb") as output:
            output.write(b"part")
        self.convert(tiny_set_description())
        self.assertFalse(os.path.exists(stray_path))

    def test_a_set_without_text_needs_no_key_file(self):
        os.remove(os.path.join(self.apk_folder, "text_key.bin"))
        self.convert(tiny_set_description())
        self.assertIn("background plate", self.read_text("manifest.txt"))

    def start_converter(self, temp_folder, stderr=subprocess.DEVNULL):
        synthetic_files.write_apk_folder(self.apk_folder, long_text_game_files())
        set_path = os.path.join(self.folder, "text_set")
        with open(set_path + ".json", "w", encoding="ascii") as output:
            json.dump(text_set_description(), output)
        os.makedirs(temp_folder)
        command = [sys.executable, "-I", convert_assets.__file__, "--set", set_path, "--output", self.output_folder, "--apk-folder", self.apk_folder]
        return subprocess.Popen(command, env=dict(os.environ, TMPDIR=temp_folder), stdout=subprocess.DEVNULL, stderr=stderr)

    def wait_for_decoded_font(self, process, temp_folder):
        deadline = time.monotonic() + CONVERTER_WAIT_SECONDS
        while time.monotonic() < deadline and process.poll() is None:
            for path in all_files_under(temp_folder) + all_files_under(self.output_folder):
                if path.endswith(".otf"):
                    return path
            time.sleep(0.002)
        process.kill()
        process.wait()
        self.fail("the converter never wrote its decoded font")

    def convert_text_set(self):
        synthetic_files.write_apk_folder(self.apk_folder, text_game_files())
        self.convert(text_set_description())

    def test_text_textures_are_compressed_white_or_black_with_alpha(self):
        self.convert_text_set()
        for name in ("text_m01_0", "text_m01_1", "text_m01_2", "bubble_words", "bubble_body"):
            header, size = self.read_gtex_header(name)
            self.assertEqual(header[2], 1)
            self.assertEqual(size, 20 + header[3])
            self.assertEqual(header[0] % 4, 0)
            self.assertEqual(header[1] % 4, 0)
        for name in ("text_m01_0", "text_m01_1", "text_m01_2"):
            self.assertEqual(self.visible_block_color_endpoints(name), {(WHITE_RGB565, WHITE_RGB565)})
        self.assertEqual(self.visible_block_color_endpoints("bubble_words"), {(BLACK_RGB565, BLACK_RGB565)})

    def test_text_is_not_listed_for_browsing(self):
        self.convert_text_set()
        self.assertNotIn("text_m01", self.read_text("manifest.txt"))
        self.assertNotIn("bubble", self.read_text("manifest.txt"))

    def assert_output_holds_no_font_or_story(self):
        font_start = synthetic_files.box_font_bytes()[:64]
        for path in all_files_under(self.output_folder):
            with open(path, "rb") as source:
                data = source.read()
            self.assertNotIn(STORY_LINE_PREFIX.encode("ascii"), data, path)
            self.assertNotIn(STORY_LINE_PREFIX.encode("utf-16-le"), data, path)
            self.assertNotIn(font_start, data, path)
            self.assertFalse(path.endswith((".otf", ".ttf", ".rgba", ".dds", ".txt")), path)
            self.assertNotIn(convert_assets.WORK_FOLDER_PREFIX, path)

    def test_no_story_text_or_decoded_font_is_left_in_the_output(self):
        self.convert_text_set()
        self.assertEqual(sorted(os.listdir(self.output_folder)), [bake_record.RECORD_FILE_NAME, "animations", "manifest.txt", "music", "music_ds", "pause", "scenes", "scripts", "sounds", "textures"])
        font_start = synthetic_files.box_font_bytes()[:64]
        for path in all_files_under(self.output_folder):
            with open(path, "rb") as source:
                data = source.read()
            self.assertNotIn(STORY_LINE_PREFIX.encode("ascii"), data, path)
            self.assertNotIn(STORY_LINE_PREFIX.encode("utf-16-le"), data, path)
            self.assertNotIn(font_start, data, path)
            self.assertFalse(path.endswith((".otf", ".ttf", ".rgba", ".dds")), path)

    def test_a_stop_while_stopping_leaves_no_font_or_text_and_no_traceback(self):
        for signal_number in (signal.SIGINT, signal.SIGTERM):
            for gap_seconds in (0, 0.02, 0.1):
                with self.subTest(signal_number=signal_number, gap_seconds=gap_seconds):
                    temp_folder = os.path.join(self.folder, "temp_twice_%d_%d" % (signal_number, gap_seconds * 1000))
                    process = self.start_converter(temp_folder, stderr=subprocess.PIPE)
                    self.wait_for_decoded_font(process, temp_folder)
                    process.send_signal(signal_number)
                    time.sleep(gap_seconds)
                    if process.poll() is None:
                        process.send_signal(signal_number)
                    _, errors = process.communicate(timeout=CONVERTER_WAIT_SECONDS)
                    self.assertEqual(process.returncode, 128 + signal_number)
                    self.assertNotIn(b"Traceback", errors)
                    self.assertEqual(all_files_under(temp_folder), [])
                    self.assert_output_holds_no_font_or_story()

    def test_a_terminated_converter_leaves_no_font_or_text_anywhere(self):
        for signal_number in (signal.SIGINT, signal.SIGTERM, signal.SIGHUP):
            with self.subTest(signal_number=signal_number):
                temp_folder = os.path.join(self.folder, "temp_%d" % signal_number)
                process = self.start_converter(temp_folder)
                font_path = self.wait_for_decoded_font(process, temp_folder)
                self.assertTrue(font_path.startswith(temp_folder + os.sep), "the font must not be written beside the output")
                process.send_signal(signal_number)
                self.assertEqual(process.wait(timeout=CONVERTER_WAIT_SECONDS), 128 + signal_number)
                self.assertEqual(all_files_under(temp_folder), [])
                self.assert_output_holds_no_font_or_story()

    def test_even_a_killed_converter_leaves_no_font_or_story_in_the_output_folder(self):
        temp_folder = os.path.join(self.folder, "temp_killed")
        process = self.start_converter(temp_folder)
        self.wait_for_decoded_font(process, temp_folder)
        process.kill()
        process.wait()
        self.assert_output_holds_no_font_or_story()

    def test_a_finished_conversion_leaves_nothing_in_the_temp_folder(self):
        temp_folder = os.path.join(self.folder, "temp_finished")
        os.makedirs(temp_folder)
        with mock.patch.object(tempfile, "tempdir", temp_folder):
            self.convert_text_set()
        self.assertEqual(os.listdir(temp_folder), [])

    def test_a_conversion_removes_a_work_folder_a_killed_run_left_long_ago(self):
        temp_folder = os.path.join(self.folder, "temp_old")
        old_work_folder = os.path.join(temp_folder, convert_assets.WORK_FOLDER_PREFIX + "killed")
        os.makedirs(old_work_folder)
        with open(os.path.join(old_work_folder, "font_0.otf"), "wb") as output:
            output.write(b"font")
        long_ago = time.time() - convert_assets.OLD_WORK_FOLDER_SECONDS - 60
        os.utime(old_work_folder, (long_ago, long_ago))
        with mock.patch.object(tempfile, "tempdir", temp_folder):
            self.convert(tiny_set_description())
        self.assertEqual(os.listdir(temp_folder), [])

    def test_the_summary_never_prints_story_text(self):
        synthetic_files.write_apk_folder(self.apk_folder, text_game_files())
        output = io.StringIO()
        with contextlib.redirect_stdout(output):
            convert_assets.convert_set_description(text_set_description(), self.output_folder, self.apk_folder, synthetic_files.FAKE_TEXT_KEY)
        self.assertNotIn(STORY_LINE_PREFIX, output.getvalue())
        self.assertIn("narration: 1 messages, 3 lines", output.getvalue())
        self.assertIn("narration lines squeezed: none", output.getvalue())

    def test_a_small_set_converts_completely(self):
        self.convert(tiny_set_description())
        self.assertEqual(self.read_text("manifest.txt"), "\n".join([
            "art remastered",
            "background plate",
            "background card",
            "animation st01_a_000",
            "animation st01_a_000_f01",
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

    def test_a_looped_sound_gets_a_loop_file_too(self):
        set_description = tiny_set_description()
        set_description["sounds"] = [{"name": "hum", "source": LOOPED_SONG_PATH}]
        self.convert(set_description)
        self.assertEqual(self.read_text("sounds", "hum.loop"), "4 9\n")

    def test_a_set_without_audio_still_converts(self):
        set_description = tiny_set_description()
        del set_description["music"]
        del set_description["sounds"]
        self.convert(set_description)
        self.assertEqual(os.listdir(os.path.join(self.output_folder, "music")), [])

    def test_an_effect_at_another_rate_is_resampled_to_the_playback_rate(self):
        set_description = tiny_set_description()
        set_description["sounds"] = [{"name": "ping", "source": ODD_RATE_PING_PATH}]
        self.convert(set_description)
        self.assertEqual(sample_rate_of(os.path.join(self.output_folder, "sounds", "ping.ogg")), 44100)

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
        with open(os.path.join(self.output_folder, "textures", "plate.gtex"), "rb") as source:
            first_plate = source.read()
        self.convert(tiny_set_description())
        self.assertFalse(os.path.exists(stale_path))
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

    def text_set_with_its_own_bubble_font(self):
        set_description = text_set_description()
        set_description["bubble_text"][0]["font"] = BUBBLE_FONT_PATH
        return set_description

    def bake_text_set_with(self, set_description=None, **file_changes):
        synthetic_files.write_apk_folder(self.apk_folder, text_game_files(**file_changes))
        with mock.patch.object(line_renderer, "render_line", wraps=line_renderer.render_line) as render:
            with mock.patch.object(texture_encoder, "encode_rgba", wraps=texture_encoder.encode_rgba) as encode:
                with mock.patch.object(font_file, "decode_font", wraps=font_file.decode_font) as decode:
                    self.convert(set_description or self.text_set_with_its_own_bubble_font())
        return {
            "rendered": sorted(call.args[1] for call in render.call_args_list),
            "encoded": sorted(call.args[1] for call in encode.call_args_list),
            "fonts_decoded": decode.call_count,
        }

    def nothing_baked(self):
        return {"rendered": [], "encoded": [], "fonts_decoded": 0}

    def test_a_repeat_run_renders_and_encodes_no_text_or_composed_texture(self):
        self.bake_text_set_with()
        before = {}
        for relative_path in self.output_tree():
            path = os.path.join(self.output_folder, relative_path)
            os.utime(path, (1000, 1000))
            before[relative_path] = 1000
        self.assertEqual(self.bake_text_set_with(), self.nothing_baked())
        for relative_path, moment in before.items():
            self.assertEqual(os.path.getmtime(os.path.join(self.output_folder, relative_path)), moment, relative_path)

    def test_a_repeat_run_counts_every_texture_as_unchanged_and_reports_the_same_widths(self):
        synthetic_files.write_apk_folder(self.apk_folder, text_game_files())
        first = self.summary_of_a_run(text_set_description())
        second = self.summary_of_a_run(text_set_description())
        self.assertIn("textures: 10, 10 baked, 0 unchanged", first)
        self.assertIn("textures: 10, 0 baked, 10 unchanged", second)
        self.assertEqual(self.narration_summary_lines(first), self.narration_summary_lines(second))

    def summary_of_a_run(self, set_description):
        output = io.StringIO()
        with contextlib.redirect_stdout(output):
            convert_assets.convert_set_description(set_description, self.output_folder, self.apk_folder, synthetic_files.FAKE_TEXT_KEY)
        return output.getvalue()

    def narration_summary_lines(self, summary):
        lines = [line for line in summary.splitlines() if line.startswith("narration")]
        self.assertEqual(len(lines), 2)
        return lines

    def test_a_changed_line_bakes_only_that_line(self):
        self.bake_text_set_with()
        baked = self.bake_text_set_with(story=story_text(last_line="other"))
        self.assertEqual(baked["rendered"], ["text_m01_2"])
        self.assertEqual(baked["encoded"], ["text_m01_2"])
        self.assertEqual(baked["fonts_decoded"], 1)

    def test_changed_bubble_text_bakes_only_the_bubble(self):
        self.bake_text_set_with()
        baked = self.bake_text_set_with(bubble="different")
        self.assertEqual(baked["rendered"], ["bubble_words"])
        self.assertEqual(baked["encoded"], ["bubble_words"])

    def test_a_moved_message_bakes_nothing(self):
        self.bake_text_set_with()
        self.assertEqual(self.bake_text_set_with(story=story_text(pos_y=30)), self.nothing_baked())
        clean = self.output_tree()
        shutil.rmtree(self.output_folder)
        self.bake_text_set_with(story=story_text(pos_y=30))
        self.assertEqual(clean, self.output_tree())

    def test_a_changed_narration_font_bakes_only_the_lines_drawn_with_it(self):
        self.bake_text_set_with()
        baked = self.bake_text_set_with(narration_font_key=OTHER_FONT_KEY)
        self.assertEqual(baked["rendered"], ["text_m01_0", "text_m01_1", "text_m01_2"])
        self.assertEqual(baked["encoded"], ["text_m01_0", "text_m01_1", "text_m01_2"])
        self.assertEqual(baked["fonts_decoded"], 1)

    def test_a_changed_bubble_font_bakes_only_the_bubble(self):
        self.bake_text_set_with()
        baked = self.bake_text_set_with(bubble_font_key=OTHER_FONT_KEY)
        self.assertEqual(baked["rendered"], ["bubble_words"])

    def test_changed_bubble_settings_bake_only_the_bubble(self):
        self.bake_text_set_with()
        for change in ({"pixels_per_em": 24}, {"color": [255, 0, 0]}):
            set_description = self.text_set_with_its_own_bubble_font()
            set_description["bubble_text"][0].update(change)
            self.assertEqual(self.bake_text_set_with(set_description)["rendered"], ["bubble_words"])

    def test_a_changed_composed_description_bakes_only_that_texture(self):
        self.bake_text_set_with()
        set_description = self.text_set_with_its_own_bubble_font()
        set_description["composed_textures"][0]["parts"][0]["x"] = 2
        baked = self.bake_text_set_with(set_description)
        self.assertEqual(baked["encoded"], ["bubble_body"])
        self.assertEqual(baked["rendered"], [])

    def test_a_changed_composed_source_bakes_it_and_no_text(self):
        synthetic_files.write_apk_folder(self.apk_folder, text_game_files())
        self.convert(self.text_set_with_its_own_bubble_font())
        changed_files = [(path, data + b"\0" if path == PAGE_1_PATH else data, in_first_pack) for path, data, in_first_pack in text_game_files()]
        synthetic_files.write_apk_folder(self.apk_folder, changed_files)
        with mock.patch.object(texture_encoder, "encode_rgba", wraps=texture_encoder.encode_rgba) as encode:
            with mock.patch.object(line_renderer, "render_line", wraps=line_renderer.render_line) as render:
                self.convert(self.text_set_with_its_own_bubble_font())
        self.assertEqual(sorted(call.args[1] for call in encode.call_args_list), ["bubble_body", "st01_a_000_01"])
        render.assert_not_called()

    def test_changed_text_code_bakes_all_the_text_and_nothing_else(self):
        self.bake_text_set_with()
        with mock.patch.object(convert_assets, "text_code_fingerprint", return_value="other code"):
            baked = self.bake_text_set_with()
        self.assertEqual(baked["encoded"], ["bubble_words", "text_m01_0", "text_m01_1", "text_m01_2"])

    def test_changed_composed_code_bakes_only_composed_textures(self):
        self.bake_text_set_with()
        with mock.patch.object(convert_assets, "composed_code_fingerprint", return_value="other code"):
            baked = self.bake_text_set_with()
        self.assertEqual(baked["encoded"], ["bubble_body"])

    def test_the_code_lists_name_what_each_kind_of_texture_is_made_with(self):
        for file_name in ("asset_writers.py", "font_file.py", "line_renderer.py", "message_file.py", "narration.py", "texture_encoder.py"):
            self.assertIn(file_name, convert_assets.TEXT_CODE_FILES)
        self.assertIn("bubble_art.py", convert_assets.COMPOSED_CODE_FILES)
        for file_name in convert_assets.TEXTURE_CODE_FILES:
            self.assertIn(file_name, convert_assets.COMPOSED_CODE_FILES)

    def test_a_deleted_text_texture_is_baked_again_alone(self):
        self.bake_text_set_with()
        os.remove(os.path.join(self.output_folder, "textures", "text_m01_1.gtex"))
        self.assertEqual(self.bake_text_set_with()["rendered"], ["text_m01_1"])

    def test_the_record_holds_no_form_of_the_key_no_story_and_no_font(self):
        self.bake_text_set_with()
        with open(os.path.join(self.output_folder, bake_record.RECORD_FILE_NAME), "rb") as source:
            data = source.read()
        for form in synthetic_files.key_text_forms(synthetic_files.FAKE_TEXT_KEY):
            self.assertNotIn(form, data)
        self.assertNotIn(hashlib.sha256(synthetic_files.FAKE_TEXT_KEY).hexdigest().encode("ascii"), data)
        self.assertNotIn(STORY_LINE_PREFIX.encode("ascii"), data)
        record = json.loads(data)
        self.assertEqual(sorted(record["textures/text_m01_0.gtex"]), ["details", "key", "output"])
        self.assertEqual(list(record["textures/text_m01_0.gtex"]["details"]), ["natural_width"])

    def test_a_different_key_that_decodes_the_same_text_bakes_nothing(self):
        self.bake_text_set_with()
        synthetic_files.write_text_key(self.apk_folder, SECOND_TEXT_KEY)
        self.assertEqual(self.bake_text_set_with(text_key=SECOND_TEXT_KEY), self.nothing_baked())

    def test_building_text_over_an_old_set_gives_the_same_files_as_a_clean_build(self):
        old_set = self.text_set_with_its_own_bubble_font()
        old_set["bubble_text"][0]["color"] = [255, 0, 0]
        old_set["composed_textures"][0]["parts"][0]["x"] = 2
        old_set["narration"] = [{"name": "old_message", "file": MESSAGE_PATH, "entry": "entry_story"}]
        self.bake_text_set_with(old_set, story=story_text(pos_y=30, last_line="older"), narration_font_key=OTHER_FONT_KEY)
        self.bake_text_set_with()
        incremental = self.output_tree()
        self.assertFalse(os.path.exists(os.path.join(self.output_folder, "textures", "text_old_message_0.gtex")))
        shutil.rmtree(self.output_folder)
        self.bake_text_set_with()
        self.assertEqual(incremental, self.output_tree())

    def test_a_line_that_fails_is_named_and_the_last_good_set_stays(self):
        self.bake_text_set_with()
        before = self.output_tree()
        real_render_line = line_renderer.render_line

        def fail_one_line(work_folder, name, *arguments):
            if name == "text_m01_1":
                raise line_renderer.LineRenderError("magick could not draw a line with its font")
            return real_render_line(work_folder, name, *arguments)

        synthetic_files.write_apk_folder(self.apk_folder, text_game_files(story=story_text(last_line="other"), bubble="different", narration_font_key=OTHER_FONT_KEY))
        with mock.patch.object(line_renderer, "render_line", side_effect=fail_one_line):
            with self.assertRaises(convert_assets.ConversionFailed) as caught:
                self.convert(self.text_set_with_its_own_bubble_font())
        self.assertEqual([name for name, _ in caught.exception.failures], ["text_m01_1"])
        self.assertNotIn(STORY_LINE_PREFIX, str(caught.exception))
        after = self.output_tree()
        self.assertEqual(after["manifest.txt"], before["manifest.txt"])
        self.assertEqual(sorted(after), sorted(before))
        self.assertEqual(after[os.path.join("textures", "text_m01_1.gtex")], before[os.path.join("textures", "text_m01_1.gtex")])
        self.assertNotEqual(after[os.path.join("textures", "bubble_words.gtex")], before[os.path.join("textures", "bubble_words.gtex")])

    def test_a_failed_bubble_line_and_a_failed_composed_texture_are_both_named(self):
        set_description = self.text_set_with_its_own_bubble_font()
        set_description["composed_textures"][0]["source"] = "natives/android/not/there.tex"
        files = [(path, b"bad font" if path == BUBBLE_FONT_PATH else data, in_first_pack) for path, data, in_first_pack in text_game_files()]
        synthetic_files.write_apk_folder(self.apk_folder, files)
        with self.assertRaises(convert_assets.ConversionFailed) as caught:
            self.convert(set_description)
        self.assertEqual([name for name, _ in caught.exception.failures], ["bubble_words", "bubble_body"])
        self.assertEqual(sorted(os.listdir(os.path.join(self.output_folder, "scenes"))), [])

    def test_the_command_line_names_a_failed_line_without_a_traceback_or_story(self):
        files = [(path, b"bad font" if path == FONT_PATH else data, in_first_pack) for path, data, in_first_pack in text_game_files()]
        set_path = os.path.join(self.folder, "text_set")
        with open(set_path + ".json", "w", encoding="ascii") as output:
            json.dump(text_set_description(), output)
        synthetic_files.write_apk_folder(self.apk_folder, files)
        command = [sys.executable, "-I", convert_assets.__file__, "--set", set_path, "--output", self.output_folder, "--apk-folder", self.apk_folder]
        result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        self.assertEqual(result.returncode, 1)
        self.assertIn("failed: text_m01_0: ", result.stderr)
        self.assertNotIn("Traceback", result.stderr)
        self.assertNotIn(STORY_LINE_PREFIX, result.stdout + result.stderr)

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
        self.assertEqual(self.read_text("manifest.txt"), "art remastered\nbackground plate\nanimation st01_a_000\n")
        self.assertEqual(os.listdir(os.path.join(self.output_folder, "scenes")), [])

    def test_a_set_description_can_be_named_by_its_path(self):
        path = os.path.join(self.folder, "my_set.json")
        with open(path, "w", encoding="ascii") as output:
            output.write('{"scene_name": "x"}')
        self.assertEqual(convert_assets.load_set_description(path), {"scene_name": "x"})

if __name__ == "__main__":
    unittest.main()
