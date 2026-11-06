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

    def test_main_reports_the_fitted_rate_and_largest_period_of_each_port(self):
        output = io.StringIO()
        with mock.patch.object(sys, "argv", ["audio_check.py", "--log", self.log_path, "--assets", self.assets_folder]):
            with contextlib.redirect_stdout(output):
                audio_check.main()
        text = output.getvalue()
        self.assertRegex(text, r"measured: music port song 4410\d\.\d\d frames per second over \d+ windows, 4410\d\.\d\d frames per second over \d+ windows, largest period 370800 us, late 1")
        self.assertRegex(text, r"measured: sound port 0 4410\d\.\d\d frames per second over \d+ windows, largest period 11609 us, late 0")

    def test_a_port_too_short_to_fit_says_so(self):
        output = io.StringIO()
        with contextlib.redirect_stdout(output):
            audio_check.print_measured_numbers(["music started name=song looping=0 loop_start=-1 loop_last=-1"])
        self.assertIn("measured: music port song no stretch of 5 windows to fit a rate on, largest period 0 us, late 0", output.getvalue())


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

    def test_a_port_on_a_clock_off_by_a_little_is_measured_exactly(self):
        windows = []
        for index in range(6):
            frames_so_far = (index + 1) * 45056
            windows.append({"at": str(index * 45056), "grains": "11", "wall_us": str(int(frames_so_far * 1000000 / 44101.8))})
        self.assertAlmostEqual(audio_check.fitted_rate(windows, audio_check.MUSIC_GRAIN_FRAMES), 44101.8, delta=0.01)

    def test_sound_windows_fit_on_their_own_grain(self):
        windows = []
        for index in range(6):
            frames_so_far = (index + 1) * 88 * audio_check.SOUND_GRAIN_FRAMES
            windows.append({"at": str(index * 88 * audio_check.SOUND_GRAIN_FRAMES), "grains": "88", "wall_us": str(frames_so_far * 1000000 // audio_check.MUSIC_RATE)})
        self.assertAlmostEqual(audio_check.fitted_rate(windows, audio_check.SOUND_GRAIN_FRAMES), audio_check.MUSIC_RATE, delta=1)

    def test_a_sound_port_one_percent_off_fails(self):
        lines = self.changed_all("sound window", lambda line: with_field(line, "wall_us", int(int(audio_check.fields_of(line)["wall_us"]) * 1.01)))
        self.assert_only_failure(lines, "position follows the wall clock")

    def test_a_sound_port_with_too_few_windows_to_fit_fails(self):
        lines = []
        kept = 0
        for line in self.good_lines:
            if "sound window" in line:
                kept = kept + 1
                if kept > 3:
                    continue
            lines.append(line)
        self.assert_only_failure(lines, "position follows the wall clock")

    def test_a_gap_splits_the_fit_so_the_stretch_after_it_is_on_the_same_line(self):
        lines = self.good_lines
        messages = audio_check.latest_run_messages(lines)
        playback = audio_check.read_playbacks(messages)[0]
        rates = audio_check.fitted_rates_of(playback, audio_check.MUSIC_GRAIN_FRAMES)
        self.assertEqual(len(rates), 2)
        for _, rate in rates:
            self.assertAlmostEqual(rate, audio_check.MUSIC_RATE, delta=1)

    def test_the_gap_does_not_bend_the_fit_when_the_log_is_split(self):
        windows = []
        for index in range(12):
            late = 0 if index < 6 else 1
            shift_us = 0 if index < 6 else 300000
            frames_so_far = (index + 1) * 45056
            windows.append({"index": str(index), "at": str(index * 45056), "grains": "11", "late": str(late), "wall_us": str(frames_so_far * 1000000 // audio_check.MUSIC_RATE + shift_us)})
        segments = audio_check.windows_between_gaps(windows)
        self.assertEqual([len(segment) for segment in segments], [5, 6])
        self.assertAlmostEqual(audio_check.fitted_rate(segments[1], audio_check.MUSIC_GRAIN_FRAMES), audio_check.MUSIC_RATE, delta=1)

    def test_a_gap_on_the_last_grain_before_the_count_rises_does_not_bend_the_fit(self):
        windows = []
        for index in range(12):
            late = 0 if index < 7 else 1
            shift_us = 0 if index < 6 else 300000
            frames_so_far = (index + 1) * 45056
            windows.append({"index": str(index), "at": str(index * 45056), "grains": "11", "late": str(late), "wall_us": str(frames_so_far * 1000000 // audio_check.MUSIC_RATE + shift_us)})
        segments = audio_check.windows_between_gaps(windows)
        self.assertEqual([window["index"] for window in segments[0]], ["0", "1", "2", "3", "4", "5"])
        self.assertAlmostEqual(audio_check.fitted_rate(segments[0], audio_check.MUSIC_GRAIN_FRAMES), audio_check.MUSIC_RATE, delta=1)

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

    def test_a_late_grain_before_the_starve_test_fails(self):
        self.assertIn("no gaps outside the starve test", self.failing_checks(self.changed_first("music window", lambda line: with_field(line, "late", 1))))

    def test_a_late_grain_after_the_starve_test_settled_fails(self):
        lines = list(self.good_lines)
        window_lines = [index for index, line in enumerate(lines) if "music window" in line]
        lines[window_lines[-2]] = with_field(lines[window_lines[-2]], "late", 2)
        lines[window_lines[-1]] = with_field(lines[window_lines[-1]], "late", 2)
        self.assertIn("no gaps outside the starve test", self.failing_checks(lines))

    def starved_window_line(self, lines):
        return [index for index, line in enumerate(lines) if "music window" in line and " gap_us_max=300" in line][0]

    def test_the_fixture_has_the_starve_inside_the_first_windows_after_the_command(self):
        command = [index for index, line in enumerate(self.good_lines) if "command music_starve" in line][0]
        window_lines = [index for index, line in enumerate(self.good_lines) if "music window" in line and index > command]
        self.assertIn(self.starved_window_line(self.good_lines), window_lines[:audio_check.WINDOWS_A_STARVE_CAN_LAND_IN])

    def test_a_real_gap_in_the_starved_window_is_not_hidden_by_the_starve(self):
        lines = list(self.good_lines)
        starved = self.starved_window_line(lines)
        for index, line in enumerate(lines):
            if index >= starved and "music window" in line:
                lines[index] = with_field(line, "late", int(audio_check.fields_of(line)["late"]) + 1)
        failures = self.failing_checks(lines)
        self.assertIn("no gaps outside the starve test", failures)
        self.assertIn("the starve test fired the detector", failures)

    def test_a_starve_sent_between_playbacks_forgives_nothing(self):
        lines = [line for line in self.with_a_replay_of_the_music() if "command music_starve" not in line]
        first_stopped = self.position_of_first(lines, "music stopped")
        lines.insert(first_stopped + 1, "[      1.000] command music_starve ms=300\n")
        failures = self.failing_checks(lines)
        self.assertIn("the starve test fired the detector", failures)
        self.assertEqual(len(failures["no gaps outside the starve test"]), 2)

    def test_a_gap_shorter_than_the_starve_asked_for_does_not_count_as_the_starve(self):
        lines = list(self.good_lines)
        starved = self.starved_window_line(lines)
        lines[starved] = with_field(lines[starved], "gap_us_max", 299999)
        failures = self.failing_checks(lines)
        self.assertIn("the starve test fired the detector", failures)
        self.assertIn("no gaps outside the starve test", failures)

    def test_a_gap_right_at_what_the_starve_asked_for_counts(self):
        lines = list(self.good_lines)
        starved = self.starved_window_line(lines)
        lines[starved] = with_field(lines[starved], "gap_us_max", 300000)
        self.assertEqual(self.failing_checks(lines), {})

    def windows_with_the_starve_in(self, gap_index, rise_index, window_count=6, rise=1):
        command_position = 10
        windows = []
        for index in range(window_count):
            windows.append({
                "index": str(index),
                "gap_us_max": "300000" if index == gap_index else "100",
                "late": str(rise if index >= rise_index else 0),
                "log_position": command_position + (index if index > 0 else -1),
            })
        playback = audio_check.Playback({"name": "song", "looping": "1", "loop_start": "-1", "loop_last": "-1"})
        playback.windows = windows
        messages = ["line"] * (command_position + window_count)
        messages[command_position] = "command music_starve ms=300"
        return [playback], messages

    def test_a_starve_on_the_last_grain_of_a_window_raises_late_in_the_next(self):
        playbacks, messages = self.windows_with_the_starve_in(1, 2)
        self.assertEqual(audio_check.check_starve_test_fired(playbacks, messages), [])
        self.assertEqual(audio_check.check_no_gaps_outside_the_starve_test(playbacks, messages), [])

    def test_a_starve_that_lands_in_the_second_window_after_the_command_counts(self):
        playbacks, messages = self.windows_with_the_starve_in(2, 2)
        self.assertEqual(audio_check.check_starve_test_fired(playbacks, messages), [])

    def test_a_gap_long_after_the_command_is_not_the_starve(self):
        playbacks, messages = self.windows_with_the_starve_in(3, 3)
        self.assertEqual(audio_check.check_starve_test_fired(playbacks, messages), ["music_starve did not raise late by one with a gap at least that long while music played"])
        self.assertEqual(len(audio_check.check_no_gaps_outside_the_starve_test(playbacks, messages)), 1)

    def test_a_rise_two_windows_after_the_gap_is_not_the_starve(self):
        playbacks, messages = self.windows_with_the_starve_in(1, 3)
        self.assertEqual(audio_check.check_starve_test_fired(playbacks, messages), ["music_starve did not raise late by one with a gap at least that long while music played"])

    def test_a_rise_of_two_is_not_the_starve(self):
        playbacks, messages = self.windows_with_the_starve_in(1, 1, rise=2)
        self.assertEqual(audio_check.check_starve_test_fired(playbacks, messages), ["music_starve did not raise late by one with a gap at least that long while music played"])
        self.assertEqual(len(audio_check.check_no_gaps_outside_the_starve_test(playbacks, messages)), 1)

    def test_a_starve_with_no_window_left_to_land_in_did_not_fire(self):
        playbacks, messages = self.windows_with_the_starve_in(1, 1, window_count=2)
        playbacks[0].windows[1]["gap_us_max"] = "100"
        self.assertEqual(audio_check.check_starve_test_fired(playbacks, messages), ["music_starve did not raise late by one with a gap at least that long while music played"])

    def test_a_starve_whose_rise_would_land_past_the_last_window_did_not_fire(self):
        playbacks, messages = self.windows_with_the_starve_in(1, 9, window_count=2)
        self.assertEqual(audio_check.check_starve_test_fired(playbacks, messages), ["music_starve did not raise late by one with a gap at least that long while music played"])

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
        messages = audio_check.latest_run_messages(lines)
        self.assertEqual(audio_check.check_capture_stall_was_survived(audio_check.read_playbacks(messages), messages), ["no capture stall of at least 100000 us happened while music played"])

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

    def test_a_missing_port_line_fails(self):
        self.assertEqual(list(self.failing_checks(self.without("sound port"))), ["port facts", "sound windows"])
        self.assert_only_failure(self.without("music port"), "port facts")


class BudgetTests(AudioCheckTestCase):
    def test_decoding_over_the_share_limit_fails_and_names_the_fallback(self):
        failures = self.failing_checks(self.changed_first("music window", lambda line: with_field(line, "decode_us_avg", 20000)))
        self.assertIn("decode budget", failures)
        self.assertIn("scePowerSetArmClockFrequency(444)", failures["decode budget"][0])
        self.assertIn("ADPCM", failures["decode budget"][0])

    def test_only_a_later_window_over_budget_still_fails(self):
        lines = list(self.good_lines)
        window_lines = [index for index, line in enumerate(lines) if "music window" in line]
        later = window_lines[len(window_lines) // 2]
        lines[later] = with_field(lines[later], "decode_us_avg", 20000)
        failures = self.failing_checks(lines)
        self.assertIn("decode budget", failures)
        self.assertIn("scePowerSetArmClockFrequency(444)", failures["decode budget"][0])

    def test_decoding_just_under_the_share_limit_passes(self):
        lines = self.changed_all("music window", lambda line: with_field(with_field(line, "decode_us_avg", 18000), "block_us_avg", 74800))
        self.assertNotIn("decode budget", self.failing_checks(lines))

    def test_the_share_limit_is_a_fifth_of_a_window(self):
        window = {"wall_us": "1000000", "decode_us_avg": "18181", "decode_us_max": "0", "grains": "11"}
        playback = mock.Mock(windows=[window])
        self.assertEqual(audio_check.check_budget([playback]), [])
        window["decode_us_avg"] = "18182"
        self.assertEqual(len(audio_check.check_budget([playback])), 1)

    def test_one_slow_grain_of_18_milliseconds_fails(self):
        self.assert_only_failure(self.changed_first("music window", lambda line: with_field(line, "decode_us_max", 18000)), "decode budget")

    def test_a_slow_grain_under_the_limit_passes(self):
        self.assertEqual(self.failing_checks(self.changed_first("music window", lambda line: with_field(line, "decode_us_max", 17999))), {})


class BlockingTests(AudioCheckTestCase):
    def blocking_problems(self, lines):
        messages = audio_check.latest_run_messages(lines)
        return audio_check.check_blocking(audio_check.read_playbacks(messages))

    def test_grains_that_return_at_once_fail_and_say_why(self):
        failures = self.failing_checks(self.changed_all("music window", lambda line: with_field(line, "block_us_avg", 200)))
        self.assertIn("blocking semantics", failures)
        self.assertIn("decode-ahead", failures["blocking semantics"][0])

    def test_the_time_a_device_spends_on_ramp_and_crc_passes(self):
        lines = self.changed_all("music window", lambda line: with_field(line, "block_us_avg", 91691))
        self.assertEqual(self.blocking_problems(lines), [])

    def test_time_a_grain_cannot_account_for_fails(self):
        lines = self.changed_all("music window", lambda line: with_field(line, "block_us_avg", 89879))
        self.assertIn("spends 89879 us per grain", self.blocking_problems(lines)[0])

    def test_right_at_the_work_limit_passes(self):
        lines = self.changed_all("music window", lambda line: with_field(line, "block_us_avg", 89880))
        self.assertEqual(self.blocking_problems(lines), [])

    def test_blocking_longer_than_a_grain_fails(self):
        lines = self.changed_all("music window", lambda line: with_field(line, "block_us_avg", 93400))
        self.assertIn("spends 93400 us per grain", self.blocking_problems(lines)[0])

    def test_right_at_the_overrun_limit_passes(self):
        lines = self.changed_all("music window", lambda line: with_field(line, "block_us_avg", 93379))
        self.assertEqual(self.blocking_problems(lines), [])

    def test_decode_counts_toward_the_grain(self):
        lines = self.changed_all("music window", lambda line: with_field(with_field(line, "block_us_avg", 85000), "decode_us_avg", 7000))
        self.assertEqual(self.blocking_problems(lines), [])

    def test_the_window_holding_a_gap_is_not_held_to_the_grain(self):
        lines = list(self.good_lines)
        late_windows = [index for index, line in enumerate(lines) if "music window" in line and " late=1 " in line]
        lines[late_windows[0]] = with_field(lines[late_windows[0]], "block_us_avg", 70800)
        self.assertEqual(self.blocking_problems(lines), [])

    def test_the_window_after_a_gap_is_held_to_the_grain(self):
        lines = list(self.good_lines)
        late_windows = [index for index, line in enumerate(lines) if "music window" in line and " late=1 " in line]
        lines[late_windows[1]] = with_field(lines[late_windows[1]], "block_us_avg", 70800)
        self.assertIn("spends 70800 us per grain", self.blocking_problems(lines)[0])

    def test_a_run_with_no_window_to_check_fails_rather_than_passing_empty(self):
        lines = [line for line in self.good_lines if "music window" not in line]
        self.assertEqual(self.blocking_problems(lines), ["no window without a gap to check the blocking on"])

    def test_every_window_after_the_first_two_is_really_checked(self):
        lines = list(self.good_lines)
        window_lines = [index for index, line in enumerate(lines) if "music window" in line]
        lines[window_lines[2]] = with_field(lines[window_lines[2]], "block_us_avg", 200)
        self.assertEqual(len(self.blocking_problems(lines)), 1)


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
    def sound_problems(self, lines):
        return audio_check.check_sound_windows(audio_check.latest_run_messages(lines))

    def test_a_dropped_sound_fails(self):
        lines = list(self.good_lines)
        lines.insert(-1, "[     30.000] sound dropped name=ping reason=all_voices_busy\n")
        self.assert_only_failure(lines, "sound windows")

    def test_a_late_grain_while_a_voice_plays_fails(self):
        lines = self.changed_all("sound window", lambda line: with_field(with_field(line, "late", 1), "voices_max", 1))
        self.assertEqual(self.sound_problems(lines), ["sound port 0 window 0 has late=1 while a voice played"])

    def test_a_late_grain_with_every_voice_silent_is_not_a_failure(self):
        lines = self.changed_all("sound window", lambda line: with_field(with_field(line, "late", 1), "voices_max", 0))
        self.assertEqual(self.sound_problems(lines), [])

    def with_late_rising_at(self, rise_position, voiced_positions):
        lines = list(self.good_lines)
        window_lines = [index for index, line in enumerate(lines) if "sound window" in line]
        for position, index in enumerate(window_lines):
            lines[index] = with_field(with_field(lines[index], "late", 1 if position >= rise_position else 0), "voices_max", 1 if position in voiced_positions else 0)
        return lines

    def test_a_late_grain_counted_just_after_a_voice_ended_fails(self):
        self.assertEqual(self.sound_problems(self.with_late_rising_at(3, [2])), ["sound port 0 window 3 has late=1 while a voice played"])

    def test_a_voice_two_windows_before_the_rise_is_not_blamed(self):
        self.assertEqual(self.sound_problems(self.with_late_rising_at(3, [1])), [])

    def test_only_the_window_where_late_rose_fails(self):
        lines = list(self.good_lines)
        window_lines = [index for index, line in enumerate(lines) if "sound window" in line]
        for position, index in enumerate(window_lines):
            lines[index] = with_field(with_field(lines[index], "late", 1 if position >= 3 else 0), "voices_max", 1)
        self.assertEqual(self.sound_problems(lines), ["sound port 0 window 3 has late=1 while a voice played"])

    def test_each_port_open_counts_late_from_zero(self):
        lines = list(self.good_lines)
        port_line = [line for line in lines if "sound port" in line][0]
        window_line = with_field(with_field([line for line in lines if "sound window" in line][0], "late", 1), "voices_max", 1)
        reopened = [port_line, window_line]
        self.assertEqual(self.sound_problems(lines + reopened), ["sound port 1 window 0 has late=1 while a voice played"])

    def test_a_submit_too_long_after_a_return_fails(self):
        lines = self.changed_first("sound window", lambda line: with_field(line, "gap_us_max", 11001))
        self.assertEqual(self.sound_problems(lines), ["sound port 0 window 0 submits 11001 us after a return, the limit is 11000 us"])

    def test_a_submit_right_at_the_limit_passes(self):
        lines = self.changed_first("sound window", lambda line: with_field(line, "gap_us_max", 11000))
        self.assertEqual(self.sound_problems(lines), [])

    def test_a_late_start_fails(self):
        self.assert_only_failure(self.changed_first("sound started", lambda line: with_field(line, "waited_us", 30000)), "sound windows")

    def test_no_sound_at_all_fails(self):
        failures = self.failing_checks(self.without("sound started"))
        self.assertEqual(list(failures), ["sound windows"])

    def test_no_sound_windows_fail(self):
        self.assertIn("sound windows", self.failing_checks(self.without("sound window")))

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
