import argparse
import json
import os
import subprocess
import sys
import time

import numpy

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import vita_loop

TOOLS_FOLDER = os.path.dirname(os.path.abspath(__file__))
SRC_VITA_FOLDER = os.path.dirname(TOOLS_FOLDER)
REPOSITORY_FOLDER = os.path.dirname(SRC_VITA_FOLDER)
SET_FOLDER = os.path.join(REPOSITORY_FOLDER, "extracted", "vita_assets", "chapter1_opening")
SET_DESCRIPTION_PATH = os.path.join(TOOLS_FOLDER, "assets", "chapter1_opening.json")
CLIP_PATH = os.path.join(REPOSITORY_FOLDER, "extracted", "reference", "chapter1_720p60.mp4")
OUTPUT_FOLDER = os.path.join(SRC_VITA_FOLDER, "build", "longplay_check")
CHECK_FILES_FOLDER = os.path.join(OUTPUT_FOLDER, "files")

COMPARED_WIDTH = 960
COMPARED_HEIGHT = 540
PLAYFIELD_LEFT = 120
PLAYFIELD_RIGHT = 840
SCREEN_PIXELS_PER_DS_PIXEL = 2.8125
UPLOADER_WATERMARK = (817, 491, 949, 531)
SPOTLIGHT_BLACK_LIMIT = 14
SHIFT_SEARCH_PIXELS = 9
SMALLEST_MEASURED_SIDE = 16
FEWEST_COMPARED_PIXELS = 64
MAX_SHIFT_DS_PIXELS = 0.4
MAX_SPRITE_ERROR = 15.0
MAX_MOTION_ERROR = 12.0
MAX_GAIN_DIFFERENCE = 0.1
MAX_SIDE_BAR_ERROR = 5.5
HOLD_FOREVER_TICKS = 1000000
SECONDS_TO_SETTLE = 1.0
MOTION_WINDOW_START = "00:00:40.200"
MOTION_WINDOW_SECONDS = 2.2
VIDEO_FRAMES_PER_SECOND = 60.0
LOAD_FAILURE_MARKERS = [
    "texture missing",
    "animation missing",
    "animation invalid",
    "animation out_of_memory",
    "animation library_full",
    "scene too_many_objects",
    "scene too_many_plates",
]

CLOSE_UP_POSES = [["cicel", "st01_cicel_999", 284, 578], ["cbag", "st01_cbag_999", 350, 577]]

VIEW_CHECKS = [
    {"name": "check_card", "is_card": True, "time": "00:00:10.500", "measure": [], "max_error": 6.0, "expected_gain": 1.0},
    {"name": "check_before_shot", "room": 11, "camera": (0, 192), "pose_group": "standing_before_the_shot", "time": "00:00:40.250", "spotlight": True, "measure": ["linne", "killer", "shadanki"], "max_error": 9.0, "expected_gain": 0.96},
    {"name": "check_close_up", "room": 11, "camera": (0, 384), "pose_group": "standing_after_the_shot", "extra_poses": CLOSE_UP_POSES, "time": "00:01:07.000", "spotlight": True, "measure": ["cicel", "cbag"], "max_error": 12.0, "expected_gain": 0.48},
    {"name": "check_room7", "room": 7, "camera": (0, 192), "pose_group": "standing_after_the_shot", "time": "00:01:11.000", "measure": ["linne", "killer", "cicel", "cbag", "shadanki"], "max_error": 7.0, "expected_gain": 0.96},
    {"name": "check_room7_middle", "room": 7, "camera": (344, 192), "pose_group": "standing_after_the_shot", "time": "00:06:27.000", "spotlight": True, "measure": ["craneTopA", "hasigo", "bicycle", "switchA", "switchB", "craneBottomA"], "max_error": 7.0, "expected_gain": 0.97},
    {"name": "check_room7_right", "room": 7, "camera": (624, 192), "pose_group": "standing_after_the_shot", "time": "00:24:00.000", "spotlight": True, "measure": ["kasa"], "max_error": 12.0, "expected_gain": 1.0},
]

MOTION_CHECKS = [
    {"object": "killer", "animation": "st01_killera_011", "x": 376, "y": 353, "frames": [0, 8, 16, 24, 30, 37], "other": ["linne", "st01_linne_004", 281, 385]},
    {"object": "linne", "animation": "st01_linne_005", "x": 281, "y": 353, "frames": [0, 3, 6, 10, 13], "other": ["killer", "st01_killera_010_f17", 408, 353]},
]


def read_text(path):
    with open(path, encoding="ascii") as source:
        return source.read()


