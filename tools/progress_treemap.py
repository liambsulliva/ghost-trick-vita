import argparse
import hashlib
import html
import json
import os
import re
from pathlib import Path
from urllib.parse import quote, urlsplit


ROOT = Path(__file__).resolve().parents[1]


def read_functions(config, excluded):
    paths = [("main", config / "symbols.txt"), ("itcm", config / "itcm/symbols.txt"), ("dtcm", config / "dtcm/symbols.txt")]
    for path in sorted((config / "overlays").glob("ov*/symbols.txt")):
        paths.append((path.parent.name, path))
    functions = []
    seen = set()
    for module, path in paths:
        if module in excluded:
            continue
        for line in path.read_text().splitlines():
            if "kind:function" not in line:
                continue
            match = re.fullmatch(r"(\S+) kind:function\((arm|thumb),size=(0x[0-9a-f]+)\) addr:(0x[0-9a-f]+)", line)
            if match is None:
                raise ValueError("function has no supported size or mode in " + str(path))
            name, mode, size, address = match.groups()
            size = int(size, 16)
            address = int(address, 16)
            key = (module, address)
            if size == 0 or key in seen:
                raise ValueError("empty or duplicate function in " + str(path))
            seen.add(key)
            functions.append({"module": module, "name": name, "mode": mode, "size": size, "address": address, "matched": False})
    if not functions:
        raise ValueError("no functions found")
    return functions


def apply_matches(functions, ledger, root):
    lookup = {(row["module"], row["address"]): row for row in functions}
    seen = set()
    for match in ledger["matched"]:
        key = (match["module"], int(match["address"], 16))
        if key not in lookup or key in seen:
            raise ValueError("unknown or duplicate matched function")
        source = (root / match["source"]).resolve()
        if not source.is_relative_to(root.resolve()) or source.suffix not in (".c", ".cpp"):
            raise ValueError("matched functions need a C or C++ source in this repository")
        if hashlib.sha256(source.read_bytes()).hexdigest() != match["source_sha256"]:
            raise ValueError("matched source changed; reverify before updating progress")
        if not re.fullmatch(r"[0-9a-f]{64}", match["verification_sha256"]):
            raise ValueError("matched functions need a verification record hash")
        if match["size"] != lookup[key]["size"]:
            raise ValueError("matched size differs from the function inventory")
        lookup[key]["matched"] = True
        lookup[key]["source"] = source.relative_to(root.resolve()).as_posix()
        seen.add(key)


def code_bytes(functions):
    total = 0
    modules = sorted({row["module"] for row in functions})
    for module in modules:
        end = 0
        for row in sorted((row for row in functions if row["module"] == module), key=lambda row: row["address"]):
            stop = row["address"] + row["size"]
            total += max(0, stop - max(end, row["address"]))
            end = max(end, stop)
    return total


def rectangles(items, x, y, width, height):
    if len(items) == 1:
        return [(items[0], x, y, width, height)]
    total = sum(item["size"] for item in items)
    split = 1
    first = items[0]["size"]
    while split < len(items) - 1 and abs(first + items[split]["size"] - total / 2) < abs(first - total / 2):
        first += items[split]["size"]
        split += 1
    ratio = first / total
    if width >= height:
        cut = width * ratio
        return rectangles(items[:split], x, y, cut, height) + rectangles(items[split:], x + cut, y, width - cut, height)
    cut = height * ratio
    return rectangles(items[:split], x, y, width, cut) + rectangles(items[split:], x, y + cut, width, height - cut)


