import sys
import os

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from lz import try_decompress, magic_text


def main():
    root = sys.argv[1]
    out_dir = sys.argv[2]
    rows = []
    for folder, subfolders, files in os.walk(root):
        subfolders.sort()
        files.sort()
        for name in files:
            path = os.path.join(folder, name)
            f = open(path, "rb")
            data = f.read()
            f.close()
            relative = os.path.relpath(path, root)
            compression = "none"
            inner = data
            plain = try_decompress(data)
            if plain is not None:
                compression = "lz" + hex(data[0])[2:]
                inner = plain
                plain_path = os.path.join(out_dir, relative + ".dec")
                os.makedirs(os.path.dirname(plain_path), exist_ok=True)
                f = open(plain_path, "wb")
                f.write(plain)
                f.close()
            rows.append(relative + "\t" + str(len(data)) + "\t" + compression + "\t" + str(len(inner)) + "\t" + magic_text(inner) + " (" + inner[:4].hex() + ")")
    os.makedirs(out_dir, exist_ok=True)
    f = open(os.path.join(out_dir, "catalog.tsv"), "w")
    f.write("path\tsize\tcompression\tplain_size\tmagic\n")
    for row in rows:
        f.write(row + "\n")
    f.close()
    print("cataloged " + str(len(rows)) + " files")


main()
