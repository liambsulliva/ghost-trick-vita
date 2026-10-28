import sys
import os
import re
import glob
import argparse

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


def motion_uses(items, strings):
    objects = []
    uses = []
    for item in items:
        while len(objects) > 0 and item[0] >= objects[-1][0]:
            objects.pop()
        if item[1] != "op":
            continue
        if item[2] == 0x16:
            end = item[3][1] | (item[3][2] << 16)
            objects.append((end, strings.get(item[3][3], "").lower()))
        if item[2] == 0x117:
            owner = ""
            if len(objects) > 0:
                owner = objects[-1][1]
            target = item[3][0] | (item[3][1] << 16)
            uses.append((item[0], target, owner))
    return uses


def idle_candidates(stage, name, owners, folders):
    candidates = []
    stem = name[:-len(".mtbl.xml")].lower()
    for owner in sorted(owners):
        if owner == "":
            continue
        if stem not in (owner + "_wait", stage + "_" + owner):
            continue
        candidate = stage + "_" + owner + "_000"
        if candidate in folders:
            candidates.append((owner, candidate))
    return candidates


def alias_candidates(stage, name, folders):
    stem = name[:-len(".mtbl.xml")].lower()
    if stem in folders:
        return []
    if stem.endswith("_wait"):
        asset = stem[:-len("_wait")]
        family = stage + "_" + asset
        matches = sorted(folder for folder in folders if folder.rsplit("_", 1)[0] == family)
        if family + "_000" in matches:
            return [(family + "_000", "wait_name")]
        if len(matches) == 1:
            return [(matches[0], "single_animation_wait")]
        if len(matches) == 0:
            matches = sorted(folder for folder in folders if folder[5:].rsplit("_", 1)[0] == asset)
            return [(folder, "cross_stage_wait") for folder in matches]
    if stem.startswith(stage + "_") and stem + "_000" in folders:
        return [(stem + "_000", "unnumbered_default")]
    if stem.startswith("sr" + stage[2:] + "_"):
        candidate = "st" + stem[2:]
        if candidate in folders:
            return [(candidate, "stage_prefix_spelling")]
    match = re.fullmatch(r"(st\d{2}_.+_)00[a-z]", stem)
    if match is not None:
        candidate = match.group(1) + "000"
        if candidate in folders:
            return [(candidate, "primary_letter_variant")]
    return []


