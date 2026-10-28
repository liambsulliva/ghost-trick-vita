import sys
import os
import csv

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from lz import read_u32, try_decompress, magic_text


def find_blocks(data):
    blocks = []
    pos = 0
    while True:
        pos = data.find(b"YEK", pos)
        if pos == -1:
            break
        if pos >= 8 and pos + 16 <= len(data) and data[pos + 8:pos + 11] == b"TAD":
            base = pos - 8
            key_offset = read_u32(data, pos + 4)
            data_offset = read_u32(data, pos + 12)
            if key_offset < data_offset:
                kind = chr(data[pos + 3])
                blocks.append((base, kind, key_offset, data_offset))
        pos = pos + 4
    return blocks


def read_entries(data, block, limit):
    base, kind, key_offset, data_offset = block
    key_size = 8
    if kind == "P":
        key_size = 4
    elif kind != "B":
        raise ValueError("unknown CPAC key kind " + kind)
    key_start = base + key_offset
    data_start = base + data_offset
    if key_start < base + 24 or data_start > limit or (data_offset - key_offset) % key_size != 0:
        raise ValueError("invalid CPAC key table bounds")
    entries = []
    count = (data_offset - key_offset) // key_size
    for index in range(count):
        value = read_u32(data, key_start + index * key_size)
        if key_size == 4:
            offset = (value >> 16) * 32
            size = ((value >> 8) & 0xFF) * 32
            compressed = False
            localized = (value & 0x80) != 0
        else:
            offset = value & 0x7FFFFFFF
            size_value = read_u32(data, key_start + index * key_size + 4)
            size = size_value & 0x7FFFFFFF
            compressed = (size_value & 0x80000000) != 0
            localized = (value & 0x80000000) != 0
        if size == 0:
            continue
        start = data_start + offset
        if start + size > limit:
            raise ValueError("CPAC entry " + str(index) + " extends past its block")
        entries.append((index, start, size, compressed, localized, key_size))
    return entries


def main():
    pack_path = sys.argv[1]
    out_dir = sys.argv[2]
    f = open(pack_path, "rb")
    data = f.read()
    f.close()
    pack_name = os.path.basename(pack_path).replace(".bin", "")

    rows = []
    blocks = find_blocks(data)
    if len(blocks) == 0:
        raise ValueError("no CPAC blocks found")
    os.makedirs(out_dir, exist_ok=True)
    block_number = 0
    for block in blocks:
        base = block[0]
        kind = block[1]
        next_block = len(data)
        if block_number + 1 < len(blocks):
            next_block = blocks[block_number + 1][0]
        for i, start, entry_size, flagged, localized, key_size in read_entries(data, block, next_block):
            end = start + entry_size
            raw = data[start:end]
            plain = None
            if flagged:
                plain = try_decompress(raw)
            if plain is None:
                plain = raw
                status = "raw"
            else:
                status = "lz"
            name = pack_name + "_b" + str(block_number) + kind + "_" + str(i).zfill(5) + ".bin"
            f = open(os.path.join(out_dir, name), "wb")
            f.write(plain)
            f.close()
            rows.append((name, hex(base), i, entry_size, status, len(plain), magic_text(plain), key_size, str(localized).lower()))
        block_number = block_number + 1

    f = open(os.path.join(out_dir, pack_name + "_entries.tsv"), "w", newline="")
    writer = csv.writer(f, delimiter="\t", lineterminator="\n")
    writer.writerow(("name", "block_base", "index", "stored_size", "status", "plain_size", "magic", "key_size", "localized"))
    writer.writerows(rows)
    f.close()
    print(pack_name + ": " + str(len(blocks)) + " blocks, " + str(len(rows)) + " entries")


if __name__ == "__main__":
    main()
