import argparse
import concurrent.futures
import json
import os
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import asset_writers
import audio_file
import bake_record
import frame_layout
import pack_files
import texture_encoder
import timeline_file

ASSET_TOOLS_FOLDER = os.path.dirname(os.path.abspath(__file__))
SRC_VITA_FOLDER = os.path.dirname(os.path.dirname(ASSET_TOOLS_FOLDER))
REPOSITORY_FOLDER = os.path.dirname(SRC_VITA_FOLDER)
DEFAULT_APK_FOLDER = os.path.join(REPOSITORY_FOLDER, "extracted", "remaster_apk")
TEXTURE_TOOL_PATH = os.path.join(SRC_VITA_FOLDER, "build", "tools", "texture_tool")
FORMAT_NAMES = {asset_writers.FORMAT_RGBA8888: "RGBA8888", asset_writers.FORMAT_DXT5: "DXT5", asset_writers.FORMAT_DXT1: "DXT1"}
OUTPUT_FOLDER_NAMES = ("textures", "animations", "scenes", "music", "sounds")
TEXTURE_CODE_FILES = ["asset_writers.py", "astc_decode.c", "astc_decode.h", "etc_decode.c", "etc_decode.h", "texture_encoder.py", "texture_tool.c"]


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


def add_plate_jobs(jobs, plates):
    for plate in plates:
        jobs[plate["name"]] = {"name": plate["name"], "source": plate["source"]}


def add_screen_texture_jobs(jobs, screen_textures):
    for texture in screen_textures:
        jobs[texture["name"]] = dict(texture)


def texture_code_fingerprint():
    magick_version = subprocess.run(["magick", "-version"], stdout=subprocess.PIPE, stderr=subprocess.DEVNULL).stdout
    return bake_record.code_fingerprint(TEXTURE_CODE_FILES, magick_version.decode("utf-8", "replace"))


def size_for_job(job, source_size):
    if "width" in job:
        return job["width"], job["height"]
    if "crop" in job:
        return frame_layout.target_size(job["crop"][2], job["crop"][3])
    return frame_layout.target_size(source_size[0], source_size[1])


def texture_result(name, source_size, gtex_bytes):
    width, height, texture_format = asset_writers.read_gtex_header(gtex_bytes)
    return {
        "name": name,
        "source_size": source_size,
        "width": width,
        "height": height,
        "format": texture_format,
        "byte_count": len(gtex_bytes),
    }


def convert_one_texture(packs, work_folder, job, record, code):
    tex_bytes = packs.read(job["source"])
    source_size = texture_encoder.read_tex_size(tex_bytes)
    width, height = size_for_job(job, source_size)
    relative_path = "textures/" + job["name"] + ".gtex"
    key = record.key_for(code, {"width": width, "height": height, "crop": job.get("crop")}, tex_bytes)
    gtex_bytes = record.read_if_current(relative_path, key)
    if gtex_bytes is None:
        gtex_bytes, _ = texture_encoder.convert_tex_to_gtex(TEXTURE_TOOL_PATH, work_folder, job["name"], tex_bytes, width, height, job.get("crop"))
        record.store(relative_path, key, gtex_bytes)
    return texture_result(job["name"], source_size, gtex_bytes)


def convert_all_textures(packs, jobs, work_folder, record):
    code = texture_code_fingerprint()
    results = []
    failures = []
    with concurrent.futures.ThreadPoolExecutor(max_workers=os.cpu_count()) as pool:
        futures = {job["name"]: pool.submit(convert_one_texture, packs, work_folder, job, record, code) for job in jobs.values()}
        for name, future in futures.items():
            try:
                results.append(future.result())
            except Exception as error:
                failures.append((name, str(error)))
    if failures:
        raise ConversionFailed(failures)
    results.sort(key=lambda result: result["name"])
    return results


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


def write_outputs(output_folder, record, texture_results, files, manifest):
    for relative_path, data in files.items():
        bake_record.write_if_different(os.path.join(output_folder, relative_path), data)
    wanted_paths = set(files) | set("textures/" + result["name"] + ".gtex" for result in texture_results)
    bake_record.remove_files_not_listed(output_folder, OUTPUT_FOLDER_NAMES, wanted_paths)
    record.forget_all_except(wanted_paths)
    bake_record.write_if_different(os.path.join(output_folder, "manifest.txt"), manifest)


def print_summary(record, texture_results, animation_texts, steps, music_results, sound_results):
    totals = {}
    for result in texture_results:
        format_name = FORMAT_NAMES[result["format"]]
        count, total_bytes = totals.get(format_name, (0, 0))
        totals[format_name] = (count + 1, total_bytes + result["byte_count"])
    print("textures: %d, %d baked, %d unchanged" % (len(texture_results), record.baked_count, record.reused_count))
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


def animation_texts_for(set_description, timelines, crops):
    texts = {}
    for animation in set_description.get("animations", []):
        timeline = timelines[animation["timeline"]]
        frames = frame_layout.frames_for_animation(timeline, animation.get("ds_frames"), crops)
        texts[animation["name"]] = asset_writers.anim_text(animation["name"], animation.get("loops", 0), frames)
    return texts


def convert_set_description(set_description, output_folder, apk_folder):
    packs = pack_files.PackSet(apk_folder)
    animations = set_description.get("animations", [])
    plates = set_description.get("plates", [])
    timelines = load_timelines(packs, animations)
    jobs = {}
    crops = add_atlas_page_jobs(jobs, animations, timelines)
    add_plate_jobs(jobs, plates)
    add_screen_texture_jobs(jobs, set_description.get("screen_textures", []))
    animation_texts = animation_texts_for(set_description, timelines, crops)
    music_results = read_audio_streams(packs, set_description.get("music", []))
    sound_results = read_audio_streams(packs, set_description.get("sounds", []))
    make_output_folders(output_folder)
    record = bake_record.BakeRecord(output_folder)
    try:
        with tempfile.TemporaryDirectory(dir=output_folder, prefix="work_") as work_folder:
            texture_results = convert_all_textures(packs, jobs, work_folder, record)
        steps = expand_steps(set_description, plate_size_lookup(texture_results, plates))
        check_steps_refer_to_known_assets(steps, set(jobs.keys()), set(animation_texts.keys()), set(result["name"] for result in music_results), set(result["name"] for result in sound_results))
        files = audio_output_files(music_results, sound_results)
        files.update(text_output_files(set_description, animation_texts, steps))
        write_outputs(output_folder, record, texture_results, files, manifest_bytes(set_description, animation_texts))
    finally:
        record.save()
    print_summary(record, texture_results, animation_texts, steps, music_results, sound_results)


def main():
    parser = argparse.ArgumentParser(description="Converts the remaster's files named by a set description into Vita assets.")
    parser.add_argument("--set", required=True, help="a set description: its name in tools/assets without .json, or a path to a JSON file")
    parser.add_argument("--output", required=True, help="folder to write manifest.txt, textures, animations, scenes, music and sounds into")
    parser.add_argument("--apk-folder", default=DEFAULT_APK_FOLDER, help="folder holding the remaster's pack files")
    arguments = parser.parse_args()
    subprocess.run(["make", "-C", ASSET_TOOLS_FOLDER], check=True)
    try:
        convert_set_description(load_set_description(arguments.set), arguments.output, arguments.apk_folder)
    except ConversionFailed as failed:
        for name, reason in failed.failures:
            print("failed: " + name + ": " + reason, file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