def main():
    parser = argparse.ArgumentParser(description="Map DS motion names to remaster animation folders.")
    parser.add_argument("source", nargs="?", default=os.path.join(REPO_DIR, "extracted", "nds_plain"))
    parser.add_argument("file_list", nargs="?", default=os.path.join(REPO_DIR, "extracted", "remaster_apk", "GTPD_ANDROID_Release.list"))
    parser.add_argument("output", nargs="?")
    parser.add_argument("--stage")
    args = parser.parse_args()
    if args.stage is not None and re.fullmatch(r"st\d{2}", args.stage) is None:
        parser.error("stage must use the form st01")
    source = args.source
    file_list = args.file_list
    out_dir = args.output
    if out_dir is None:
        out_dir = os.path.join(REPO_DIR, "extracted", "motion_map")
        if args.stage is not None:
            out_dir = os.path.join(out_dir, args.stage)
    folders = read_remaster_folders(file_list)
    if args.stage is not None:
        folders = {stem: paths for stem, paths in folders.items() if stem.startswith(args.stage + "_")}
        if len(folders) == 0:
            raise ValueError("no remaster animation folders found for " + args.stage)
    rows = []
    uses_rows = []
    candidate_rows = []
    alias_rows = []
    unresolved = []
    matched = set()
    exact = set()
    scripts = 0
    table_count = 0
    plays = 0
    resolved = 0
    zero_jumps = 0
    zero_motions = 0
    for path in sorted(glob.glob(os.path.join(source, "**", "*.dec"), recursive=True)):
        relative = os.path.relpath(path, source)
        stage = relative.split(os.sep)[0]
        if args.stage is not None and stage != args.stage:
            continue
        with open(path, "rb") as script:
            data = script.read()
        if data[:4] != b"1LMG":
            continue
        header, strings, labels, items, errors = lmg_decode.decode(data)
        if len(errors) > 0:
            raise ValueError(path + ": " + errors[0])
        scripts = scripts + 1
        if re.fullmatch(r"st\d{2}", stage) is None:
            stage = ""
        uses = motion_uses(items, strings)
        owners = {}
        for pos, target, owner in uses:
            if target not in owners:
                owners[target] = {}
            if owner not in owners[target]:
                owners[target][owner] = []
            owners[target][owner].append(pos)
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
                if kind == "unmatched" and stage != "":
                    frames = [record[2] for record in item[2] if record[1] == "frame"]
                    frame_ids = ",".join(str(frame[0]) for frame in frames)
                    ticks = ",".join(str(frame[1]) for frame in frames)
                    loops = str(any(record[1] == -1 for record in item[2])).lower()
                    long_frames = str(sum(record[1] == -5 for record in item[2]))
                    for owner, candidate in idle_candidates(stage, name, owners.get(item[0], {}), folders):
                        calls = ",".join(hex(pos) for pos in owners[item[0]][owner])
                        for folder in sorted(folders[candidate]):
                            candidate_rows.append((relative, hex(item[0]), name, owner, candidate, folder, calls, frame_ids, ticks, loops, long_frames))
                    calls = ",".join(hex(pos) for pos, target, owner in uses if target == item[0])
                    object_names = ",".join(sorted(owner for owner in owners.get(item[0], {}) if owner != ""))
                    aliases = alias_candidates(stage, name, folders)
                    alternatives = ",".join(candidate for candidate, reason in aliases)
                    for candidate, reason in aliases:
                        for folder in sorted(folders[candidate]):
                            alias_rows.append((relative, hex(item[0]), name, object_names, candidate, folder, reason, calls, frame_ids, ticks, loops, long_frames, alternatives))
        for item in items:
            if item[1] != "op":
                continue
            if item[2] in (0x22, 0x23, 0x2D) and item[3][0] == item[3][1] == 0:
                zero_jumps = zero_jumps + 1
        for pos, target, owner in uses:
            plays = plays + 1
            kind = "motion"
            if target in tables:
                resolved = resolved + 1
            else:
                kind = "unresolved"
                if target == 0:
                    zero_motions = zero_motions + 1
                    kind = "zero_target"
                unresolved.append((relative, hex(pos), hex(target), owner, kind))
            names = ",".join(name for name in labels.get(target, []) if name.endswith(".mtbl.xml"))
            uses_rows.append((relative, hex(pos), owner, hex(target), names, kind))
    if scripts == 0:
        raise ValueError("no 1LMG scripts found")
    alias_rows = [row for row in alias_rows if row[4] not in matched]
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
        out.write("script\toffset\ttarget\tobject\treason\n")
        for row in unresolved:
            out.write("\t".join(row) + "\n")
    with open(os.path.join(out_dir, "motion_uses.tsv"), "w") as out:
        out.write("script\toffset\tobject\ttarget\tmotion_labels\tstatus\n")
        for row in uses_rows:
            out.write("\t".join(row) + "\n")
    with open(os.path.join(out_dir, "idle_candidates.tsv"), "w") as out:
        out.write("script\ttable_offset\tmotion_label\tobject\tanimation\tremaster_folder\tplay_offsets\tframe_ids\tframe_ticks\tloop\tlong_frame_commands\n")
        for row in candidate_rows:
            out.write("\t".join(row) + "\n")
    with open(os.path.join(out_dir, "alias_candidates.tsv"), "w") as out:
        out.write("script\ttable_offset\tmotion_label\tobjects\tanimation\tremaster_folder\treason\tplay_offsets\tframe_ids\tframe_ticks\tloop\tlong_frame_commands\talternative_animations\n")
        for row in alias_rows:
            out.write("\t".join(row) + "\n")
    candidate_stems = set(row[4] for row in candidate_rows)
    alias_stems = set(row[4] for row in alias_rows)
    with open(os.path.join(out_dir, "animation_gaps.tsv"), "w") as out:
        out.write("animation\tremaster_folder\tstatus\tcandidate_reasons\tsource_stages\tmotion_labels\n")
        for stem in sorted(set(folders) - matched):
            rows_for_stem = [row for row in alias_rows if row[4] == stem]
            reasons = ",".join(sorted(set(row[6] for row in rows_for_stem)))
            stages = ",".join(sorted(set(row[0].split(os.sep)[0] for row in rows_for_stem)))
            names = ",".join(sorted(set(row[2] for row in rows_for_stem)))
            status = "no_candidate"
            if stem in candidate_stems:
                status = "object_name_candidate"
            elif stem in alias_stems:
                status = "alias_candidate"
            for folder in sorted(folders[stem]):
                out.write("\t".join((stem, folder, status, reasons, stages, names)) + "\n")
    print("scripts: " + str(scripts) + ", motion tables: " + str(table_count) + ", mapping rows: " + str(len(rows)))
    print("remaster animation stems: " + str(len(folders)) + ", matched: " + str(len(matched)) + ", unmatched: " + str(len(folders) - len(matched)))
    print("exact matches: " + str(len(exact)) + ", additional variant matches: " + str(len(matched - exact)))
    print("play-motion targets: " + str(resolved) + "/" + str(plays) + ", unresolved: " + str(len(unresolved)) + ", zero jumps: " + str(zero_jumps))
    print("unresolved zero motion targets: " + str(zero_motions) + ", unresolved nonzero targets: " + str(len(unresolved) - zero_motions))
    print("idle candidate stems: " + str(len(candidate_stems)) + ", additional unmatched stems with candidates: " + str(len(candidate_stems - matched)))
    remaining = set(folders) - matched - candidate_stems - alias_stems
    print("unmatched stems with alias candidates: " + str(len(alias_stems - matched)) + ", without any candidate: " + str(len(remaining)))
    print("written to " + out_dir)


if __name__ == "__main__":
    main()
