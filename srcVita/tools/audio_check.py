import argparse
import os
import subprocess
import sys

SRC_VITA_FOLDER = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DEFAULT_REFERENCE_PATH = os.path.join(SRC_VITA_FOLDER, "build", "tools", "music_reference")
LAUNCH_MARKER = "--- launch"
MUSIC_RATE = 44100
STEREO_PORT_MODE = 1
MUSIC_GRAIN_FRAMES = 4096
SOUND_GRAIN_FRAMES = 512
MUSIC_GRAIN_US = MUSIC_GRAIN_FRAMES * 1000000 / MUSIC_RATE
RATE_TOLERANCE = 0.005
FEWEST_WINDOWS_FOR_A_RATE_FIT = 5
DECODE_SHARE_LIMIT = 0.20
DECODE_US_MAX_LIMIT = 18000
LARGEST_WORK_PER_GRAIN_US = 3000
LARGEST_OVERRUN_PER_GRAIN_US = 500
LONGEST_SOUND_RETURN_TO_SUBMIT_US = 11000
LONGEST_SOUND_WAIT_US = 25000
SHORTEST_CAPTURE_STALL_US = 100000
WINDOWS_A_STARVE_CAN_LAND_IN = 2
FALLBACK_ADVICE = "fallback: first try scePowerSetArmClockFrequency(444); if it still misses, put the ADPCM decoder behind vorbis_stream"
BLOCKING_ADVICE = "the assumption that sceAudioOutOutput returns while a grain is still queued looks wrong; the design then needs a decode-ahead thread and a small ring"
ERROR_LINE_STARTS = (
    "music port_open_failed", "music submit_failed", "music out_of_memory", "music file_missing", "music decoder_failed",
    "music wrong_rate", "music bad_loop_file", "music thread_failed", "music path_too_long",
    "sound port_open_failed", "sound submit_failed", "sound out_of_memory", "sound file_missing", "sound decoder_failed",
    "sound wrong_rate", "sound thread_failed", "sound path_too_long", "sound prepare_list_full",
    "scene audio_wait_gave_up", "log dropped_bytes",
)


def message_of(line):
    closing = line.find("] ")
    if line.startswith("[") and closing >= 0:
        return line[closing + 2:]
    return line


def latest_run_messages(lines):
    start = 0
    for index, line in enumerate(lines):
        if LAUNCH_MARKER in line:
            start = index
    return [message_of(line.rstrip("\n")) for line in lines[start:]]


def fields_of(message):
    fields = {}
    for part in message.split():
        if "=" in part:
            key, value = part.split("=", 1)
            fields[key] = value
    return fields


def number(fields, key):
    return int(fields[key])


class Playback:
    def __init__(self, started_fields):
        self.name = started_fields["name"]
        self.looping = number(started_fields, "looping")
        self.loop_start = number(started_fields, "loop_start")
        self.loop_last = number(started_fields, "loop_last")
        self.events = []
        self.windows = []
        self.stopped = None


def read_playbacks(messages):
    playbacks = []
    current = None
    for position, message in enumerate(messages):
        if message.startswith("music started "):
            current = Playback(fields_of(message))
            playbacks.append(current)
        elif current is None:
            continue
        elif message.startswith("music ramp "):
            current.events.append(("ramp", fields_of(message)))
        elif message.startswith("music window "):
            fields = fields_of(message)
            fields["log_position"] = position
            current.events.append(("window", fields))
            current.windows.append(fields)
        elif message.startswith("music stopped "):
            current.stopped = fields_of(message)
            current.stopped["log_position"] = position
    return playbacks


class SoundPort:
    def __init__(self, index):
        self.name = "sound port %d" % index
        self.windows = []


def read_sound_ports(messages):
    ports = []
    for message in messages:
        if message.startswith("sound port "):
            ports.append(SoundPort(len(ports)))
        elif message.startswith("sound window ") and len(ports) > 0:
            ports[-1].windows.append(fields_of(message))
    return ports


def position_of(messages, start_of_line):
    for position, message in enumerate(messages):
        if message.startswith(start_of_line):
            return position
    return None


