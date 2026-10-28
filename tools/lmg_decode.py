import sys
import os
import struct
import glob

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

REPO_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CODE_START = 0x34

LENGTHS = {
    0x0: 1, 0x1: 1, 0x8: 1, 0x9: 1, 0xa: 1, 0xb: 2, 0xc: 2, 0xd: 2, 0xe: 2, 0xf: 2, 0x10: 2,
    0x11: 2, 0x12: 2, 0x13: 2, 0x14: 3, 0x15: 4, 0x16: 5, 0x17: 2, 0x18: 2, 0x19: 5, 0x1a: 6,
    0x1b: 4, 0x1c: 1, 0x1d: 4, 0x1e: 1, 0x1f: 1, 0x20: 1, 0x21: 1, 0x22: 3, 0x23: 3, 0x24: 2,
    0x26: 2, 0x27: 2, 0x28: 2, 0x29: 2, 0x2a: 3, 0x2b: 1, 0x2d: 3, 0x2e: 1, 0x2f: 2, 0x30: 1,
    0x31: 1, 0x32: 3, 0x33: 1, 0x34: 1, 0x37: 3, 0x38: 2, 0x39: 1, 0x3a: 1, 0x3b: 1, 0x3c: 7,
    0x3d: 8, 0x3e: 1, 0x3f: 1, 0x40: 8, 0x41: 9, 0x42: 1, 0x43: 1, 0x44: 1, 0x45: 7, 0x46: 8,
    0x47: 7, 0x48: 2, 0x49: 1, 0x4a: 2, 0x4b: 1, 0x4c: 7, 0x4d: 1, 0x4e: 8, 0x4f: 7, 0x50: 1,
    0x51: 5, 0x52: 3, 0x53: 5, 0x54: 5, 0x55: 5, 0x56: 3, 0x57: 1, 0x58: 1, 0x59: 1, 0x5a: 1,
    0x5b: 1, 0x5c: 4, 0x5d: 3, 0x5e: 1, 0x61: 3, 0x62: 3, 0x63: 4, 0x64: 3, 0x65: 5, 0x66: 2,
    0x67: 1, 0x68: 1, 0x69: 1, 0x6a: 1, 0x6b: 1, 0x6c: 1, 0x6d: 1, 0x6e: 1, 0x6f: 1, 0x70: 1,
    0x71: 1, 0x72: 1, 0x73: 2, 0x74: 2, 0x75: 2, 0x76: 4, 0x77: 3, 0x78: 4, 0x79: 1, 0x7a: 1,
    0x7b: 1, 0x7c: 2, 0x7d: 2, 0x7e: 1, 0x7f: 1, 0x80: 1, 0x81: 3, 0x82: 3, 0x83: 2, 0x84: 3,
    0x85: 4, 0x86: 4, 0x87: 7, 0x8c: 2, 0x8d: 3, 0x8e: 3, 0x8f: 2, 0x90: 4, 0x91: 3, 0x92: 5,
    0x93: 5, 0x94: 1, 0x95: 1, 0x96: 2, 0x97: 2, 0x98: 2, 0x99: 2, 0x9a: 2, 0x9b: 2, 0x9c: 2,
    0x9d: 3, 0x9e: 3, 0x9f: 3, 0xa0: 2, 0xa1: 2, 0xa2: 2, 0xa3: 2, 0xa4: 2, 0xa5: 2, 0xa6: 2,
    0xa7: 2, 0xa8: 2, 0xa9: 1, 0xaa: 3, 0xab: 4, 0xac: 1, 0xad: 1, 0xae: 2, 0xaf: 2, 0xb0: 18,
    0xb1: 2, 0xb2: 2, 0xb3: 3, 0xb4: 6, 0xb5: 3, 0xb6: 6, 0xb7: 7, 0xb8: 3, 0xb9: 3, 0xba: 3,
    0xbb: 18, 0xbc: 10, 0xbd: 2, 0xbe: 14, 0xbf: 10, 0xc0: 2, 0xc1: 4, 0xc2: 1, 0xc3: 1,
    0xc4: 1, 0xc5: 4, 0xc6: 3, 0xc7: 5, 0xc8: 3, 0xc9: 1, 0xca: 3, 0xcb: 1, 0xcc: 1, 0xcd: 3,
    0xce: 7, 0xcf: 1, 0xd0: 1, 0xd1: 1, 0xd2: 1, 0xd3: 3, 0xd4: 5, 0xd5: 3, 0xd6: 1, 0xd7: 3,
    0xd8: 2, 0xd9: 2, 0xda: 1, 0xdb: 4, 0xdc: 3, 0xdd: 1, 0xde: 1, 0xdf: 2, 0xe0: 2, 0xe1: 1,
    0xe2: 2, 0xe3: 1, 0xe4: 1, 0xe5: 1, 0xe6: 1, 0xe7: 1, 0xe8: 1, 0xe9: 2, 0xea: 2, 0xeb: 2,
    0xec: 3, 0xed: 1, 0xee: 2, 0xef: 1, 0xf0: 1, 0xf1: 2, 0xf2: 1, 0xf3: 2, 0xf4: 2, 0xf5: 1,
    0xf6: 2, 0xf7: 3, 0xf8: 2, 0xf9: 1, 0xfa: 1, 0xfb: 1, 0xfc: 1, 0xfd: 2, 0xfe: 1, 0xff: 1,
    0x100: 1, 0x101: 1, 0x102: 1, 0x103: 2, 0x104: 1, 0x105: 1, 0x106: 3, 0x107: 4, 0x108: 2,
    0x109: 3, 0x10a: 2, 0x10b: 3, 0x10c: 5, 0x10d: 6, 0x10e: 3, 0x10f: 2, 0x112: 1, 0x113: 1,
    0x114: 1, 0x115: 1, 0x116: 1, 0x117: 5, 0x118: 2, 0x119: 2, 0x127: 2, 0x128: 4, 0x129: 2,
    0x12a: 2, 0x12b: 2, 0x12c: 2, 0x12d: 2, 0x12e: 1, 0x12f: 1, 0x130: 1, 0x131: 1, 0x132: 1,
    0x169: 2, 0x16a: 2,
}