def write_text(folder, file_name, text):
    os.makedirs(folder, exist_ok=True)
    with open(os.path.join(folder, file_name), "w", encoding="ascii", newline="\n") as output:
        output.write(text)


def animation_frame_lines(animation_name):
    for line in read_text(os.path.join(SET_FOLDER, "animations", animation_name + ".anim")).splitlines():
        if line.startswith("frame "):
            yield line


def room_plate_lines():
    plates = {}
    room = None
    for line in read_text(os.path.join(SET_FOLDER, "scenes", "opening.scene")).splitlines():
        if line.startswith("room "):
            room = int(line.split()[1])
            plates[room] = []
        elif line.startswith("plate "):
            plates[room].append(line)
    return plates


def object_line(set_description, pose):
    object_name, animation_name, x, y = pose
    settings = set_description["objects"][object_name]
    return "object %s %s %s %s %d %d" % (object_name, animation_name, x, y, settings["layer"], settings["flip"])


def check_scene_text(set_description, plates, check):
    lines = ["scene " + check["name"], "side_bars side_bar_left side_bar_right"]
    if check.get("is_card"):
        lines.append("card chapter_card")
    else:
        lines.append("room %d" % check["room"])
        lines.extend(plates[check["room"]])
        lines.append("camera %d %d" % check["camera"])
        for pose in check["placed_objects"]:
            lines.append(object_line(set_description, pose))
    lines.append("wait %d" % HOLD_FOREVER_TICKS)
    return "\n".join(lines) + "\n"


def pose_of_object(check, object_name):
    return [candidate for candidate in check["placed_objects"] if candidate[0] == object_name][-1]


def only_object_scene_name(check_name, object_name):
    return check_name + "_only_" + object_name


def only_object_scene_text(set_description, check, object_name):
    lines = [
        "scene " + only_object_scene_name(check["name"], object_name),
        "room %d" % check["room"],
        "camera %d %d" % check["camera"],
        object_line(set_description, pose_of_object(check, object_name)),
        "wait %d" % HOLD_FOREVER_TICKS,
    ]
    return "\n".join(lines) + "\n"


def measured_objects(check):
    if "object" in check:
        return [check["object"]]
    return check["measure"]


def expand_pose_group(set_description, check):
    check["poses"] = list(set_description["object_groups"].get(check.get("pose_group"), [])) + list(check.get("extra_poses", []))
    return check


def build_motion_checks():
    checks = []
    for motion in MOTION_CHECKS:
        frames = list(animation_frame_lines(motion["animation"]))
        for frame_index in motion["frames"]:
            animation_name = "%s_check%02d" % (motion["animation"], frame_index)
            text = "animation %s\nloops 0\n%s\n" % (animation_name, frames[frame_index])
            write_text(os.path.join(CHECK_FILES_FOLDER, "animations"), animation_name + ".anim", text)
            checks.append({
                "name": "check_%s_%02d" % (motion["object"], frame_index),
                "room": 11,
                "camera": (0, 192),
                "poses": [motion["other"], [motion["object"], animation_name, motion["x"], motion["y"]]],
                "object": motion["object"],
            })
    return checks


def write_check_files():
    set_description = json.loads(read_text(SET_DESCRIPTION_PATH))
    plates = room_plate_lines()
    view_checks = [expand_pose_group(set_description, dict(check)) for check in VIEW_CHECKS]
    motion_checks = build_motion_checks()
    background_objects = set_description["object_groups"]["scenery"] + set_description["object_groups"]["body_on_floor"]
    scenes_folder = os.path.join(CHECK_FILES_FOLDER, "scenes")
    only_object_scene_names = []
    for check in view_checks + motion_checks:
        check["placed_objects"] = background_objects + check.get("poses", [])
        write_text(scenes_folder, check["name"] + ".scene", check_scene_text(set_description, plates, check))
        for object_name in measured_objects(check):
            scene_name = only_object_scene_name(check["name"], object_name)
            write_text(scenes_folder, scene_name + ".scene", only_object_scene_text(set_description, check, object_name))
            only_object_scene_names.append(scene_name)
    return view_checks, motion_checks, only_object_scene_names


def remote_check_files():
    pairs = []
    for folder_name in ("scenes", "animations"):
        folder = os.path.join(CHECK_FILES_FOLDER, folder_name)
        for file_name in sorted(os.listdir(folder)):
            pairs.append((os.path.join(folder, file_name), vita_loop.ASSETS_FOLDER + "/" + folder_name + "/" + file_name))
    return pairs


