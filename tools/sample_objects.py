import sys
import os
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import gdb_remote
import melonds
import trace_cpac

REPO_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def main():
    rom_path = sys.argv[1]
    window_start = int(sys.argv[2], 16)
    window_end = int(sys.argv[3], 16)
    sample_seconds = 25.0
    interval = 0.3
    scene_file = "st01/st01_game000_Expand.xml.lz"
    out_dir = os.path.join(REPO_DIR, "extracted", "trace")
    os.makedirs(out_dir, exist_ok=True)

    process, sock, log = melonds.start(rom_path, "sample_objects.log")
    if sock is None:
        print("could not connect to the ARM9 GDB stub")
        sys.exit(1)
    if not trace_cpac.prepare(sock):
        melonds.stop(process, sock, log)
        sys.exit(1)
    gdb_remote.add_breakpoint(sock, trace_cpac.OPEN_FILE, True)

    schedule = trace_cpac.make_schedule(120)
    current_buttons = 0
    scene_time = None
    next_sample = None
    samples = []
    start_time = time.time()
    gdb_remote.send_packet(sock, "c")
    while True:
        elapsed = time.time() - start_time
        if elapsed > 110:
            break
        if scene_time is not None and elapsed > scene_time + sample_seconds:
            break
        edge = trace_cpac.next_edge(schedule, elapsed)
        if next_sample is not None and next_sample < edge:
            edge = next_sample
        wait = edge - elapsed
        if wait < 0.02:
            wait = 0.02
        stop = gdb_remote.wait_for_stop(sock, wait)
        if stop is None:
            gdb_remote.interrupt(sock)
        elapsed = time.time() - start_time
        registers = gdb_remote.read_registers(sock)
        if registers[15] == trace_cpac.OPEN_FILE:
            path = trace_cpac.read_text(sock, registers[1])
            if path == scene_file and scene_time is None:
                scene_time = elapsed
                next_sample = elapsed + 1.0
                gdb_remote.remove_breakpoint(sock, trace_cpac.OPEN_FILE, True)
                print("scene opened at " + str(round(elapsed, 1)) + "s")
            else:
                trace_cpac.step_over_breakpoint(sock, trace_cpac.OPEN_FILE)
        if next_sample is not None and elapsed >= next_sample:
            data = gdb_remote.read_memory(sock, window_start, window_end - window_start)
            if data is not None:
                samples.append((elapsed - scene_time, data))
            next_sample = time.time() - start_time + interval
        wanted = trace_cpac.buttons_at(schedule, elapsed)
        if wanted != current_buttons:
            trace_cpac.set_buttons(sock, wanted)
            current_buttons = wanted
        gdb_remote.send_packet(sock, "c")

    melonds.stop(process, sock, log)
    name = "samples_" + format(window_start, "08x") + "_" + format(window_end, "08x")
    f = open(os.path.join(out_dir, name + ".bin"), "wb")
    for sample in samples:
        f.write(sample[1])
    f.close()
    f = open(os.path.join(out_dir, name + ".times"), "w")
    for sample in samples:
        f.write(str(round(sample[0], 2)) + "\n")
    f.close()
    print("samples taken: " + str(len(samples)))


if __name__ == "__main__":
    main()
