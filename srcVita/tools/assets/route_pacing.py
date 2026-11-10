import argparse
import json
import os
import subprocess
import sys
import zlib

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import bake_record

TICKS_PER_SECOND = 60
PACING_FILE_NAME = "route_pacing.txt"
ROUTE_STEPS_FILE_NAME = "route_steps.txt"
READY_MARK = "route_step_ready"
BEGAN_MARK = "route_step_began"
MOST_PASSES = 6
MOST_EXTRA_TICKS = 300
GAME_SYNC_WAIT_PREFIXES = ("reach:", "act:", "flag:", "noflag:")
GAME_DRIVEN_GAP_ACTIONS = ("phone_call",)


def step_times(cue_lines):
    last_ready = {}
    began = {}
    ready_before_began = {}
    for line in cue_lines:
        fields = line.rstrip("\n").split("\t")
        if len(fields) < 2:
            continue
        words = fields[1].split()
        if len(words) < 3 or words[0] != "mark" or words[1] not in (READY_MARK, BEGAN_MARK):
            continue
        step_index = int(words[2].split(":", 1)[0])
        tick = int(fields[0])
        if words[1] == READY_MARK:
            last_ready[step_index] = tick
        elif step_index not in began:
            began[step_index] = tick
            if step_index in last_ready:
                ready_before_began[step_index] = last_ready[step_index]
    return ready_before_began, began


def waits_on_the_game(step):
    return any(token.strip().startswith(GAME_SYNC_WAIT_PREFIXES) for token in step.get("wait", "").split("&"))


def follows_a_ride_or_a_scene_change(previous, step):
    return previous.get("action") in GAME_DRIVEN_GAP_ACTIONS or previous.get("script") != step.get("script")


def base_dwell(route, step):
    return route["dwell_ticks"][step.get("pace", "normal")]


def paced_dwells(route, ready, began, windows_too_short, drift):
    dwells = {}
    steps = route["steps"]
    for index in range(1, len(steps)):
        step = steps[index]
        previous = steps[index - 1]
        if "footage_s" not in step or "footage_s" not in previous or index in windows_too_short or waits_on_the_game(step) or follows_a_ride_or_a_scene_change(previous, step):
            continue
        if index not in ready or index - 1 not in began:
            continue
        footage_gap = round((step["footage_s"] - previous["footage_s"]) * TICKS_PER_SECOND)
        ready_gap = ready[index] - began[index - 1]
        wanted = footage_gap - ready_gap
        if wanted > base_dwell(route, step):
            dwells[index] = min(wanted, base_dwell(route, step) + MOST_EXTRA_TICKS)
            if wanted > dwells[index]:
                drift[index] = wanted - dwells[index]
    return dwells


def route_stamp(route_steps_bytes):
    return "route %08x" % (zlib.crc32(route_steps_bytes) & 0xffffffff)


def pacing_file_text(route_steps_bytes, dwells):
    return route_stamp(route_steps_bytes) + "\n" + pacing_text(dwells)


def dwells_baked_for(path, route_steps_bytes):
    dwells = {}
    if not os.path.exists(path):
        return dwells
    with open(path, encoding="ascii") as source:
        lines = source.read().splitlines()
    if not lines or lines[0] != route_stamp(route_steps_bytes):
        return dwells
    for line in lines[1:]:
        words = line.split()
        if len(words) == 3 and words[0] == "dwell":
            dwells[int(words[1])] = int(words[2])
    return dwells


def never_shorter_than_before(dwells, earlier_dwells, windows_too_short):
    kept = dict(dwells)
    for index, dwell in earlier_dwells.items():
        if index not in windows_too_short and dwell > kept.get(index, 0):
            kept[index] = dwell
    return kept


def pacing_text(dwells):
    return "".join("dwell %d %d\n" % (index, dwells[index]) for index in sorted(dwells))


def read_pacing(path):
    stretched = set()
    if os.path.exists(path):
        with open(path, encoding="ascii") as source:
            for line in source:
                words = line.split()
                if len(words) == 3 and words[0] == "dwell":
                    stretched.add(int(words[1]))
    return stretched


def replay(replay_binary, scripts_folder, cues_path, stop_tick):
    subprocess.run([replay_binary, "--scripts", scripts_folder, "--timing", "remaster", "--auto-paced", "--cues", cues_path, "--stop-tick", str(stop_tick)], check=True, stdout=subprocess.DEVNULL)


def main():
    parser = argparse.ArgumentParser(description="Stretches each route step's dwell so auto-play never acts sooner after the previous step than the reference footage did.")
    parser.add_argument("replay", help="the script_replay binary")
    parser.add_argument("set_description", help="the set's json, whose route steps carry footage_s")
    parser.add_argument("scripts", help="the baked scripts folder; route_pacing.txt is written here")
    parser.add_argument("cues", help="where the paced replay writes its cues")
    parser.add_argument("--stop-tick", type=int, default=200000)
    arguments = parser.parse_args()
    with open(arguments.set_description, encoding="utf-8") as source:
        route = json.load(source)["route"]
    pacing_path = os.path.join(arguments.scripts, PACING_FILE_NAME)
    windows_too_short = set()
    with open(os.path.join(arguments.scripts, ROUTE_STEPS_FILE_NAME), "rb") as source:
        route_steps_bytes = source.read()
    for pass_number in range(1, MOST_PASSES + 1):
        stretched = read_pacing(pacing_path)
        earlier_dwells = dwells_baked_for(pacing_path, route_steps_bytes)
        replay(arguments.replay, arguments.scripts, arguments.cues, arguments.stop_tick)
        with open(arguments.cues, encoding="utf-8", errors="replace") as source:
            ready, began = step_times(source)
        windows_too_short.update(index for index in stretched if index not in began)
        drift = {}
        dwells = never_shorter_than_before(paced_dwells(route, ready, began, windows_too_short, drift), earlier_dwells, windows_too_short)
        changed = bake_record.write_if_different(pacing_path, pacing_file_text(route_steps_bytes, dwells).encode("ascii"))
        print("route pacing pass %d: %d steps stretched, %d extra ticks, %d kept short%s" % (pass_number, len(dwells), sum(dwells[index] - base_dwell(route, route["steps"][index]) for index in dwells), len(windows_too_short), "" if changed else ", settled"))
        if not changed:
            for index in sorted(drift):
                step = route["steps"][index]
                print("  footage runs %.1f s longer before step %d %s %s" % (drift[index] / TICKS_PER_SECOND, index, step["action"], step.get("object", "-")))
            return 0
    print("route pacing did not settle after %d passes" % MOST_PASSES)
    return 1


if __name__ == "__main__":
    sys.exit(main())
