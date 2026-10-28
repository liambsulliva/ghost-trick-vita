import sys
import os
import struct
import zlib
import subprocess


def read_entry_head(f, offset, stored_size, attributes):
    f.seek(offset)
    raw = f.read(stored_size)
    method = attributes & 0xF
    if method == 0:
        return raw
    if method == 1:
        try:
            return zlib.decompress(raw, -15)
        except zlib.error:
            return b""
    if method == 2:
        result = subprocess.run(["zstd", "-d", "-c", "-q"], input=raw, stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)
        return result.stdout
    return b""


def tag_of(data):
    head = data[:4]
    text = ""
    for b in head:
        if b >= 0x20 and b < 0x7F:
            text = text + chr(b)
        else:
            text = text + "."
    return text


def main():
    pak_path = sys.argv[1]
    out_path = sys.argv[2]
    f = open(pak_path, "rb")
    header = f.read(16)
    magic = header[0:4]
    major = header[4]
    flags = struct.unpack_from("<H", header, 6)[0]
    count = struct.unpack_from("<I", header, 8)[0]
    print("magic " + str(magic) + " version " + str(major) + " flags " + hex(flags) + " entries " + str(count))
    table = f.read(count * 48)
    rows = []
    i = 0
    while i < count:
        base = i * 48
        hash_low = struct.unpack_from("<I", table, base)[0]
        hash_high = struct.unpack_from("<I", table, base + 4)[0]
        offset = struct.unpack_from("<Q", table, base + 8)[0]
        stored_size = struct.unpack_from("<Q", table, base + 16)[0]
        plain_size = struct.unpack_from("<Q", table, base + 24)[0]
        attributes = struct.unpack_from("<Q", table, base + 32)[0]
        data = read_entry_head(f, offset, stored_size, attributes)
        tag = tag_of(data)
        bone_words = 0
        if tag == "MESH":
            lower = data.lower()
            for word in [b"root", b"hip", b"spine", b"neck", b"head", b"_arm", b"leg", b"clavicle", b"shoulder", b"pelvis", b"joint"]:
                if word in lower:
                    bone_words = bone_words + 1
        rows.append(hex(hash_high) + hex(hash_low)[2:].zfill(8) + "\t" + str(plain_size) + "\t" + str(attributes & 0xF) + "\t" + tag + "\t" + str(bone_words))
        i = i + 1
    f.close()
    out = open(out_path, "w")
    out.write("hash\tplain_size\tmethod\ttag\tbone_words\n")
    for row in rows:
        out.write(row + "\n")
    out.close()
    print("wrote " + str(len(rows)) + " rows")


main()
