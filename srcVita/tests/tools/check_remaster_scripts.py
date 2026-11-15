import argparse
import collections
import difflib
import json
import os
import sys

SRC_VITA = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(SRC_VITA, "tools", "assets"))
sys.path.insert(0, os.path.join(SRC_VITA, "..", "tools"))

import chapter_assets
import check_remaster_tables
import ds_rom
import lmg_decode
import message_file
import pack_files
import remaster_scripts

LEAST_SAME_SHARE = 0.985
NO_OPS = (0x12e, 0x12f, 0x130, 0xc4)
JUMP_OP = 0x22
DEBUG_OP = 0x12e
MOTION_SEND = -7
MOTION_EFFECT = -8
MOTION_BEGIN = -3
MOTION_EFFECT_NAME_WORD = 5
TABLES_FILE = "remaster_tables.json"


def in_packs(packs, path):
    return pack_files.path_hash_key(path) in packs.entries


def word_kinds(classes):
    kinds = {}
    for row in classes.values():
        if "op" in row and "words" in row and not row.get("unpaired"):
            kinds[row["op"]] = [rule.rstrip("0123456789|") for rule in row["words"]]
    return kinds


def motion_arguments(record, strings):
    if record[1] == MOTION_SEND:
        return tuple(strings.get(value & 0xffff) for value in record[2])
    if record[1] == MOTION_EFFECT:
        return tuple(strings.get(value & 0xffff) if index == MOTION_EFFECT_NAME_WORD else value for index, value in enumerate(record[2]))
    return () if record[1] == MOTION_BEGIN else tuple(record[2])


def normalized(decoded, kinds):
    header, strings, _, items, errors = decoded
    real = [item for item in items if item[1] in ("op", "motion")]
    end = lmg_decode.CODE_START + header["code_size"]
    index = {item[0]: number for number, item in enumerate(real)}
    tables = {item[0]: tuple((record[1], motion_arguments(record, strings)) for record in item[2]) for item in real if item[1] == "motion"}
    alias = {}
    for number, item in enumerate(real):
        later = number
        while later < len(real) and real[later][1] == "op" and (real[later][2] in NO_OPS or (real[later][2] == JUMP_OP and later and real[later - 1][2] == DEBUG_OP)):
            later = later + 1
        alias[item[0]] = real[later][0] if later < len(real) else end
    if real and real[-1][1] == "op" and real[-1][2] == 0:
        alias[real[-1][0]] = end
        real = real[:-1]
    out = []
    for item in real:
        if item[1] == "motion":
            out.append(("motion", tables[item[0]]))
            continue
        op, words = item[2], item[3]
        rules = kinds.get(op, [])
        values = []
        position = 0
        while position < len(words):
            rule = rules[position] if position < len(rules) else ""
            if rule in ("s", "m"):
                values.append(("text", strings.get(words[position])))
            elif rule == "o" and position + 1 < len(words):
                offset = alias.get(words[position] | words[position + 1] << 16, words[position] | words[position + 1] << 16)
                values.append(("table", tables[offset]) if offset in tables else ("at", "end" if offset == end else index.get(offset, ("inside", offset))))
                position = position + 1
            else:
                values.append(words[position])
            position = position + 1
        out.append((op, tuple(values)))
    return out, errors


def compare(mine, theirs):
    counts = collections.Counter()
    differing = collections.Counter()
    first = {}
    ops_mine = [item[0] for item in mine]
    ops_theirs = [item[0] for item in theirs]
    blocks = difflib.SequenceMatcher(None, ops_mine, ops_theirs, autojunk=False).get_opcodes()
    mapping = {}
    for tag, start_mine, end_mine, start_theirs, _ in blocks:
        if tag == "equal":
            for step in range(end_mine - start_mine):
                mapping[start_mine + step] = start_theirs + step

    def mapped(item):
        if item[0] == "motion":
            return item
        return (item[0], tuple(("at", mapping.get(value[1], ("unpaired", value[1]))) if isinstance(value, tuple) and value[0] == "at" and isinstance(value[1], int) else value for value in item[1]))

    for tag, start_mine, end_mine, start_theirs, end_theirs in blocks:
        if tag != "equal":
            counts["only_mine"] += end_mine - start_mine
            counts["only_ds"] += end_theirs - start_theirs
            continue
        for step in range(end_mine - start_mine):
            counts["paired"] += 1
            left, right = mapped(mine[start_mine + step]), theirs[start_theirs + step]
            if left == right or (left[0] == "motion" and right[0] == "motion"):
                counts["same"] += 1
            else:
                differing[left[0]] += 1
                first.setdefault(left[0], (left, right))
    return counts, differing, first


