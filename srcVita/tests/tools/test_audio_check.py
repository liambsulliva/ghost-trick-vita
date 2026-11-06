import contextlib
import io
import os
import re
import shutil
import subprocess
import sys
import tempfile
import unittest
from unittest import mock

import audio_check

TESTS_FOLDER = os.path.dirname(os.path.abspath(__file__))
SRC_VITA_FOLDER = os.path.dirname(os.path.dirname(TESTS_FOLDER))
LOG_WRITER_PATH = os.path.join(SRC_VITA_FOLDER, "build", "tools", "music_log_writer")
SCENE_TEXT = "scene opening\nmusic song 1\nsound ping\nwait 60\nsound pong\nmusic_stop 4\n"
ERROR_LINE_STARTS_THAT_MUST_FAIL = (
    "music port_open_failed", "music submit_failed", "music out_of_memory", "music file_missing", "music decoder_failed",
    "music wrong_rate", "music bad_loop_file", "music thread_failed", "music path_too_long",
    "sound port_open_failed", "sound submit_failed", "sound out_of_memory", "sound file_missing", "sound decoder_failed",
    "sound wrong_rate", "sound thread_failed", "sound path_too_long", "sound too_many_clips",
    "scene audio_wait_gave_up", "log dropped_bytes",
)


def with_field(line, key, value):
    return re.sub(r"\b" + key + r"=\S+", key + "=" + str(value), line)