STRING_OPERANDS = {
    0x14: [1], 0x17: [1], 0x1b: [1], 0x1d: [1, 2, 3], 0x24: [1], 0x38: [1], 0x40: [5, 6, 7],
    0x41: [6, 7, 8], 0x46: [3, 4, 5], 0x4e: [3, 4, 5], 0x7c: [1], 0x96: [1], 0x97: [1],
    0x98: [1], 0x99: [1], 0x9d: [1], 0x9e: [1], 0xb0: [10], 0xb1: [1], 0xb2: [1], 0xb3: [1],
    0xb4: [1], 0xb5: [1], 0xb6: [1], 0xb7: [1], 0xb8: [1], 0xb9: [1], 0xba: [1], 0xeb: [1],
    0xee: [1], 0xf7: [1], 0x10b: [2], 0x10c: [4], 0x10d: [4], 0x10e: [2],
}

OFFSET_OPERANDS = {
    0x22: [1], 0x23: [1], 0x2d: [1], 0x32: [1], 0x40: [3], 0x45: [1, 3], 0x46: [1],
    0x47: [1, 3], 0x4f: [1], 0x52: [1], 0x53: [1, 3], 0x54: [1, 3], 0x55: [1, 3], 0x61: [1],
    0x62: [1], 0x63: [1], 0x64: [1], 0x82: [1], 0x84: [1], 0x87: [1, 3], 0x8d: [1], 0xb0: [2],
    0xc7: [3], 0xca: [1], 0xcd: [1], 0xd4: [3], 0xec: [1], 0x117: [1], 0x128: [2],
}

BLOCK_NAMES = {0x15: "root", 0x16: "object", 0x19: "act", 0x1a: "handler"}
MOTION_LENGTHS = {-16: 4, -13: 2, -12: 3, -11: 4, -10: 2, -9: 3, -8: 11, -7: 4, -6: 2, -5: 3, -4: 5, -3: 3, -2: 1, -1: 1}


def read_u16(data, offset):
    return struct.unpack_from("<H", data, offset)[0]


def read_s16(data, offset):
    return struct.unpack_from("<h", data, offset)[0]


def read_u32(data, offset):
    return struct.unpack_from("<I", data, offset)[0]


def read_header(data):
    if len(data) < CODE_START or data[:4] != b"1LMG":
        raise ValueError("missing or truncated 1LMG header")
    header = {}
    header["field4"] = read_u32(data, 4)
    header["code_size"] = read_u32(data, 8)
    header["string_size"] = read_u32(data, 12)
    header["label_size"] = read_u32(data, 16)
    total = CODE_START + header["code_size"] + header["string_size"] + header["label_size"]
    if total != len(data):
        raise ValueError("header sizes do not match the file size")
    if header["code_size"] % 2 != 0:
        raise ValueError("code size is not a whole number of words")
    return header


def read_strings(data, header):
    start = CODE_START + header["code_size"]
    table = data[start:start + header["string_size"]]
    strings = {}
    pos = 0
    while pos < len(table):
        end = table.find(b"\x00", pos)
        if end == -1:
            end = len(table)
        strings[pos] = table[pos:end].decode("latin-1")
        pos = end + 1
    return strings


