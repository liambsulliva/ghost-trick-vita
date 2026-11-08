import os
import subprocess
import sys
import tempfile

SRC_VITA = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
REPLAY = os.path.join(SRC_VITA, "build", "tests", "golden", "script_replay")
SCRIPTS = os.path.join(SRC_VITA, "..", "extracted", "vita_assets", "chapter1", "scripts")
STOP_TICK = "200000"
HUD_PUBLISH_LAG_TICKS = 1
EXPECTATIONS = [
    ("mark route_step_done 17:", "mark core linne", 0, 400),
    ("mark core linne", "sound 28", 0, 0),
    ("mark core linne", "music_stop 0", 0, 0),
    ("mark core linne", "music_start 9", 77, 79),
    ("mark act_begun bicycleAct000", "second_core_marked bicycle", 0, 40000),
]


def read_cues(path):
    cues = []
    with open(path) as source:
        for line in source:
            tick, _, text = line.rstrip("\n").partition("\t")
            cues.append((int(tick), text))
    return cues


def first_tick(cues, prefix, from_tick=0):
    for tick, text in cues:
        if tick >= from_tick and text.startswith(prefix):
            return tick
    return None


def main():
    with tempfile.TemporaryDirectory() as folder:
        cues_path = os.path.join(folder, "cues.tsv")
        subprocess.run([REPLAY, "--scripts", SCRIPTS, "--timing", "remaster", "--auto-paced", "--cues", cues_path, "--stop-tick", STOP_TICK], check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        cues = read_cues(cues_path)
    failures = 0
    film_is_on = False
    film_started = 0
    pills = 0
    pill_ticks_in_film = []
    for tick, text in cues:
        if text.startswith("film_frame "):
            film_is_on = text.endswith(" 1")
            film_started = tick
        elif text.startswith("pills_on_screen "):
            pills = int(text.split()[1])
        if film_is_on and pills and tick > film_started + HUD_PUBLISH_LAG_TICKS:
            pill_ticks_in_film.append(tick)
    film_ok = not pill_ticks_in_film
    failures += 0 if film_ok else 1
    print("%s  no pill on screen during any film segment: %s" % ("ok  " if film_ok else "FAIL", "none" if film_ok else "first at tick %d" % pill_ticks_in_film[0]))
    for after, wanted, least, most in EXPECTATIONS:
        start = first_tick(cues, after)
        found = first_tick(cues, wanted, start) if start is not None else None
        ok = start is not None and found is not None and least <= found - start <= most
        failures += 0 if ok else 1
        print("%s  %s -> %s after %d to %d: at %s then %s" % ("ok  " if ok else "FAIL", after, wanted, least, most, start, found))
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
