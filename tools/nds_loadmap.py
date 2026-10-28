import sys
import os

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from lz import read_u32


def save(path, data):
    folder = os.path.dirname(path)
    if folder != "":
        os.makedirs(folder, exist_ok=True)
    f = open(path, "wb")
    f.write(data)
    f.close()


def main():
    rom_path = sys.argv[1]
    nds_dir = sys.argv[2]
    f = open(rom_path, "rb")
    rom = f.read()
    f.close()
    f = open(os.path.join(nds_dir, "arm9_decompressed.bin"), "rb")
    arm9 = f.read()
    f.close()

    arm9_offset = read_u32(rom, 0x20)
    arm9_entry = read_u32(rom, 0x24)
    arm9_load = read_u32(rom, 0x28)
    arm9_size = read_u32(rom, 0x2C)
    params_offset = read_u32(rom, arm9_offset + arm9_size + 4)

    autoload_list_start = read_u32(arm9, params_offset)
    autoload_list_end = read_u32(arm9, params_offset + 4)
    autoload_start = read_u32(arm9, params_offset + 8)
    bss_start = read_u32(arm9, params_offset + 12)
    bss_end = read_u32(arm9, params_offset + 16)
    sdk_version = read_u32(arm9, params_offset + 24)
    magic_one = read_u32(arm9, params_offset + 28)
    magic_two = read_u32(arm9, params_offset + 32)
    if magic_one != 0xDEC00621 or magic_two != 0x2106C0DE:
        print("module params not found at " + hex(params_offset))
        sys.exit(1)

    lines = []
    lines.append("entry " + hex(arm9_entry))
    lines.append("sdk_version " + hex(sdk_version))

    main_size = autoload_start - arm9_load
    save(os.path.join(nds_dir, "segments", "main.bin"), arm9[0:main_size])
    lines.append("segment main " + hex(arm9_load) + " " + hex(main_size) + " segments/main.bin")
    lines.append("bss main_bss " + hex(bss_start) + " " + hex(bss_end - bss_start))

    list_offset = autoload_list_start - arm9_load
    list_end_offset = autoload_list_end - arm9_load
    data_offset = autoload_start - arm9_load
    number = 0
    while list_offset < list_end_offset:
        address = read_u32(arm9, list_offset)
        size = read_u32(arm9, list_offset + 4)
        bss_size = read_u32(arm9, list_offset + 8)
        name = "autoload" + str(number)
        if address >= 0x01FF8000 and address < 0x02000000:
            name = "itcm"
        if address >= 0x027C0000 and address < 0x02800000:
            name = "dtcm"
        file_name = "segments/" + name + ".bin"
        save(os.path.join(nds_dir, file_name), arm9[data_offset:data_offset + size])
        lines.append("segment " + name + " " + hex(address) + " " + hex(size) + " " + file_name)
        if bss_size > 0:
            lines.append("bss " + name + "_bss " + hex(address + size) + " " + hex(bss_size))
        data_offset = data_offset + size
        list_offset = list_offset + 12
        number = number + 1

    f = open(os.path.join(nds_dir, "overlays.txt"), "r")
    for line in f:
        parts = line.split()
        overlay_id = parts[0]
        address = int(parts[1].split("=")[1], 16)
        size = int(parts[2].split("=")[1], 16)
        bss_size = int(parts[3].split("=")[1], 16)
        file_name = "overlays/overlay9_" + overlay_id + "_decompressed.bin"
        if not os.path.exists(os.path.join(nds_dir, file_name)):
            file_name = "overlays/overlay9_" + overlay_id + ".bin"
        lines.append("overlay ov" + overlay_id + " " + hex(address) + " " + hex(size) + " " + file_name)
        if bss_size > 0:
            lines.append("overlay_bss ov" + overlay_id + "_bss " + hex(address + size) + " " + hex(bss_size))
    f.close()

    save(os.path.join(nds_dir, "loadmap.txt"), ("\n".join(lines) + "\n").encode())
    for line in lines:
        print(line)


main()
