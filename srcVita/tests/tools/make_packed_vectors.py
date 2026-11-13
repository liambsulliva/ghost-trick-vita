import os
import random
import sys
import zlib

sys.path.insert(0, os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))), "tools", "assets"))

import packed_file

VECTOR_COUNT = 160
SEED = 20261113


def plain_bytes(index, rng):
    kind = index % 5
    size = rng.randrange(0, 70000) if index % 7 else rng.randrange(0, 64)
    if kind == 0:
        return bytes(rng.randrange(256) for _ in range(size))
    if kind == 1:
        return bytes([rng.randrange(256)]) * size
    if kind == 2:
        words = [b"act", b"frame", b"table ", b"0x1f ", b"route\n", b"texture_name "]
        return b"".join(rng.choice(words) for _ in range(size // 4))
    if kind == 3:
        return bytes((i * 31 + index) & 0xff for i in range(size))
    return "".join(rng.choice("abcdefghij \n0123456789") for _ in range(size)).encode("ascii")


def main():
    folder = sys.argv[1]
    os.makedirs(folder, exist_ok=True)
    rng = random.Random(SEED)
    lines = []
    for index in range(VECTOR_COUNT):
        plain = plain_bytes(index, rng)
        path = os.path.join(folder, "vector_%03d.gpk" % index)
        with open(path, "wb") as output:
            output.write(packed_file.packed(plain))
        lines.append("%s %d %08x\n" % (path, len(plain), zlib.crc32(plain) & 0xffffffff))
    with open(os.path.join(folder, "manifest.txt"), "w", encoding="ascii") as manifest:
        manifest.writelines(lines)


main()
