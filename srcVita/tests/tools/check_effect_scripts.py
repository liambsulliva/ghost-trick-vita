import os
import re
import struct
import sys

SRC_VITA = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(SRC_VITA, "tools", "assets"))

import effect_file
import effect_movies
import pack_files

APK = os.path.join(SRC_VITA, "..", "extracted", "remaster_apk")
RESOURCE_LIST = os.path.join(APK, "GTPD_ANDROID_Release.list")
TABLES_HEADER = os.path.join(SRC_VITA, "include", "script_tables.h")
STILL_PLAYED_AS_A_PLAIN_LOOP = {"st03_onnpu_00"}
MOVIE_PATH = re.compile(r"charfiles/3d/05_effect/([^/]+)/23_\1\.user\.2$")


def limit_in_header(name):
    with open(TABLES_HEADER) as header:
        return int(re.search(r"#define %s (\d+)" % name, header.read()).group(1))


def movie_names():
    with open(RESOURCE_LIST, errors="replace") as paths:
        return sorted(set(found.group(1) for found in (MOVIE_PATH.search(line.strip().lower()) for line in paths) if found))


def main():
    packs = pack_files.PackSet(APK)
    jump_limit = limit_in_header("SCRIPT_FX_JUMP_MAX")
    status_limit = limit_in_header("SCRIPT_FX_STATUS_MAX")
    failures = []
    unreadable = 0
    with_jumps = []
    jump_rows = 0
    names = movie_names()
    for name in names:
        try:
            user_bytes = packs.read(effect_movies.USER_PATH % (name, name))
            effect = effect_file.read_effect(user_bytes)
            timeline = effect_file.read_timeline(user_bytes)
        except (ValueError, LookupError, StopIteration, struct.error):
            unreadable += 1
            continue
        main_sprite = next(sprite for sprite in timeline["sprites"] if sprite["sprite_id"] == 0)
        frame_count = len(effect["frames"])
        frames_not_described = [script["frame"] for script in main_sprite["scripts"] if script["rows"] is effect_file.UNRESOLVED]
        if frames_not_described and name not in STILL_PLAYED_AS_A_PLAIN_LOOP:
            failures.append("%s: the scripts on movie frames %r branch in a way the converter cannot describe" % (name, frames_not_described))
        if not frames_not_described and name in STILL_PLAYED_AS_A_PLAIN_LOOP:
            failures.append("%s is described now, so take it out of STILL_PLAYED_AS_A_PLAIN_LOOP" % name)
        for jump in effect["jumps"]:
            if not 0 <= jump["frame"] < frame_count or not (jump["target"] == effect_file.FALL_THROUGH or 0 <= jump["target"] < frame_count):
                failures.append("%s: jump %r points outside its %d frames" % (name, jump, frame_count))
        for status, frame in effect["statuses"].items():
            if not 0 <= frame < frame_count:
                failures.append("%s: status %d starts at frame %d of %d" % (name, status, frame, frame_count))
            if status >= status_limit:
                failures.append("%s: status %d does not fit SCRIPT_FX_STATUS_MAX %d" % (name, status, status_limit))
        if effect["jumps"]:
            with_jumps.append("%s(%d)" % (name, len(effect["jumps"])))
            jump_rows += len(effect["jumps"])
    if jump_rows > jump_limit:
        failures.append("%d jump rows across all movies do not fit SCRIPT_FX_JUMP_MAX %d" % (jump_rows, jump_limit))
    if not with_jumps:
        failures.append("no movie has jump rows, so the script reader found nothing")
    print("movies", len(names), "unreadable", unreadable, "jump rows", jump_rows)
    print("movies with jumps:", " ".join(with_jumps))
    for failure in failures:
        print("FAIL", failure)
    print("ok" if not failures else "failed")
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
