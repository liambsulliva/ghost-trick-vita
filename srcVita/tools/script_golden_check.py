import argparse
import csv
import os
import sys

SRC_VITA_FOLDER = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
REPOSITORY_FOLDER = os.path.dirname(SRC_VITA_FOLDER)
DEFAULT_GOLDEN = os.path.join(REPOSITORY_FOLDER, "extracted", "trace", "vm_golden", "golden_trace.tsv")
ROUTE_START_TICK = 1024
ROUTE_GAP_FIRST_TICK = 1025
ROUTE_GAP_LAST_TICK = 1039
ROUTE_GAP_ROWS = ROUTE_GAP_LAST_TICK - ROUTE_GAP_FIRST_TICK + 1
EXIT_MISSING_DATA = 2


def baked_name(ds_file):
    name = ds_file
    if name.endswith(".lz"):
        name = name[:-3]
    if name.endswith(".xml"):
        name = name[:-4]
    return name.replace("/", "-")


def app_tick(ds_tick):
    if ds_tick <= ROUTE_GAP_LAST_TICK:
        return ds_tick - ROUTE_START_TICK
    return ds_tick - ROUTE_START_TICK - ROUTE_GAP_ROWS


def golden_rows(path, last_ds_tick):
    rows = []
    gap_rows = 0
    with open(path, newline="", encoding="latin-1") as source:
        for row in csv.DictReader(source, delimiter="\t"):
            tick = int(row["tick"])
            if tick < ROUTE_START_TICK or tick > last_ds_tick:
                continue
            if ROUTE_GAP_FIRST_TICK <= tick <= ROUTE_GAP_LAST_TICK and row["op"] == "0":
                gap_rows = gap_rows + 1
                continue
            rows.append((app_tick(tick), row["vm_class"], baked_name(row["file"]), row["offset"], row["op"], row["words"], row["b10"]))
    return rows, gap_rows


def replay_rows(path):
    rows = []
    with open(path, encoding="latin-1") as source:
        for line in source:
            fields = line.rstrip("\n").split("\t")
            rows.append((int(fields[0]), fields[1], fields[2], fields[3], fields[4], fields[5], fields[6]))
    return rows


COLUMN_NAMES = ("tick", "class", "file", "offset", "op", "words", "cond")


def compare(golden, replay, stop_is_expected):
    limit = min(len(golden), len(replay))
    for index in range(limit):
        if golden[index] != replay[index]:
            different = [name for name, left, right in zip(COLUMN_NAMES, golden[index], replay[index]) if left != right]
            print("first difference at row %d: %s" % (index, ",".join(different)))
            print(" golden", golden[index])
            print(" replay", replay[index])
            for row in golden[max(0, index - 5):index]:
                print(" before", row)
            return False
    print("%d rows match" % limit)
    if len(golden) != len(replay):
        print("golden has %d rows, replay %d" % (len(golden), len(replay)))
        return stop_is_expected
    return True


def main():
    parser = argparse.ArgumentParser(description="Compares a script_replay row log with the DS trace.")
    parser.add_argument("rows")
    parser.add_argument("--golden", default=DEFAULT_GOLDEN)
    parser.add_argument("--stop-tick", type=int, default=5896)
    parser.add_argument("--allow-short", action="store_true")
    arguments = parser.parse_args()
    if not os.path.isfile(arguments.golden):
        print("no golden trace at " + arguments.golden)
        return EXIT_MISSING_DATA
    golden, gap_rows = golden_rows(arguments.golden, arguments.stop_tick)
    print("allow-listed %d route rows, later ticks shifted by %d" % (gap_rows, gap_rows))
    return 0 if compare(golden, replay_rows(arguments.rows), arguments.allow_short) else 1


if __name__ == "__main__":
    sys.exit(main())