def upload_check_files():
    connection = vita_loop.open_ftp()
    try:
        for local_path, remote_path in remote_check_files():
            vita_loop.upload_verified(connection, local_path, remote_path)
    finally:
        connection.close()


def delete_check_files_from_device():
    connection = vita_loop.open_ftp()
    try:
        for _, remote_path in remote_check_files():
            vita_loop.delete_if_present(connection, remote_path)
    finally:
        connection.close()


def device_matches_local_files():
    expected_files = [
        (vita_loop.APP_FOLDER + "/eboot.bin", os.path.join(vita_loop.BUILD_FOLDER, "eboot.bin"), "upload-eboot"),
        (vita_loop.ASSETS_FOLDER + "/manifest.txt", os.path.join(SET_FOLDER, "manifest.txt"), "upload-assets " + SET_FOLDER),
    ]
    for remote_path, local_path, action in expected_files:
        remote_bytes = vita_loop.download_or_none(remote_path)
        if remote_bytes is None or vita_loop.sha256_of_bytes(remote_bytes) != vita_loop.sha256_of_file(local_path):
            print(remote_path, "differs from", local_path, "- run: vita_loop.py", action)
            return False
    return True


def capture_one_scene(scene_name):
    vita_loop.write_app_commands("play_scene " + scene_name + "\n")
    if vita_loop.wait_for_text(vita_loop.scene_ready_text(scene_name), 30) is None:
        print("scene never became ready:", scene_name)
        return False
    time.sleep(SECONDS_TO_SETTLE)
    if vita_loop.request_capture(scene_name, OUTPUT_FOLDER) != 0:
        return False
    return True


def run_log_shows_no_load_failures():
    failure_lines = [line for line in vita_loop.latest_run_lines() if any(marker in line for marker in LOAD_FAILURE_MARKERS)]
    for line in failure_lines:
        print(line)
    return not failure_lines


def capture_on_device(scene_names):
    vita_loop.quit_and_wait()
    if not device_matches_local_files():
        return False
    try:
        if vita_loop.launch() != 0:
            return False
        if vita_loop.wait_for_text(vita_loop.SCENE_READY_TEXT, 60) is None:
            print("the first scene never became ready")
            return False
        for scene_name in scene_names:
            if not capture_one_scene(scene_name):
                return False
        return run_log_shows_no_load_failures()
    finally:
        vita_loop.quit_and_wait()


def read_capture(name):
    command = ["magick", os.path.join(OUTPUT_FOLDER, name + ".bmp"), "-gravity", "center", "-crop", "%dx%d+0+0" % (COMPARED_WIDTH, COMPARED_HEIGHT), "+repage", "-depth", "8", "rgb:-"]
    pixels = subprocess.run(command, capture_output=True, check=True).stdout
    return numpy.frombuffer(pixels, dtype=numpy.uint8).reshape(COMPARED_HEIGHT, COMPARED_WIDTH, 3).astype(numpy.float32)


def read_clip_frames(start, seconds=None):
    command = ["ffmpeg", "-nostdin", "-loglevel", "error", "-ss", start, "-i", CLIP_PATH]
    if seconds is None:
        command.extend(["-frames:v", "1"])
    else:
        command.extend(["-t", str(seconds)])
    command.extend(["-vf", "scale=%d:%d" % (COMPARED_WIDTH, COMPARED_HEIGHT), "-f", "rawvideo", "-pix_fmt", "rgb24", "-"])
    pixels = subprocess.run(command, capture_output=True, check=True).stdout
    return numpy.frombuffer(pixels, dtype=numpy.uint8).reshape(-1, COMPARED_HEIGHT, COMPARED_WIDTH, 3).astype(numpy.float32)


def first_drawn_frame(animation_name):
    for folder in (CHECK_FILES_FOLDER, SET_FOLDER):
        path = os.path.join(folder, "animations", animation_name + ".anim")
        if not os.path.exists(path):
            continue
        for line in read_text(path).splitlines():
            if line.startswith("frame ") and not line.startswith("frame - "):
                return [float(value) for value in line.split()[3:7]]
    raise KeyError(animation_name)


