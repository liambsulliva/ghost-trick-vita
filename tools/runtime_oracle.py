import sys
import os
import subprocess
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import gdb_remote

REPO_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
MELONDS = os.path.join(REPO_DIR, "extracted", "melonds", "melonDS.app", "Contents", "MacOS", "melonDS")
NDS_DIR = os.path.join(REPO_DIR, "extracted", "nds")


CONFIG = os.path.join(REPO_DIR, "extracted", "melonds", "portable", "melonDS.toml")


def make_default_config(rom_path):
    os.makedirs(os.path.dirname(CONFIG), exist_ok=True)
    log = open(os.path.join(REPO_DIR, "extracted", "melonds", "default_run.log"), "w")
    process = subprocess.Popen([MELONDS, rom_path], stdout=log, stderr=log)
    time.sleep(6)
    process.send_signal(2)
    process.wait(15)
    log.close()


def set_gdb_options():
    f = open(CONFIG, "r")
    lines = f.read().split("\n")
    f.close()
    section = ""
    new_lines = []
    for line in lines:
        text = line.strip()
        if text.startswith("["):
            section = text
        if section == "[Instance0.Gdb]" and text.startswith("Enabled"):
            line = "Enabled = true"
        if section == "[Instance0.Gdb.ARM9]" and text.startswith("BreakOnStartup"):
            line = "BreakOnStartup = true"
        if section == "[Instance0.Gdb.ARM7]" and text.startswith("BreakOnStartup"):
            line = "BreakOnStartup = false"
        if section == "[JIT]" and text.startswith("Enable "):
            line = "Enable = false"
        new_lines.append(line)
    f = open(CONFIG, "w")
    f.write("\n".join(new_lines))
    f.close()


def load_segments():
    segments = []
    entry = 0
    f = open(os.path.join(NDS_DIR, "loadmap.txt"), "r")
    for line in f:
        parts = line.split()
        if parts[0] == "entry":
            entry = int(parts[1], 16)
        if parts[0] == "segment" or parts[0] == "overlay":
            seg_file = open(os.path.join(NDS_DIR, parts[4]), "rb")
            data = seg_file.read()
            seg_file.close()
            segments.append((parts[0], parts[1], int(parts[2], 16), data))
    f.close()
    return entry, segments


def match_fraction(expected, actual):
    if actual is None or len(expected) < 4:
        return 0.0
    same = 0
    total = 0
    i = 0
    while i + 4 <= len(expected):
        total = total + 1
        if expected[i:i + 4] == actual[i:i + 4]:
            same = same + 1
        i = i + 4
    return same / total


def main():
    rom_path = sys.argv[1]
    run_seconds = 10
    if len(sys.argv) > 2:
        run_seconds = int(sys.argv[2])
    entry, segments = load_segments()
    if not os.path.exists(CONFIG) or os.path.getsize(CONFIG) == 0:
        make_default_config(rom_path)
    set_gdb_options()

    log = open(os.path.join(REPO_DIR, "extracted", "melonds", "oracle_run.log"), "w")
    process = subprocess.Popen([MELONDS, rom_path], stdout=log, stderr=log)
    sock = gdb_remote.connect(3333, 30)
    if sock is None:
        print("could not connect to the ARM9 GDB stub on port 3333")
        process.terminate()
        sys.exit(1)

    passed = True
    stop = gdb_remote.command(sock, "?")
    registers = gdb_remote.read_registers(sock)
    start_pc = registers[15]
    print("stopped at startup: " + str(stop) + ", pc = " + hex(start_pc))
    if start_pc == entry:
        print("PASS entry point " + hex(entry) + " matches the ROM header")
    else:
        print("FAIL entry point: header says " + hex(entry))
        passed = False

    gdb_remote.send_packet(sock, "c")
    time.sleep(run_seconds)
    stop = gdb_remote.interrupt(sock)
    registers = gdb_remote.read_registers(sock)
    print("interrupted after " + str(run_seconds) + "s: " + str(stop) + ", pc = " + hex(registers[15]))

    overlay_scores = []
    for segment in segments:
        kind = segment[0]
        name = segment[1]
        address = segment[2]
        data = segment[3]
        live = gdb_remote.read_memory(sock, address, len(data))
        fraction = match_fraction(data, live)
        if kind == "overlay":
            overlay_scores.append((fraction, name))
            continue
        need = 0.90
        if name == "dtcm":
            need = 0.0
        verdict = "PASS"
        if fraction < need:
            verdict = "FAIL"
            passed = False
        print(verdict + " " + name.ljust(5) + " at " + hex(address) + ": " + str(round(fraction * 100, 1)) + "% of words match the extracted segment")

    overlay_scores.sort(reverse=True)
    best = overlay_scores[0]
    line = "overlay region 0x20d3a40: best match " + best[1] + " " + str(round(best[0] * 100, 1)) + "%"
    if len(overlay_scores) > 1:
        line = line + ", next " + overlay_scores[1][1] + " " + str(round(overlay_scores[1][0] * 100, 1)) + "%"
    print(line)

    gdb_remote.send_packet(sock, "k")
    sock.close()
    time.sleep(1)
    process.send_signal(2)
    try:
        process.wait(10)
    except subprocess.TimeoutExpired:
        process.kill()
        process.wait(10)
    log.close()
    if not passed:
        sys.exit(1)
    print("runtime oracle passed")


main()
