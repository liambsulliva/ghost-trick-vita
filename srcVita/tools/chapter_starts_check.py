import argparse
import os
import re
import subprocess
import sys
import tempfile

START_LINE = re.compile(r"^chapter_start (\d+)\n", re.M)
PASSING_SCRIPT = re.compile(r"(_root|^route|^chapter|^phone)$")
CHAPTER_SELECT_SCRIPT = "chapter"
PHONE_SCRIPT = "phone"
FIRST_PLACE = 1
NOTE_IDS = 64
PHONE_BOOK_MOST = 24
SHORT_RUN_TICKS = 3000


def cue_text(line):
    return line.rstrip("\n").split("\t", 1)[-1]


def story_scenes(cue_lines):
    mark = 0
    opened_by = {}
    waiting = None
    for line in cue_lines:
        text = cue_text(line)
        if text.startswith("chapter_mark "):
            mark = int(text.split()[1])
        elif text.startswith("mark script_begun "):
            name = text.split()[2]
            if waiting is not None and not PASSING_SCRIPT.search(name):
                opened_by.setdefault(waiting, name)
                waiting = None
            if name == CHAPTER_SELECT_SCRIPT:
                waiting = mark + 1
    return opened_by


def chapter_starts(text):
    pieces = START_LINE.split(text)
    return {int(pieces[index]): pieces[index + 1] for index in range(1, len(pieces) - 1, 2)}


def book_places(state):
    match = re.search(r"^note_levels ([0-9a-f]+)$", state, re.M)
    places = {FIRST_PLACE}
    if match is not None:
        levels = bytes.fromhex(match.group(1))[:NOTE_IDS]
        places.update(place for place in range(1, min(len(levels), PHONE_BOOK_MOST)) if levels[place] > 0)
    place = re.search(r"^phone_book_place (\d+)$", state, re.M)
    if place is not None and int(place.group(1)) > 0:
        places.add(int(place.group(1)))
    return sorted(places)


def first_scene_and_lowest_mark(replay, scripts, state, start, folder):
    state_path = os.path.join(folder, "state.txt")
    cues_path = os.path.join(folder, "cues.txt")
    with open(state_path, "w", encoding="ascii") as target:
        target.write(state)
    subprocess.run([replay, "--scripts", scripts, "--timing", "remaster", "--stop-tick", str(SHORT_RUN_TICKS), "--load-state", state_path, "--start", start, "--cues", cues_path], check=True, stdout=subprocess.DEVNULL)
    with open(cues_path, encoding="utf-8", errors="replace") as source:
        lines = source.readlines()
    scene = None
    lowest = None
    for line in lines:
        text = cue_text(line)
        if text.startswith("chapter_mark "):
            mark = int(text.split()[1])
            lowest = mark if lowest is None else min(lowest, mark)
        elif text.startswith("mark script_begun ") and scene is None and not PASSING_SCRIPT.search(text.split()[2]):
            scene = text.split()[2]
    return scene, lowest


def with_place(state, place):
    return re.sub(r"^phone_book_place .*$", "phone_book_place %d" % place, state, flags=re.M)


def mark_of(state):
    return int(re.search(r"^chapter_mark (-?\d+)$", state, re.M).group(1))


def problems_of(replay, scripts, baked_text, fresh_text, cue_lines):
    problems = []
    if baked_text != fresh_text:
        problems.append("chapter_starts.txt differs from the whole-game replay's; run make read-ahead")
    opened_by = story_scenes(cue_lines)
    starts = chapter_starts(baked_text)
    with tempfile.TemporaryDirectory() as folder:
        for chapter in sorted(opened_by):
            if chapter not in starts:
                problems.append("chapter %d has no baked start" % chapter)
                continue
            state = starts[chapter]
            scene, _ = first_scene_and_lowest_mark(replay, scripts, state, CHAPTER_SELECT_SCRIPT, folder)
            if scene != opened_by[chapter]:
                problems.append("chapter %d select opens %s, the story opens %s" % (chapter, scene, opened_by[chapter]))
            for place in book_places(state):
                scene, lowest = first_scene_and_lowest_mark(replay, scripts, with_place(state, place), PHONE_SCRIPT, folder)
                if scene is None:
                    problems.append("chapter %d phone ride to place %d opens no scene" % (chapter, place))
                elif lowest is not None and lowest < mark_of(state):
                    problems.append("chapter %d phone ride to place %d lowers the chapter mark to %d in %s" % (chapter, place, lowest, scene))
    return problems


def main():
    parser = argparse.ArgumentParser(description="Checks that Chapter select starts each chapter from the state the whole-game replay reached it with, so it opens the chapter's scene and a phone ride keeps the story point.")
    parser.add_argument("replay", help="the script_replay binary")
    parser.add_argument("scripts", help="the baked scripts folder holding chapter_starts.txt")
    parser.add_argument("fresh", help="the chapter starts the whole-game replay just wrote")
    parser.add_argument("cues", help="the whole-game replay's cues")
    arguments = parser.parse_args()
    baked_path = os.path.join(arguments.scripts, "chapter_starts.txt")
    if not os.path.isfile(baked_path):
        print("chapter starts check: %s is missing; run make read-ahead" % baked_path)
        return 1
    with open(baked_path, encoding="ascii") as source:
        baked_text = source.read()
    with open(arguments.fresh, encoding="ascii") as source:
        fresh_text = source.read()
    with open(arguments.cues, encoding="utf-8", errors="replace") as source:
        cue_lines = source.readlines()
    problems = problems_of(arguments.replay, arguments.scripts, baked_text, fresh_text, cue_lines)
    for problem in problems:
        print("  " + problem)
    print("chapter starts check %s %d chapters, %d problems" % ("passed" if not problems else "failed", len(chapter_starts(baked_text)), len(problems)))
    return 1 if problems else 0


if __name__ == "__main__":
    sys.exit(main())
