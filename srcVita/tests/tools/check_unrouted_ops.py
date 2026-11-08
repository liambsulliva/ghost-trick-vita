import collections
import glob
import os
import re
import sys

REPO = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "..")
sys.path.insert(0, os.path.join(REPO, "tools"))
import lmg_decode

COUNTER_REGISTERS = range(0x0C, 0x11)
REGISTER_COMPARE_OP = 0x65
REGISTER_STORE_OPS = (0x128, 0x129, 0x12A, 0x12B)


def routed_ops():
    table = open(os.path.join(REPO, "srcVita/src/app/script_opcodes.c")).read()
    families = table[table.index("families"):]
    return set(int(m, 16) for m in re.findall(r"\[0x([0-9a-f]+)\] = SCRIPT_OP_", families))


def scan(folder):
    uses = collections.Counter()
    scripts = collections.defaultdict(set)
    counter_uses = collections.Counter()
    for path in sorted(glob.glob(os.path.join(folder, "*.lmg"))):
        name = os.path.basename(path)[:-4]
        decoded = lmg_decode.decode(open(path, "rb").read())
        for item in decoded["items"] if isinstance(decoded, dict) else decoded[3]:
            if len(item) < 4 or item[1] != "op":
                continue
            opcode = item[2]
            operands = item[3]
            uses[opcode] += 1
            scripts[opcode].add(name)
            if (opcode == REGISTER_COMPARE_OP or opcode in REGISTER_STORE_OPS) and operands and operands[0] in COUNTER_REGISTERS:
                counter_uses[(opcode, operands[0])] += 1
    return uses, scripts, counter_uses


if __name__ == "__main__":
    folder = sys.argv[1] if len(sys.argv) > 1 else os.path.join(REPO, "extracted/vita_assets/game/scripts")
    uses, scripts, counter_uses = scan(folder)
    routed = routed_ops()
    unrouted = sorted((op for op in uses if op not in routed), key=lambda op: -uses[op])
    for op in unrouted:
        print("unrouted 0x%x uses=%d scripts=%d" % (op, uses[op], len(scripts[op])))
    for (op, register), count in sorted(counter_uses.items()):
        print("counter register op=0x%x register=0x%x uses=%d" % (op, register, count))
    sys.exit(1 if unrouted else 0)
