import argparse
import os
import pickle
import struct
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import gdb_remote
import melonds
import trace_cpac

REPO_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
REGIONS = {
    "io_a": (0x04000000, 0x70),
    "io_b": (0x04001000, 0x70),
    "vramcnt": (0x04000240, 0x0a),
    "palette": (0x05000000, 0x800),
    "oam": (0x07000000, 0x800),
    "bg_a": (0x06000000, 0x80000),
    "bg_b": (0x06200000, 0x20000),
    "obj_a": (0x06400000, 0x40000),
    "obj_b": (0x06600000, 0x20000),
}
PACKET_SIZE = 0x100
CAPTURE_BANK_CONTROL = 0x04000243
CAPTURE_CONTROL = 0x04000064
CAPTURE_BANK_MAPPED = 0x06200000
CAPTURE_BANK_PLAIN = 0x06860000
CAPTURE_BANK_SIZE = 0x20000
CAPTURE_BYTES = 256 * 192 * 2
CAPTURE_REQUEST = 0x80330000
CAPTURE_SECONDS = 0.1
BUTTONS = {"a": 0x0001, "b": 0x0002, "select": 0x0004, "start": 0x0008, "right": 0x0010, "left": 0x0020, "up": 0x0040, "down": 0x0080, "r": 0x0100, "l": 0x0200, "x": 0x0400, "y": 0x0800}


def read_region(sock, address, length):
    result = bytearray()
    while len(result) < length:
        size = min(PACKET_SIZE, length - len(result))
        reply = gdb_remote.command(sock, "m" + format(address + len(result), "x") + "," + format(size, "x"))
        if reply is None or reply == "" or reply.startswith("E"):
            result = result + bytes(size)
        else:
            result = result + bytes.fromhex(reply)
    return bytes(result)


def read_state(sock):
    return {name: read_region(sock, address, length) for name, (address, length) in REGIONS.items()}


def write_region(sock, address, data):
    for start in range(0, len(data), PACKET_SIZE):
        gdb_remote.write_memory(sock, address + start, data[start:start + PACKET_SIZE])


def run_for(sock, seconds):
    gdb_remote.send_packet(sock, "c")
    if gdb_remote.wait_for_stop(sock, seconds) is None:
        gdb_remote.interrupt(sock)


def capture_main_screen(sock):
    bank_control = read_region(sock, CAPTURE_BANK_CONTROL, 1)
    capture_control = read_region(sock, CAPTURE_CONTROL, 4)
    saved = read_region(sock, CAPTURE_BANK_MAPPED, CAPTURE_BANK_SIZE)
    write_region(sock, CAPTURE_BANK_CONTROL, bytes([0x80]))
    write_region(sock, CAPTURE_CONTROL, struct.pack("<I", CAPTURE_REQUEST))
    run_for(sock, CAPTURE_SECONDS)
    picture = read_region(sock, CAPTURE_BANK_PLAIN, CAPTURE_BYTES)
    write_region(sock, CAPTURE_CONTROL, capture_control)
    write_region(sock, CAPTURE_BANK_CONTROL, bank_control)
    write_region(sock, CAPTURE_BANK_MAPPED, saved)
    return picture


def parse_presses(texts):
    schedule = []
    for text in texts:
        name, begin, length = text.split(":")
        schedule.append((float(begin), float(begin) + float(length), BUTTONS[name]))
    return schedule


def repeat_presses(name, begin, end, period, length):
    schedule = []
    moment = begin
    while moment < end:
        schedule.append((moment, moment + length, BUTTONS[name]))
        moment = moment + period
    return schedule


def main():
    parser = argparse.ArgumentParser(description="Run the DS game in melonDS, press buttons on a schedule and save the video memory at chosen moments, for tools/ds_screen_render.py to draw.")
    parser.add_argument("rom")
    parser.add_argument("output", help="folder for the dumps, one dump_<seconds>.pickle per moment")
    parser.add_argument("--at", type=float, action="append", default=[], help="a moment to dump, in seconds of emulated run")
    parser.add_argument("--press", action="append", default=[], help="button:begin:length, such as a:14:0.3")
    parser.add_argument("--repeat", action="append", default=[], help="button:begin:end:period:length")
    parser.add_argument("--capture", action="store_true", help="also capture the main engine's finished picture, 3D layer included, through the display capture unit into VRAM bank D, which is put back afterwards")
    arguments = parser.parse_args()
    os.makedirs(arguments.output, exist_ok=True)
    schedule = parse_presses(arguments.press)
    for text in arguments.repeat:
        name, begin, end, period, length = text.split(":")
        schedule = schedule + repeat_presses(name, float(begin), float(end), float(period), float(length))
    moments = sorted(arguments.at)
    process, sock, log = melonds.start(arguments.rom, "screen_dump.log")
    if sock is None:
        print("could not connect to the ARM9 GDB stub")
        sys.exit(1)
    if not trace_cpac.prepare(sock):
        melonds.stop(process, sock, log)
        sys.exit(1)
    elapsed = 0.0
    current = 0
    while moments:
        target = min(moments[0], trace_cpac.next_edge(schedule, elapsed))
        gdb_remote.send_packet(sock, "c")
        started = time.time()
        if gdb_remote.wait_for_stop(sock, max(0.02, target - elapsed)) is None:
            gdb_remote.interrupt(sock)
        elapsed = elapsed + time.time() - started
        wanted = trace_cpac.buttons_at(schedule, elapsed)
        if wanted != current:
            trace_cpac.set_buttons(sock, wanted)
            current = wanted
        while moments and elapsed >= moments[0]:
            moment = moments.pop(0)
            path = os.path.join(arguments.output, "dump_%07.2f.pickle" % moment)
            state = read_state(sock)
            if arguments.capture:
                state["capture"] = capture_main_screen(sock)
            with open(path, "wb") as dump:
                pickle.dump(state, dump)
            print("dumped", path)
    melonds.stop(process, sock, log)


if __name__ == "__main__":
    main()
