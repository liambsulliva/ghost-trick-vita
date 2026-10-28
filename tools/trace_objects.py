import sys
import os
import re
import time
import struct
import glob

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import gdb_remote
import melonds
import trace_cpac

REPO_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
RAM_START = 0x02000000
RAM_SIZE = 0x00400000
PACK_2D_BLOCK = 3


def load_names(stage):
    names = set()
    list_path = os.path.join(REPO_DIR, "extracted", "remaster_apk", "GTPD_ANDROID_Release.list")
    if os.path.exists(list_path):
        f = open(list_path, "r")
        for line in f:
            match = re.match(r"natives/android/charfiles/2d/[^/]+/" + stage + r"/([a-z0-9]+)/", line)
            if match:
                names.add(match.group(1))
        f.close()
    for path in glob.glob(os.path.join(REPO_DIR, "extracted", "nds_plain", stage, "*_Expand.xml.lz.dec")):
        f = open(path, "rb")
        data = f.read()
        f.close()
        for match in re.finditer(rb"\x00([a-z][a-z0-9]{2,15})\x00", data):
            names.add(match.group(1).decode())
    return names


def read_word(ram, address):
    offset = address - RAM_START
    if offset < 0 or offset + 4 > len(ram):
        return None
    return struct.unpack_from("<I", ram, offset)[0]


def find_name_strings(ram, names):
    found = {}
    for name in names:
        pattern = b"\x00" + name.encode() + b"\x00"
        pos = ram.find(pattern)
        while pos != -1:
            found[RAM_START + pos + 1] = name
            pos = ram.find(pattern, pos + 1)
    return found


def object_names(ram, address, name_strings, depth):
    results = []
    if address in name_strings:
        results.append((name_strings[address], depth, "direct"))
        return results
    offset = 0
    while offset < 0x100:
        word = read_word(ram, address + offset)
        if word is None:
            break
        if word in name_strings:
            results.append((name_strings[word], depth, "+" + hex(offset)))
        elif depth < 2 and RAM_START <= word < RAM_START + RAM_SIZE and word % 4 == 0 and word != address:
            for inner in object_names(ram, word, name_strings, depth + 1):
                results.append((inner[0], inner[1], "+" + hex(offset) + inner[2]))
        offset = offset + 4
    return results


def resolve(hit, ram, name_strings):
    candidates = []
    registers = hit[4]
    for reg in [4, 5, 6, 7, 8, 9, 10, 11]:
        candidates.append(("r" + str(reg), registers[reg]))
    stack = hit[5]
    i = 0
    while i + 4 <= len(stack):
        candidates.append(("sp+" + hex(i), struct.unpack_from("<I", stack, i)[0]))
        i = i + 4
    best = {}
    for source, value in candidates:
        if not (RAM_START <= value < RAM_START + RAM_SIZE) or value % 4 != 0:
            continue
        for name, depth, path in object_names(ram, value, name_strings, 0):
            if name not in best or depth < best[name][0]:
                best[name] = (depth, source + path)
    ordered = sorted(best.items(), key=lambda item: item[1][0])
    return ordered


def main():
    rom_path = sys.argv[1]
    stage = "st01"
    scene_file = "st01/st01_game000_Expand.xml.lz"
    out_dir = os.path.join(REPO_DIR, "extracted", "trace")
    os.makedirs(out_dir, exist_ok=True)
    names = load_names(stage)
    print("known names for " + stage + ": " + str(len(names)))

    process, sock, log = melonds.start(rom_path, "trace_objects.log")
    if sock is None:
        print("could not connect to the ARM9 GDB stub")
        sys.exit(1)
    if not trace_cpac.prepare(sock):
        melonds.stop(process, sock, log)
        sys.exit(1)
    gdb_remote.add_breakpoint(sock, trace_cpac.LOAD_ENTRY, True)
    gdb_remote.add_breakpoint(sock, trace_cpac.OPEN_FILE, True)

    run_seconds = 60
    schedule = trace_cpac.make_schedule(run_seconds)
    pack_2d = None
    scene_open = False
    dump_at = None
    ram = None
    hits = []
    current_buttons = 0
    start_time = time.time()
    gdb_remote.send_packet(sock, "c")
    while ram is None:
        elapsed = time.time() - start_time
        if elapsed >= run_seconds:
            break
        edge = trace_cpac.next_edge(schedule, elapsed)
        if dump_at is not None and dump_at < edge:
            edge = dump_at
        wait = edge - elapsed
        if wait < 0.02:
            wait = 0.02
        stop = gdb_remote.wait_for_stop(sock, wait)
        if stop is None:
            gdb_remote.interrupt(sock)
        elapsed = time.time() - start_time
        registers = gdb_remote.read_registers(sock)
        pc = registers[15]
        if pc == trace_cpac.LOAD_ENTRY:
            if pack_2d is None:
                pack_2d = registers[0]
            if scene_open and registers[0] == pack_2d and registers[1] == PACK_2D_BLOCK:
                stack = gdb_remote.read_memory(sock, registers[13], 0x200)
                hits.append((elapsed, registers[2], registers[14], registers[0], registers, stack))
            trace_cpac.step_over_breakpoint(sock, trace_cpac.LOAD_ENTRY)
        if pc == trace_cpac.OPEN_FILE:
            path = trace_cpac.read_text(sock, registers[1])
            if path == scene_file:
                scene_open = True
                dump_at = elapsed + 1.5
                print("scene opened at " + str(round(elapsed, 1)) + "s")
            trace_cpac.step_over_breakpoint(sock, trace_cpac.OPEN_FILE)
        if dump_at is not None and elapsed >= dump_at:
            print("dumping main RAM")
            ram = gdb_remote.read_memory(sock, RAM_START, RAM_SIZE)
            break
        wanted = trace_cpac.buttons_at(schedule, elapsed)
        if wanted != current_buttons:
            trace_cpac.set_buttons(sock, wanted)
            current_buttons = wanted
        gdb_remote.send_packet(sock, "c")

    gdb_remote.remove_breakpoint(sock, trace_cpac.LOAD_ENTRY, True)
    gdb_remote.remove_breakpoint(sock, trace_cpac.OPEN_FILE, True)
    melonds.stop(process, sock, log)
    if ram is None:
        print("never reached the scene, no RAM dump")
        sys.exit(1)
    f = open(os.path.join(out_dir, "ram_" + stage + "_game000.bin"), "wb")
    f.write(ram)
    f.close()

    name_strings = find_name_strings(ram, names)
    print("name strings found in RAM: " + str(len(name_strings)))
    out = open(os.path.join(out_dir, "object_loads.tsv"), "w")
    out.write("seconds\tindex\tcaller\tnames\n")
    for hit in hits:
        matches = resolve(hit, ram, name_strings)
        text = ", ".join(name + "(" + str(info[0]) + " " + info[1] + ")" for name, info in matches[:4])
        out.write(str(round(hit[0], 2)) + "\t" + str(hit[1]) + "\t" + hex(hit[2]) + "\t" + text + "\n")
        print(str(round(hit[0], 2)) + "s index " + str(hit[1]) + ": " + text)
    out.close()


if __name__ == "__main__":
    main()
