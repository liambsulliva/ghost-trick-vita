import argparse
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import bake_record

AHEAD_EXTENSION = ".ahead"
MOTIONS_EXTENSION = ".motions"
ANCHOR_ACT_PREFIX = "game"


def read_lines(path):
    with open(path) as source:
        return [line.rstrip("\n") for line in source]


def animation_pages(animation_folder, animation_name, cache):
    if animation_name not in cache:
        pages = []
        path = os.path.join(animation_folder, animation_name + ".anim")
        if os.path.exists(path):
            for line in read_lines(path):
                fields = line.split()
                if len(fields) > 1 and fields[0] == "frame" and fields[1] != "-" and fields[1] not in pages:
                    pages.append(fields[1])
        cache[animation_name] = pages
    return cache[animation_name]


def tiles_of_pages(tiles_path):
    tiles = {}
    if os.path.exists(tiles_path):
        for line in read_lines(tiles_path):
            fields = line.split()
            if len(fields) > 4 and fields[0] == "tile":
                tiles.setdefault(fields[1], []).append(fields[4])
    return tiles


def movie_effects(effects_path):
    movies = {}
    pages = {}
    current = None
    for line in read_lines(effects_path):
        fields = line.split()
        if len(fields) > 3 and fields[0] == "effect" and fields[2] == "movie":
            movies[fields[1]] = fields[3]
        elif len(fields) > 6 and fields[0] == "fxmovie":
            current = fields[1]
            pages[current] = int(fields[6])
    return {effect: ["fx_%s_%02d" % (movie, page) for page in range(pages.get(movie, 0))] for effect, movie in movies.items()}


def motion_animations(scripts_folder, script_name, cache):
    if script_name not in cache:
        animations = {}
        path = os.path.join(scripts_folder, script_name + MOTIONS_EXTENSION)
        if os.path.exists(path):
            for line in read_lines(path):
                fields = line.split()
                if len(fields) > 2 and fields[0] == "motion":
                    animations[fields[1]] = fields[2]
        cache[script_name] = animations
    return cache[script_name]


def schedules_from_cues(cues_path, scripts_folder, animation_folder):
    tiles = tiles_of_pages(os.path.join(scripts_folder, "tiles.txt"))
    movies = movie_effects(os.path.join(scripts_folder, "effects.txt"))
    motion_cache = {}
    page_cache = {}
    schedules = {}
    script_name = None
    anchor = None
    anchor_tick = 0
    seen = set()
    for line in read_lines(cues_path):
        tick_text, _, text = line.partition("\t")
        fields = text.split()
        if not fields or not tick_text.isdigit():
            continue
        tick = int(tick_text)
        if fields[0] == "mark" and len(fields) > 2 and fields[1] == "script_begun" and fields[2] != "route" and "_root" not in fields[2]:
            script_name = fields[2]
            anchor, anchor_tick = "*", tick
            seen = set()
            schedules.setdefault(script_name, [])
            continue
        if script_name is None:
            continue
        if fields[0] == "mark" and len(fields) > 2 and fields[1] == "act_begun" and fields[2].startswith(ANCHOR_ACT_PREFIX):
            anchor, anchor_tick = fields[2], tick
            continue
        wanted = []
        if fields[0] == "actor" and len(fields) > 2 and fields[2].startswith("table="):
            animation = motion_animations(scripts_folder, script_name, motion_cache).get(fields[2][len("table="):])
            if animation is not None:
                wanted = animation_pages(animation_folder, animation, page_cache)
        elif fields[0] == "effect" and len(fields) > 1:
            wanted = movies.get(fields[1], [])
        for page in wanted:
            for texture in tiles.get(page, [page]):
                if texture not in seen:
                    seen.add(texture)
                    schedules[script_name].append((anchor, tick - anchor_tick, texture))
    return schedules


def ahead_text(entries):
    return "".join("ahead %s %d %s\n" % entry for entry in entries)


def main():
    parser = argparse.ArgumentParser(description="Writes per-script read-ahead lists from a paced replay's cues.")
    parser.add_argument("cues", help="cues file from script_replay --auto-paced --cues")
    parser.add_argument("scripts_folder", help="baked scripts folder")
    parser.add_argument("animation_folder", help="baked animations folder")
    arguments = parser.parse_args()
    schedules = schedules_from_cues(arguments.cues, arguments.scripts_folder, arguments.animation_folder)
    for script_name, entries in sorted(schedules.items()):
        bake_record.write_if_different(os.path.join(arguments.scripts_folder, script_name + AHEAD_EXTENSION), ahead_text(entries).encode("ascii"))
        print("read ahead %s: %d textures" % (script_name, len(entries)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
