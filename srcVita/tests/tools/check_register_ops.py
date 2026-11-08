import os
import re
import subprocess
import sys

SRC_VITA = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..")
REPLAY = os.path.join(SRC_VITA, "build/tests/golden/script_replay")
REGISTER_OPS = ("0x127", "0x128", "0x12a", "0x12c", "0x12d")
SCRIPT_AFTER_PUZZLE = "st06-st06_game002_Expand"


def replay_marks(scripts):
    result = subprocess.run([REPLAY, "--scripts", scripts, "--timing", "remaster", "--auto-paced", "--marks", "--stop-tick", "4000000"], capture_output=True, text=True)
    return result.stdout


if __name__ == "__main__":
    marks = replay_marks(sys.argv[1])
    problems = []
    for op in REGISTER_OPS:
        if re.search(r"op_not_ported\s+op=%s\b" % op, marks):
            problems.append("op %s still not ported" % op)
    if "script_begun %s" % SCRIPT_AFTER_PUZZLE not in marks:
        problems.append("the four-minute puzzle never handed over to %s" % SCRIPT_AFTER_PUZZLE)
    for problem in problems:
        print(problem)
    print("register ops check: %d problems" % len(problems))
    sys.exit(1 if problems else 0)
