import sys
import os
import time
import struct

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import gdb_remote
import melonds
import trace_cpac

REPO_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
MOTION_START = 0x0205991C
SCRIPT_FILE = 0x0214063C


def main():
    rom_path = sys.argv[1]
    run_seconds = 40
    if len(sys.argv) > 2:
        run_seconds = int(sys.argv[2])
    out_dir = os.path.join(REPO_DIR, "extracted", "trace")
    os.makedirs(out_dir, exist_ok=True)

    process, sock, log = melonds.start(rom_path, "trace_mtbl.log")
    if sock is None:
        print("could not connect to the ARM9 GDB stub")
        sys.exit(1)
    if not trace_cpac.prepare(sock):
        melonds.stop(process, sock, log)
        sys.exit(1)
    gdb_remote.add_breakpoint(sock, MOTION_START, True)

    schedule = trace_cpac.make_schedule(run_seconds)
    starts = []
    reads = []
    watch = None
    watch_ok = None
    current_buttons = 0
    start_time = time.time()
    gdb_remote.send_packet(sock, "c")
    while True:
        elapsed = time.time() - start_time
        if elapsed >= run_seconds:
            break
        wait = trace_cpac.next_edge(schedule, elapsed) - elapsed
        if wait < 0.02:
            wait = 0.02
        stop = gdb_remote.wait_for_stop(sock, wait)
        timed_out = stop is None
        if timed_out:
            stop = gdb_remote.interrupt(sock)
        elapsed = time.time() - start_time
        registers = gdb_remote.read_registers(sock)
        if registers[15] == MOTION_START:
            starts.append((elapsed, registers[0], registers[1], registers[2], registers[3], registers[14]))
            if watch is None and registers[1] > SCRIPT_FILE:
                watch = registers[0] + 0x144
                reply = gdb_remote.command(sock, "Z4," + format(watch, "x") + ",8")
                watch_ok = reply
            trace_cpac.step_over_breakpoint(sock, MOTION_START)
        elif not timed_out and stop is not None and watch_ok == "OK" and len(reads) < 40:
            reads.append((elapsed, registers[15], stop, list(registers[:16])))
            gdb_remote.command(sock, "z4," + format(watch, "x") + ",8")
            gdb_remote.command(sock, "s")
            if len(reads) < 40:
                gdb_remote.command(sock, "Z4," + format(watch, "x") + ",8")
        wanted = trace_cpac.buttons_at(schedule, elapsed)
        if wanted != current_buttons:
            trace_cpac.set_buttons(sock, wanted)
            current_buttons = wanted
        gdb_remote.send_packet(sock, "c")

    gdb_remote.interrupt(sock)
    gdb_remote.remove_breakpoint(sock, MOTION_START, True)
    melonds.stop(process, sock, log)

    f = open(os.path.join(out_dir, "mtbl_starts.tsv"), "w")
    f.write("seconds\tobject\tmtbl_address\tmtbl_offset\tid\tflags\tcaller\n")
    for item in starts:
        f.write(str(round(item[0], 2)) + "\t" + hex(item[1]) + "\t" + hex(item[2]) + "\t" + hex(item[2] - SCRIPT_FILE) + "\t" + hex(item[3]) + "\t" + hex(item[4]) + "\t" + hex(item[5]) + "\n")
    for item in reads:
        f.write("read\t" + str(round(item[0], 2)) + "\t" + hex(item[1]) + "\t" + item[2] + "\t" + " ".join(hex(r) for r in item[3]) + "\n")
    f.close()
    print("motion starts: " + str(len(starts)) + ", watchpoint reply: " + str(watch_ok) + ", watch hits: " + str(len(reads)))


if __name__ == "__main__":
    main()