def make_svg(functions, source_prefix="..", interactive=True):
    matched = [row for row in functions if row["matched"]]
    total_bytes = code_bytes(functions)
    matched_bytes = code_bytes(matched)
    modules = []
    for module in sorted({row["module"] for row in functions}):
        rows = [row for row in functions if row["module"] == module]
        rows.sort(key=lambda row: (-row["size"], row["address"]))
        modules.append({"name": module, "size": sum(row["size"] for row in rows), "functions": rows})
    modules.sort(key=lambda row: (-row["size"], row["name"]))
    subtitle = f'Functions matched: {len(matched):,} / {len(functions):,} ({len(matched) / len(functions):.2%}) | Code bytes matched: {matched_bytes:,} / {total_bytes:,} ({matched_bytes / total_bytes:.2%}) | Modules: {len(modules)}'
    role = "group" if interactive else "img"
    explanation = "Green tiles link to verified C or C++ source; gray tiles have no verified source yet." if interactive else "Static preview. Green means verified C or C++; gray means unmatched."
    out = [f'<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1600 1092" width="1600" height="1092" role="{role}" aria-labelledby="title description" font-family="Segoe UI, system-ui, sans-serif">',
           '<title id="title">Ghost Trick decompilation progress</title>',
           '<desc id="description">' + html.escape(subtitle) + '. Each rectangle is one function. ' + explanation + ' This is the prepared code inventory, not the Chapter 1 dependency count.</desc>',
           '<style>a { cursor: pointer; } a:hover rect, a:focus-visible rect { stroke: #fff; stroke-width: 1; }</style>',
           '<rect width="1600" height="1092" fill="#14191f"/>',
           '<text x="16" y="33" fill="#e6eaf0" font-size="24" font-weight="600">Ghost Trick decompilation progress</text>',
           '<text x="16" y="60" fill="#e6eaf0" font-size="15">' + html.escape(subtitle) + '</text>',
           '<text x="16" y="83" fill="#a7b0bc" font-size="13">Prepared code across all chapters. Area = function span bytes. Shared byte ranges count once in the totals.</text>']
    for module, x, y, width, height in rectangles(modules, 12, 101, 1576, 922):
        pad = min(4, width / 8, height / 8)
        header = min(24, height / 5)
        out.append(f'<rect x="{x:.3f}" y="{y:.3f}" width="{width:.3f}" height="{height:.3f}" fill="#202832" stroke="#8993a1" stroke-width="1"/>')
        done = code_bytes([row for row in module["functions"] if row["matched"]])
        percentage = done / code_bytes(module["functions"])
        if width > 100 and header >= 14:
            out.append(f'<text x="{x + 6:.3f}" y="{y + 17:.3f}" fill="#e6eaf0" font-size="12" font-weight="600">{module["name"]} · {percentage:.1%}</text>')
        leaves = rectangles(module["functions"], x + pad, y + header, width - pad * 2, height - header - pad)
        paths = {"#3b9b72": [], "#535e6d": []}
        labels = []
        for row, left, top, wide, high in leaves:
            gap = min(0.35, wide / 5, high / 5)
            color = "#3b9b72" if row["matched"] else "#535e6d"
            if not interactive:
                paths[color].append(f'M{left + gap:.2f} {top + gap:.2f}h{wide - gap * 2:.2f}v{high - gap * 2:.2f}h-{wide - gap * 2:.2f}z')
                if wide > 120 and high > 25:
                    label = row["name"][:int(wide / 7)]
                    labels.append(f'<text x="{left + 5:.3f}" y="{top + 17:.3f}" fill="#f1f4f8" font-size="11">{html.escape(label)}</text>')
                continue
            state = "Verified C/C++" if row["matched"] else "Unmatched"
            title = f'{row["module"]} / {row["name"]} | 0x{row["address"]:08x} | {row["mode"]} | {row["size"]} bytes | {state}'
            if row["matched"]:
                title += " | " + row["source"]
                if source_prefix.startswith("https://"):
                    href = source_prefix.rstrip("/") + "/" + quote(row["source"], safe="/")
                else:
                    href = quote((Path(source_prefix) / row["source"]).as_posix(), safe="/")
                label = "Open " + row["name"] + " in " + row["source"]
                out.append(f'<a href="{html.escape(href, quote=True)}" target="_top" aria-label="{html.escape(label, quote=True)}">')
            out.append(f'<rect data-function="{html.escape(row["module"] + ":" + row["name"], quote=True)}" x="{left + gap:.3f}" y="{top + gap:.3f}" width="{wide - gap * 2:.3f}" height="{high - gap * 2:.3f}" fill="{color}"><title>{html.escape(title)}</title></rect>')
            if wide > 120 and high > 25:
                label = row["name"][:int(wide / 7)]
                out.append(f'<text x="{left + 5:.3f}" y="{top + 17:.3f}" fill="#f1f4f8" font-size="11" pointer-events="none">{html.escape(label)}</text>')
            if row["matched"]:
                out.append('</a>')
        for color, parts in paths.items():
            if parts:
                data = "\n".join("".join(parts[start:start + 128]) for start in range(0, len(parts), 128))
                out.append(f'<path fill="{color}" d="{data}"/>')
        out.extend(labels)
    legend = "Verified C/C++ · click to open source" if interactive else "Verified C/C++"
    out.extend(['<rect x="16" y="1042" width="12" height="12" fill="#3b9b72"/>',
                '<text x="35" y="1053" fill="#e6eaf0" font-size="13">' + legend + '</text>',
                '<rect x="300" y="1042" width="12" height="12" fill="#535e6d"/>',
                '<text x="319" y="1053" fill="#e6eaf0" font-size="13">Unmatched</text>',
                '<text x="16" y="1079" fill="#a7b0bc" font-size="12">Source: config/arm9 symbols + config/decomp_progress.json. Assembly rebuilds and asset preparation do not count as C matches. DS Protect excluded.</text>',
                '</svg>'])
    return "\n".join(out) + "\n"


