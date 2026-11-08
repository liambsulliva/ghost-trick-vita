import os
import subprocess
import tempfile

import audio_file
import bake_record
import sseq_render

MUSIC_FOLDER = "music_ds"
CODE_FILES = ["audio_file.py", "ds_music.py", "sseq_render.py"]
ENCODER_QUALITY = "8"


def track_path(name, suffix):
    return MUSIC_FOLDER + "/" + name + suffix


def frames_at_playback_rate(frames, sample_rate):
    return int(round(frames * audio_file.PLAYBACK_RATE / sample_rate))


def encode_ogg(samples, sample_rate):
    with tempfile.TemporaryDirectory() as work_folder:
        output_path = os.path.join(work_folder, "track.ogg")
        subprocess.run(["ffmpeg", "-v", "error", "-f", "s16le", "-ar", str(sample_rate), "-ac", "2", "-i", "-", "-ar", str(audio_file.PLAYBACK_RATE), "-ac", "2", "-c:a", "vorbis", "-strict", "-2", "-q:a", ENCODER_QUALITY, output_path], input=samples, check=True)
        with open(output_path, "rb") as source:
            return source.read()


def bake_track(sdat, sequence, name, record, code):
    relative_path = track_path(name, ".ogg")
    key = record.key_for(code, {"sequence": sequence}, sdat)
    ogg = record.read_if_current(relative_path, key)
    loop_points = record.details_of(relative_path).get("loop_points")
    if ogg is None:
        rendered = sseq_render.render_sequence(sdat, sequence)
        ogg = encode_ogg(rendered["samples"], rendered["sample_rate"])
        loop_points = None
        if rendered["loop_start"] is not None:
            loop_points = [frames_at_playback_rate(rendered["loop_start"], rendered["sample_rate"]), frames_at_playback_rate(rendered["loop_end"], rendered["sample_rate"]) - 1]
        record.store(relative_path, key, ogg, {"loop_points": loop_points})
    return ogg, loop_points


def sequence_of_music_id(sdat, music_ids):
    if not music_ids:
        return {}
    by_index = {index: name for name, index in sseq_render.named_sequences(sseq_render.read_archive(sdat)).items()}
    return {name: by_index[int(music_id)] for music_id, name in music_ids.items() if int(music_id) in by_index}


def music_files(section, sdat, remaster_tracks, record, music_ids=None):
    code = bake_record.code_fingerprint(CODE_FILES, subprocess.run(["ffmpeg", "-version"], capture_output=True).stdout.decode("utf-8", "replace"))
    files = {}
    derived = sequence_of_music_id(sdat, music_ids or {})
    for track in remaster_tracks:
        name = track["name"]
        sequence = section.get("sequences", {}).get(name, derived.get(name))
        if sequence is None:
            files[track_path(name, ".ogg")] = track["ogg"]
            if track["loop_points"] is not None:
                files[track_path(name, ".loop")] = audio_file.loop_text(track["loop_points"]).encode("ascii")
            continue
        ogg, loop_points = bake_track(sdat, sequence, name, record, code)
        files[track_path(name, ".ogg")] = ogg
        if loop_points is not None:
            files[track_path(name, ".loop")] = audio_file.loop_text(tuple(loop_points)).encode("ascii")
    return files
