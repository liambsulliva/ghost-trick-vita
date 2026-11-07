import re
import sys

NOT_PORTED_MARK = re.compile(r"^mark (\w*not_ported|\w*not_drawn|no_\w+)\s+(.*)$")
OP_OF_DETAIL = re.compile(r"op=(0x[0-9a-f]+)")


def report_lines(mark_lines):
    counts = {}
    for line in mark_lines:
        match = NOT_PORTED_MARK.match(line.strip())
        if match is None:
            continue
        op = OP_OF_DETAIL.search(match.group(2))
        name = op.group(1) if op is not None else match.group(2).split(" op=")[0].strip()
        key = (match.group(1), name)
        counts[key] = counts.get(key, 0) + 1
    return ["%s %s x%d" % (reason, name, counts[(reason, name)]) for reason, name in sorted(counts)]


def main():
    with open(sys.argv[1], "r", encoding="utf-8", errors="replace") as source:
        lines = report_lines(source.readlines())
    for line in lines:
        print(line)
    print("%d things not ported on the route" % len(lines))
    return 0


if __name__ == "__main__":
    sys.exit(main())