def check_windows_are_contiguous(playbacks):
    problems = []
    if len(playbacks) == 0:
        return ["no music played in this run"]
    for playback in playbacks:
        expected_at = 0
        for expected_index, window in enumerate(playback.windows):
            if number(window, "index") != expected_index:
                problems.append("%s window %s came where index %d belongs" % (playback.name, window["index"], expected_index))
            if number(window, "at") != expected_at:
                problems.append("%s window %s starts at %s, expected %d" % (playback.name, window["index"], window["at"], expected_at))
            if number(window, "frames") != number(window, "grains") * MUSIC_GRAIN_FRAMES:
                problems.append("%s window %s has %s frames in %s grains" % (playback.name, window["index"], window["frames"], window["grains"]))
            expected_at = number(window, "at") + number(window, "frames")
        if playback.stopped is None:
            problems.append(playback.name + " has no stopped line")
        elif number(playback.stopped, "submitted") != expected_at:
            problems.append("%s stopped line says %s frames, the windows add up to %d" % (playback.name, playback.stopped["submitted"], expected_at))
        elif len(playback.windows) > 0 and playback.stopped["crc_total"] != playback.windows[-1]["crc_total"]:
            problems.append("%s stopped line crc_total differs from its last window" % playback.name)
    return problems


def reference_script(playback):
    lines = []
    for kind, fields in playback.events:
        if kind == "ramp":
            lines.append("ramp %s %s %s %s" % (fields["at"], fields["frames"], fields["from"], fields["to"]))
        else:
            lines.append("window %s" % fields["frames"])
    return "\n".join(lines) + "\n"


