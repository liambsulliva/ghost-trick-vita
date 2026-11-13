import json
import re
import sys

SCRIPT_BEGUN = re.compile(r"^mark script_begun (\S+)")
CHAPTER_SCRIPT = "chapter"
SCENE_SUFFIX = "_Expand"


def scenes_by_chapter(lines):
    chapter = 1
    seen = set()
    chapters = {}
    for line in lines:
        match = SCRIPT_BEGUN.match(line)
        if match is None:
            continue
        name = match.group(1)
        if name == CHAPTER_SCRIPT:
            chapter += 1
            continue
        if not name.endswith(SCENE_SUFFIX) or (chapter, name) in seen:
            continue
        seen.add((chapter, name))
        chapters.setdefault(str(chapter), []).append(name)
    return chapters


def main():
    if len(sys.argv) != 2:
        print("usage: clock_scenes.py <replay marks file>", file=sys.stderr)
        return 2
    with open(sys.argv[1], encoding="utf-8", errors="replace") as source:
        print(json.dumps(scenes_by_chapter(source), indent=2))
    return 0


if __name__ == "__main__":
    sys.exit(main())