def sprite_rectangle(check, object_name):
    _, animation_name, pose_x, pose_y = pose_of_object(check, object_name)
    frame_x, frame_y, frame_width, frame_height = first_drawn_frame(animation_name)
    left = PLAYFIELD_LEFT + (pose_x + frame_x - check["camera"][0]) * SCREEN_PIXELS_PER_DS_PIXEL
    top = (pose_y + frame_y - check["camera"][1]) * SCREEN_PIXELS_PER_DS_PIXEL
    margin = SHIFT_SEARCH_PIXELS + 1
    right = min(int(left + frame_width * SCREEN_PIXELS_PER_DS_PIXEL), PLAYFIELD_RIGHT - margin)
    bottom = min(int(top + frame_height * SCREEN_PIXELS_PER_DS_PIXEL), COMPARED_HEIGHT - margin)
    left = max(int(left), PLAYFIELD_LEFT + margin)
    top = max(int(top), margin)
    if right - left < SMALLEST_MEASURED_SIDE or bottom - top < SMALLEST_MEASURED_SIDE:
        return None
    return left, top, right, bottom


def comparable_pixels(reference, spotlight):
    comparable = numpy.ones((COMPARED_HEIGHT, COMPARED_WIDTH), dtype=bool)
    left, top, right, bottom = UPLOADER_WATERMARK
    comparable[top:bottom, left:right] = False
    if spotlight:
        comparable = comparable & (reference.max(axis=2) > SPOTLIGHT_BLACK_LIMIT)
    return comparable


def brightness_gain(capture_pixels, reference_pixels):
    return (capture_pixels * reference_pixels).sum() / max((capture_pixels * capture_pixels).sum(), 1.0)


def error_after_brightness_fit(capture, reference, comparable):
    if comparable.sum() < FEWEST_COMPARED_PIXELS:
        return None
    capture_pixels = capture[comparable]
    reference_pixels = reference[comparable]
    return float(numpy.abs(capture_pixels * brightness_gain(capture_pixels, reference_pixels) - reference_pixels).mean())


def sprite_pixels_of_object(check_name, object_name):
    return read_capture(only_object_scene_name(check_name, object_name)).max(axis=2) > SPOTLIGHT_BLACK_LIMIT


def best_shift_in_ds_pixels(capture, reference, comparable, rectangle, sprite_pixels):
    left, top, right, bottom = rectangle
    capture_patch = capture[top:bottom, left:right]
    sprite_patch = sprite_pixels[top:bottom, left:right]
    best_error = None
    best_shift = None
    for shift_y in range(-SHIFT_SEARCH_PIXELS, SHIFT_SEARCH_PIXELS + 1):
        for shift_x in range(-SHIFT_SEARCH_PIXELS, SHIFT_SEARCH_PIXELS + 1):
            reference_patch = reference[top + shift_y:bottom + shift_y, left + shift_x:right + shift_x]
            comparable_patch = comparable[top + shift_y:bottom + shift_y, left + shift_x:right + shift_x] & sprite_patch
            error = error_after_brightness_fit(capture_patch, reference_patch, comparable_patch)
            if error is not None and (best_error is None or error < best_error):
                best_error = error
                best_shift = (shift_x / SCREEN_PIXELS_PER_DS_PIXEL, shift_y / SCREEN_PIXELS_PER_DS_PIXEL)
    return best_shift, best_error


def shift_is_acceptable(shift):
    return abs(shift[0]) <= MAX_SHIFT_DS_PIXELS and abs(shift[1]) <= MAX_SHIFT_DS_PIXELS


def write_pair_image(name, capture, reference):
    pair = numpy.concatenate((capture, reference), axis=0).astype(numpy.uint8)
    path = os.path.join(OUTPUT_FOLDER, name + "_pair.png")
    subprocess.run(["magick", "-size", "%dx%d" % (pair.shape[1], pair.shape[0]), "-depth", "8", "rgb:-", path], input=pair.tobytes(), check=True)


def side_bar_error(capture, reference):
    side_bars = numpy.zeros((COMPARED_HEIGHT, COMPARED_WIDTH), dtype=bool)
    side_bars[:, 0:PLAYFIELD_LEFT] = True
    side_bars[:, PLAYFIELD_RIGHT:COMPARED_WIDTH] = True
    compared = side_bars & comparable_pixels(reference, False)
    return float(numpy.abs(capture[compared] - reference[compared]).mean())