def read_labels(data, header):
    start = CODE_START + header["code_size"] + header["string_size"]
    labels = {}
    if header["label_size"] < 4:
        return labels
    count = read_u32(data, start)
    names_start = start + 4 + 8 * count
    if names_start > len(data):
        raise ValueError("label entries extend past the label table")
    index = 0
    while index < count:
        name_offset = read_u32(data, start + 4 + 8 * index)
        target = read_u32(data, start + 8 + 8 * index)
        if names_start + name_offset >= len(data):
            raise ValueError("label name extends past the label table")
        end = data.find(b"\x00", names_start + name_offset)
        if end == -1:
            raise ValueError("label name has no terminator")
        name = data[names_start + name_offset:end].decode("latin-1")
        if target not in labels:
            labels[target] = []
        labels[target].append(name)
        index = index + 1
    return labels


def string_text(strings, value):
    if value in strings:
        return '"' + strings[value] + '"'
    return "str?" + hex(value)


def is_motion_label(labels, pos):
    if pos not in labels:
        return False
    for name in labels[pos]:
        if name.endswith(".mtbl.xml"):
            return True
    return False


def decode_motion(data, pos, end):
    records = []
    while pos < end:
        raw = read_u16(data, pos)
        value = raw - 0x10000
        if raw >= 0xFFF0 and value in MOTION_LENGTHS:
            length = MOTION_LENGTHS[value]
            if pos + 2 * length > end:
                return records, pos, "truncated motion command"
            args = []
            i = 1
            while i < length:
                args.append(read_s16(data, pos + 2 * i))
                i = i + 1
            records.append((pos, value, args))
            pos = pos + 2 * length
            if value == -2:
                break
        elif raw >= 0xFFF0:
            return records, pos, "unknown motion tag " + str(value)
        else:
            records.append((pos, "frame", [raw & 0x3FF, raw >> 10]))
            pos = pos + 2
    return records, pos, None


def motion_text(record):
    tag = record[1]
    args = record[2]
    if tag == "frame":
        return "frame " + str(args[0]) + " ticks " + str(args[1])
    names = {-3: "begin", -4: "box", -12: "offset", -11: "layer_offset", -5: "frame_long", -2: "end", -1: "loop",
             -16: "sub", -13: "set13", -10: "set10", -9: "sound9", -8: "effect", -7: "message", -6: "set6"}
    return names.get(tag, str(tag)) + " " + " ".join(str(a) for a in args)


def next_label_after(labels, pos, end):
    best = end
    for target in labels:
        if target > pos and target < best:
            best = target
    return best


def decode(data):
    header = read_header(data)
    strings = read_strings(data, header)
    labels = read_labels(data, header)
    end = CODE_START + header["code_size"]
    items = []
    starts = set()
    targets = []
    errors = []
    pos = CODE_START
    previous = []
    while pos < end:
        if is_motion_label(labels, pos):
            limit = next_label_after(labels, pos, end)
            records, stop, error = decode_motion(data, pos, limit)
            items.append((pos, "motion", records))
            starts.add(pos)
            if error is not None:
                errors.append(hex(stop) + " " + error)
                pos = limit
            else:
                pos = stop
            previous = []
            continue
        op = read_u16(data, pos)
        if op == 0xFF2D and pos in labels:
            limit = next_label_after(labels, pos, end)
            items.append((pos, "message", limit - pos))
            starts.add(pos)
            pos = limit
            previous = []
            continue
        if len(previous) >= 2 and previous[-1][1] == 0x22 and previous[-2][1] == 0x12E:
            limit = read_u32(data, previous[-1][0] + 2)
            if limit > pos and limit <= end:
                items.append((pos, "inline", limit - pos))
                starts.add(pos)
                pos = limit
                previous = []
                continue
        if op not in LENGTHS:
            errors.append(hex(pos) + " unknown opcode " + hex(op))
            break
        length = LENGTHS[op]
        if pos + 2 * length > end:
            errors.append(hex(pos) + " truncated opcode " + hex(op))
            break
        operands = []
        i = 1
        while i < length:
            operands.append(read_u16(data, pos + 2 * i))
            i = i + 1
        items.append((pos, "op", op, operands))
        starts.add(pos)
        if op in BLOCK_NAMES:
            starts.add(pos + 2)
            targets.append((pos, read_u32(data, pos + 4)))
        elif op in OFFSET_OPERANDS and op in (0x22, 0x23, 0x2D):
            targets.append((pos, read_u32(data, pos + 2)))
        previous.append((pos, op))
        pos = pos + 2 * length
    if pos != end and len(errors) == 0:
        errors.append("decode stopped at " + hex(pos) + " instead of " + hex(end))
    for item in targets:
        if item[1] != 0 and item[1] not in starts and item[1] != end:
            errors.append("jump at " + hex(item[0]) + " lands inside an instruction at " + hex(item[1]))
    return header, strings, labels, items, errors


