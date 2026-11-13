import os
import subprocess
import sys
import tempfile

SRC_VITA = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
REPLAY = os.path.join(SRC_VITA, "build", "tests", "golden", "script_replay")
SCRIPTS = os.path.join(SRC_VITA, "..", "extracted", "vita_assets", "game", "scripts")
STOP_TICK = "4200000"
FULLY_BLACK = -16


def steps_begun_in_the_dark(cue_lines):
    brightness = 0
    script = "-"
    found = []
    for line in cue_lines:
        tick, _, text = line.rstrip("\n").partition("\t")
        words = text.split(" ")
        if words[0] == "brightness":
            brightness = int(words[1])
        elif words[:2] == ["mark", "script_begun"]:
            script = words[2]
            brightness = 0
        elif words[:2] == ["mark", "route_step_began"] and brightness <= FULLY_BLACK:
            found.append((int(tick), script, words[2]))
    return found


def main():
    with tempfile.TemporaryDirectory() as folder:
        cues_path = os.path.join(folder, "cues.tsv")
        subprocess.run([REPLAY, "--scripts", SCRIPTS, "--timing", "remaster", "--auto-paced", "--marks", "--cues", cues_path, "--stop-tick", STOP_TICK], check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        with open(cues_path, errors="replace") as source:
            found = steps_begun_in_the_dark(source)
    for tick, script, step in found:
        print("FAIL  step %s of %s began at tick %d on a fully black screen" % (step, script, tick))
    print("%s  no route step begins on a fully black screen: %d found" % ("FAIL" if found else "ok  ", len(found)))
    return 1 if found else 0


if __name__ == "__main__":
    sys.exit(main())
