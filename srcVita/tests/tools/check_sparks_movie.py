import os
import sys

SRC_VITA = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(SRC_VITA, "tools", "assets"))

import effect_file
import effect_movies
import pack_files

APK = os.path.join(SRC_VITA, "..", "extracted", "remaster_apk")


def main():
    packs = pack_files.PackSet(APK)
    source = effect_file.read_effect(packs.read(effect_movies.USER_PATH % ("deadtricklight", "deadtricklight")))
    expanded = effect_movies.add_to_set({"effect_movies": [{"effect": "deadtricklight"}], "effects": [], "screen_textures": []}, packs)
    lines = expanded["effect_movie_text"].splitlines()
    frames = [line for line in lines if line.startswith("fxframe")]
    failures = []
    if expanded["effects"]:
        failures.append("a movie row without an id added a script effect")
    if not lines or not lines[0].startswith("fxmovie deadtricklight"):
        failures.append("no fxmovie row for deadtricklight")
    if len(frames) != len(source["frames"]):
        failures.append("frame count %d, source has %d" % (len(frames), len(source["frames"])))
    centres = set()
    for row, frame in zip(frames, source["frames"]):
        if not frame["quads"]:
            if row.split()[1] != "-1":
                failures.append("an empty source frame was given art: " + row[:40])
            continue
        numbers = [float(value) for value in row.split()[6:14]]
        corners = [coordinate for corner in frame["quads"][0]["corners"] for coordinate in corner]
        if any(abs(a - b) > 0.01 for a, b in zip(numbers, corners)):
            failures.append("corners differ from the source on " + row[:40])
            break
        centres.add((round((numbers[0] + numbers[2]) / 2), round((numbers[1] + numbers[5]) / 2)))
    if len(centres) < 2:
        failures.append("every frame shares one centre, so per-frame placement was lost")
    print("frames", len(frames), "distinct centres", len(centres))
    for failure in failures:
        print("FAIL", failure)
    print("ok" if not failures else "failed")
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
