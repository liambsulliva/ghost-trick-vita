import glob
import os
import re
import sys

REPO = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "..")
sys.path.insert(0, os.path.join(REPO, "tools"))
import lmg_decode

SEND_OP = 0x1D
BEAM_RAMP_OP = 0xBC
LIGHT_RAMP_OP = 0xBF
MODE_OPERAND = 7
MOTION_WORDS_ALWAYS_HANDLED = (0xFFFE, 0xFFFF)


def source(name):
    return open(os.path.join(REPO, "srcVita/src/app", name)).read()


def engine_modes():
    soul = source("script_soul.c")
    view = source("script_ops_view.c")
    motion = source("script_motion.c")
    table = soul[soul.index("gameapp_commands[] = {"):]
    eyecatch = soul[soul.index("static void start_eye_catch("):]
    lengths = motion[motion.index("static unsigned int table_word_length("):]
    return {
        "gameapp": set(re.findall(r'\{ "(\w+)",', table[:table.index("};")])),
        "eyecatch": set(re.findall(r'strcmp\(kind, "(\w+)"\)', eyecatch[:eyecatch.index("eyecatch_kind_not_ported")])),
        "beam": set(int(value) for value in re.findall(r"#define BEAM_MODE_\w+ (\d+)", view)),
        "light": set(int(value) for value in re.findall(r"#define IRIS_MODE_\w+ (\d+)", view)),
        "motion": set(int(value, 16) for value in re.findall(r"case 0x(fff[0-9a-f])u:", lengths[:lengths.index("default:")])) | set(MOTION_WORDS_ALWAYS_HANDLED),
    }


def modes_the_engine_lacks(script_name, strings, items, engine):
    lacking = []
    for item in items:
        if item[1] == "motion":
            for record in item[2]:
                if isinstance(record[1], int) and (record[1] & 0xFFFF) not in engine["motion"]:
                    lacking.append((script_name, "motion word", hex(record[1] & 0xFFFF)))
            continue
        if item[1] != "op":
            continue
        opcode = item[2]
        operands = item[3]
        if opcode == SEND_OP and len(operands) >= 3 and strings.get(operands[0]) == "GameApp":
            verb = strings.get(operands[1])
            if verb not in engine["gameapp"]:
                lacking.append((script_name, "GameApp", verb))
            elif verb == "EyeCatch" and strings.get(operands[2]) not in engine["eyecatch"]:
                lacking.append((script_name, "EyeCatch", strings.get(operands[2])))
        elif opcode in (BEAM_RAMP_OP, LIGHT_RAMP_OP) and len(operands) > MODE_OPERAND + 1:
            mode = operands[MODE_OPERAND] | (operands[MODE_OPERAND + 1] << 16)
            kind = "beam" if opcode == BEAM_RAMP_OP else "light"
            if mode not in engine[kind]:
                lacking.append((script_name, kind + " mode", str(mode)))
    return lacking


def scan(folder, engine):
    lacking = []
    for path in sorted(glob.glob(os.path.join(folder, "*.lmg"))):
        decoded = lmg_decode.decode(open(path, "rb").read())
        lacking.extend(modes_the_engine_lacks(os.path.basename(path)[:-4], decoded[1], decoded[3], engine))
        for problem in decoded[4]:
            if "motion" in str(problem):
                lacking.append((os.path.basename(path)[:-4], "motion word", str(problem)))
    return lacking


if __name__ == "__main__":
    folders = sys.argv[1:] or [os.path.join(REPO, "extracted/vita_assets", name, "scripts") for name in ("game", "game_ds")]
    engine = engine_modes()
    found = []
    for folder in folders:
        found.extend(scan(folder, engine))
    for script_name, kind, value in sorted(set(found)):
        print("not handled: %s %s in %s" % (kind, value, script_name))
    print("script modes the engine lacks: %d" % len(set(found)))
    sys.exit(1 if found else 0)
