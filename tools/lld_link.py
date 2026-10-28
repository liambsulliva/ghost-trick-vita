import sys
import os
import glob
import subprocess
import hashlib

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from elf_fixups import fix_object

REPO_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def brew_bin(formula, name):
    prefix = subprocess.run(["brew", "--prefix", formula], stdout=subprocess.PIPE, text=True).stdout.strip()
    return os.path.join(prefix, "bin", name)


def read_sections(delinks_path):
    sections = []
    f = open(delinks_path, "r")
    for line in f:
        if line.strip() == "":
            continue
        if not line.startswith(" ") and not line.startswith("\t"):
            break
        parts = line.split()
        name = parts[0]
        start = 0
        end = 0
        kind = ""
        for part in parts[1:]:
            if part.startswith("start:"):
                start = int(part[6:], 16)
            if part.startswith("end:"):
                end = int(part[4:], 16)
            if part.startswith("kind:"):
                kind = part[5:]
        sections.append((name, start, end, kind))
    f.close()
    return sections


def find_modules(config_dir, objects_dir):
    modules = []
    modules.append(("main", os.path.join(config_dir, "delinks.txt"), "arm9/arm9.bin"))
    modules.append(("itcm", os.path.join(config_dir, "itcm", "delinks.txt"), "arm9/itcm.bin"))
    modules.append(("dtcm", os.path.join(config_dir, "dtcm", "delinks.txt"), "arm9/dtcm.bin"))
    for folder in sorted(glob.glob(os.path.join(config_dir, "overlays", "ov*"))):
        name = os.path.basename(folder)
        modules.append((name, os.path.join(folder, "delinks.txt"), "arm9_overlays/" + name + ".bin"))
    result = []
    for module in modules:
        objects = glob.glob(os.path.join(objects_dir, "_dsd_gap@" + module[0] + "_*.o"))
        object_name = ""
        if len(objects) == 1:
            object_name = os.path.basename(objects[0])
        if len(objects) > 1:
            print("more than one object for " + module[0] + ", this tool only handles one")
            sys.exit(1)
        result.append((module[0], read_sections(module[1]), module[2], object_name))
    return result


def symbol_prefix(name):
    if name == "main":
        return "ARM9"
    return name.upper()


def write_script(modules, script_path, objects_dir):
    lines = []
    for module in modules:
        prefix = symbol_prefix(module[0])
        for section in module[1]:
            label = section[0][1:].upper()
            lines.append(prefix + "_" + label + "_START = " + hex(section[1]) + ";")
            lines.append(prefix + "_" + label + "_END = " + hex(section[2]) + ";")
            if module[0] == "main" and section[0] == ".exceptix":
                lines.append("__exception_table_start__ = " + hex(section[1]) + ";")
                lines.append("__exception_table_end__ = " + hex(section[2]) + ";")
    lines.append("SECTIONS {")
    for module in modules:
        name = module[0]
        object_name = module[3]
        for section in module[1]:
            section_name = section[0]
            start = section[1]
            output_name = "." + name + section_name
            noload = ""
            if section[3] == "bss":
                noload = " (NOLOAD)"
            content = ""
            if object_name != "":
                content = "\"" + os.path.join(objects_dir, object_name) + "\"(" + section_name + ")"
            lines.append("    " + output_name + " " + hex(start) + noload + " : { " + content + " }")
    lines.append("    /DISCARD/ : { \"" + os.path.join(objects_dir, "armv5te.o") + "\"(.text .data .bss) }")
    lines.append("}")
    f = open(script_path, "w")
    f.write("\n".join(lines) + "\n")
    f.close()


def sha1_of(path):
    f = open(path, "rb")
    data = f.read()
    f.close()
    return hashlib.sha1(data).hexdigest()


def main():
    config_dir = sys.argv[1]
    objects_dir = sys.argv[2]
    extract_dir = sys.argv[3]
    out_dir = sys.argv[4]
    os.makedirs(out_dir, exist_ok=True)

    lld = brew_bin("lld", "ld.lld")
    objcopy = brew_bin("llvm", "llvm-objcopy")

    modules = find_modules(config_dir, objects_dir)
    fixed_dir = os.path.abspath(os.path.join(out_dir, "objects"))
    os.makedirs(fixed_dir, exist_ok=True)
    script_path = os.path.join(out_dir, "link.ld")
    write_script(modules, script_path, fixed_dir)

    object_paths = []
    for module in modules:
        if module[3] != "":
            fixed_path = os.path.join(fixed_dir, module[3])
            subprocess.run([objcopy, os.path.join(objects_dir, module[3]), fixed_path], check=True)
            fix_object(fixed_path)
            object_paths.append(fixed_path)
    attributes_source = os.path.join(out_dir, "armv5te.s")
    f = open(attributes_source, "w")
    f.write(".cpu arm946e-s\n.eabi_attribute 6, 4\n")
    f.close()
    attributes_object = os.path.join(fixed_dir, "armv5te.o")
    subprocess.run([brew_bin("llvm", "llvm-mc"), "-triple=armv5te-none-eabi", "-filetype=obj", attributes_source, "-o", attributes_object], check=True)
    object_paths.append(attributes_object)
    elf_path = os.path.join(out_dir, "linked.elf")
    command = [lld, "--no-check-sections", "--orphan-handling=warn", "-static", "--entry=0x02000800", "-T", script_path, "-o", elf_path] + object_paths
    result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    if result.returncode != 0:
        print(result.stdout)
        print("link failed")
        sys.exit(1)
    for line in result.stdout.splitlines():
        if "is being placed" in line and "<internal>" in line:
            continue
        if "is being placed" in line and "(.ARM.attributes)" in line:
            continue
        print(line)
        if "is being placed" in line:
            print("a game section was not placed by the script")
            sys.exit(1)

    failed = 0
    for module in modules:
        name = module[0]
        sections = module[1]
        base = sections[0][1]
        bss_start = base
        for section in sections:
            if section[3] == "bss":
                bss_start = section[1]
        expected_size = bss_start - base
        bin_path = os.path.join(out_dir, name + ".bin")
        command = [objcopy, "-O", "binary"]
        has_content = False
        for section in sections:
            if section[3] != "bss" and section[2] > section[1]:
                command.append("--only-section=." + name + section[0])
                has_content = True
        data = b""
        if has_content:
            command = command + [elf_path, bin_path]
            subprocess.run(command, check=True)
            f = open(bin_path, "rb")
            data = f.read()
            f.close()
        if len(data) < expected_size:
            data = data + bytes(expected_size - len(data))
        f = open(bin_path, "wb")
        f.write(data)
        f.close()

        original_path = os.path.join(extract_dir, module[2])
        if sha1_of(bin_path) == sha1_of(original_path):
            print("PASS " + name.ljust(6) + " " + str(len(data)) + " bytes, sha1 " + sha1_of(bin_path)[:12])
        else:
            f = open(original_path, "rb")
            original = f.read()
            f.close()
            first = -1
            i = 0
            while i < len(original) and i < len(data):
                if original[i] != data[i]:
                    first = i
                    break
                i = i + 1
            print("FAIL " + name.ljust(6) + " size " + str(len(data)) + " vs " + str(len(original)) + ", first difference at " + hex(base + first))
            failed = failed + 1
    if failed > 0:
        sys.exit(1)
    print("all " + str(len(modules)) + " modules rebuilt byte-identical")


main()
