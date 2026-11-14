import os
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import bake_record

PACK_FILE_NAME = "scripts.pack"
MAGIC = b"GTSP"
VERSION = 1
LEFT_OUT = ("tiles.txt",)
CHAIN_FILE_NAME = "chain.txt"


def packed_names(scripts_folder):
    names = []
    for name in sorted(os.listdir(scripts_folder)):
        if name == PACK_FILE_NAME or name in LEFT_OUT or name.endswith(bake_record.TEMPORARY_SUFFIX) or name.startswith("."):
            continue
        if os.path.isfile(os.path.join(scripts_folder, name)):
            names.append(name)
    return names


def pack_bytes(scripts_folder):
    names = packed_names(scripts_folder)
    contents = []
    for name in names:
        with open(os.path.join(scripts_folder, name), "rb") as source:
            contents.append(source.read())
    index_size = sum(2 + len(name.encode("ascii")) + 8 for name in names)
    offset = len(MAGIC) + 8 + index_size
    index = b""
    for name, data in zip(names, contents):
        encoded = name.encode("ascii")
        index = index + struct.pack("<H", len(encoded)) + encoded + struct.pack("<II", offset, len(data))
        offset = offset + len(data)
    return MAGIC + struct.pack("<II", VERSION, len(names)) + index + b"".join(contents)


def write_pack(scripts_folder):
    if not os.path.isdir(scripts_folder):
        return False
    if not os.path.isfile(os.path.join(scripts_folder, CHAIN_FILE_NAME)):
        if os.path.exists(os.path.join(scripts_folder, PACK_FILE_NAME)):
            os.remove(os.path.join(scripts_folder, PACK_FILE_NAME))
        return False
    return bake_record.write_if_different(os.path.join(scripts_folder, PACK_FILE_NAME), pack_bytes(scripts_folder))


def main():
    if len(sys.argv) != 2:
        print("usage: script_pack.py <baked scripts folder>")
        return 1
    changed = write_pack(sys.argv[1])
    print("script pack %s" % ("written" if changed else "unchanged"))
    return 0


if __name__ == "__main__":
    sys.exit(main())
