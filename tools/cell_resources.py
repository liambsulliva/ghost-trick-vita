import argparse
import csv
import struct
from pathlib import Path


MAIN_BASE = 0x02000000


def read_value(data, offset, size, limit):
    if offset < 0 or offset + size > min(limit, len(data)):
        raise ValueError("cell resource lookup is outside its table")
    if size == 2:
        return struct.unpack_from("<H", data, offset)[0]
    return struct.unpack_from("<I", data, offset)[0]


def read_tables(data):
    tables = []
    for literal in [0xB750, 0xB754, 0xB758]:
        address = read_value(data, literal, 4, len(data))
        tables.append(address - MAIN_BASE)
    language, index, descriptor = tables
    if not 0 <= language < index < descriptor < len(data):
        raise ValueError("expected the USA BGTE ARM9 main segment")
    return language, index, descriptor


def resolve_resource(data, resource_id, language=1):
    if resource_id < 0 or resource_id > 0xFFFF:
        raise ValueError("resource ID must fit in 16 bits")
    if language < 1 or language > 5:
        raise ValueError("language must be between 1 and 5")
    language_table, index_table, descriptor_table = read_tables(data)
    resolved_id = resource_id
    if resource_id & 0x8000:
        offset = language_table + (resource_id & 0x7FFF) * 10
        if offset + 10 > index_table:
            raise ValueError("localized resource ID is outside its table")
        resolved_id = read_value(data, offset + (language - 1) * 2, 2, index_table)
    row = {"resource_id": resource_id, "language": language, "resolved_id": resolved_id,
           "descriptor": "", "sequence": "", "palette": "", "graphics": "", "cell": "", "animation": "", "status": "none"}
    if resolved_id == 0:
        return row
    packed = read_value(data, index_table + resolved_id * 2, 2, descriptor_table)
    descriptor = packed >> 2
    row["descriptor"] = descriptor
    row["sequence"] = packed & 3
    for i, name in enumerate(["palette", "graphics", "cell", "animation"]):
        row[name] = read_value(data, descriptor_table + descriptor * 8 + i * 2, 2, len(data))
    row["status"] = "resolved"
    return row


def main():
    parser = argparse.ArgumentParser(description="Resolve cell resource IDs in a USA BGTE ARM9 main segment.")
    parser.add_argument("main_segment", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("resource_ids", nargs="+", type=lambda value: int(value, 0))
    parser.add_argument("--language", type=int, default=1)
    args = parser.parse_args()
    data = args.main_segment.read_bytes()
    rows = []
    for resource_id in args.resource_ids:
        rows.append(resolve_resource(data, resource_id, args.language))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", newline="") as out:
        writer = csv.DictWriter(out, fieldnames=list(rows[0]), delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    print("resource IDs: " + str(len(rows)))


if __name__ == "__main__":
    main()
