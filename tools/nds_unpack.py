import sys
import os
import struct
import hashlib


def read_u16(data, offset):
    return struct.unpack_from("<H", data, offset)[0]


def read_u32(data, offset):
    return struct.unpack_from("<I", data, offset)[0]


def blz_decompress(data):
    info = read_u32(data, len(data) - 8)
    extra = read_u32(data, len(data) - 4)
    if extra == 0:
        return bytes(data)
    header_length = info >> 24
    compressed_length = info & 0xFFFFFF
    output = bytearray(data) + bytearray(extra)
    read_pos = len(data) - header_length
    write_pos = len(output)
    stop_pos = len(data) - compressed_length
    while read_pos > stop_pos:
        read_pos = read_pos - 1
        flags = data[read_pos]
        bit = 0
        while bit < 8:
            if read_pos <= stop_pos:
                break
            if flags & 0x80:
                read_pos = read_pos - 1
                high = data[read_pos]
                read_pos = read_pos - 1
                low = data[read_pos]
                value = (high << 8) | low
                length = (value >> 12) + 3
                distance = (value & 0xFFF) + 3
                count = 0
                while count < length:
                    write_pos = write_pos - 1
                    output[write_pos] = output[write_pos + distance]
                    count = count + 1
            else:
                read_pos = read_pos - 1
                write_pos = write_pos - 1
                output[write_pos] = data[read_pos]
            flags = (flags << 1) & 0xFF
            bit = bit + 1
    return bytes(output)


def read_directory(rom, fnt_offset, dir_id, path, names):
    entry_offset = fnt_offset + (dir_id & 0xFFF) * 8
    sub_offset = read_u32(rom, entry_offset)
    file_id = read_u16(rom, entry_offset + 4)
    pos = fnt_offset + sub_offset
    while True:
        kind = rom[pos]
        pos = pos + 1
        if kind == 0:
            break
        name_length = kind & 0x7F
        name = rom[pos:pos + name_length].decode("ascii", "replace")
        pos = pos + name_length
        if kind & 0x80:
            child_id = read_u16(rom, pos)
            pos = pos + 2
            read_directory(rom, fnt_offset, child_id, path + name + "/", names)
        else:
            names[file_id] = path + name
            file_id = file_id + 1


def save(path, data):
    folder = os.path.dirname(path)
    if folder != "":
        os.makedirs(folder, exist_ok=True)
    f = open(path, "wb")
    f.write(data)
    f.close()


def main():
    rom_path = sys.argv[1]
    out_dir = sys.argv[2]
    f = open(rom_path, "rb")
    rom = f.read()
    f.close()

    arm9_offset = read_u32(rom, 0x20)
    arm9_load = read_u32(rom, 0x28)
    arm9_size = read_u32(rom, 0x2C)
    arm7_offset = read_u32(rom, 0x30)
    arm7_size = read_u32(rom, 0x3C)
    fnt_offset = read_u32(rom, 0x40)
    fat_offset = read_u32(rom, 0x48)
    fat_size = read_u32(rom, 0x4C)
    ovt_offset = read_u32(rom, 0x50)
    ovt_size = read_u32(rom, 0x54)

    manifest = []

    arm9 = rom[arm9_offset:arm9_offset + arm9_size]
    save(os.path.join(out_dir, "arm9.bin"), arm9)
    manifest.append(("arm9.bin", arm9))

    footer = rom[arm9_offset + arm9_size:arm9_offset + arm9_size + 12]
    if len(footer) == 12 and read_u32(footer, 0) == 0xDEC00621:
        params_offset = read_u32(footer, 4)
        compressed_end = read_u32(arm9, params_offset + 0x14)
        if compressed_end != 0:
            split = compressed_end - arm9_load
            arm9_plain = arm9[:split]
            arm9_plain = blz_decompress(arm9_plain)
            save(os.path.join(out_dir, "arm9_decompressed.bin"), arm9_plain)
            manifest.append(("arm9_decompressed.bin", arm9_plain))

    arm7 = rom[arm7_offset:arm7_offset + arm7_size]
    save(os.path.join(out_dir, "arm7.bin"), arm7)
    manifest.append(("arm7.bin", arm7))

    file_count = fat_size // 8
    starts = []
    ends = []
    i = 0
    while i < file_count:
        starts.append(read_u32(rom, fat_offset + i * 8))
        ends.append(read_u32(rom, fat_offset + i * 8 + 4))
        i = i + 1

    names = {}
    read_directory(rom, fnt_offset, 0xF000, "", names)

    overlay_lines = []
    overlay_count = ovt_size // 32
    i = 0
    while i < overlay_count:
        base = ovt_offset + i * 32
        overlay_id = read_u32(rom, base)
        ram_address = read_u32(rom, base + 4)
        ram_size = read_u32(rom, base + 8)
        bss_size = read_u32(rom, base + 12)
        file_id = read_u32(rom, base + 24)
        flags = read_u32(rom, base + 28)
        data = rom[starts[file_id]:ends[file_id]]
        name = "overlays/overlay9_" + str(overlay_id) + ".bin"
        save(os.path.join(out_dir, name), data)
        manifest.append((name, data))
        if flags & 0x01000000:
            plain = blz_decompress(data)
            plain_name = "overlays/overlay9_" + str(overlay_id) + "_decompressed.bin"
            save(os.path.join(out_dir, plain_name), plain)
            manifest.append((plain_name, plain))
        overlay_lines.append(str(overlay_id) + " ram=" + hex(ram_address) + " size=" + hex(ram_size) + " bss=" + hex(bss_size) + " file=" + str(file_id) + " flags=" + hex(flags))
        i = i + 1

    save(os.path.join(out_dir, "overlays.txt"), ("\n".join(overlay_lines) + "\n").encode())

    i = 0
    while i < file_count:
        if i in names:
            data = rom[starts[i]:ends[i]]
            name = "files/" + names[i]
            save(os.path.join(out_dir, name), data)
            manifest.append((name, data))
        i = i + 1

    lines = []
    for item in manifest:
        lines.append(hashlib.sha1(item[1]).hexdigest() + "  " + str(len(item[1])) + "  " + item[0])
    save(os.path.join(out_dir, "manifest.txt"), ("\n".join(lines) + "\n").encode())
    print("wrote " + str(len(manifest)) + " entries to " + out_dir)


main()
