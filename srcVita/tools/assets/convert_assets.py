import argparse
import concurrent.futures
import json
import os
import shutil
import signal
import subprocess
import sys
import tempfile
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import asset_writers
import audio_file
import bake_record
import bubble_art
import ds_rom
import frame_layout
import hud_art
import narration
import pack_files
import script_assets
import chapter_assets
import talk_assets
import texture_encoder
import timeline_file

MOVIE_CODE_FILES = ["convert_assets.py", "texture_encoder.py"]
ASSET_TOOLS_FOLDER = os.path.dirname(os.path.abspath(__file__))
SRC_VITA_FOLDER = os.path.dirname(os.path.dirname(ASSET_TOOLS_FOLDER))
REPOSITORY_FOLDER = os.path.dirname(SRC_VITA_FOLDER)
DEFAULT_APK_FOLDER = os.path.join(REPOSITORY_FOLDER, "extracted", "remaster_apk")
DEFAULT_DS_ROM_PATH = os.path.join(REPOSITORY_FOLDER, "extracted", "ds_rom", "game.nds")
TEXTURE_TOOL_PATH = os.path.join(SRC_VITA_FOLDER, "build", "tools", "texture_tool")
FORMAT_NAMES = {asset_writers.FORMAT_RGBA8888: "RGBA8888", asset_writers.FORMAT_DXT5: "DXT5", asset_writers.FORMAT_DXT1: "DXT1"}
OUTPUT_FOLDER_NAMES = ("textures", "animations", "scenes", "music", "sounds", "scripts")
TEXTURE_CODE_FILES = ["asset_writers.py", "astc_decode.c", "astc_decode.h", "etc_decode.c", "etc_decode.h", "texture_encoder.py", "texture_tool.c"]
COMPOSED_CODE_FILES = TEXTURE_CODE_FILES + ["bubble_art.py"]
TEXT_CODE_FILES = ["asset_writers.py", "chapter_assets.py", "font_file.py", "line_renderer.py", "message_cues.py", "message_file.py", "narration.py", "talk_assets.py", "texture_encoder.py"]
WORK_FOLDER_PREFIX = "ghost_trick_convert_"
OLD_WORK_FOLDER_SECONDS = 3600
EXIT_CODE_BASE_FOR_SIGNALS = 128
STOP_SIGNALS = (signal.SIGINT, signal.SIGTERM, signal.SIGHUP)

stop_signal_number = None


class ConversionFailed(ValueError):
    def __init__(self, failures):
        super().__init__("; ".join(name + ": " + reason for name, reason in failures))
        self.failures = failures


def load_set_description(set_name):
    path = set_name if os.path.isfile(set_name) else os.path.join(ASSET_TOOLS_FOLDER, set_name + ".json")
    with open(path, "r", encoding="ascii") as source:
        return json.load(source)


def load_timelines(packs, animations):
    timelines = {}
    for animation in animations:
        path = animation["timeline"]
        if path not in timelines:
            timelines[path] = timeline_file.decode_timeline(packs.read(path))
    return timelines


def add_atlas_page_jobs(jobs, animations, timelines):
    crops = {}
    for animation in animations:
        timeline = timelines[animation["timeline"]]
        ds_frames = animation.get("ds_frames")
        frame_layout.add_used_cells_to_crops(crops, timeline, ds_frames)
        for record in frame_layout.select_records(timeline, ds_frames):
            if frame_layout.record_draws_art(timeline, record):
                logical_path = timeline["pages"][record["page"]]
                name = frame_layout.texture_name_for(logical_path)
                jobs[name] = {"name": name, "source": frame_layout.pack_path_for(logical_path)}
    for name, crop in crops.items():
        jobs[name]["crop"] = list(crop)
    return crops


