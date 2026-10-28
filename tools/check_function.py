import argparse
import hashlib
import json
import struct
from pathlib import Path


def region(data, offset, size):
    if offset < 0 or size < 0 or offset + size > len(data):
        raise ValueError("ELF region is outside the file")
    return data[offset:offset + size]


def read_name(data, offset):
    if offset < 0 or offset >= len(data):
        raise ValueError("ELF symbol name is outside its string table")
    end = data.find(b"\x00", offset)
    if end == -1:
        raise ValueError("unterminated ELF symbol name")
    return data[offset:end].decode("ascii")


def read_function(data, name):
    header = struct.unpack("<16sHHIIIIIHHHHHH", region(data, 0, 52))
    if header[0][:7] != b"\x7fELF\x01\x01\x01" or header[2] != 40:
        raise ValueError("expected a little-endian ELF32 ARM file")
    if header[1] != 2 or header[3] != 1:
        raise ValueError("expected a linked executable ELF, not a relocatable object")
    if header[11] != 40 or header[12] == 0:
        raise ValueError("invalid ELF section table")
    table = region(data, header[6], header[11] * header[12])
    sections = []
    for offset in range(0, len(table), 40):
        section = struct.unpack_from("<10I", table, offset)
        if section[1] not in (0, 8):
            region(data, section[4], section[5])
        if section[1] == 6:
            raise ValueError("dynamic linking is not supported")
        sections.append(section)
    functions = []
    for section in sections:
        if section[1] != 2:
            continue
        if section[9] != 16 or section[5] % 16 != 0 or section[6] >= len(sections):
            raise ValueError("invalid ELF symbol table")
        string_section = sections[section[6]]
        if string_section[1] != 3:
            raise ValueError("ELF symbols need a string table")
        strings = region(data, string_section[4], string_section[5])
        symbols = region(data, section[4], section[5])
        for offset in range(0, len(symbols), 16):
            name_offset, value, size, info, other, section_index = struct.unpack_from("<IIIBBH", symbols, offset)
            symbol_name = read_name(strings, name_offset)
            if symbol_name and section_index == 0 and info >> 4 != 0:
                raise ValueError("unresolved ELF symbol: " + symbol_name)
            if symbol_name != name or info & 15 != 2 or section_index == 0:
                continue
            if section_index >= len(sections) or size == 0:
                raise ValueError("function needs a nonempty section-backed body")
            body_section = sections[section_index]
            if body_section[1] != 1 or body_section[2] & 6 != 6:
                raise ValueError("function is outside allocated executable data")
            address = value & ~1
            thumb = bool(value & 1)
            if not thumb and address % 4 != 0:
                raise ValueError("unaligned ARM function")
            start = address - body_section[3]
            if start < 0 or start + size > body_section[5]:
                raise ValueError("function body extends past its section")
            body = region(data, body_section[4] + start, size)
            functions.append({"address": address, "size": size, "thumb": thumb, "body": body})
    if len(functions) != 1:
        raise ValueError("expected exactly one defined function named " + name)
    return functions[0]


def compare_functions(reference, candidate):
    problems = []
    for field in ["address", "size", "thumb"]:
        if reference[field] != candidate[field]:
            problems.append(field + " differs")
    different = 0
    first = None
    for offset in range(max(len(reference["body"]), len(candidate["body"]))):
        if reference["body"][offset:offset + 1] != candidate["body"][offset:offset + 1]:
            different += 1
            if first is None:
                first = reference["address"] + offset
    if different:
        problems.append("linked function bytes differ")
    return {"status": "pass" if not problems else "fail", "problems": problems,
            "different_bytes": different, "first_difference": hex(first) if first is not None else None}


def main():
    parser = argparse.ArgumentParser(description="Compare every byte of a function in two fully linked ARM ELFs at the same address.")
    parser.add_argument("reference", type=Path)
    parser.add_argument("candidate", type=Path)
    parser.add_argument("function")
    parser.add_argument("--candidate-name")
    parser.add_argument("--report", type=Path)
    args = parser.parse_args()
    report = {"status": "error", "reference": str(args.reference), "candidate": str(args.candidate), "function": args.function}
    try:
        reference_data = args.reference.read_bytes()
        candidate_data = args.candidate.read_bytes()
        reference = read_function(reference_data, args.function)
        candidate = read_function(candidate_data, args.candidate_name or args.function)
        report.update(compare_functions(reference, candidate))
        report["reference_sha256"] = hashlib.sha256(reference_data).hexdigest()
        report["candidate_sha256"] = hashlib.sha256(candidate_data).hexdigest()
        report["address"] = hex(reference["address"])
        report["size"] = reference["size"]
        report["mode"] = "thumb" if reference["thumb"] else "arm"
    except (OSError, ValueError, struct.error) as error:
        report["problems"] = [str(error)]
    if args.report is not None:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(json.dumps(report, indent=2) + "\n")
    print(report["status"].upper() + " " + args.function)
    for problem in report.get("problems", []):
        print(problem)
    if report["status"] != "pass":
        raise SystemExit(1)


if __name__ == "__main__":
    main()
