import argparse
import json
import os
import re
import subprocess
import sys

REPO = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..")
DEFAULT_SET = os.path.join(REPO, "srcVita", "tools", "assets", "game.json")
DEFAULT_FOOTAGE = os.path.join(REPO, "extracted", "reference", "pc_longplay_720p60.mp4")
DEFAULT_FRAMES = os.path.join(REPO, "extracted", "reference_frames", "frames")
TICKS_PER_SECOND = 60.0
REFERENCE_FRAMES_PER_SECOND = 3
FOOTAGE_FILTER = "crop=960:720,scale=480:360:in_color_matrix=bt601"
PAIR_HEIGHT = 272
MATCH_WIDTH = 64
MATCH_HEIGHT = 48
MATCH_FRAMES_PER_SECOND = 3
DEFAULT_SEARCH_SECONDS = 45.0
CLOSE_MATCH_MOST_DIFFERENCE = 0.08
POSSIBLE_MATCH_MOST_DIFFERENCE = 0.11
MATCH_CHANNELS = 3
MATCH_ROWS_ABOVE_THE_TALK_BOX = 36
FOOTAGE_MATCH_FILTER = "fps=%d,crop=960:720,scale=%d:%d:in_color_matrix=bt601,format=rgb24" % (MATCH_FRAMES_PER_SECOND, MATCH_WIDTH, MATCH_HEIGHT)
LABEL_FONTS = ("/System/Library/Fonts/Supplemental/Arial.ttf", "/System/Library/Fonts/Helvetica.ttc", "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")
ACTION_LINE = re.compile(r"script route_step_began name=(\d+):\S* tick=(\d+)")
TIMES_LINE = re.compile(r"(f\d+) round=\d+ sequence=\d+ device_ms=(\d+) tick=(\d+) (route_step|acted_step)=(-?\d+) (?:ticks_into_step|ticks_since_action)=(\d+)")


def read_route_footage_seconds(set_path):
    route = json.load(open(set_path))["route"]
    return [step.get("footage_s") for step in route["steps"]]


def read_burst_times(burst_folder):
    pictures = []
    for line in open(os.path.join(burst_folder, "times.txt"), encoding="ascii"):
        found = TIMES_LINE.search(line)
        if found:
            pictures.append({"name": found.group(1), "device_ms": int(found.group(2)), "tick": int(found.group(3)), "anchor": found.group(4), "route_step": int(found.group(5)), "ticks_into_step": int(found.group(6))})
    return pictures


def read_action_ticks(log_path):
    action_ticks = {}
    if log_path is None:
        return action_ticks
    for line in open(log_path, encoding="utf-8", errors="replace"):
        found = ACTION_LINE.search(line)
        if found and int(found.group(1)) not in action_ticks:
            action_ticks[int(found.group(1))] = int(found.group(2))
    return action_ticks


def next_timed_action(step, action_tick, action_ticks, footage_seconds_of_steps):
    later = [index for index in action_ticks if index > step and index < len(footage_seconds_of_steps) and footage_seconds_of_steps[index] is not None and action_ticks[index] > action_tick]
    return min(later) if later else None


def footage_seconds_of(picture, footage_seconds_of_steps, action_ticks):
    step = picture["route_step"]
    if step < 0 or step >= len(footage_seconds_of_steps) or footage_seconds_of_steps[step] is None:
        return None
    seconds_since_anchor = picture["ticks_into_step"] / TICKS_PER_SECOND
    if picture["anchor"] == "acted_step":
        action_tick = picture["tick"] - picture["ticks_into_step"]
        following = next_timed_action(step, action_tick, action_ticks, footage_seconds_of_steps)
        if following is not None:
            footage_window = footage_seconds_of_steps[following] - footage_seconds_of_steps[step]
            device_window = (action_ticks[following] - action_tick) / TICKS_PER_SECOND
            if footage_window > 0 and device_window > 0:
                seconds_since_anchor = min(seconds_since_anchor * footage_window / device_window, footage_window)
    return footage_seconds_of_steps[step] + seconds_since_anchor


def reference_frame_name(seconds):
    nearest = round(seconds * REFERENCE_FRAMES_PER_SECOND) / REFERENCE_FRAMES_PER_SECOND
    return "t%07.1f.jpg" % nearest, nearest


def reference_frame(seconds, frames_folder, footage_path):
    name, nearest = reference_frame_name(seconds)
    path = os.path.join(frames_folder, name)
    if not os.path.exists(path):
        os.makedirs(frames_folder, exist_ok=True)
        subprocess.run(["ffmpeg", "-v", "error", "-y", "-ss", "%.3f" % nearest, "-i", footage_path, "-frames:v", "1", "-vf", FOOTAGE_FILTER, path], check=True)
    return path


def label_font_arguments():
    for font in LABEL_FONTS:
        if os.path.exists(font):
            return ["-font", font]
    return []


def picture_for_matching(burst_path):
    size = subprocess.run(["magick", "identify", "-format", "%w %h", burst_path], check=True, capture_output=True, text=True).stdout.split()
    height = int(size[1])
    view_width = min(int(size[0]), round(height * 4 / 3))
    return subprocess.run(["magick", burst_path, "-gravity", "center", "-crop", "%dx%d+0+0" % (view_width, height), "+repage", "-resize", "%dx%d!" % (MATCH_WIDTH, MATCH_HEIGHT), "-colorspace", "sRGB", "-depth", "8", "rgb:-"], check=True, capture_output=True).stdout


