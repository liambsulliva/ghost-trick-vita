import sys
import os

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from lz import read_u32, try_decompress, magic_text


def find_blocks(data):
    blocks = []
    pos = 0
    while True:
        pos = data.find(b"YEK", pos)
        if pos == -1:
            break
        if pos >= 8 and data[pos + 8:pos + 11] == b"TAD":
            base = pos - 8
            key_offset = read_u32(data, pos + 4)
            data_offset = read_u32(data, pos + 12)
            if key_offset < data_offset:
                kind = chr(data[pos + 3])
                blocks.append((base, kind, key_offset, data_offset))
        pos = pos + 4
    return blocks


def main():
    pack_path = sys.argv[1]
    out_dir = sys.argv[2]
    f = open(pack_path, "rb")
    data = f.read()
    f.close()
    pack_name = os.path.basename(pack_path).replace(".bin", "")

    rows = []
    blocks = find_blocks(data)
    block_number = 0
    for block in blocks:
        base = block[0]
        kind = block[1]
        key_start = base + block[2]
        data_start = base + block[3]
        entry_count = (block[3] - block[2]) // 8
        next_block = len(data)
        if block_number + 1 < len(blocks):
            next_block = blocks[block_number + 1][0]
        i = 0
        while i < entry_count:
            entry_offset = read_u32(data, key_start + i * 8)
            entry_value = read_u32(data, key_start + i * 8 + 4)
            entry_size = entry_value & 0x7FFFFFFF
            flagged = (entry_value & 0x80000000) != 0
            start = data_start + entry_offset
            end = start + entry_size
            if entry_size > 0 and end <= next_block:
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
                rows.append(name + "\t" + hex(base) + "\t" + str(i) + "\t" + str(entry_size) + "\t" + status + "\t" + str(len(plain)) + "\t" + magic_text(plain))
            i = i + 1
        block_number = block_number + 1

    f = open(os.path.join(out_dir, pack_name + "_entries.tsv"), "w")
    f.write("name\tblock_base\tindex\tstored_size\tstatus\tplain_size\tmagic\n")
    for row in rows:
        f.write(row + "\n")
    f.close()
    print(pack_name + ": " + str(len(blocks)) + " blocks, " + str(len(rows)) + " entries")


main()