def score_view_check(check):
    capture = read_capture(check["name"])
    reference = read_clip_frames(check["time"])[0]
    comparable = comparable_pixels(reference, check.get("spotlight", False))
    playfield = numpy.zeros((COMPARED_HEIGHT, COMPARED_WIDTH), dtype=bool)
    playfield[:, PLAYFIELD_LEFT:PLAYFIELD_RIGHT] = True
    compared = comparable & playfield
    error = error_after_brightness_fit(capture, reference, compared)
    gain = brightness_gain(capture[compared], reference[compared])
    passed = error <= check["max_error"] and abs(gain - check["expected_gain"]) <= MAX_GAIN_DIFFERENCE
    print("%s %-22s error %5.2f of 255, limit %.1f, brightness gain %.2f, expected %.2f" % ("ok  " if passed else "FAIL", check["name"], error, check["max_error"], gain, check["expected_gain"]))
    bar_error = side_bar_error(capture, reference)
    bars_are_acceptable = bar_error <= MAX_SIDE_BAR_ERROR
    passed = passed and bars_are_acceptable
    print("%s     %-16s error %5.2f of 255, limit %.1f" % ("ok  " if bars_are_acceptable else "FAIL", "side bars", bar_error, MAX_SIDE_BAR_ERROR))
    for object_name in check["measure"]:
        rectangle = sprite_rectangle(check, object_name)
        shift = None
        sprite_error = None
        if rectangle is not None:
            shift, sprite_error = best_shift_in_ds_pixels(capture, reference, comparable, rectangle, sprite_pixels_of_object(check["name"], object_name))
        if shift is None:
            print("FAIL     %-16s could not be measured" % object_name)
            passed = False
            continue
        is_acceptable = shift_is_acceptable(shift) and sprite_error <= MAX_SPRITE_ERROR
        passed = passed and is_acceptable
        print("%s     %-16s shift %+5.2f %+5.2f DS pixels, error %5.2f, limit %.1f" % ("ok  " if is_acceptable else "FAIL", object_name, shift[0], shift[1], sprite_error, MAX_SPRITE_ERROR))
    write_pair_image(check["name"], capture, reference)
    return passed


def find_best_clip_frame(capture, frames, rectangle, sprite_pixels):
    left, top, right, bottom = rectangle
    sprite_patch = sprite_pixels[top:bottom, left:right]
    best_error = None
    best_index = None
    for index, frame in enumerate(frames):
        comparable = comparable_pixels(frame, True)
        error = error_after_brightness_fit(capture[top:bottom, left:right], frame[top:bottom, left:right], comparable[top:bottom, left:right] & sprite_patch)
        if error is not None and (best_error is None or error < best_error):
            best_error = error
            best_index = index
    return best_index, best_error


def score_motion_checks(checks):
    frames = read_clip_frames(MOTION_WINDOW_START, MOTION_WINDOW_SECONDS)
    latest_index = {}
    all_passed = True
    for check in checks:
        capture = read_capture(check["name"])
        rectangle = sprite_rectangle(check, check["object"])
        sprite_pixels = sprite_pixels_of_object(check["name"], check["object"])
        index = None
        error = None
        if rectangle is not None:
            index, error = find_best_clip_frame(capture, frames, rectangle, sprite_pixels)
        if index is None:
            print("FAIL %-22s could not be measured" % check["name"])
            all_passed = False
            continue
        shift, _ = best_shift_in_ds_pixels(capture, frames[index], comparable_pixels(frames[index], True), rectangle, sprite_pixels)
        in_order = index > latest_index.get(check["object"], -1)
        latest_index[check["object"]] = index
        passed = in_order and shift_is_acceptable(shift) and error <= MAX_MOTION_ERROR
        all_passed = all_passed and passed
        print("%s %-22s window %.3f s error %5.2f shift %+5.2f %+5.2f%s" % ("ok  " if passed else "FAIL", check["name"], index / VIDEO_FRAMES_PER_SECOND, error, shift[0], shift[1], "" if in_order else " out of order"))
        write_pair_image(check["name"], capture, frames[index])
    return all_passed


def main():
    parser = argparse.ArgumentParser(description="Compares the Vita's screen against the reference video, one held pose at a time.")
    parser.add_argument("--score-only", action="store_true", help="score the captures already in build/longplay_check without using the device")
    arguments = parser.parse_args()
    os.makedirs(OUTPUT_FOLDER, exist_ok=True)
    view_checks, motion_checks, only_object_scene_names = write_check_files()
    if not arguments.score_only:
        scene_names = [check["name"] for check in view_checks + motion_checks] + only_object_scene_names
        try:
            upload_check_files()
            captured = capture_on_device(scene_names)
        finally:
            delete_check_files_from_device()
        if not captured:
            return 1
    views_passed = [score_view_check(check) for check in view_checks]
    motion_passed = score_motion_checks(motion_checks)
    print("pair images:", OUTPUT_FOLDER)
    return 0 if all(views_passed) and motion_passed else 1


if __name__ == "__main__":
    sys.exit(main())