class AudioCheckTestCase(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.folder = tempfile.mkdtemp()
        cls.assets_folder = os.path.join(cls.folder, "assets")
        os.makedirs(os.path.join(cls.assets_folder, "music"))
        os.makedirs(os.path.join(cls.assets_folder, "scenes"))
        cls.log_path = os.path.join(cls.folder, "run.log")
        subprocess.run([LOG_WRITER_PATH, cls.log_path, os.path.join(cls.assets_folder, "music", "song.ogg")], check=True)
        with open(os.path.join(cls.assets_folder, "scenes", "opening.scene"), "w", encoding="ascii") as output:
            output.write(SCENE_TEXT)
        with open(cls.log_path, encoding="ascii") as source:
            cls.good_lines = source.readlines()

    @classmethod
    def tearDownClass(cls):
        shutil.rmtree(cls.folder)

    def failing_checks(self, lines):
        messages = audio_check.latest_run_messages(lines)
        results = audio_check.run_checks(messages, self.assets_folder, audio_check.DEFAULT_REFERENCE_PATH)
        return {name: problems for name, problems in results if problems}

    def changed_first(self, line_start, change):
        lines = list(self.good_lines)
        for index, line in enumerate(lines):
            if audio_check.message_of(line).startswith(line_start):
                lines[index] = change(line)
                return lines
        self.fail("no line starts with " + line_start)

    def changed_all(self, line_start, change):
        return [change(line) if audio_check.message_of(line).startswith(line_start) else line for line in self.good_lines]

    def without(self, line_start):
        return [line for line in self.good_lines if not audio_check.message_of(line).startswith(line_start)]

    def assert_only_failure(self, lines, name):
        failures = self.failing_checks(lines)
        self.assertEqual(list(failures), [name], failures)


class GoodLogTests(AudioCheckTestCase):
    def test_a_log_written_by_the_real_engine_passes_every_check(self):
        self.assertEqual(self.failing_checks(self.good_lines), {})

    def test_the_log_covers_a_starve_a_loop_and_a_ramp_after_a_gap(self):
        text = "".join(self.good_lines)
        self.assertIn("command music_starve ms=300", text)
        self.assertIn("music ramp at=614400 frames=512 from=0 to=32768", text)
        self.assertIn("loops=", text)
        self.assertNotIn("loops=0 decode_us_avg=0 decode_us_max=0 block_us_avg=92879 block_us_max=92879 gap_us_max=300000", text)

    def test_only_the_latest_run_counts(self):
        older_run = ["[      0.000] --- launch\n", "[      1.000] music port_open_failed code=0x80260002\n"]
        self.assertEqual(self.failing_checks(older_run + self.good_lines), {})

    def test_main_prints_a_verdict_and_returns_zero(self):
        output = io.StringIO()
        with mock.patch.object(sys, "argv", ["audio_check.py", "--log", self.log_path, "--assets", self.assets_folder]):
            with contextlib.redirect_stdout(output):
                self.assertEqual(audio_check.main(), 0)
        self.assertIn("PASS decode budget", output.getvalue())
        self.assertIn("measured: worst decode share 0.0%, decode_us_max 0 us", output.getvalue())


class WindowTests(AudioCheckTestCase):
    def test_a_missing_window_fails(self):
        lines = list(self.good_lines)
        window_lines = [index for index, line in enumerate(lines) if "music window" in line]
        del lines[window_lines[3]]
        failures = self.failing_checks(lines)
        self.assertIn("windows are contiguous", failures)

    def test_reordered_windows_fail(self):
        lines = list(self.good_lines)
        window_lines = [index for index, line in enumerate(lines) if "music window" in line]
        lines[window_lines[3]], lines[window_lines[4]] = lines[window_lines[4]], lines[window_lines[3]]
        self.assertIn("windows are contiguous", self.failing_checks(lines))

    def test_a_wrong_frame_count_fails(self):
        self.assertIn("windows are contiguous", self.failing_checks(self.changed_first("music window", lambda line: with_field(line, "frames", 4096))))

    def test_a_stopped_line_that_disagrees_fails(self):
        self.assertIn("windows are contiguous", self.failing_checks(self.changed_first("music stopped", lambda line: with_field(line, "submitted", 4096))))
        self.assertIn("windows are contiguous", self.failing_checks(self.changed_first("music stopped", lambda line: with_field(line, "crc_total", "00000000"))))
        self.assertIn("windows are contiguous", self.failing_checks(self.without("music stopped")))

    def test_no_music_at_all_fails(self):
        self.assertIn("windows are contiguous", self.failing_checks(self.without("music started")))


class ChecksumTests(AudioCheckTestCase):
    def test_a_wrong_crc_fails(self):
        self.assert_only_failure(self.changed_first("music window", lambda line: with_field(line, "crc32", "00000000")), "checksums match the reference")

    def test_a_wrong_running_crc_fails(self):
        self.assert_only_failure(self.changed_first("music window", lambda line: with_field(line, "crc_total", "00000000")), "checksums match the reference")

    def test_a_wrong_source_position_fails(self):
        self.assert_only_failure(self.changed_first("music window", lambda line: with_field(line, "source_frame", 5)), "checksums match the reference")

    def test_a_wrong_loop_count_fails(self):
        self.assert_only_failure(self.changed_first("music window", lambda line: with_field(line, "loops", 99)), "checksums match the reference")

    def test_a_forgotten_ramp_fails(self):
        lines = [line for line in self.good_lines if "music ramp at=614400" not in line]
        self.assertIn("checksums match the reference", self.failing_checks(lines))

    def test_a_missing_ogg_file_is_reported(self):
        messages = audio_check.latest_run_messages(self.good_lines)
        with self.assertRaises(RuntimeError):
            audio_check.check_checksums(audio_check.read_playbacks(messages), os.path.join(self.folder, "nowhere"), audio_check.DEFAULT_REFERENCE_PATH)


class RateTests(AudioCheckTestCase):
    def scaled_wall_clock(self, factor):
        return self.changed_all("music window", lambda line: with_field(line, "wall_us", int(int(audio_check.fields_of(line)["wall_us"]) * factor)))

    def test_a_port_one_percent_off_fails(self):
        self.assertIn("position follows the wall clock", self.failing_checks(self.scaled_wall_clock(1.01)))

    def test_a_port_running_at_48000_fails(self):
        self.assertIn("position follows the wall clock", self.failing_checks(self.scaled_wall_clock(44100.0 / 48000.0)))

    def test_a_small_error_passes(self):
        self.assertNotIn("position follows the wall clock", self.failing_checks(self.scaled_wall_clock(1.002)))

    def test_frames_still_waiting_in_the_port_do_not_count_as_played(self):
        grains_of_frames = 45056
        windows = []
        for index in range(6):
            at = index * grains_of_frames
            rest = index * 2000
            played = at + grains_of_frames - rest
            windows.append({"at": str(at), "frames": str(grains_of_frames), "rest": str(rest), "wall_us": str(played * 1000000 // audio_check.MUSIC_RATE)})
        self.assertAlmostEqual(audio_check.fitted_rate(windows), audio_check.MUSIC_RATE, delta=1)

    def test_too_few_windows_to_fit_fail(self):
        lines = []
        kept = 0
        for line in self.good_lines:
            if "music window" in line:
                kept = kept + 1
                if kept > 3:
                    continue
            lines.append(line)
        self.assertIn("position follows the wall clock", self.failing_checks(lines))


class GapTests(AudioCheckTestCase):
    def with_a_replay_of_the_music(self, change_window=lambda line: line):
        started = [index for index, line in enumerate(self.good_lines) if "music started" in line][0]
        stopped = [index for index, line in enumerate(self.good_lines) if "music stopped" in line][0]
        scene_line = [line for line in self.good_lines if "scene started" in line][0]
        replay = []
        for line in self.good_lines[started:stopped + 1]:
            if "command music_starve" in line or "capture " in line:
                continue
            replay.append(change_window(line) if "music window" in line else line)
        return self.good_lines[:stopped + 1] + [scene_line] + replay + self.good_lines[stopped + 1:]

    def gap_problems(self, lines):
        messages = audio_check.latest_run_messages(lines)
        return audio_check.check_no_gaps_outside_the_starve_test(audio_check.read_playbacks(messages), messages)

    def test_a_gap_in_a_replay_after_the_starve_test_fails(self):
        self.assertIn("no gaps outside the starve test", self.failing_checks(self.with_a_replay_of_the_music()))

    def test_a_replay_without_a_gap_after_the_starve_test_passes(self):
        without_gaps = self.with_a_replay_of_the_music(lambda line: with_field(line, "late", 0))
        self.assertEqual(self.gap_problems(without_gaps), [])

    def test_the_gap_a_starve_test_causes_is_forgiven_only_once(self):
        problems = self.gap_problems(self.with_a_replay_of_the_music())
        self.assertEqual(len(problems), 1)
        self.assertIn("outside the starve test", problems[0])

    def test_underruns_before_the_starve_test_fail(self):
        self.assertIn("no gaps outside the starve test", self.failing_checks(self.changed_first("music window", lambda line: with_field(line, "underruns", 1))))

    def test_a_late_grain_after_the_starve_test_settled_fails(self):
        lines = list(self.good_lines)
        window_lines = [index for index, line in enumerate(lines) if "music window" in line]
        lines[window_lines[-2]] = with_field(lines[window_lines[-2]], "late", 2)
        lines[window_lines[-1]] = with_field(lines[window_lines[-1]], "late", 2)
        self.assertIn("no gaps outside the starve test", self.failing_checks(lines))

    def test_a_run_without_the_starve_test_fails(self):
        self.assertIn("the starve test fired the detector", self.failing_checks(self.without("command music_starve")))

    def test_a_detector_that_never_fires_fails(self):
        lines = self.changed_all("music window", lambda line: with_field(line, "late", 0))
        lines = [line for line in lines if "music ramp at=614400" not in line]
        self.assertIn("the starve test fired the detector", self.failing_checks(lines))

    def test_a_run_without_a_capture_stall_fails(self):
        self.assert_only_failure(self.without("capture "), "a capture stall did not reach the audio")

    def test_a_short_capture_does_not_count(self):
        self.assert_only_failure(self.changed_first("capture ", lambda line: with_field(line, "took_us", 500)), "a capture stall did not reach the audio")

    def test_a_capture_after_the_starve_test_does_not_count(self):
        lines = self.without("capture ")
        index = [position for position, line in enumerate(lines) if "command music_starve" in line][0]
        lines.insert(index + 1, "[      1.000] capture name=late result=0 took_us=447409\n")
        self.assertIn("a capture stall did not reach the audio", self.failing_checks(lines))

    def with_the_capture_at(self, find_position):
        lines = self.without("capture ")
        lines.insert(find_position(lines), "[      1.000] capture name=moved result=0 took_us=447409\n")
        return lines

    def position_of_first(self, lines, wanted):
        return [position for position, line in enumerate(lines) if wanted in line][0]

    def test_a_capture_before_any_music_does_not_count(self):
        self.assert_only_failure(self.with_the_capture_at(lambda lines: self.position_of_first(lines, "music started")), "a capture stall did not reach the audio")

    def test_a_capture_before_the_first_window_does_not_count(self):
        self.assert_only_failure(self.with_the_capture_at(lambda lines: self.position_of_first(lines, "music window")), "a capture stall did not reach the audio")

    def test_a_capture_after_the_last_window_does_not_count(self):
        lines = [line for line in self.without("capture ") if "command music_starve" not in line]
        lines.insert(self.position_of_first(lines, "music stopped"), "[      1.000] capture name=late result=0 took_us=447409\n")
        messages = audio_check.latest_run_messages(lines)
        self.assertEqual(audio_check.check_capture_stall_was_survived(audio_check.read_playbacks(messages), messages), ["no capture stall of at least 100000 us happened while music played"])

    def test_a_capture_after_the_stopped_line_does_not_count_even_with_windows_logged_later(self):
        lines = self.without("music stopped")
        stopped_line = [line for line in self.good_lines if "music stopped" in line][0]
        capture = self.position_of_first(lines, "capture name=stall")
        lines.insert(capture, stopped_line)
        self.assert_only_failure(lines, "a capture stall did not reach the audio")

    def test_a_capture_between_two_playbacks_does_not_count(self):
        lines = [line for line in self.without("capture ") if "command music_starve" not in line]
        started = self.position_of_first(lines, "music started")
        stopped = self.position_of_first(lines, "music stopped")
        replay = lines[started:stopped + 1]
        between = lines[:stopped + 1] + ["[      1.000] capture name=between result=0 took_us=447409\n"] + replay + lines[stopped + 1:]
        messages = audio_check.latest_run_messages(between)
        self.assertEqual(audio_check.check_capture_stall_was_survived(audio_check.read_playbacks(messages), messages), ["no capture stall of at least 100000 us happened while music played"])

    def test_a_useless_capture_is_skipped_when_a_later_one_lands_during_the_music(self):
        lines = list(self.good_lines)
        lines.insert(self.position_of_first(lines, "music started"), "[      1.000] capture name=early result=0 took_us=447409\n")
        self.assertEqual(self.failing_checks(lines), {})


class PortFactTests(AudioCheckTestCase):
    def test_adopt_zero_fails(self):
        self.assert_only_failure(self.changed_first("music port", lambda line: with_field(line, "adopt", 0)), "port facts")

    def test_a_wrong_rate_fails(self):
        self.assert_only_failure(self.changed_first("music port", lambda line: with_field(line, "rate", 48000)), "port facts")
        self.assert_only_failure(self.changed_first("sound port", lambda line: with_field(line, "rate", 48000)), "port facts")

    def test_a_wrong_grain_fails(self):
        self.assert_only_failure(self.changed_first("music port", lambda line: with_field(line, "grain", 1024)), "port facts")
        self.assert_only_failure(self.changed_first("sound port", lambda line: with_field(line, "grain", 1024)), "port facts")

    def test_a_mono_port_fails(self):
        self.assert_only_failure(self.changed_first("music port", lambda line: with_field(line, "mode", 0)), "port facts")
        self.assert_only_failure(self.changed_first("sound port", lambda line: with_field(line, "mode", 0)), "port facts")

    def test_a_sound_port_with_adopt_zero_fails(self):
        self.assert_only_failure(self.changed_first("sound port", lambda line: with_field(line, "adopt", 0)), "port facts")

    def test_zero_system_volume_fails(self):
        self.assert_only_failure(self.changed_first("music port", lambda line: with_field(line, "system_volume", 0)), "port facts")

    def test_a_system_volume_that_could_not_be_read_does_not_fail(self):
        self.assertEqual(self.failing_checks(self.changed_first("music port", lambda line: with_field(line, "system_volume", -1))), {})

    def test_a_missing_port_line_fails(self):
        self.assert_only_failure(self.without("sound port"), "port facts")


class BudgetTests(AudioCheckTestCase):
    def test_decoding_over_forty_percent_of_a_window_fails_and_names_the_fallback(self):
        failures = self.failing_checks(self.changed_first("music window", lambda line: with_field(line, "decode_us_avg", 40000)))
        self.assertIn("decode budget", failures)
        self.assertIn("scePowerSetArmClockFrequency(444)", failures["decode budget"][0])
        self.assertIn("ADPCM", failures["decode budget"][0])

    def test_only_a_later_window_over_budget_still_fails(self):
        lines = list(self.good_lines)
        window_lines = [index for index, line in enumerate(lines) if "music window" in line]
        later = window_lines[len(window_lines) // 2]
        lines[later] = with_field(lines[later], "decode_us_avg", 40000)
        failures = self.failing_checks(lines)
        self.assertIn("decode budget", failures)
        self.assertIn("scePowerSetArmClockFrequency(444)", failures["decode budget"][0])

    def test_decoding_just_under_forty_percent_passes(self):
        failures = self.failing_checks(self.changed_all("music window", lambda line: with_field(line, "decode_us_avg", 37000)))
        self.assertNotIn("decode budget", failures)

    def test_one_slow_grain_of_46_milliseconds_fails(self):
        self.assert_only_failure(self.changed_first("music window", lambda line: with_field(line, "decode_us_max", 46000)), "decode budget")

    def test_a_slow_grain_under_the_limit_passes(self):
        self.assertEqual(self.failing_checks(self.changed_first("music window", lambda line: with_field(line, "decode_us_max", 45999))), {})


class BlockingTests(AudioCheckTestCase):
    def test_nothing_ever_waiting_in_the_port_fails_and_says_why(self):
        failures = self.failing_checks(self.changed_all("music window", lambda line: with_field(line, "rest", 0)))
        self.assertIn("blocking semantics", failures)
        self.assertIn("decode-ahead", failures["blocking semantics"][0])

    def test_grains_that_return_at_once_fail(self):
        self.assertIn("blocking semantics", self.failing_checks(self.changed_all("music window", lambda line: with_field(line, "block_us_avg", 200))))


class FrameTests(AudioCheckTestCase):
    def test_slow_frames_fail(self):
        lines = list(self.good_lines)
        stats_lines = [index for index, line in enumerate(lines) if "stats fps" in line]
        lines[stats_lines[2]] = with_field(lines[stats_lines[2]], "slow", 3)
        self.assert_only_failure(lines, "frames stayed fast")

    def test_slow_frames_right_after_a_capture_are_excused(self):
        lines = list(self.good_lines)
        capture = [index for index, line in enumerate(lines) if "capture name=stall" in line][0]
        following_stats = [index for index, line in enumerate(lines) if "stats fps" in line and index > capture][0]
        lines[following_stats] = with_field(lines[following_stats], "slow", 3)
        self.assertEqual(self.failing_checks(lines), {})

    def test_slow_frames_while_preloading_are_excused(self):
        lines = ["[      0.000] --- launch\n", "[      0.000] stats fps=20.0 frames=20 slow=20 frame_us_min=1 frame_us_avg=1 frame_us_max=1 update_us=1 draw_us=1 present_us=1\n"] + self.good_lines[1:]
        self.assertEqual(self.failing_checks(lines), {})

    def test_a_scene_that_never_got_ready_fails(self):
        self.assert_only_failure(self.without("scene ready"), "frames stayed fast")


class ExitTests(AudioCheckTestCase):
    def test_no_clean_exit_fails(self):
        self.assert_only_failure(self.without("exit clean"), "clean exit")

    def test_music_stopping_after_the_exit_fails(self):
        lines = self.without("music stopped")
        stopped = [line for line in self.good_lines if "music stopped" in line]
        self.assertIn("clean exit", self.failing_checks(lines + stopped))

    def test_an_error_line_fails(self):
        for text in ("music file_missing name=song code=-2", "music submit_failed code=0x80260002", "sound out_of_memory name=ping", "scene audio_wait_gave_up name=opening", "log dropped_bytes=5"):
            lines = list(self.good_lines)
            lines.insert(-1, "[     30.000] " + text + "\n")
            self.assert_only_failure(lines, "no audio errors")

    def test_every_kind_of_error_line_fails(self):
        for line_start in ERROR_LINE_STARTS_THAT_MUST_FAIL:
            with self.subTest(line_start):
                lines = list(self.good_lines)
                lines.insert(-1, "[     30.000] " + line_start + " name=x\n")
                self.assert_only_failure(lines, "no audio errors")


class SoundTests(AudioCheckTestCase):
    def test_a_dropped_sound_fails(self):
        lines = list(self.good_lines)
        lines.insert(-1, "[     30.000] sound dropped name=ping reason=all_voices_busy\n")
        self.assert_only_failure(lines, "sound windows")

    def test_sound_underruns_fail(self):
        self.assert_only_failure(self.changed_first("sound window", lambda line: with_field(line, "underruns", 1)), "sound windows")

    def test_a_late_start_fails(self):
        self.assert_only_failure(self.changed_first("sound started", lambda line: with_field(line, "waited_us", 30000)), "sound windows")

    def test_no_sound_at_all_fails(self):
        failures = self.failing_checks(self.without("sound started"))
        self.assertEqual(list(failures), ["sound windows"])

    def test_no_sound_windows_fail(self):
        self.assert_only_failure(self.without("sound window"), "sound windows")

    def test_sounds_out_of_the_scene_order_fail(self):
        self.assert_only_failure(self.changed_first("sound started", lambda line: with_field(line, "name", "pong")), "sounds played in the scene's order")

    def test_a_track_the_scene_does_not_name_fails(self):
        failures = self.failing_checks(self.changed_first("music started", lambda line: with_field(line, "name", "song")))
        self.assertEqual(failures, {})
        messages = ["scene started name=opening steps=1", "music started name=other looping=0 loop_start=-1 loop_last=-1"]
        self.assertEqual(audio_check.check_cues_played_in_scene_order(messages, self.assets_folder, "music", "music started "), ["other played out of the scene's order"])

    def test_a_replay_starts_the_order_again(self):
        messages = ["scene started name=opening steps=1", "sound started name=ping", "scene started name=opening steps=1", "sound started name=ping", "sound started name=pong"]
        self.assertEqual(audio_check.check_cues_played_in_scene_order(messages, self.assets_folder, "sound", "sound started "), [])


if __name__ == "__main__":
    unittest.main()