def operand_text(op, operands, strings, labels):
    words = []
    skip = set()
    if op in OFFSET_OPERANDS:
        for index in OFFSET_OPERANDS[op]:
            if index + 1 <= len(operands):
                value = operands[index - 1] | (operands[index] << 16)
                name = ""
                if value in labels:
                    name = " " + labels[value][0]
                words.append("@" + hex(value) + name)
                skip.add(index)
                skip.add(index + 1)
    string_indexes = []
    if op in STRING_OPERANDS:
        string_indexes = STRING_OPERANDS[op]
    i = 1
    while i <= len(operands):
        if i not in skip:
            if i in string_indexes:
                words.append(string_text(strings, operands[i - 1]))
            else:
                words.append(str(read_signed(operands[i - 1])))
        i = i + 1
    return " ".join(words)


def read_signed(value):
    if value >= 0x8000:
        return value - 0x10000
    return value


def write_tree(path, data, out):
    header, strings, labels, items, errors = decode(data)
    out.write("# " + os.path.basename(path) + "\n")
    out.write("# header field4 " + hex(header["field4"]) + " code " + hex(header["code_size"]) + " strings " + hex(header["string_size"]) + " labels " + hex(header["label_size"]) + "\n")
    closes = []
    for item in items:
        pos = item[0]
        while len(closes) > 0 and pos >= closes[-1]:
            closes.pop()
        indent = "  " * len(closes)
        if pos in labels:
            for name in labels[pos]:
                if not name.startswith("_L$"):
                    out.write(indent + "<" + name + ">\n")
        if item[1] == "motion":
            out.write(indent + hex(pos) + " motion table, " + str(len(item[2])) + " records\n")
            for record in item[2]:
                out.write(indent + "    " + motion_text(record) + "\n")
            continue
        if item[1] == "message":
            out.write(indent + hex(pos) + " message text, " + str(item[2]) + " bytes (not shown)\n")
            continue
        if item[1] == "inline":
            out.write(indent + hex(pos) + " inline data, " + str(item[2]) + " bytes (not shown)\n")
            continue
        op = item[2]
        operands = item[3]
        if op in BLOCK_NAMES:
            block_end = operands[1] | (operands[2] << 16)
            text = BLOCK_NAMES[op] + " until " + hex(block_end)
            if op in (0x16, 0x19):
                text = text + " name " + string_text(strings, operands[3])
            if op == 0x1A:
                text = text + " on " + string_text(strings, operands[3]) + " " + string_text(strings, operands[4])
            out.write(indent + hex(pos) + " " + text + "\n")
            closes.append(block_end)
            continue
        out.write(indent + hex(pos) + " op_" + format(op, "03x") + " " + operand_text(op, operands, strings, labels) + "\n")
    for error in errors:
        out.write("# error " + error + "\n")
    return errors, items


def main():
    source = os.path.join(REPO_DIR, "extracted", "nds_plain")
    out_dir = os.path.join(REPO_DIR, "extracted", "lmg")
    if len(sys.argv) > 1:
        source = sys.argv[1]
    if len(sys.argv) > 2:
        out_dir = sys.argv[2]
    paths = sorted(glob.glob(os.path.join(source, "**", "*.dec"), recursive=True))
    total = 0
    clean = 0
    message_files = 0
    counts = {"op": 0, "motion": 0, "message": 0, "inline": 0}
    failures = []
    for path in paths:
        f = open(path, "rb")
        data = f.read()
        f.close()
        if data[:4] != b"1LMG":
            continue
        total = total + 1
        relative = os.path.relpath(path, source)
        target = os.path.join(out_dir, relative[:-4] + ".txt")
        os.makedirs(os.path.dirname(target), exist_ok=True)
        out = open(target, "w")
        try:
            errors, items = write_tree(path, data, out)
        except (ValueError, struct.error) as error:
            errors = [str(error)]
            items = []
            out.write("# error " + str(error) + "\n")
        out.close()
        for item in items:
            counts[item[1]] = counts[item[1]] + 1
        if len(items) == 1 and items[0][1] == "message":
            message_files = message_files + 1
        if len(errors) == 0:
            clean = clean + 1
        else:
            failures.append(relative + ": " + errors[0])
    print("scripts: " + str(total) + ", decoded without errors: " + str(clean) + ", message-only files: " + str(message_files))
    print("instructions: " + str(counts["op"]) + ", motion tables: " + str(counts["motion"]) + ", message blocks: " + str(counts["message"]) + ", inline data blocks: " + str(counts["inline"]))
    for line in failures[:20]:
        print("  " + line)
    print("written to " + out_dir)
    if total == 0 or len(failures) > 0:
        sys.exit(1)


if __name__ == "__main__":
    main()
