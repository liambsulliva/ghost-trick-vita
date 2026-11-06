import json
import os
import socket
import struct
import sys
import time

sys.dont_write_bytecode = True
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import gdb_remote
import melonds
import trace_cpac
import lmg_decode

REPO_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ROM = os.path.join(REPO_DIR, "extracted", "ds_rom", "game.nds")
GAME_SCRIPT_PLAIN = os.path.join(REPO_DIR, "extracted", "nds_plain", "st01", "st01_game000_Expand.xml.lz.dec")
USAGE = "usage: trace_vm.py OUT_DIR MAX_TICKS [--probe] [--tail TICKS] [--no-a-after TICK]"

TICK = 0x02000C06
VM_FETCH = 0x0202400C
OPEN_FILE = 0x0201EFB4
CTOR_A = 0x02050A9C
CTOR_B = 0x02023FB4
DTOR_A = 0x02050884
DTOR_B = 0x0202871C
DTOR_C = 0x0202873C
GAME_FRAME = 0x020CFF68
VBLANK = 0x027FFC3C
EVENT_POINTS = {
    0x0202A044: "bgm_start",
    0x0202A068: "bgm_stop",
    0x0202A450: "bgm_load",
    0x0202A890: "se_play",
    0x0202A16C: "se_stop_all",
    0x020071F0: "seq_start_low",
    0x020118A8: "message",
    0x0205991C: "motion_start",
}
VM_VTABLES = {0x020A4CB4, 0x020ABE14, 0x020ABEC4, 0x020ABE6C, 0x020ABDBC}
FPS = 59.8261


class Rsp:
    def __init__(self, sock):
        self.sock = sock
        self.buf = bytearray()
        sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
        sock.settimeout(120)

    def _fill(self):
        data = self.sock.recv(65536)
        if not data:
            raise EOFError("stub closed")
        self.buf += data

    def read_packet(self):
        while True:
            start = self.buf.find(b"$")
            if start != -1:
                end = self.buf.find(b"#", start)
                if end != -1 and len(self.buf) >= end + 3:
                    text = bytes(self.buf[start + 1:end]).decode("latin-1")
                    del self.buf[:end + 3]
                    self.sock.sendall(b"+")
                    return text
            self._fill()

    @staticmethod
    def frame(text):
        return ("$" + text + "#" + format(sum(text.encode("latin-1")) & 0xff, "02x")).encode("latin-1")

    def _drain_acks(self, count):
        while count > 0:
            while not self.buf:
                self._fill()
            ch = self.buf[0]
            if ch == 0x2b:
                del self.buf[0]
                count -= 1
            elif ch == 0x2d:
                raise RuntimeError("stub sent a negative ack")
            else:
                raise RuntimeError("expected ack, got " + repr(bytes(self.buf[:20])))

    def transact(self, commands, last_has_reply=True):
        replies = []
        for index, command in enumerate(commands):
            self.sock.sendall(self.frame(command))
            self._drain_acks(1)
            if index == len(commands) - 1 and not last_has_reply:
                break
            replies.append(self.read_packet())
        return replies

    def registers(self):
        reply = self.transact(["g"])[0]
        return struct.unpack("<17I", bytes.fromhex(reply[:136]))


def read_cstr(rsp, address):
    reply = rsp.transact(["m" + format(address, "x") + ",60"])[0]
    data = bytes.fromhex(reply) if reply and not reply.startswith("E") else b""
    end = data.find(b"\0")
    return data[:end if end >= 0 else len(data)].decode("latin-1")


def make_schedule(max_ticks):
    schedule = []
    for seconds in [6.0, 9.0, 12.0]:
        schedule.append((int(seconds * FPS), int((seconds + 0.4) * FPS), trace_cpac.BUTTON_START))
    seconds = 14.0
    while seconds * FPS < max_ticks:
        schedule.append((int(seconds * FPS), int((seconds + 0.3) * FPS), trace_cpac.BUTTON_A))
        seconds += 1.5
    return schedule


def buttons_at(schedule, tick):
    value = 0
    for begin, end, button in schedule:
        if begin <= tick < end:
            value |= button
    return value


def game_script_head_hex():
    with open(GAME_SCRIPT_PLAIN, "rb") as source:
        return source.read(12)[4:].hex()