def footage_for_matching(footage_path, first_second, last_second):
    first_second = max(0.0, first_second)
    raw = subprocess.run(["ffmpeg", "-v", "error", "-ss", "%.3f" % first_second, "-t", "%.3f" % (last_second - first_second), "-i", footage_path, "-vf", FOOTAGE_MATCH_FILTER, "-f", "rawvideo", "-"], check=True, capture_output=True).stdout
    frame_bytes = MATCH_WIDTH * MATCH_HEIGHT * MATCH_CHANNELS
    return [(first_second + index / MATCH_FRAMES_PER_SECOND, raw[index * frame_bytes:(index + 1) * frame_bytes]) for index in range(len(raw) // frame_bytes)]


def difference(picture, frame):
    compared = MATCH_WIDTH * MATCH_ROWS_ABOVE_THE_TALK_BOX * MATCH_CHANNELS
    return sum(abs(a - b) for a, b in zip(picture[:compared], frame[:compared])) / (255.0 * compared)


def closest_footage(picture, footage_frames, estimate, search_seconds):
    best = None
    for seconds, frame in footage_frames:
        if abs(seconds - estimate) > search_seconds:
            continue
        score = difference(picture, frame)
        if best is None or score < best[1]:
            best = (seconds, score)
    return best


def write_pair(burst_path, reference_path, label, pair_path):
    subprocess.run(["magick", "(", burst_path, "-resize", "x%d" % PAIR_HEIGHT, ")", "(", reference_path, "-resize", "x%d" % PAIR_HEIGHT, ")", "+append", "-gravity", "north", "-background", "black", "-splice", "0x18"] + label_font_arguments() + ["-fill", "white", "-pointsize", "14", "-annotate", "+0+2", label, pair_path], check=True)


def write_sheet(pair_paths, sheet_path):
    if pair_paths:
        subprocess.run(["magick", "montage"] + pair_paths + ["-tile", "2x", "-geometry", "+2+2", "-background", "gray20", sheet_path], check=True)


def match(burst_folder, set_path, footage_path, frames_folder, output_folder, log_path, search_seconds):
    footage_seconds_of_steps = read_route_footage_seconds(set_path)
    action_ticks = read_action_ticks(log_path)
    os.makedirs(output_folder, exist_ok=True)
    pictures = []
    unmatched = 0
    for picture in read_burst_times(burst_folder):
        picture["estimate"] = footage_seconds_of(picture, footage_seconds_of_steps, action_ticks)
        if picture["estimate"] is None:
            unmatched = unmatched + 1
            continue
        pictures.append(picture)
    if not pictures:
        print("paired 0 pictures, %d without a route step" % unmatched)
        return 1
    estimates = [picture["estimate"] for picture in pictures]
    footage_frames = footage_for_matching(footage_path, min(estimates) - search_seconds, max(estimates) + search_seconds)
    pair_paths = []
    lines = ["picture\testimate_s\tclosest_s\toffset_s\tdifference\tmatch"]
    close = 0
    possible = 0
    for picture in pictures:
        burst_path = os.path.join(burst_folder, picture["name"] + ".jpg")
        best = closest_footage(picture_for_matching(burst_path), footage_frames, picture["estimate"], search_seconds)
        seconds, score = best if best is not None else (picture["estimate"], 1.0)
        verdict = "close" if score <= CLOSE_MATCH_MOST_DIFFERENCE else ("possible" if score <= POSSIBLE_MATCH_MOST_DIFFERENCE else "none_close")
        close = close + (verdict == "close")
        possible = possible + (verdict == "possible")
        lines.append("%s\t%.1f\t%.1f\t%+.1f\t%.3f\t%s" % (picture["name"], picture["estimate"], seconds, seconds - picture["estimate"], score, verdict))
        pair_path = os.path.join(output_folder, picture["name"] + "_pair.jpg")
        label = "%s  closest footage %.1f s (%+.1f)  difference %.3f  %s" % (picture["name"], seconds, seconds - picture["estimate"], score, {"close": "same shot", "possible": "same shot?", "none_close": "NO CLOSE SHOT"}[verdict])
        write_pair(burst_path, reference_frame(seconds, frames_folder, footage_path), label, pair_path)
        pair_paths.append(pair_path)
    write_sheet(pair_paths, os.path.join(output_folder, "pairs_sheet.jpg"))
    with open(os.path.join(output_folder, "matches.tsv"), "w", encoding="ascii") as matches:
        matches.write("\n".join(lines) + "\n")
    print("paired %d pictures with their closest footage within %.0f s of the expected time: %d same shot, %d possibly the same shot, %d with no close shot; %d without a route step; see matches.tsv" % (len(pair_paths), search_seconds, close, possible, len(pair_paths) - close - possible, unmatched))
    return 0


def main():
    parser = argparse.ArgumentParser(description="Pairs each burst picture with the closest-looking reference footage frame near its expected time. The expected time comes from the last route action stamped on the picture and the footage time of that action; the search around it absorbs drift, so a close match shows the same shot and any difference in it is worth a look, while a picture with no close shot nearby is reported as such.")
    parser.add_argument("burst_folder")
    parser.add_argument("--set", default=DEFAULT_SET)
    parser.add_argument("--footage", default=DEFAULT_FOOTAGE)
    parser.add_argument("--frames", default=DEFAULT_FRAMES)
    parser.add_argument("--output")
    parser.add_argument("--search-seconds", type=float, default=DEFAULT_SEARCH_SECONDS, help="how far from the expected footage time to look for the closest-looking frame")
    parser.add_argument("--log", help="the device log of the same run; its route action ticks stretch each window between two actions to the footage's length, so long talks line up at both ends")
    arguments = parser.parse_args()
    return match(arguments.burst_folder, arguments.set, arguments.footage, arguments.frames, arguments.output or os.path.join(arguments.burst_folder, "pairs"), arguments.log, arguments.search_seconds)


if __name__ == "__main__":
    sys.exit(main())