def main():
    parser = argparse.ArgumentParser(description="Compile every remaster script from the Android package and compare it with the DS script.")
    parser.add_argument("--rom", default=check_remaster_tables.DEFAULT_ROM)
    parser.add_argument("--apk", default=check_remaster_tables.DEFAULT_APK_FOLDER)
    parser.add_argument("--show", type=int, default=12)
    arguments = parser.parse_args()
    packs = pack_files.PackSet(arguments.apk)
    with open(os.path.join(arguments.apk, message_file.TEXT_KEY_FILE_NAME), "rb") as handle:
        text_key = handle.read()
    with open(os.path.join(arguments.apk, TABLES_FILE)) as handle:
        symbols = json.load(handle)["symbols"]
    classes, by_type = remaster_scripts.load_classes()
    logged = collections.Counter()
    kinds = word_kinds(classes)
    listed = [line.strip() for line in open(os.path.join(arguments.apk, "GTPD_ANDROID_Release.list"))]
    message_paths = [path for path in listed if path.endswith(".msg.22") and "/text/" in path and in_packs(packs, path)]
    shared_tables = remaster_scripts.shared_table_names(packs, remaster_scripts.chara_list_names(listed))
    messages = remaster_scripts.message_labels(packs, text_key, message_paths, lambda kind, detail: logged.update([kind]))
    totals = collections.Counter()
    differing = collections.Counter()
    first = {}
    failures = []
    with ds_rom.DsRom(arguments.rom) as rom:
        for ds_path in check_remaster_tables.script_paths(rom):
            pack_path = remaster_scripts.script_pack_path(ds_path)
            if not in_packs(packs, pack_path):
                totals["ds_only_scripts"] += 1
                continue
            compiled = remaster_scripts.compile_script(ds_path, packs.read(pack_path), classes, by_type, symbols, messages, remaster_scripts.table_names(packs, ds_path, shared_tables), lambda kind, detail: logged.update([kind]))
            mine, errors = normalized(lmg_decode.decode(compiled), kinds)
            if errors:
                failures.append(ds_path + ": " + errors[0])
                continue
            theirs, _ = normalized(lmg_decode.decode(chapter_assets.read_script(rom, ds_path).plain), kinds)
            counts, file_differing, file_first = compare(mine, theirs)
            totals.update(counts)
            totals["scripts"] += 1
            differing.update(file_differing)
            for op, example in file_first.items():
                first.setdefault(op, (ds_path,) + example)
        compared = {remaster_scripts.script_pack_path(path) for path in check_remaster_tables.script_paths(rom)}
    for pack_path in listed:
        if pack_path.startswith(remaster_scripts.SCRIPT_FOLDER) and pack_path.endswith(remaster_scripts.SCRIPT_SUFFIX) and pack_path not in compared and in_packs(packs, pack_path):
            ds_path = pack_path[len(remaster_scripts.SCRIPT_FOLDER):-len(remaster_scripts.SCRIPT_SUFFIX)] + remaster_scripts.DS_SCRIPT_SUFFIX
            compiled = remaster_scripts.compile_script(ds_path, packs.read(pack_path), classes, by_type, symbols, messages, remaster_scripts.table_names(packs, ds_path, shared_tables), lambda kind, detail: logged.update([kind]))
            errors = lmg_decode.decode(compiled)[4]
            totals["remaster_only_scripts"] += 1
            if errors:
                failures.append(pack_path + ": " + errors[0])
    share = totals["same"] / max(totals["paired"], 1)
    print("scripts %d, paired ops %d, same %d (%.4f), only in the remaster %d, only on the DS %d, DS scripts with no remaster file %d, remaster scripts with no DS script %d" % (totals["scripts"], totals["paired"], totals["same"], share, totals["only_mine"], totals["only_ds"], totals["ds_only_scripts"], totals["remaster_only_scripts"]))
    print("logged: " + ", ".join("%s %d" % item for item in logged.most_common()))
    for op, count in differing.most_common(arguments.show):
        path, left, right = first[op]
        print("op %s differs %d times, first in %s\n  remaster %s\n  ds       %s" % (hex(op), count, path, str(left)[:240], str(right)[:240]))
    for failure in failures:
        print("decode failed: " + failure)
    return 1 if failures or share < LEAST_SAME_SHARE else 0


if __name__ == "__main__":
    sys.exit(main())
