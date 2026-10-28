import sys
import os
import json
import shutil
import subprocess

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import check_function
import elf_fixups

REPO_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
REFERENCE_ELF = os.path.join(REPO_DIR, "extracted", "lld", "linked.elf")
WINE = "/Applications/CrossOver.app/Contents/SharedSupport/CrossOver/bin/wine"
BOTTLE = os.path.join(REPO_DIR, "toolchain", "wine-mwcc-run")
COMPILER_VERSION = "dsi/1.2p1"
FLAGS = ["-proc", "arm946e", "-thumb", "-O4,s", "-fp", "soft", "-interworking", "-sym", "on"]


def brew_bin(formula, name):
    prefix = subprocess.run(["brew", "--prefix", formula], stdout=subprocess.PIPE, text=True).stdout.strip()
    return os.path.join(prefix, "bin", name)


def read_symbols(path, defined):
    f = open(path, "rb")
    data = f.read()
    f.close()
    sections = elf_fixups.read_sections(data)
    symbols = {}
    for section in sections:
        if section[0] != 2:
            continue
        strtab_offset = sections[section[3]][1]
        count = section[2] // 16
        i = 0
        while i < count:
            base = section[1] + i * 16
            name = elf_fixups.read_name(data, strtab_offset + elf_fixups.read_u32(data, base))
            value = elf_fixups.read_u32(data, base + 4)
            binding = data[base + 12] >> 4
            section_index = elf_fixups.read_u16(data, base + 14)
            is_defined = section_index != 0
            if name != "" and binding != 0 and is_defined == defined:
                symbols[name] = value
            i = i + 1
    return symbols


def disassembly(path, start, end):
    objdump = brew_bin("llvm", "llvm-objdump")
    command = [objdump, "-d", "--start-address=" + hex(start), "--stop-address=" + hex(end), path]
    result = subprocess.run(command, stdout=subprocess.PIPE, text=True)
    lines = []
    for line in result.stdout.splitlines():
        parts = line.split("\t")
        if len(parts) >= 2 and ":" in parts[0] and not parts[0].strip().endswith(">:"):
            lines.append(" ".join(parts[1:]).split("@")[0].strip())
    return lines


def main():
    source = os.path.abspath(sys.argv[1])
    reference_name = sys.argv[2]
    candidate_name = sys.argv[3]
    compiler_version = COMPILER_VERSION
    flags = list(FLAGS)
    i = 4
    while i < len(sys.argv):
        if sys.argv[i] == "--compiler" and i + 1 < len(sys.argv):
            compiler_version = sys.argv[i + 1]
            i = i + 1
        elif sys.argv[i] == "--no-interworking":
            flags.remove("-interworking")
        i = i + 1
    work_dir = os.path.join(REPO_DIR, "extracted", "decomp", reference_name, compiler_version.replace("/", "_"))
    os.makedirs(work_dir, exist_ok=True)
    source_copy = os.path.join(work_dir, os.path.basename(source))
    shutil.copyfile(source, source_copy)
    object_name = candidate_name + ".o"
    object_path = os.path.join(work_dir, object_name)
    linked_path = os.path.join(work_dir, candidate_name + ".elf")
    if os.path.exists(object_path):
        os.remove(object_path)

    f = open(REFERENCE_ELF, "rb")
    reference = check_function.read_function(f.read(), reference_name)
    f.close()
    address = reference["address"]

    env = dict(os.environ)
    env["WINEDEBUG"] = "-all"
    env["LM_LICENSE_FILE"] = "Z:" + os.path.join(REPO_DIR, "toolchain", "mwccarm", "license.dat").replace("/", "\\")
    compiler = os.path.join(REPO_DIR, "toolchain", "mwccarm", compiler_version, "mwccarm.exe")
    command = [WINE, "--bottle", BOTTLE, "--no-gui", "--workdir", work_dir, compiler, "-c", os.path.basename(source), "-o", object_name] + flags
    result = subprocess.run(command, env=env, capture_output=True, text=True, timeout=120)
    if result.returncode != 0 or not os.path.exists(object_path):
        print(result.stdout + result.stderr)
        print("compile failed")
        sys.exit(1)
    elf_fixups.fix_object(object_path)

    needed = read_symbols(object_path, False)
    known = read_symbols(REFERENCE_ELF, True)
    linker = [brew_bin("lld", "ld.lld"), "--no-undefined", "--entry=" + candidate_name, "-Ttext=" + hex(address), object_path, "-o", linked_path]
    for name in sorted(needed):
        if name not in known:
            print("no value for external symbol " + name)
            sys.exit(1)
        linker.append("--defsym=" + name + "=" + hex(known[name]))
    result = subprocess.run(linker, capture_output=True, text=True)
    if result.returncode != 0:
        print(result.stdout + result.stderr)
        print("link failed")
        sys.exit(1)

    f = open(linked_path, "rb")
    candidate = check_function.read_function(f.read(), candidate_name)
    f.close()
    report = check_function.compare_functions(reference, candidate)
    report["reference_function"] = reference_name
    report["candidate_function"] = candidate_name
    report["source"] = os.path.relpath(source, REPO_DIR)
    report["compiler"] = compiler_version
    report["flags"] = flags
    report_path = os.path.join(work_dir, candidate_name + "_check.json")
    f = open(report_path, "w")
    f.write(json.dumps(report, indent=2) + "\n")
    f.close()

    if report["status"] == "pass":
        print("PASS " + reference_name + " (" + str(reference["size"]) + " bytes), report " + os.path.relpath(report_path, REPO_DIR))
        return
    print("FAIL " + reference_name + ": " + str(report.get("different_bytes")) + " bytes differ, sizes " + str(reference["size"]) + " vs " + str(candidate["size"]))
    left = disassembly(REFERENCE_ELF, address, address + reference["size"])
    right = disassembly(linked_path, address, address + candidate["size"])
    i = 0
    while i < len(left) or i < len(right):
        a = ""
        b = ""
        if i < len(left):
            a = left[i]
        if i < len(right):
            b = right[i]
        mark = "  "
        if a != b:
            mark = "!="
        print(mark + " " + a.ljust(44) + " | " + b)
        i = i + 1
    sys.exit(1)


if __name__ == "__main__":
    main()
