import sys
import struct

R_ARM_PC24 = 1
R_ARM_CALL = 28
R_ARM_JUMP24 = 29


def read_u16(data, offset):
    return struct.unpack_from("<H", data, offset)[0]


def read_u32(data, offset):
    return struct.unpack_from("<I", data, offset)[0]


def read_name(data, offset):
    end = data.index(b"\x00", offset)
    return data[offset:end].decode("latin-1")


def read_sections(data):
    section_offset = read_u32(data, 0x20)
    section_size = read_u16(data, 0x2E)
    section_count = read_u16(data, 0x30)
    sections = []
    i = 0
    while i < section_count:
        base = section_offset + i * section_size
        kind = read_u32(data, base + 4)
        offset = read_u32(data, base + 16)
        size = read_u32(data, base + 20)
        link = read_u32(data, base + 24)
        info = read_u32(data, base + 28)
        sections.append((kind, offset, size, link, info))
        i = i + 1
    return sections


def mark_thumb_functions(data, sections):
    symtab = None
    for section in sections:
        if section[0] == 2:
            symtab = section
    if symtab is None:
        return 0
    strtab_offset = sections[symtab[3]][1]
    symbol_count = symtab[2] // 16

    thumb_starts = set()
    i = 0
    while i < symbol_count:
        base = symtab[1] + i * 16
        name = read_name(data, strtab_offset + read_u32(data, base))
        value = read_u32(data, base + 4)
        section_index = read_u16(data, base + 14)
        if name == "$t" or name.startswith("$t."):
            thumb_starts.add((section_index, value))
        i = i + 1

    marked = 0
    i = 0
    while i < symbol_count:
        base = symtab[1] + i * 16
        value = read_u32(data, base + 4)
        symbol_type = data[base + 12] & 0xF
        section_index = read_u16(data, base + 14)
        if symbol_type == 2 and (value & 1) == 0 and (section_index, value) in thumb_starts:
            struct.pack_into("<I", data, base + 4, value | 1)
            marked = marked + 1
        i = i + 1
    return marked


def retype_arm_branches(data, sections):
    changed = 0
    for section in sections:
        if section[0] != 4:
            continue
        target_offset = sections[section[4]][1]
        entry_count = section[2] // 12
        i = 0
        while i < entry_count:
            base = section[1] + i * 12
            where = read_u32(data, base)
            info = read_u32(data, base + 4)
            if (info & 0xFF) == R_ARM_PC24:
                top = data[target_offset + where + 3]
                new_type = R_ARM_JUMP24
                if top == 0xEB or (top & 0xFE) == 0xFA:
                    new_type = R_ARM_CALL
                struct.pack_into("<I", data, base + 4, (info & 0xFFFFFF00) | new_type)
                changed = changed + 1
            i = i + 1
    return changed


def relax_thumb_alignment(data, sections):
    symtab = None
    for section in sections:
        if section[0] == 2:
            symtab = section
    if symtab is None:
        return 0
    strtab_offset = sections[symtab[3]][1]
    thumb_sections = set()
    i = 0
    while i < symtab[2] // 16:
        base = symtab[1] + i * 16
        name = read_name(data, strtab_offset + read_u32(data, base))
        if name == "$t" or name.startswith("$t."):
            thumb_sections.add(read_u16(data, base + 14))
        i = i + 1
    section_offset = read_u32(data, 0x20)
    section_size = read_u16(data, 0x2E)
    relaxed = 0
    for index in thumb_sections:
        base = section_offset + index * section_size
        if read_u32(data, base + 4) == 1 and read_u32(data, base + 8) & 4 and read_u32(data, base + 32) == 4:
            struct.pack_into("<I", data, base + 32, 2)
            relaxed = relaxed + 1
    return relaxed


def relax_empty_text(path):
    f = open(path, "rb")
    data = bytearray(f.read())
    f.close()
    section_offset = read_u32(data, 0x20)
    section_size = read_u16(data, 0x2E)
    sections = read_sections(data)
    relaxed = 0
    i = 0
    while i < len(sections):
        base = section_offset + i * section_size
        if sections[i][0] == 1 and sections[i][2] == 0 and read_u32(data, base + 8) & 4 and read_u32(data, base + 32) == 4:
            struct.pack_into("<I", data, base + 32, 2)
            relaxed = relaxed + 1
        i = i + 1
    if relaxed:
        f = open(path, "wb")
        f.write(data)
        f.close()
    return relaxed


def fix_object(path):
    f = open(path, "rb")
    data = bytearray(f.read())
    f.close()
    sections = read_sections(data)
    marked = mark_thumb_functions(data, sections)
    relax_thumb_alignment(data, sections)
    retyped = retype_arm_branches(data, sections)
    f = open(path, "wb")
    f.write(data)
    f.close()
    return marked, retyped


def main():
    for path in sys.argv[1:]:
        marked, retyped = fix_object(path)
        print(path + ": marked " + str(marked) + " thumb functions, retyped " + str(retyped) + " branch relocations")


if __name__ == "__main__":
    main()
