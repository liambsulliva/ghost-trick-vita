import sys
import os
import time
import struct

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import gdb_remote
import melonds
import trace_cpac

REPO_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
VM_FETCH = 0x0202400C
OPEN_FILE = 0x0201EFB4
MAX_HITS = 400000


def main():
    rom_path = sys.argv[1]
    run_seconds = 40
    if len(sys.argv) > 2:
        run_seconds = int(sys.argv[2])
    out_dir = os.path.join(REPO_DIR, "extracted", "trace")
    os.makedirs(out_dir, exist_ok=True)

    process, sock, log = melonds.start(rom_path, "trace_lmg.log")
    if sock is None:
        print("could not connect to the ARM9 GDB stub")
        sys.exit(1)
    if not trace_cpac.prepare(sock):
        melonds.stop(process, sock, log)
        sys.exit(1)
    gdb_remote.add_breakpoint(sock, VM_FETCH, True)
    gdb_remote.add_breakpoint(sock, OPEN_FILE, True)

    schedule = trace_cpac.make_schedule(run_seconds)
    hits = []
    opens = []
    vms = {}
    current_buttons = 0
    start_time = time.time()
    gdb_remote.send_packet(sock, "c")
    while len(hits) < MAX_HITS:
        elapsed = time.time() - start_time
        if elapsed >= run_seconds:
            break
        wait = trace_cpac.next_edge(schedule, elapsed) - elapsed
        if wait < 0.02:
            wait = 0.02
        stop = gdb_remote.wait_for_stop(sock, wait)
        if stop is None:
            stop = gdb_remote.interrupt(sock)
        elapsed = time.time() - start_time
        registers = gdb_remote.read_registers(sock)
        if registers[15] == VM_FETCH:
            vm = registers[4]
            if vm not in vms:
                fields = gdb_remote.read_memory(sock, vm, 16)
                vms[vm] = fields
            code_base = registers[6] - 2 * struct.unpack_from("<I", gdb_remote.read_memory(sock, vm + 8, 4), 0)[0]
            hits.append((elapsed, vm, code_base, registers[6] - code_base, registers[5]))
            trace_cpac.step_over_breakpoint(sock, VM_FETCH)
        if registers[15] == OPEN_FILE:
            opens.append((elapsed, trace_cpac.read_text(sock, registers[1])))
            hits.append((elapsed, 0, 0, 0, -1, opens[-1][1]))
            trace_cpac.step_over_breakpoint(sock, OPEN_FILE)
        wanted = trace_cpac.buttons_at(schedule, elapsed)
        if wanted != current_buttons:
            trace_cpac.set_buttons(sock, wanted)
            current_buttons = wanted
        gdb_remote.send_packet(sock, "c")

    gdb_remote.interrupt(sock)
    gdb_remote.remove_breakpoint(sock, VM_FETCH, True)
    gdb_remote.remove_breakpoint(sock, OPEN_FILE, True)
    heads = {}
    for hit in hits:
        if hit[4] >= 0 and hit[2] not in heads:
            heads[hit[2]] = gdb_remote.read_memory(sock, hit[2] - 0x34, 0x34)
    melonds.stop(process, sock, log)

    f = open(os.path.join(out_dir, "lmg_steps.tsv"), "w")
    f.write("seconds\tvm\tcode_base\toffset\topcode\n")
    for hit in hits:
        if hit[4] < 0:
            f.write(str(round(hit[0], 3)) + "\topen\t" + hit[5] + "\n")
        else:
            f.write(str(round(hit[0], 3)) + "\t" + hex(hit[1]) + "\t" + hex(hit[2]) + "\t" + hex(hit[3] + 0x34) + "\t" + hex(hit[4]) + "\n")
    f.close()
    f = open(os.path.join(out_dir, "lmg_vms.tsv"), "w")
    for vm in sorted(vms):
        f.write(hex(vm) + "\t" + vms[vm].hex() + "\n")
    for base in sorted(heads):
        head = heads[base]
        if head is None:
            head = b""
        f.write("base " + hex(base) + "\t" + head.hex() + "\n")
    f.close()
    print("steps logged: " + str(len(hits)) + ", machines: " + str(len(vms)) + ", code bases: " + str(len(heads)))


if __name__ == "__main__":
    main()
