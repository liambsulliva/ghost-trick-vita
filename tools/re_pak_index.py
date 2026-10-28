import argparse
import csv
import os
import re
import struct
import sys
import zipfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from re_names import path_hashes


def read_index(source, size):
    header = source.read(16)
    if len(header) != 16 or header[:4] != b"KPKA":
        raise ValueError("missing or truncated KPKA header")
    if header[4] != 4 or struct.unpack_from("<H", header, 6)[0] != 0:
        raise ValueError("unsupported KPKA version or flags")
    count = struct.unpack_from("<I", header, 8)[0]
    if 16 + count * 48 > size:
        raise ValueError("entry table extends past the pack")
    table = source.read(count * 48)
    if len(table) != count * 48:
        raise ValueError("truncated KPKA entry table")
    entries = {}
    for index in range(count):
        first, second, offset, stored, plain, attributes = struct.unpack_from("<IIQQQQ", table, index * 48)
        if offset < 16 + count * 48 or offset + stored > size:
            raise ValueError("entry " + str(index) + " extends outside the data area")
        key = (first, second)
        if key not in entries:
            entries[key] = []
        entries[key].append((index, offset, stored, plain, attributes & 15))
    return entries, count


def match_paths(queries, entries, label, matches):
    for path, first, second in queries:
        found = list(entries.get((first, second), []))
        if first != second:
            found.extend(entries.get((second, first), []))
        for entry in found:
            matches[path].append((label,) + entry)


def scan_input(path, queries, matches):
    packs = 0
    entries_count = 0
    if zipfile.is_zipfile(path):
        with zipfile.ZipFile(path) as archive:
            for info in sorted(archive.infolist(), key=lambda entry: entry.filename):
                if not info.filename.lower().endswith(".pak"):
                    continue
                with archive.open(info) as source:
                    entries, count = read_index(source, info.file_size)
                label = path + "::" + info.filename
                match_paths(queries, entries, label, matches)
                packs = packs + 1
                entries_count = entries_count + count
    else:
        with open(path, "rb") as source:
            entries, entries_count = read_index(source, os.path.getsize(path))
        match_paths(queries, entries, path, matches)
        packs = 1
    if packs == 0:
        raise ValueError(path + ": no complete .pak files found")
    return packs, entries_count


def main():
    parser = argparse.ArgumentParser(description="Find named resources in local RE Engine pack indexes without extracting their contents.")
    parser.add_argument("file_list")
    parser.add_argument("output")
    parser.add_argument("inputs", nargs="+")
    parser.add_argument("--filter", default=r"charfiles/2d/.*\.user\.")
    args = parser.parse_args()
    pattern = re.compile(args.filter)
    with open(args.file_list, "r") as source:
        paths = sorted(set(line.strip().replace("\\", "/") for line in source if pattern.search(line.strip())))
    if len(paths) == 0:
        raise ValueError("no resource paths match the filter")
    queries = []
    matches = {}
    for path in paths:
        first, second = path_hashes(path)
        queries.append((path, first, second))
        matches[path] = []
    packs = 0
    entries_count = 0
    for path in args.inputs:
        input_packs, input_entries = scan_input(path, queries, matches)
        packs = packs + input_packs
        entries_count = entries_count + input_entries
    parent = os.path.dirname(os.path.abspath(args.output))
    os.makedirs(parent, exist_ok=True)
    with open(args.output, "w", newline="") as out:
        writer = csv.writer(out, delimiter="\t")
        writer.writerow(("path", "status", "source", "entry", "offset", "stored_size", "plain_size", "method"))
        for path in paths:
            if len(matches[path]) == 0:
                writer.writerow((path, "missing", "", "", "", "", "", ""))
            for match in matches[path]:
                writer.writerow((path, "found") + match)
    found = sum(len(matches[path]) > 0 for path in paths)
    print("packs: " + str(packs) + ", entries: " + str(entries_count))
    print("queried resources: " + str(len(paths)) + ", found: " + str(found) + ", missing: " + str(len(paths) - found))
    print("written to " + args.output)


if __name__ == "__main__":
    main()
