import glob
import os
import sys

SRC_VITA = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(SRC_VITA, "tools", "assets"))

import chapter_assets

PLAIN_KINDS_WITH_NO_REMASTER_ART = {5, 6, 8, 9, 42, 43, 61, 62, 207}
CLASS_EFFECTS_NOT_PORTED = {50, 54, 56, 57, 59, 64, 65, 99, 100, 101, 102, 118, 119, 146, 147, 148, 149, 150, 151, 165, 166, 167, 168, 191, 192, 193, 227, 228, 229, 242, 243, 270}
KNOWN_UNDRAWN = PLAIN_KINDS_WITH_NO_REMASTER_ART | CLASS_EFFECTS_NOT_PORTED


def ids_with_rows(scripts_folder):
    with open(os.path.join(scripts_folder, "effects.txt")) as rows:
        return {int(line.split()[1]) for line in rows if line.startswith("effect ")}


def spawned_ids(scripts_folder):
    spawned = {}
    for path in sorted(glob.glob(os.path.join(scripts_folder, "*.lmg"))):
        with open(path, "rb") as source:
            script = chapter_assets.Script(path, source.read())
        for effect_id in chapter_assets.spawned_effect_ids(script):
            spawned.setdefault(effect_id, os.path.basename(path))
    return spawned


def problems(spawned, with_rows, known):
    undrawn = set(spawned) - with_rows
    found = []
    for effect_id in sorted(undrawn - known):
        found.append("effect %d (first in %s) has no row and is not in the README's list of effects that are not drawn" % (effect_id, spawned[effect_id]))
    for effect_id in sorted(known & with_rows):
        found.append("effect %d has a row now, so take it out of this check and the README" % effect_id)
    return found


def main():
    if len(sys.argv) != 2:
        print("usage: check_undrawn_effects.py <set scripts folder>")
        return 2
    found = problems(spawned_ids(sys.argv[1]), ids_with_rows(sys.argv[1]), KNOWN_UNDRAWN)
    for line in found:
        print(line)
    if found:
        return 1
    print("every spawned effect without a row is named: %d ids" % len(KNOWN_UNDRAWN))
    return 0


if __name__ == "__main__":
    sys.exit(main())
