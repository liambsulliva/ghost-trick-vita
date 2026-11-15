import argparse
import os
import re
import subprocess
import sys

OBJECT_LINE = re.compile(r"\[\s*(\d+)\]\s+(.*)")
DEAD_LINE = re.compile(r"<<dead>>\s+\S+\s+\[\s*(\d+)\]\s+(.*)")
WORD = re.compile(r"[A-Za-z_]\w*")


def dead_symbols_by_object(map_text):
    objects = {}
    dead = {}
    section = None
    for line in map_text.splitlines():
        if line.startswith("# Object files"):
            section = "objects"
        elif line.startswith("# Dead Stripped Symbols"):
            section = "dead"
        elif line.startswith("# Sections") or line.startswith("# Symbols"):
            section = None
        elif section == "objects":
            match = OBJECT_LINE.match(line)
            if match:
                objects[int(match.group(1))] = match.group(2).strip()
        elif section == "dead":
            match = DEAD_LINE.match(line)
            if match and int(match.group(1)) in objects:
                dead.setdefault(objects[int(match.group(1))], set()).add(match.group(2).strip())
    return dead


def live_symbols(map_text):
    names = set()
    section = None
    for line in map_text.splitlines():
        if line.startswith("# Symbols"):
            section = "symbols"
        elif line.startswith("# "):
            if section == "symbols" and not line.startswith("# Address"):
                section = None
        elif section == "symbols":
            match = OBJECT_LINE.search(line)
            if match:
                name = match.group(2).strip()
                names.add(name[1:] if name.startswith("_") else name)
    return names


def linked_but_never_allowed(map_text, never_linked):
    return sorted(set(never_linked) & live_symbols(map_text))


def exported_symbols(object_path):
    listing = subprocess.run(["nm", "-gU", object_path], capture_output=True, text=True, check=True).stdout
    return {line.split()[-1] for line in listing.splitlines() if line.strip()}


def words_in(paths):
    words = set()
    for path in paths:
        with open(path, encoding="utf-8") as source:
            words.update(WORD.findall(source.read()))
    return words


def unreached_names(map_text, game_object_marker, names_the_device_layer_uses, exports_of=exported_symbols):
    names = set()
    for object_path, symbols in dead_symbols_by_object(map_text).items():
        if game_object_marker not in object_path:
            continue
        exported = exports_of(object_path)
        for symbol in symbols & exported:
            name = symbol[1:] if symbol.startswith("_") else symbol
            if name not in names_the_device_layer_uses:
                names.add(name)
    return names


def read_list(path):
    with open(path, encoding="utf-8") as listing:
        return {line.strip() for line in listing if line.strip()}


def problems(found, listed):
    lines = []
    for name in sorted(found - listed):
        lines.append("unreached in the player build and not listed: %s" % name)
    for name in sorted(listed - found):
        lines.append("listed as unreached but the player build reaches it or it is gone: %s" % name)
    return lines


def main():
    parser = argparse.ArgumentParser(description="Fails when the player build links a game function nothing in it reaches, unless the list names it as kept for the tests or the debug build.")
    parser.add_argument("map_file")
    parser.add_argument("listed")
    parser.add_argument("--game-objects", required=True, help="text every game object path contains")
    parser.add_argument("--never-linked", nargs="*", default=[], help="functions the player build must not reach, such as the run log's file writer")
    parser.add_argument("--device-sources", nargs="*", default=[], help="device layer sources the host link replaces with fakes")
    arguments = parser.parse_args()
    with open(arguments.map_file, encoding="utf-8", errors="replace") as map_file:
        map_text = map_file.read()
    if "# Dead Stripped Symbols" not in map_text:
        print("player link check skipped: this linker's map lists no dead symbols")
        return 0
    found = unreached_names(map_text, arguments.game_objects, words_in(arguments.device_sources))
    lines = problems(found, read_list(arguments.listed))
    lines.extend("the player build reaches %s, which only the debug build may use" % name for name in linked_but_never_allowed(map_text, arguments.never_linked))
    for line in lines:
        print(line)
    print("player link check: %d unreached, %d problems" % (len(found), len(lines)))
    return 1 if lines else 0


if __name__ == "__main__":
    sys.exit(main())