def main():
    if len(sys.argv) < 3:
        print(USAGE)
        sys.exit(2)
    out_dir = os.path.abspath(sys.argv[1])
    max_ticks = int(sys.argv[2])
    probe_rehit = "--probe" in sys.argv
    tail_ticks = 900
    no_a_after = None
    for index, arg in enumerate(sys.argv):
        if arg == "--tail":
            tail_ticks = int(sys.argv[index + 1])
        if arg == "--no-a-after":
            no_a_after = int(sys.argv[index + 1])
    game_head = game_script_head_hex()
    os.makedirs(out_dir, exist_ok=True)

    process, sock, log = melonds.start(ROM, os.path.join(out_dir, "melonds.log"))
    if sock is None:
        print("no stub")
        sys.exit(1)
    if not trace_cpac.prepare(sock):
        melonds.stop(process, sock, log)
        sys.exit(1)

    rsp = Rsp(sock)
    points = [TICK, VM_FETCH, OPEN_FILE, CTOR_A, CTOR_B, DTOR_A, DTOR_B, DTOR_C] + list(EVENT_POINTS)
    for address in points:
        reply = rsp.transact(["Z0," + format(address, "x") + ",2"])[0]
        if reply != "OK":
            print("breakpoint failed", hex(address), reply)

    schedule = make_schedule(max_ticks + 1)
    if no_a_after is not None:
        schedule = [item for item in schedule if item[0] < no_a_after]
    steps = open(os.path.join(out_dir, "golden_trace.tsv"), "w", buffering=1 << 20)
    steps.write("tick\tseq\tvm\tcode_base\toffset\top\twords\tb10\n")
    events = open(os.path.join(out_dir, "events.tsv"), "w", buffering=1 << 16)
    events.write("tick\tseq\tkind\tr0\tr1\tr2\tr3\tlr\textra\n")
    frames = open(os.path.join(out_dir, "frames.tsv"), "w", buffering=1 << 16)
    frames.write("tick\tgame_frame\tvblank\tbuttons\twall\n")
    first_vm = open(os.path.join(out_dir, "vm_first.tsv"), "w", buffering=1 << 12)
    first_vm.write("tick\tseq\tvm\tvtable\tcode_base\tpc\tcaller\tvm_bytes\n")
    heads = open(os.path.join(out_dir, "code_heads.tsv"), "w", buffering=1 << 12)
    heads.write("tick\tcode_base\thead_hex\n")

    tick = -1
    seq = 0
    current = 0
    seen_vm = set()
    seen_base = set()
    word_cache = {}
    tutorial_tick = None
    start = time.time()
    last_report = start
    counts = {}
    finish_reason = "max_ticks"
    game_base = None
    cont = ["c"]
    rsp.transact(cont, last_has_reply=False)
    try:
        while True:
            stop = rsp.read_packet()
            regs = rsp.registers()
            pc = regs[15]
            counts[pc] = counts.get(pc, 0) + 1
            if seq < 3:
                print("stop packet", stop[:80], hex(pc), flush=True)
            seq += 1
            if pc == VM_FETCH:
                vm, op, ptr, sp = regs[4], regs[5], regs[6], regs[13]
                words = word_cache.get(ptr)
                if words is None:
                    words = bytes.fromhex(rsp.transact(["m" + format(ptr, "x") + ",28"])[0])
                    word_cache[ptr] = words
                fields = bytes.fromhex(rsp.transact(["m" + format(vm, "x") + ",40"])[0])
                vtable, code_base, vm_pc = struct.unpack_from("<III", fields, 0)
                b10 = fields[0x10]
                caller = 0
                if vm not in seen_vm:
                    caller = struct.unpack("<I", bytes.fromhex(rsp.transact(["m" + format(sp + 0x13c, "x") + ",4"])[0]))[0]
                length = lmg_decode.LENGTHS.get(op, 8)
                operand = words[2:length * 2].hex()
                offset = ptr - code_base + 0x34
                steps.write("%d\t%d\t%x\t%x\t%x\t%x\t%s\t%d\n" % (tick, seq, vm, code_base, offset, op, operand, b10))
                if vm not in seen_vm:
                    seen_vm.add(vm)
                    first_vm.write("%d\t%d\t%x\t%x\t%x\t%x\t%x\t%s\n" % (tick, seq, vm, vtable, code_base, vm_pc, caller, fields.hex()))
                key = code_base
                if key not in seen_base:
                    seen_base.add(key)
                    head_replies = rsp.transact(["m" + format(code_base - 0x34, "x") + ",74"])
                    heads.write("%d\t%x\t%s\n" % (tick, code_base, head_replies[0]))
                if game_base is not None and code_base == game_base and offset == 0x53c and tutorial_tick is None:
                    tutorial_tick = tick
                    print("gameAct020 reached at tick", tick, flush=True)
            elif pc == TICK:
                tick += 1
                replies = rsp.transact(["m" + format(GAME_FRAME, "x") + ",4", "m" + format(VBLANK, "x") + ",4"])
                game_frame = struct.unpack("<I", bytes.fromhex(replies[0]))[0]
                vblank = struct.unpack("<I", bytes.fromhex(replies[1]))[0]
                wanted = buttons_at(schedule, tick)
                if wanted != current:
                    data = struct.pack("<I", wanted).hex()
                    rsp.transact(["M" + format(reader + 4, "x") + ",4:" + data for reader in trace_cpac.PAD_READERS])
                    current = wanted
                frames.write("%d\t%d\t%d\t%x\t%.3f\n" % (tick, game_frame, vblank, current, time.time() - start))
                now = time.time()
                if now - last_report > 15:
                    last_report = now
                    print("tick", tick, "wall", round(now - start), "steps", counts.get(VM_FETCH, 0), flush=True)
                    steps.flush()
                    events.flush()
                    frames.flush()
                if tick >= max_ticks:
                    finish_reason = "max_ticks"
                    break
                if tutorial_tick is not None and tick >= tutorial_tick + tail_ticks:
                    finish_reason = "tutorial_tail"
                    break
                if probe_rehit and tick == 300:
                    print("counts at tick 300", {hex(k): v for k, v in counts.items()}, "wall", time.time() - start, flush=True)
            elif pc == OPEN_FILE:
                path = read_cstr(rsp, regs[1])
                events.write("%d\t%d\topen_file\t%x\t%x\t%x\t%x\t%x\t%s\n" % (tick, seq, regs[0], regs[1], regs[2], regs[3], regs[14], path))
                seen_base.clear()
                word_cache.clear()
                print("open", tick, path, flush=True)
            elif pc in (CTOR_A, CTOR_B):
                events.write("%d\t%d\tvm_ctor\t%x\t%x\t%x\t%x\t%x\t\n" % (tick, seq, regs[0], regs[1], regs[2], regs[3], regs[14]))
            elif pc in (DTOR_A, DTOR_B, DTOR_C):
                vtable_reply = rsp.transact(["m" + format(regs[0], "x") + ",8"])[0]
                vtable, code_base = struct.unpack("<II", bytes.fromhex(vtable_reply))
                if vtable in VM_VTABLES:
                    events.write("%d\t%d\tvm_dtor\t%x\t%x\t%x\t%x\t%x\t%x\n" % (tick, seq, regs[0], regs[1], regs[2], regs[3], regs[14], vtable))
                    seen_vm.discard(regs[0])
            elif pc in EVENT_POINTS:
                events.write("%d\t%d\t%s\t%x\t%x\t%x\t%x\t%x\t\n" % (tick, seq, EVENT_POINTS[pc], regs[0], regs[1], regs[2], regs[3], regs[14]))
            else:
                events.write("%d\t%d\tother_stop\t%x\t%x\t%x\t%x\t%x\t%s\n" % (tick, seq, regs[0], regs[1], regs[2], regs[3], regs[14], stop[:40]))
            if game_base is None and pc == VM_FETCH:
                head = rsp.transact(["m" + format(code_base - 0x34, "x") + ",34"])[0]
                if head[8:24] == game_head:
                    game_base = code_base
                    print("st01_game000 code base", hex(game_base), "tick", tick, flush=True)
            rsp.transact(cont, last_has_reply=False)
    finally:
        print("finished:", finish_reason, "tick", tick, "seq", seq, "wall", round(time.time() - start), flush=True)
        print({hex(k): v for k, v in counts.items()}, flush=True)
        for handle in (steps, events, frames, first_vm, heads):
            handle.close()
        try:
            for address in points:
                rsp.transact(["z0," + format(address, "x") + ",2"])
            ram = bytearray()
            address = 0x02000000
            while address < 0x02400000:
                reply = rsp.transact(["m" + format(address, "x") + ",100"])[0]
                ram += bytes.fromhex(reply)
                address += 0x100
            ram_file = open(os.path.join(out_dir, "ram_end.bin"), "wb")
            ram_file.write(ram)
            ram_file.close()
        except Exception as error:
            print("ram dump failed", error, flush=True)
        json.dump({"finish": finish_reason, "tick": tick, "seq": seq, "tutorial_tick": tutorial_tick, "wall": time.time() - start,
                   "counts": {hex(k): v for k, v in counts.items()}, "schedule": schedule},
                  open(os.path.join(out_dir, "run.json"), "w"))
        melonds.stop(process, sock, log)


if __name__ == "__main__":
    main()
