import os
import re
import sys

SRC_VITA = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..")


def hidden_handlers():
    table = open(os.path.join(SRC_VITA, "src/app/script_opcodes.c")).read()
    stores_only = set(int(m, 16) for m in re.findall(r"\[0x([0-9a-f]+)\] = SCRIPT_OP_STORES_ONLY", table))
    machine = open(os.path.join(SRC_VITA, "src/app/script_machine.c")).read()
    body = machine[machine.index("static enum script_step_result run_control_op("):machine.index("static enum script_step_result run_op(")]
    control_cases = set(int(m, 16) for m in re.findall(r"case 0x([0-9a-f]+):", body))
    return sorted(stores_only & control_cases)


if __name__ == "__main__":
    hidden = hidden_handlers()
    print("ops filed as stores-only that have a control handler: %s" % ([hex(op) for op in hidden] or "none"))
    sys.exit(1 if hidden else 0)
