import sys
import os
import re
import glob

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import lmg_decode

REPO_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def read_remaster_folders(path):
    folders = {}
    with open(path, "r") as source:
        for line in source:
            parts = line.strip().replace("\\", "/").lower().split("/")
            if len(parts) < 3 or "/charfiles/2d/" not in "/".join(parts):
                continue
            stem = parts[-2]
            if not parts[-1].startswith(stem + ".user."):
                continue
            if re.fullmatch(r"st\d{2}_.+_\d{3}", stem) is None:
                continue
            if stem not in folders:
                folders[stem] = set()
            folders[stem].add("/".join(parts[:-1]))
    if len(folders) == 0:
        raise ValueError("no stage animation folders found in the remaster file list")
    return folders


def match_motion(name, folders):
    stem = name[:-len(".mtbl.xml")].lower()
    if stem in folders:
        return stem, "exact"
    match = re.fullmatch(r"(st\d{2}_.+_\d{3})_(?:\d{3}|\d{2}[a-z])", stem)
    if match is not None and match.group(1) in folders:
        return match.group(1), "variant"
    return "", "unmatched"


def main():
    source = os.path.join(REPO_DIR, "extracted", "nds_plain")
    file_list = os.path.join(REPO_DIR, "extracted", "remaster_apk", "GTPD_ANDROID_Release.list")
    out_dir = os.path.join(REPO_DIR, "extracted", "motion_map")
    if len(sys.argv) > 1:
        source = sys.argv[1]
    if len(sys.argv) > 2:
        file_list = sys.argv[2]
    if len(sys.argv) > 3:
        out_dir = sys.argv[3]
    folders = read_remaster_folders(file_list)
    rows = []
    unresolved = []
    matched = set()
    exact = set()
    scripts = 0
    table_count = 0
    plays = 0
    resolved = 0
    zero_jumps = 0
    for path in sorted(glob.glob(os.path.join(source, "**", "*.dec"), recursive=True)):
        with open(path, "rb") as script:
            data = script.read()
        if data[:4] != b"1LMG":
            continue
        header, strings, labels, items, errors = lmg_decode.decode(data)
        if len(errors) > 0:
            raise ValueError(path + ": " + errors[0])
        scripts = scripts + 1
        relative = os.path.relpath(path, source)
        tables = set()
        for item in items:
            if item[1] != "motion":
                continue
            table_count = table_count + 1
            tables.add(item[0])
            for name in labels.get(item[0], []):
                if not name.endswith(".mtbl.xml"):
                    continue
                stem, kind = match_motion(name, folders)
                paths = [""]
                if stem != "":
                    matched.add(stem)
                    if kind == "exact":
                        exact.add(stem)
                    paths = sorted(folders[stem])
                for folder in paths:
                    rows.append((relative, hex(item[0]), name, folder, kind))
        for item in items:
            if item[1] != "op":
                continue
            if item[2] in (0x22, 0x23, 0x2D) and item[3][0] == item[3][1] == 0:
                zero_jumps = zero_jumps + 1
            if item[2] == 0x117:
                plays = plays + 1
                target = item[3][0] | (item[3][1] << 16)
                if target in tables:
                    resolved = resolved + 1
                else:
                    unresolved.append((relative, hex(item[0]), hex(target)))
    if scripts == 0:
        raise ValueError("no 1LMG scripts found")
    os.makedirs(out_dir, exist_ok=True)
    with open(os.path.join(out_dir, "motions.tsv"), "w") as out:
        out.write("script\toffset\tmotion_label\tremaster_folder\tmatch\n")
        for row in rows:
            out.write("\t".join(row) + "\n")
    with open(os.path.join(out_dir, "unmatched_remaster.tsv"), "w") as out:
        out.write("animation\tremaster_folder\n")
        for stem in sorted(set(folders) - matched):
            for folder in sorted(folders[stem]):
                out.write(stem + "\t" + folder + "\n")
    with open(os.path.join(out_dir, "unresolved_plays.tsv"), "w") as out:
        out.write("script\toffset\ttarget\n")
        for row in unresolved:
            out.write("\t".join(row) + "\n")
    print("scripts: " + str(scripts) + ", motion tables: " + str(table_count) + ", mapping rows: " + str(len(rows)))
    print("remaster animation stems: " + str(len(folders)) + ", matched: " + str(len(matched)) + ", unmatched: " + str(len(folders) - len(matched)))
    print("exact matches: " + str(len(exact)) + ", additional variant matches: " + str(len(matched - exact)))
    print("play-motion targets: " + str(resolved) + "/" + str(plays) + ", unresolved: " + str(len(unresolved)) + ", zero jumps: " + str(zero_jumps))
    print("written to " + out_dir)


if __name__ == "__main__":
    main()
