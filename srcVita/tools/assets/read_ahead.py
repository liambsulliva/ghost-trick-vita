import argparse
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import bake_record
import packed_file

AHEAD_EXTENSION = ".ahead"
BUNDLE_EXTENSION = ".anims"
MOTIONS_EXTENSION = ".motions"
ANCHOR_ACT_PREFIX = "game"
SCREEN_FILTER_GRAYSCALE = 1
GRAYSCALE_SUFFIX = "-mono"
GREY_WORLD_KIND = "grey_world"
ALL_EFFECTS_NAME = "*"
OPENING_TICKS = 600


def read_lines(path):
    with open(path) as source:
        return [line.rstrip("\n") for line in source]


def animation_pages(animation_folder, animation_name, cache):
    if animation_name not in cache:
        pages = []
        ticks_before = 0
        path = os.path.join(animation_folder, animation_name + ".anim")
        if os.path.exists(path):
            for line in read_lines(path):
                fields = line.split()
                if len(fields) > 2 and fields[0] == "frame":
                    if fields[1] != "-" and fields[1] not in [page for page, _ in pages]:
                        pages.append((fields[1], ticks_before))
                    ticks_before += int(fields[2]) if fields[2].isdigit() else 0
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


def effect_textures(effects_path):
    movies = {}
    cards = {}
    pages = {}
    current = None
    if not os.path.exists(effects_path):
        return {}
    for line in read_lines(effects_path):
        fields = line.split()
        if len(fields) > 3 and fields[0] == "effect" and fields[2] == "movie":
            movies[fields[1]] = fields[3]
        elif len(fields) > 3 and fields[0] == "effect" and fields[2] == "card":
            cards[fields[1]] = [fields[3]]
        elif len(fields) > 6 and fields[0] == "fxmovie":
            current = fields[1]
            pages[current] = int(fields[6])
    textures = {effect: ["fx_%s_%02d" % (movie, page) for page in range(pages.get(movie, 0))] for effect, movie in movies.items()}
    textures.update(cards)
    return textures


def grey_world_effect_ids(effects_path):
    if not os.path.exists(effects_path):
        return set()
    return set(fields[1] for fields in (line.split() for line in read_lines(effects_path)) if len(fields) > 2 and fields[0] == "effect" and fields[2] == GREY_WORLD_KIND)


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
    textures_of_effect = effect_textures(os.path.join(scripts_folder, "effects.txt"))
    grey_world_ids = grey_world_effect_ids(os.path.join(scripts_folder, "effects.txt"))
    grey_world_name = None
    motion_cache = {}
    page_cache = {}
    schedules = {}
    script_name = None
    anchor = None
    anchor_tick = 0
    script_tick = 0
    seen = set()
    previous = None
    filter_is_grayscale = False
    soul_view_is_on = False
    current_pages = {}

    def schedule(pages, tick, suffix):
        for page, delay in pages:
            due = tick + delay
            for texture in [name + suffix for name in tiles.get(page, [page])]:
                if texture not in seen:
                    seen.add(texture)
                    schedules[script_name].append((anchor, due - anchor_tick, texture))
                    if previous is not None and due - script_tick < OPENING_TICKS and texture not in previous[3]:
                        previous[3].add(texture)
                        schedules[previous[0]].append((previous[1], due - previous[2], texture))

    for line in read_lines(cues_path):
        tick_text, _, text = line.partition("\t")
        fields = text.split()
        if not fields or not tick_text.isdigit():
            continue
        tick = int(tick_text)
        if fields[0] == "mark" and len(fields) > 2 and fields[1] == "script_begun" and fields[2] != "route" and "_root" not in fields[2]:
            if script_name is not None and anchor is not None:
                previous = (script_name, anchor, anchor_tick, set(texture for _, _, texture in schedules[script_name]))
            script_name = fields[2]
            anchor, anchor_tick = "*", tick
            script_tick = tick
            seen = set()
            current_pages = {}
            schedules.setdefault(script_name, [])
            continue
        was_grayscale = filter_is_grayscale and not soul_view_is_on
        if fields[0] == "screen_filter" and len(fields) > 1:
            filter_is_grayscale = int(fields[1]) == SCREEN_FILTER_GRAYSCALE
        elif fields[0] == "effect" and len(fields) > 2 and fields[1] in grey_world_ids:
            grey_world_name = fields[2]
            filter_is_grayscale = True
        elif fields[0] == "effect_deleted" and len(fields) > 1 and grey_world_name is not None and fields[1] in (grey_world_name, ALL_EFFECTS_NAME):
            grey_world_name = None
            filter_is_grayscale = False
        elif fields[0] == "stage_cleared":
            grey_world_name = None
            filter_is_grayscale = False
            current_pages = {}
        elif fields[0] == "soul_vision" and len(fields) > 1:
            soul_view_is_on = fields[1] == "1"
        is_grayscale = filter_is_grayscale and not soul_view_is_on
        suffix = GRAYSCALE_SUFFIX if is_grayscale else ""
        if script_name is None:
            continue
        if is_grayscale != was_grayscale:
            schedule([(page, 0) for pages in current_pages.values() for page, _ in pages], tick, suffix)
            continue
        if fields[0] == "mark" and len(fields) > 2 and fields[1] == "act_begun" and fields[2].startswith(ANCHOR_ACT_PREFIX):
            anchor, anchor_tick = fields[2], tick
            continue
        wanted = []
        if fields[0] == "actor" and len(fields) > 2 and fields[2].startswith("table="):
            animation = motion_animations(scripts_folder, script_name, motion_cache).get(fields[2][len("table="):])
            if animation is not None:
                wanted = animation_pages(animation_folder, animation, page_cache)
                current_pages[fields[1]] = wanted
        elif fields[0] == "effect" and len(fields) > 1:
            wanted = [(texture, 0) for texture in textures_of_effect.get(fields[1], [])]
        schedule(wanted, tick, suffix)
    return schedules