def make_viewer(source_url):
    project = html.escape(source_url.rstrip("/") + "/README.md", quote=True)
    return '\n'.join([
        '<!doctype html>',
        '<html lang="en">',
        '<head>',
        '<meta charset="utf-8">',
        '<meta name="viewport" content="width=device-width, initial-scale=1">',
        '<title>Ghost Trick function progress</title>',
        '<style>body { margin: 0; background: #14191f; color: #e6eaf0; font: 16px system-ui, sans-serif; } p { margin: 16px; } a { color: #83dcb4; } iframe { display: block; width: 100%; height: 68.25vw; min-height: 600px; border: 0; }</style>',
        '</head>',
        '<body>',
        '<p>Click a green rectangle to open its verified C/C++ source on GitHub. Gray rectangles have no verified source yet. Use browser zoom for small rectangles.</p>',
        '<p><a href="treemap.svg">Open the full chart</a> · <a href="' + project + '">Project README at this revision</a></p>',
        '<iframe src="treemap.svg" title="Function treemap with links to verified source"></iframe>',
        '</body>',
        '</html>',
        '',
    ])


def main():
    parser = argparse.ArgumentParser(description="Generate the README function progress treemap from the dsd inventory and verified-source ledger.")
    parser.add_argument("--check", action="store_true")
    parser.add_argument("--output", type=Path, default=ROOT / "docs/progress-treemap.svg")
    parser.add_argument("--site-dir", type=Path)
    parser.add_argument("--source-url")
    args = parser.parse_args()
    if args.site_dir:
        url = urlsplit(args.source_url or "")
        if url.scheme != "https" or not url.netloc or url.query or url.fragment:
            parser.error("--site-dir needs --source-url with an HTTPS repository source base URL")
    ledger = json.loads((ROOT / "config/decomp_progress.json").read_text())
    functions = read_functions(ROOT / "config/arm9", ledger["excluded_modules"])
    apply_matches(functions, ledger, ROOT)
    source_prefix = os.path.relpath(ROOT, args.output.resolve().parent)
    outputs = {args.output: make_svg(functions, source_prefix, interactive=False)}
    if args.site_dir:
        outputs[args.site_dir / "treemap.svg"] = make_svg(functions, args.source_url)
        outputs[args.site_dir / "index.html"] = make_viewer(args.source_url)
    for path, content in outputs.items():
        if args.check:
            if not path.exists() or path.read_text() != content:
                raise SystemExit("progress treemap is stale: " + str(path))
        else:
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(content)
    print(f'functions matched: {sum(row["matched"] for row in functions)}/{len(functions)}, code bytes: {code_bytes(functions)}')


if __name__ == "__main__":
    main()