def add_plate_jobs(jobs, plates, soul_ramp):
    for plate in plates:
        jobs[plate["name"]] = {"name": plate["name"], "source": plate["source"]}
        if "soul_plate" in plate:
            jobs[plate["soul_plate"]] = {"name": plate["soul_plate"], "source": plate["source"], "soul_ramp": soul_ramp}


def add_screen_texture_jobs(jobs, screen_textures):
    for texture in screen_textures:
        jobs[texture["name"]] = dict(texture)


def bake_movie_frames(packs, record, movie):
    with tempfile.TemporaryDirectory() as work_folder:
        return bake_movie_frames_in(packs, work_folder, record, movie)


def bake_movie_frames_in(packs, work_folder, record, movie):
    source = packs.read_by_key(movie["pack_key"])
    source_path = os.path.join(work_folder, movie["name"] + ".mov")
    with open(source_path, "wb") as output:
        output.write(source)
    width = movie["width"]
    height = movie["height"]
    frames = subprocess.run(["ffmpeg", "-v", "error", "-i", source_path, "-vf", "fps=%d,scale=%d:%d" % (movie["fps"], width, height), "-f", "rawvideo", "-pix_fmt", "rgba", "-"], capture_output=True, check=True).stdout
    os.remove(source_path)
    code = bake_record.code_fingerprint(MOVIE_CODE_FILES)
    frame_size = width * height * 4
    results = []
    for index in range(len(frames) // frame_size):
        name = "movie_%s_%03d" % (movie["name"], index)
        relative_path = "textures/" + name + ".gtex"
        pixels = frames[index * frame_size:(index + 1) * frame_size]
        key = record.key_for(code, {"width": width, "height": height}, pixels)
        gtex_bytes = record.read_if_current(relative_path, key)
        if gtex_bytes is None:
            blocks = texture_encoder.encode_rgba(work_folder, name, width, height, pixels, "dxt1")
            gtex_bytes = asset_writers.gtex_bytes(width, height, asset_writers.FORMAT_DXT1, blocks)
            record.store(relative_path, key, gtex_bytes)
        results.append(bake_record.result_of_own_size(name, gtex_bytes))
    movie["frames"] = len(results)
    return results


def code_fingerprint_with_magick(file_names):
    magick_version = subprocess.run(["magick", "-version"], stdout=subprocess.PIPE, stderr=subprocess.DEVNULL).stdout
    return bake_record.code_fingerprint(file_names, magick_version.decode("utf-8", "replace"))


def texture_code_fingerprint():
    return code_fingerprint_with_magick(TEXTURE_CODE_FILES)


def composed_code_fingerprint():
    return code_fingerprint_with_magick(COMPOSED_CODE_FILES)


def text_code_fingerprint():
    return code_fingerprint_with_magick(TEXT_CODE_FILES)


def size_for_job(job, source_size):
    if "width" in job:
        return job["width"], job["height"]
    if "crop" in job:
        return frame_layout.target_size(job["crop"][2], job["crop"][3])
    return frame_layout.target_size(source_size[0], source_size[1])


def convert_one_texture(packs, work_folder, job, record, code):
    tex_bytes = packs.read(job["source"])
    source_size = texture_encoder.read_tex_size(tex_bytes)
    width, height = size_for_job(job, source_size)
    relative_path = "textures/" + job["name"] + ".gtex"
    settings = {"width": width, "height": height, "crop": job.get("crop")}
    if "soul_ramp" in job:
        settings["soul_ramp"] = job["soul_ramp"]
    if "flat_color" in job:
        settings["flat_color"] = job["flat_color"]
    key = record.key_for(code, settings, tex_bytes)
    gtex_bytes = record.read_if_current(relative_path, key)
    if gtex_bytes is None:
        gtex_bytes, _ = texture_encoder.convert_tex_to_gtex(TEXTURE_TOOL_PATH, work_folder, job["name"], tex_bytes, width, height, job.get("crop"), job.get("soul_ramp"), flat_color=job.get("flat_color"))
        record.store(relative_path, key, gtex_bytes)
    return bake_record.texture_result(job["name"], source_size, gtex_bytes)


def convert_all_textures(packs, jobs, work_folder, record):
    code = texture_code_fingerprint()
    results = []
    failures = []
    with concurrent.futures.ThreadPoolExecutor(max_workers=os.cpu_count()) as pool:
        futures = {job["name"]: pool.submit(convert_one_texture, packs, work_folder, job, record, code) for job in jobs.values()}
        try:
            for name, future in futures.items():
                try:
                    results.append(future.result())
                except Exception as error:
                    failures.append((name, str(error)))
        except BaseException:
            for future in futures.values():
                future.cancel()
            raise
    return results, failures


def convert_composed_textures(packs, descriptions, work_folder, record):
    code = composed_code_fingerprint() if descriptions else None
    results = []
    failures = []
    for description in descriptions:
        try:
            results.append(bubble_art.bake_composed_texture(packs, TEXTURE_TOOL_PATH, work_folder, description, record, code))
        except Exception as error:
            failures.append((description["name"], str(error)))
    return results, failures


def convert_hud_sheet(packs, set_description, work_folder, record):
    hud = set_description.get("hud")
    if hud is None:
        return []
    fonts = narration.TextFonts(packs, work_folder)
    return [hud_art.bake_hud_sheet(fonts, work_folder, record, code_fingerprint_with_magick(TEXT_CODE_FILES + ["hud_art.py"]), hud["label_font"], hud["key_font"])]


def convert_text_textures(packs, set_description, messages, work_folder, record):
    code = text_code_fingerprint() if narration.set_uses_text(set_description) else None
    return narration.bake_text_textures(packs, set_description, messages, work_folder, record, code)


def plate_size_lookup(texture_results, plates):
    plate_names = set(plate["name"] for plate in plates)
    sizes = {}
    for result in texture_results:
        if result["name"] in plate_names:
            sizes[result["name"]] = (result["source_size"][0] / frame_layout.ART_PER_DS_PIXEL, result["source_size"][1] / frame_layout.ART_PER_DS_PIXEL)
    return sizes


def object_step(set_description, name, animation_name, x, y):
    settings = set_description["objects"][name]
    return ["object", name, animation_name, x, y, settings["layer"], settings["flip"]]


def expand_steps(set_description, plate_sizes):
    steps = []
    for step in set_description.get("steps", []):
        kind = step[0]
        if kind == "object_group":
            for name, animation_name, x, y in set_description["object_groups"][step[1]]:
                steps.append(object_step(set_description, name, animation_name, x, y))
        elif kind == "object":
            _, name, animation_name, x, y = step
            steps.append(object_step(set_description, name, animation_name, x, y))
        elif kind == "plate":
            _, name, x, y = step
            width, height = plate_sizes[name]
            steps.append(["plate", name, x, y, width, height])
        else:
            steps.append(list(step))
    return steps


def check_steps_refer_to_known_assets(steps, texture_names, animation_names, music_names, sound_names):
    for step in steps:
        kind = step[0]
        if kind in ("plate", "card") and step[1] not in texture_names:
            raise ValueError(kind + " names an unknown texture: " + step[1])
        if kind == "object" and step[2] not in animation_names:
            raise ValueError("object names an unknown animation: " + step[2])
        if kind == "music" and step[1] not in music_names:
            raise ValueError("music step names an unknown track: " + step[1])
        if kind == "sound" and step[1] not in sound_names:
            raise ValueError("sound step names an unknown sound: " + step[1])
        if kind in ("text_show", "overlay") and step[2] not in texture_names:
            raise ValueError(kind + " names an unknown texture: " + step[2])


def check_texture_names_differ(names):
    seen = set()
    repeated = []
    for name in names:
        if name.lower() in seen:
            repeated.append((name, "another texture has this name"))
        seen.add(name.lower())
    if repeated:
        raise ConversionFailed(repeated)


def read_audio_streams(packs, entries):
    results = []
    for entry in entries:
        asset_writers.check_name(entry["name"])
        stream = audio_file.read_stream(packs.read(entry["source"]), entry["source"])
        audio_file.check_playback_rate(stream, entry["source"])
        results.append({"name": entry["name"], "ogg": stream["ogg"], "loop_points": stream["loop_points"]})
    return results


def audio_output_files(music_results, sound_results):
    files = {}
    for result in music_results:
        files["music/" + result["name"] + ".ogg"] = result["ogg"]
        if result["loop_points"] is not None:
            files["music/" + result["name"] + ".loop"] = audio_file.loop_text(result["loop_points"]).encode("ascii")
    for result in sound_results:
        files["sounds/" + result["name"] + ".ogg"] = result["ogg"]
    return files


def text_output_files(set_description, animation_texts, steps):
    files = {"animations/" + name + ".anim": text.encode("ascii") for name, text in animation_texts.items()}
    scene_name = set_description.get("scene_name")
    if scene_name is not None:
        files["scenes/" + scene_name + ".scene"] = asset_writers.scene_text(scene_name, steps).encode("ascii")
    return files


def manifest_bytes(set_description, animation_texts):
    scene_name = set_description.get("scene_name")
    scene_names = [] if scene_name is None else [scene_name]
    background_names = [plate["name"] for plate in set_description.get("plates", [])]
    for texture in set_description.get("screen_textures", []):
        if texture.get("list_as_background"):
            background_names.append(texture["name"])
    return asset_writers.manifest_text(scene_names, background_names, list(animation_texts.keys())).encode("ascii")


def make_output_folders(output_folder):
    for folder_name in OUTPUT_FOLDER_NAMES:
        os.makedirs(os.path.join(output_folder, folder_name), exist_ok=True)


def write_outputs(output_folder, record, texture_results, files, manifest, script_paths):
    for relative_path, data in files.items():
        bake_record.write_if_different(os.path.join(output_folder, relative_path), data)
    wanted_paths = set(files) | set("textures/" + result["name"] + ".gtex" for result in texture_results) | set(script_paths)
    bake_record.remove_files_not_listed(output_folder, OUTPUT_FOLDER_NAMES, wanted_paths)
    record.forget_all_except(wanted_paths)
    bake_record.write_if_different(os.path.join(output_folder, "manifest.txt"), manifest)
    bake_record.remove_leftover_temporary_file(os.path.join(output_folder, "manifest.txt"))


def print_summary(record, texture_results, animation_texts, steps, music_results, sound_results):
    totals = {}
    for result in texture_results:
        format_name = FORMAT_NAMES[result["format"]]
        count, total_bytes = totals.get(format_name, (0, 0))
        totals[format_name] = (count + 1, total_bytes + result["byte_count"])
    print("textures: %d, %d baked, %d unchanged" % (len(texture_results), record.baked_in("textures"), record.reused_in("textures")))
    for format_name in sorted(totals):
        print("  %s: %d files, %d bytes" % (format_name, totals[format_name][0], totals[format_name][1]))
    print("  total: %d bytes" % sum(result["byte_count"] for result in texture_results))
    print("  memory on the Vita: %d bytes" % sum(asset_writers.memory_bytes(result["width"], result["height"], result["format"]) for result in texture_results))
    print("largest textures:")
    largest_first = sorted(texture_results, key=lambda result: result["byte_count"], reverse=True)
    for result in largest_first[:8]:
        print("  %s: %dx%d %s, %d bytes" % (result["name"], result["width"], result["height"], FORMAT_NAMES[result["format"]], result["byte_count"]))
    animation_bytes = sum(len(text) for text in animation_texts.values())
    frame_count = sum(text.count("\nframe ") for text in animation_texts.values())
    print("animations: %d files, %d frames, %d bytes" % (len(animation_texts), frame_count, animation_bytes))
    print("scene steps: %d" % len(steps))
    print("music: %d files, %d bytes" % (len(music_results), sum(len(result["ogg"]) for result in music_results)))
    print("sounds: %d files, %d bytes" % (len(sound_results), sum(len(result["ogg"]) for result in sound_results)))
    if record.baked_in("scripts") + record.reused_in("scripts") > 0:
        print("scripts: %d baked" % record.baked_in("scripts"))


def animation_texts_for(set_description, timelines, crops):
    texts = {}
    for animation in set_description.get("animations", []):
        if animation.get("table_only"):
            continue
        timeline = timelines[animation["timeline"]]
        frames = frame_layout.frames_for_animation(timeline, animation.get("ds_frames"), crops)
        texts[animation["name"]] = asset_writers.anim_text(animation["name"], animation.get("loops", 0), frames)
    return texts


def work_folder_is_old(path, oldest_kept_time):
    try:
        return os.path.getmtime(path) < oldest_kept_time
    except OSError:
        return False


def remove_old_work_folders():
    temp_folder = tempfile.gettempdir()
    oldest_kept_time = time.time() - OLD_WORK_FOLDER_SECONDS
    for name in os.listdir(temp_folder):
        path = os.path.join(temp_folder, name)
        if name.startswith(WORK_FOLDER_PREFIX) and work_folder_is_old(path, oldest_kept_time):
            shutil.rmtree(path, ignore_errors=True)


def delete_work_folder(work_folder):
    try:
        shutil.rmtree(work_folder, ignore_errors=True)
    except SystemExit:
        shutil.rmtree(work_folder, ignore_errors=True)
        raise


def convert_set_description(set_description, output_folder, apk_folder, text_key, ds_rom_path=DEFAULT_DS_ROM_PATH):
    remove_old_work_folders()
    packs = pack_files.PackSet(apk_folder)
    set_description = chapter_assets.expand_set(set_description, packs, ds_rom_path, text_key)
    animations = set_description.get("animations", [])
    plates = set_description.get("plates", [])
    timelines = load_timelines(packs, animations)
    jobs = {}
    crops = add_atlas_page_jobs(jobs, animations, timelines)
    atlas_page_names = list(jobs)
    add_plate_jobs(jobs, plates, set_description.get("soul_view", {}).get("ramp"))
    used_faces = talk_assets.faces_the_set_will_use(packs, set_description, text_key)
    chapter_assets.add_face_textures(set_description, used_faces) if used_faces else None
    add_screen_texture_jobs(jobs, set_description.get("screen_textures", []))
    animation_texts = animation_texts_for(set_description, timelines, crops)
    music_results = read_audio_streams(packs, set_description.get("music", []))
    sound_results = read_audio_streams(packs, set_description.get("sounds", []))
    messages = narration.read_text_messages(packs, set_description, text_key)
    check_texture_names_differ(atlas_page_names + [entry["name"] for entry in plates + set_description.get("screen_textures", []) + set_description.get("composed_textures", [])] + narration.text_texture_names(set_description, messages))
    make_output_folders(output_folder)
    record = bake_record.BakeRecord(output_folder)
    try:
        script_paths = script_assets.bake_or_keep_scripts(set_description, ds_rom_path, output_folder, record)
        work_folder = tempfile.mkdtemp(prefix=WORK_FOLDER_PREFIX)
        try:
            texture_results, failures = convert_all_textures(packs, jobs, work_folder, record)
            narration_lines, text_results, text_failures = convert_text_textures(packs, set_description, messages, work_folder, record)
            composed_results, composed_failures = convert_composed_textures(packs, set_description.get("composed_textures", []), work_folder, record)
            talk_files, talk_results, talk_failures, _ = talk_assets.bake_talk(packs, set_description, text_key, work_folder, record, text_code_fingerprint())
            talk_results = talk_results + convert_hud_sheet(packs, set_description, work_folder, record)
        finally:
            delete_work_folder(work_folder)
        failures = failures + text_failures + composed_failures + talk_failures
        if failures:
            raise ConversionFailed(failures)
        for movie in set_description.get("movies", []):
            talk_results = talk_results + bake_movie_frames(packs, record, movie)
        texture_results = sorted(texture_results + text_results + composed_results + talk_results, key=lambda result: result["name"])
        steps = expand_steps(set_description, plate_size_lookup(texture_results, plates))
        steps = narration.expand_text_steps(steps, narration_lines, {result["name"]: (result["width"], result["height"]) for result in texture_results})
        check_steps_refer_to_known_assets(steps, set(result["name"] for result in texture_results), set(animation_texts.keys()), set(result["name"] for result in music_results), set(result["name"] for result in sound_results))
        files = audio_output_files(music_results, sound_results)
        files.update(text_output_files(set_description, animation_texts, steps))
        table_texts, motion_lines = script_assets.table_animations(set_description, output_folder, timelines, crops)
        files.update({"animations/" + name + ".anim": text.encode("ascii") for name, text in table_texts.items()})
        texture_sizes = {result["name"]: (result["width"], result["height"]) for result in texture_results}
        files.update(talk_files)
        files.update(script_assets.table_files(set_description, output_folder, motion_lines, narration_lines, texture_sizes, plate_size_lookup(texture_results, plates), list(table_texts), talk_assets.talk_texture_names(talk_files)))
        write_outputs(output_folder, record, texture_results, files, manifest_bytes(set_description, animation_texts), script_paths)
    finally:
        record.save()
    print_summary(record, texture_results, animation_texts, steps, music_results, sound_results)
    narration.print_narration_summary(narration_lines)
    talk_assets.print_talk_summary(talk_files)


def exit_so_the_work_folder_is_deleted(signal_number, _frame):
    global stop_signal_number
    for stop_signal in STOP_SIGNALS:
        signal.signal(stop_signal, signal.SIG_IGN)
    stop_signal_number = signal_number
    sys.exit(EXIT_CODE_BASE_FOR_SIGNALS + signal_number)


def convert_and_report(set_description, arguments):
    try:
        text_key = narration.load_text_key(set_description, arguments.apk_folder)
        subprocess.run(["make", "-C", ASSET_TOOLS_FOLDER], check=True)
        convert_set_description(set_description, arguments.output, arguments.apk_folder, text_key, arguments.ds_rom)
    except (narration.NarrationError, ds_rom.RomRefused, script_assets.ScriptBakeError) as error:
        print("error: " + str(error), file=sys.stderr)
        return 1
    except ConversionFailed as failed:
        for name, reason in failed.failures:
            print("failed: " + name + ": " + reason, file=sys.stderr)
        return 1
    return 0


def main():
    for stop_signal in STOP_SIGNALS:
        signal.signal(stop_signal, exit_so_the_work_folder_is_deleted)
    parser = argparse.ArgumentParser(description="Converts the remaster's files named by a set description into Vita assets.")
    parser.add_argument("--set", required=True, help="a set description: its name in tools/assets without .json, or a path to a JSON file")
    parser.add_argument("--output", required=True, help="folder to write manifest.txt, textures, animations, scenes, music, sounds and scripts into")
    parser.add_argument("--apk-folder", default=DEFAULT_APK_FOLDER, help="folder holding the remaster's pack files")
    parser.add_argument("--ds-rom", default=DEFAULT_DS_ROM_PATH, help="your own USA DS ROM, read only for a set with a scripts section")
    arguments = parser.parse_args()
    set_description = load_set_description(arguments.set)
    exit_code = convert_and_report(set_description, arguments)
    if stop_signal_number is not None:
        return EXIT_CODE_BASE_FOR_SIGNALS + stop_signal_number
    return exit_code


if __name__ == "__main__":
    sys.exit(main())