def bundle_text(scripts_folder, animation_folder, script_name):
    parts = []
    path = os.path.join(scripts_folder, script_name + MOTIONS_EXTENSION)
    if not os.path.exists(path):
        return ""
    seen = set()
    for line in read_lines(path):
        fields = line.split()
        if len(fields) > 2 and fields[0] == "motion" and fields[2] not in seen:
            seen.add(fields[2])
            animation_path = os.path.join(animation_folder, fields[2] + ".anim")
            if os.path.exists(animation_path):
                with open(animation_path) as source:
                    text = source.read()
                parts.append(text if text.endswith("\n") else text + "\n")
    return "".join(parts)


def ahead_text(entries):
    return "".join("ahead %s %d %s\n" % entry for entry in entries)


def main():
    parser = argparse.ArgumentParser(description="Writes per-script read-ahead lists from a paced replay's cues.")
    parser.add_argument("cues", help="cues file from script_replay --auto-paced --cues")
    parser.add_argument("scripts_folder", help="baked scripts folder")
    parser.add_argument("animation_folder", help="baked animations folder")
    arguments = parser.parse_args()
    schedules = schedules_from_cues(arguments.cues, arguments.scripts_folder, arguments.animation_folder)
    for path in os.listdir(arguments.scripts_folder):
        if path.endswith(MOTIONS_EXTENSION):
            schedules.setdefault(path[:-len(MOTIONS_EXTENSION)], [])
    for script_name, entries in sorted(schedules.items()):
        bake_record.write_if_different(os.path.join(arguments.scripts_folder, script_name + AHEAD_EXTENSION), ahead_text(entries).encode("ascii"))
        bundle = bundle_text(arguments.scripts_folder, arguments.animation_folder, script_name)
        bake_record.write_if_different(os.path.join(arguments.scripts_folder, script_name + BUNDLE_EXTENSION), packed_file.packed(bundle.encode("ascii")))
        print("read ahead %s: %d textures, %d animations" % (script_name, len(entries), bundle.count("animation ")))
    return 0


if __name__ == "__main__":
    sys.exit(main())
