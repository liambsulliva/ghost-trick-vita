import sys
import os
import struct
import zlib
import subprocess
import re


def rotate_left(value, amount):
    return ((value << amount) | (value >> (32 - amount))) & 0xFFFFFFFF


def murmur3(data):
    h = 0xFFFFFFFF
    length = len(data)
    block_count = length // 4
    i = 0
    while i < block_count:
        k = data[i * 4] | (data[i * 4 + 1] << 8) | (data[i * 4 + 2] << 16) | (data[i * 4 + 3] << 24)
        k = (k * 0xCC9E2D51) & 0xFFFFFFFF
        k = rotate_left(k, 15)
        k = (k * 0x1B873593) & 0xFFFFFFFF
        h = h ^ k
        h = rotate_left(h, 13)
        h = (h * 5 + 0xE6546B64) & 0xFFFFFFFF
        i = i + 1
    tail_start = block_count * 4
    left = length - tail_start
    k = 0
    if left == 3:
        k = k ^ (data[tail_start + 2] << 16)
    if left >= 2:
        k = k ^ (data[tail_start + 1] << 8)
    if left >= 1:
        k = k ^ data[tail_start]
        k = (k * 0xCC9E2D51) & 0xFFFFFFFF
        k = rotate_left(k, 15)
        k = (k * 0x1B873593) & 0xFFFFFFFF
        h = h ^ k
    h = h ^ length
    h = h ^ (h >> 16)
    h = (h * 0x85EBCA6B) & 0xFFFFFFFF
    h = h ^ (h >> 13)
    h = (h * 0xC2B2AE35) & 0xFFFFFFFF
    h = h ^ (h >> 16)
    return h


def path_hashes(path):
    lower = murmur3(path.lower().encode("utf-16-le"))
    upper = murmur3(path.upper().encode("utf-16-le"))
    return lower, upper


def tag_name(data):
    text = ""
    for b in data[:4]:
        if b >= 0x21 and b < 0x7F:
            text = text + chr(b)
    if text == "":
        text = "?"
    return text


def load_versions(path):
    prefix = ""
    versions = {}
    f = open(path, "r")
    for line in f:
        parts = line.strip().split(" ")
        if len(parts) != 2:
            continue
        if parts[0] == "prefix":
            prefix = parts[1]
        else:
            versions[parts[0]] = parts[1]
    f.close()
    return prefix, versions


def read_entry(f, offset, stored_size, attributes):
    f.seek(offset)
    raw = f.read(stored_size)
    method = attributes & 0xF
    if method == 1:
        try:
            return zlib.decompress(raw, -15)
        except zlib.error:
            return b""
    if method == 2:
        result = subprocess.run(["zstd", "-d", "-c", "-q"], input=raw, stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)
        return result.stdout
    return raw


def main():
    pak_path = sys.argv[1]
    versions_path = sys.argv[2]
    out_path = sys.argv[3]
    prefix, versions = load_versions(versions_path)

    f = open(pak_path, "rb")
    header = f.read(16)
    count = struct.unpack_from("<I", header, 8)[0]
    table = f.read(count * 48)

    entries = {}
    tags = {}
    references = set()
    string_pattern = re.compile(rb"(?:[\x20-\x7e]\x00){6,200}")
    i = 0
    while i < count:
        base = i * 48
        first = struct.unpack_from("<I", table, base)[0]
        second = struct.unpack_from("<I", table, base + 4)[0]
        offset = struct.unpack_from("<Q", table, base + 8)[0]
        stored_size = struct.unpack_from("<Q", table, base + 16)[0]
        attributes = struct.unpack_from("<Q", table, base + 32)[0]
        entries[(first, second)] = i
        data = read_entry(f, offset, stored_size, attributes)
        tags[i] = data[:4]
        if data[:4] in (b"USR\x00", b"PFB\x00", b"SCN\x00", b"SDF\x00", b"MDF\x00", b"RSZ\x00"):
            for match in string_pattern.finditer(data):
                text = match.group(0).decode("utf-16-le")
                if "/" in text and re.search(r"\.[A-Za-z0-9]{2,8}$", text):
                    references.add(text)
        i = i + 1
    f.close()

    names = {}
    for reference in references:
        extension = reference.split(".")[-1].lower()
        if extension not in versions:
            continue
        full = prefix + reference + "." + versions[extension]
        lower, upper = path_hashes(full)
        if (lower, upper) in entries:
            names[entries[(lower, upper)]] = full
        elif (upper, lower) in entries:
            names[entries[(upper, lower)]] = full
    print("named from in-game references: " + str(len(names)))

    if len(sys.argv) > 4:
        list_file = open(sys.argv[4], "r")
        for line in list_file:
            full = line.strip()
            if full == "":
                continue
            lower, upper = path_hashes(full)
            if (lower, upper) in entries:
                names[entries[(lower, upper)]] = full
            elif (upper, lower) in entries:
                names[entries[(upper, lower)]] = full
        list_file.close()
        print("named after adding the file list: " + str(len(names)))

    out = open(out_path, "w")
    out.write("index\ttag\tpath\n")
    i = 0
    while i < count:
        tag = tag_name(tags[i])
        path = ""
        if i in names:
            path = names[i]
        out.write(str(i) + "\t" + tag + "\t" + path + "\n")
        i = i + 1
    out.close()

    resolved = {}
    total = {}
    i = 0
    while i < count:
        tag = tag_name(tags[i])
        total[tag] = total.get(tag, 0) + 1
        if i in names:
            resolved[tag] = resolved.get(tag, 0) + 1
        i = i + 1
    print("resolved " + str(len(names)) + " of " + str(count) + " entries")
    for tag in sorted(total, key=lambda t: -total[t]):
        print("  " + tag.ljust(6) + " " + str(resolved.get(tag, 0)) + " / " + str(total[tag]))


main()