def reference_lines(playback, assets_folder, reference_path):
    ogg_path = os.path.join(assets_folder, "music", playback.name + ".ogg")
    command = [reference_path, ogg_path, str(playback.looping), str(playback.loop_start), str(playback.loop_last)]
    result = subprocess.run(command, input=reference_script(playback), stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
    if result.returncode != 0:
        raise RuntimeError("music_reference failed: " + result.stderr.strip())
    return result.stdout.splitlines()


def check_checksums(playbacks, assets_folder, reference_path):
    problems = []
    for playback in playbacks:
        expected_lines = reference_lines(playback, assets_folder, reference_path)
        for window, expected_line in zip(playback.windows, expected_lines):
            crc, crc_total, source_frame, loops = expected_line.split()
            if window["crc32"] != crc:
                problems.append("%s window %s crc32 is %s, the reference says %s" % (playback.name, window["index"], window["crc32"], crc))
            if window["crc_total"] != crc_total:
                problems.append("%s window %s crc_total is %s, the reference says %s" % (playback.name, window["index"], window["crc_total"], crc_total))
            if window["source_frame"] != source_frame:
                problems.append("%s window %s source_frame is %s, the reference says %s" % (playback.name, window["index"], window["source_frame"], source_frame))
            if window["loops"] != loops:
                problems.append("%s window %s loops is %s, the reference says %s" % (playback.name, window["index"], window["loops"], loops))
    return problems


def fitted_rate(windows, grain_frames):
    times = [number(window, "wall_us") / 1000000.0 for window in windows]
    positions = [number(window, "at") + number(window, "grains") * grain_frames for window in windows]
    time_mean = sum(times) / len(times)
    position_mean = sum(positions) / len(positions)
    spread = sum((time - time_mean) ** 2 for time in times)
    if spread == 0:
        return 0.0
    return sum((time - time_mean) * (position - position_mean) for time, position in zip(times, positions)) / spread


def windows_between_gaps(windows):
    segments = [[]]
    previous_late = None
    for window in windows:
        late = number(window, "late")
        if previous_late is not None and late != previous_late:
            segments[-1].pop()
            segments.append([])
        previous_late = late
        segments[-1].append(window)
    return segments


def fitted_rates_of(port, grain_frames):
    rates = []
    for segment in windows_between_gaps(port.windows[1:]):
        if len(segment) >= FEWEST_WINDOWS_FOR_A_RATE_FIT:
            rates.append((segment, fitted_rate(segment, grain_frames)))
    return rates


def rate_problems(ports, grain_frames):
    problems = []
    was_fitted = False
    for port in ports:
        for segment, rate in fitted_rates_of(port, grain_frames):
            was_fitted = True
            if abs(rate - MUSIC_RATE) > MUSIC_RATE * RATE_TOLERANCE:
                problems.append("%s plays at %.1f frames per second from window %s" % (port.name, rate, segment[0]["index"]))
    if not was_fitted and len(ports) > 0:
        problems.append("no stretch of %d windows without a gap to fit a rate on" % FEWEST_WINDOWS_FOR_A_RATE_FIT)
    return problems


def check_rate(playbacks, sound_ports):
    return rate_problems(playbacks, MUSIC_GRAIN_FRAMES) + rate_problems(sound_ports, SOUND_GRAIN_FRAMES)


def music_played_on_both_sides_of(playback, position):
    stopped_before = playback.stopped is not None and playback.stopped["log_position"] < position
    window_before = any(window["log_position"] < position for window in playback.windows)
    window_after = any(window["log_position"] > position for window in playback.windows)
    return window_before and window_after and not stopped_before


def rise_in_late(windows, index):
    return number(windows[index], "late") - number(windows[index - 1], "late")


def window_the_starve_raised_late_in(playbacks, messages):
    starve_position = position_of(messages, "command music_starve")
    if starve_position is None:
        return None
    starve_us = number(fields_of(messages[starve_position]), "ms") * 1000
    for playback in playbacks:
        if not music_played_on_both_sides_of(playback, starve_position):
            continue
        windows = playback.windows
        first_after = min(index for index, window in enumerate(windows) if window["log_position"] > starve_position)
        for index in range(first_after, min(first_after + WINDOWS_A_STARVE_CAN_LAND_IN, len(windows))):
            if number(windows[index], "gap_us_max") >= starve_us:
                for rise_index in range(index, min(index + 2, len(windows))):
                    if rise_in_late(windows, rise_index) == 1:
                        return windows[rise_index]
                return None
    return None


def check_no_gaps_outside_the_starve_test(playbacks, messages):
    problems = []
    forgiven_window = window_the_starve_raised_late_in(playbacks, messages)
    for playback in playbacks:
        previous_late = 0
        for window in playback.windows:
            late = number(window, "late")
            if late > previous_late and window is not forgiven_window:
                problems.append("%s window %s has late=%s outside the starve test" % (playback.name, window["index"], window["late"]))
            previous_late = late
    return problems


def check_starve_test_fired(playbacks, messages):
    if position_of(messages, "command music_starve") is None:
        return ["this run has no music_starve command, so a count of zero proves nothing"]
    if window_the_starve_raised_late_in(playbacks, messages) is None:
        return ["music_starve did not raise late by one with a gap at least that long while music played"]
    return []


def check_capture_stall_was_survived(playbacks, messages):
    starve_position = position_of(messages, "command music_starve")
    for position, message in enumerate(messages):
        if not message.startswith("capture "):
            continue
        fields = fields_of(message)
        if number(fields, "took_us") < SHORTEST_CAPTURE_STALL_US:
            continue
        if starve_position is not None and position > starve_position:
            return ["the capture stall came after the starve test, so it proves nothing"]
        if any(music_played_on_both_sides_of(playback, position) for playback in playbacks):
            return []
    return ["no capture stall of at least %d us happened while music played" % SHORTEST_CAPTURE_STALL_US]


def check_port_facts(messages):
    problems = []
    music_ports = [fields_of(message) for message in messages if message.startswith("music port ")]
    sound_ports = [fields_of(message) for message in messages if message.startswith("sound port ")]
    if len(music_ports) == 0:
        problems.append("no music port line")
    if len(sound_ports) == 0:
        problems.append("no sound port line")
    for fields in music_ports:
        if number(fields, "rate") != MUSIC_RATE or number(fields, "grain") != MUSIC_GRAIN_FRAMES:
            problems.append("music port is rate=%s grain=%s" % (fields["rate"], fields["grain"]))
        if number(fields, "mode") != STEREO_PORT_MODE:
            problems.append("music port mode=%s" % fields["mode"])
        if number(fields, "adopt") != 1:
            problems.append("music port adopt=%s" % fields["adopt"])
    for fields in sound_ports:
        if number(fields, "rate") != MUSIC_RATE or number(fields, "grain") != SOUND_GRAIN_FRAMES:
            problems.append("sound port is rate=%s grain=%s" % (fields["rate"], fields["grain"]))
        if number(fields, "mode") != STEREO_PORT_MODE:
            problems.append("sound port mode=%s" % fields["mode"])
        if number(fields, "adopt") != 1:
            problems.append("sound port adopt=%s" % fields["adopt"])
    return problems


def measured_budget(playbacks):
    worst_share = 0.0
    worst_decode_us = 0
    for playback in playbacks:
        previous_wall_us = 0
        for window in playback.windows:
            wall_us = number(window, "wall_us") - previous_wall_us
            previous_wall_us = number(window, "wall_us")
            if wall_us > 0:
                worst_share = max(worst_share, number(window, "decode_us_avg") * number(window, "grains") / wall_us)
            worst_decode_us = max(worst_decode_us, number(window, "decode_us_max"))
    return worst_share, worst_decode_us


def check_budget(playbacks):
    worst_share, worst_decode_us = measured_budget(playbacks)
    problems = []
    if worst_share >= DECODE_SHARE_LIMIT:
        problems.append("decoding took %.1f%% of a window, the limit is %d%%; %s" % (worst_share * 100, DECODE_SHARE_LIMIT * 100, FALLBACK_ADVICE))
    if worst_decode_us >= DECODE_US_MAX_LIMIT:
        problems.append("decode_us_max reached %d us, the limit is %d us; %s" % (worst_decode_us, DECODE_US_MAX_LIMIT, FALLBACK_ADVICE))
    return problems


def check_blocking(playbacks):
    problems = []
    windows_checked = 0
    for playback in playbacks:
        for segment in windows_between_gaps(playback.windows[1:]):
            for window in segment[1:]:
                if number(window, "grains") < 2:
                    continue
                windows_checked = windows_checked + 1
                grain_time = number(window, "block_us_avg") + number(window, "decode_us_avg")
                left_for_other_work = MUSIC_GRAIN_US - grain_time
                if left_for_other_work > LARGEST_WORK_PER_GRAIN_US or left_for_other_work < -LARGEST_OVERRUN_PER_GRAIN_US:
                    problems.append("%s window %s spends %d us per grain, a grain lasts %d us; %s" % (playback.name, window["index"], grain_time, MUSIC_GRAIN_US, BLOCKING_ADVICE))
    if windows_checked == 0 and len(playbacks) > 0:
        problems.append("no window without a gap to check the blocking on")
    return problems


def check_frames_stayed_fast(messages):
    problems = []
    ready_position = position_of(messages, "scene ready ")
    if ready_position is None:
        return ["the scene never got ready"]
    excused = True
    for message in messages[ready_position:]:
        if message.startswith("capture ") or message.startswith("command "):
            excused = True
        if message.startswith("stats "):
            fields = fields_of(message)
            if number(fields, "slow") > 0 and not excused:
                problems.append("slow=%s frame_us_max=%s outside a capture or a command" % (fields["slow"], fields["frame_us_max"]))
            excused = False
    return problems


def check_clean_exit(playbacks, messages):
    problems = []
    exit_position = position_of(messages, "exit clean")
    if exit_position is None:
        return ["no exit clean line"]
    for position, message in enumerate(messages):
        if message.startswith("music stopped ") and position > exit_position:
            problems.append("music stopped came after exit clean")
    for playback in playbacks:
        if playback.stopped is None:
            problems.append(playback.name + " never stopped")
    return problems


def check_no_audio_errors(messages):
    return [message for message in messages if message.startswith(ERROR_LINE_STARTS)]


def scene_audio_names(assets_folder, scene_name, step_kind):
    path = os.path.join(assets_folder, "scenes", scene_name + ".scene")
    names = []
    with open(path, encoding="ascii") as source:
        for line in source:
            parts = line.split()
            if len(parts) >= 2 and parts[0] == step_kind:
                names.append(parts[1])
    return names


def check_cues_played_in_scene_order(messages, assets_folder, step_kind, started_line_start):
    problems = []
    expected = []
    position = 0
    for message in messages:
        if message.startswith("scene started "):
            expected = scene_audio_names(assets_folder, fields_of(message)["name"], step_kind)
            position = 0
        elif message.startswith(started_line_start):
            name = fields_of(message)["name"]
            if position >= len(expected) or expected[position] != name:
                problems.append("%s played out of the scene's order" % name)
            position = position + 1
    return problems


def check_sound_windows(messages):
    problems = []
    ports = read_sound_ports(messages)
    if not any(port.windows for port in ports):
        problems.append("no sound window lines")
    for port in ports:
        previous_late = 0
        previous_voices = 0
        for window in port.windows:
            late = number(window, "late")
            voices = number(window, "voices_max")
            if late > previous_late and (voices > 0 or previous_voices > 0):
                problems.append("%s window %s has late=%s while a voice played" % (port.name, window["index"], window["late"]))
            previous_late = late
            previous_voices = voices
            if number(window, "gap_us_max") > LONGEST_SOUND_RETURN_TO_SUBMIT_US:
                problems.append("%s window %s submits %s us after a return, the limit is %d us" % (port.name, window["index"], window["gap_us_max"], LONGEST_SOUND_RETURN_TO_SUBMIT_US))
    for message in messages:
        if message.startswith("sound dropped "):
            problems.append(message)
        if message.startswith("sound started "):
            fields = fields_of(message)
            if number(fields, "waited_us") > LONGEST_SOUND_WAIT_US:
                problems.append("%s waited %s us to start" % (fields["name"], fields["waited_us"]))
    if position_of(messages, "sound started ") is None:
        problems.append("no sound started")
    return problems


def run_checks(messages, assets_folder, reference_path):
    playbacks = read_playbacks(messages)
    sound_ports = read_sound_ports(messages)
    return [
        ("windows are contiguous", check_windows_are_contiguous(playbacks)),
        ("checksums match the reference", check_checksums(playbacks, assets_folder, reference_path)),
        ("position follows the wall clock", check_rate(playbacks, sound_ports)),
        ("no gaps outside the starve test", check_no_gaps_outside_the_starve_test(playbacks, messages)),
        ("the starve test fired the detector", check_starve_test_fired(playbacks, messages)),
        ("a capture stall did not reach the audio", check_capture_stall_was_survived(playbacks, messages)),
        ("port facts", check_port_facts(messages)),
        ("decode budget", check_budget(playbacks)),
        ("blocking semantics", check_blocking(playbacks)),
        ("frames stayed fast", check_frames_stayed_fast(messages)),
        ("clean exit", check_clean_exit(playbacks, messages)),
        ("no audio errors", check_no_audio_errors(messages)),
        ("music played in the scene's order", check_cues_played_in_scene_order(messages, assets_folder, "music", "music started ")),
        ("sounds played in the scene's order", check_cues_played_in_scene_order(messages, assets_folder, "sound", "sound started ")),
        ("sound windows", check_sound_windows(messages)),
    ]


def largest_period_of(port):
    return max((number(window, "period_us_max") for window in port.windows), default=0)


def print_port_numbers(label, port, grain_frames):
    rates = fitted_rates_of(port, grain_frames)
    if len(rates) == 0:
        rate_text = "no stretch of %d windows to fit a rate on" % FEWEST_WINDOWS_FOR_A_RATE_FIT
    else:
        rate_text = ", ".join("%.2f frames per second over %d windows" % (rate, len(segment)) for segment, rate in rates)
    print("measured: %s %s, largest period %d us, late %d" % (label, rate_text, largest_period_of(port), number(port.windows[-1], "late") if port.windows else 0))


def print_measured_numbers(messages):
    playbacks = read_playbacks(messages)
    worst_share, worst_decode_us = measured_budget(playbacks)
    print("measured: worst decode share %.1f%%, decode_us_max %d us" % (worst_share * 100, worst_decode_us))
    for playback in playbacks:
        print_port_numbers("music port " + playback.name, playback, MUSIC_GRAIN_FRAMES)
    for port in read_sound_ports(messages):
        print_port_numbers(port.name, port, SOUND_GRAIN_FRAMES)


def main():
    parser = argparse.ArgumentParser(description="Judges a run log of the app's audio with no listener: the right samples reached the port at the right rate while frames held.")
    parser.add_argument("--log", required=True, help="file holding the log, as printed by tools/vita_loop.py log")
    parser.add_argument("--assets", required=True, help="the converted asset set the run used")
    parser.add_argument("--reference", default=DEFAULT_REFERENCE_PATH, help="the music_reference tool, built by make -C tools/assets")
    arguments = parser.parse_args()
    with open(arguments.log, encoding="utf-8", errors="replace") as source:
        messages = latest_run_messages(source.readlines())
    failed = False
    for name, problems in run_checks(messages, arguments.assets, arguments.reference):
        if len(problems) == 0:
            print("PASS", name)
            continue
        failed = True
        print("FAIL", name)
        for problem in problems:
            print("    " + problem)
    print_measured_numbers(messages)
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
