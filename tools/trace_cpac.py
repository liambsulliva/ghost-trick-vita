import sys
import os
import time
import struct

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import gdb_remote
import melonds

REPO_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
MAIN_FUNCTION = 0x02000BEC
PAD_READERS = [0x0202292C, 0x020316DC]
LOAD_ENTRY = 0x0201E664
OPEN_FILE = 0x0201EFB4

BUTTON_A = 0x0001
BUTTON_START = 0x0008


def make_schedule(run_seconds):
    schedule = []
    for t in [6.0, 9.0, 12.0]:
        schedule.append((t, t + 0.4, BUTTON_START))
    t = 14.0
    while t < run_seconds:
        schedule.append((t, t + 0.3, BUTTON_A))
        t = t + 1.5
    return schedule


def buttons_at(schedule, elapsed):
    value = 0
    for item in schedule:
        if item[0] <= elapsed < item[1]:
            value = value | item[2]
    return value


def next_edge(schedule, elapsed):
    best = elapsed + 1.0
    for item in schedule:
        if item[0] > elapsed and item[0] < best:
            best = item[0]
        if item[1] > elapsed and item[1] < best:
            best = item[1]
    return best


def patch_pad_readers(sock):
    for address in PAD_READERS:
        code = bytes([0x00, 0x48, 0x70, 0x47]) + struct.pack("<I", 0)
        if not gdb_remote.write_memory(sock, address, code):
            return False
        back = gdb_remote.read_memory(sock, address, 8)
        if back != code:
            return False
    return True


def prepare(sock):
    gdb_remote.command(sock, "?")
    if not gdb_remote.add_breakpoint(sock, MAIN_FUNCTION, True):
        return False
    gdb_remote.send_packet(sock, "c")
    stop = gdb_remote.wait_for_stop(sock, 30)
    if stop is None:
        gdb_remote.interrupt(sock)
    registers = gdb_remote.read_registers(sock)
    gdb_remote.remove_breakpoint(sock, MAIN_FUNCTION, True)
    if len(registers) < 16 or registers[15] != MAIN_FUNCTION:
        print("did not stop at main")
        return False
    if not patch_pad_readers(sock):
        print("could not patch the pad readers")
        return False
    return True


def set_buttons(sock, value):
    for address in PAD_READERS:
        gdb_remote.write_memory(sock, address + 4, struct.pack("<I", value))


def step_over_breakpoint(sock, address):
    gdb_remote.remove_breakpoint(sock, address, True)
    gdb_remote.command(sock, "s")
    gdb_remote.add_breakpoint(sock, address, True)


def read_text(sock, address):
    data = gdb_remote.read_memory(sock, address, 96)
    if data is None:
        return "?"
    end = data.find(b"\x00")
    if end == -1:
        end = len(data)
    return data[:end].decode("latin-1")


def main():
    rom_path = sys.argv[1]
    run_seconds = 90
    if len(sys.argv) > 2:
        run_seconds = int(sys.argv[2])
    out_dir = os.path.join(REPO_DIR, "extracted", "trace")
    os.makedirs(out_dir, exist_ok=True)

    process, sock, log = melonds.start(rom_path, "trace_run.log")
    if sock is None:
        print("could not connect to the ARM9 GDB stub")
        sys.exit(1)
    if not prepare(sock):
        melonds.stop(process, sock, log)
        sys.exit(1)
    print("stopped at main, pad readers patched")
    gdb_remote.add_breakpoint(sock, LOAD_ENTRY, True)
    gdb_remote.add_breakpoint(sock, OPEN_FILE, True)

    schedule = make_schedule(run_seconds)
    hits = []
    opens = []
    current_buttons = 0
    start_time = time.time()
    gdb_remote.send_packet(sock, "c")
    while True:
        elapsed = time.time() - start_time
        if elapsed >= run_seconds:
            break
        wait = next_edge(schedule, elapsed) - elapsed
        if wait < 0.02:
            wait = 0.02
        stop = gdb_remote.wait_for_stop(sock, wait)
        if stop is None:
            stop = gdb_remote.interrupt(sock)
        elapsed = time.time() - start_time
        registers = gdb_remote.read_registers(sock)
        if registers[15] == LOAD_ENTRY:
            hits.append((elapsed, registers[0], registers[1], registers[2], registers[14], current_buttons))
            step_over_breakpoint(sock, LOAD_ENTRY)
        if registers[15] == OPEN_FILE:
            opens.append((elapsed, read_text(sock, registers[1])))
            step_over_breakpoint(sock, OPEN_FILE)
        wanted = buttons_at(schedule, elapsed)
        if wanted != current_buttons:
            set_buttons(sock, wanted)
            current_buttons = wanted
        gdb_remote.send_packet(sock, "c")

    gdb_remote.interrupt(sock)
    gdb_remote.remove_breakpoint(sock, LOAD_ENTRY, True)
    gdb_remote.remove_breakpoint(sock, OPEN_FILE, True)
    melonds.stop(process, sock, log)

    f = open(os.path.join(out_dir, "cpac_loads.tsv"), "w")
    f.write("seconds\tpack\tblock\tindex\tcaller\tbuttons\n")
    for hit in hits:
        f.write(str(round(hit[0], 2)) + "\t" + hex(hit[1]) + "\t" + str(hit[2]) + "\t" + str(hit[3]) + "\t" + hex(hit[4]) + "\t" + hex(hit[5]) + "\n")
    f.close()

    f = open(os.path.join(out_dir, "file_opens.tsv"), "w")
    f.write("seconds\tpath\n")
    for item in opens:
        f.write(str(round(item[0], 2)) + "\t" + item[1] + "\n")
    f.close()
    print("file opens logged: " + str(len(opens)))
    for item in opens:
        print("  " + str(round(item[0], 1)) + "s " + item[1])

    counts = {}
    for hit in hits:
        key = (hex(hit[1]), hit[2])
        if key not in counts:
            counts[key] = [0, hit[3], hit[3]]
        counts[key][0] = counts[key][0] + 1
        counts[key][1] = min(counts[key][1], hit[3])
        counts[key][2] = max(counts[key][2], hit[3])
    print("loads logged: " + str(len(hits)))
    for key in sorted(counts):
        print("  pack " + key[0] + " block " + str(key[1]) + ": " + str(counts[key][0]) + " loads, indices " + str(counts[key][1]) + " to " + str(counts[key][2]))


if __name__ == "__main__":
    main()
