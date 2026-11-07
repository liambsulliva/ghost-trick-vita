import asset_writers
import gtex_pack

TILE_SIDE_MOST = 2048
TILE_OVERLAP = 4
TABLE_PATH = "scripts/tiles.txt"


def spans_along(length):
    if length <= TILE_SIDE_MOST:
        return [(0, length, 0, length)]
    spans = []
    start = 0
    while True:
        end = min(start + TILE_SIDE_MOST, length)
        draw_start = 0 if start == 0 else start + TILE_OVERLAP // 2
        draw_end = length if end == length else end - TILE_OVERLAP // 2
        spans.append((start, end, draw_start, draw_end))
        if end == length:
            return spans
        start = end - TILE_OVERLAP


def needs_tiles(width, height):
    return width > TILE_SIDE_MOST or height > TILE_SIDE_MOST


def tile_name(page_name, column, row):
    name = "%s_t%d%d" % (page_name, column, row)
    asset_writers.check_name(name)
    return name


def tile_layout(page_name, width, height):
    tiles = []
    for row, span_y in enumerate(spans_along(height)):
        for column, span_x in enumerate(spans_along(width)):
            tiles.append({
                "name": tile_name(page_name, column, row),
                "left": span_x[0],
                "top": span_y[0],
                "right": span_x[1],
                "bottom": span_y[1],
                "draw": (span_x[2], span_y[2], span_x[3], span_y[3]),
            })
    return tiles


def cut_gtex(page_gtex, left, top, right, bottom):
    page_gtex = gtex_pack.unpack_gtex(page_gtex)
    width, height, texture_format = asset_writers.read_gtex_header(page_gtex)
    if texture_format == asset_writers.FORMAT_RGBA8888:
        side, unit_bytes = 1, 4
    else:
        side, unit_bytes = 4, asset_writers.BLOCK_BYTES[texture_format]
    if left % side or top % side or right % side or bottom % side:
        raise ValueError("tile edges must fall on block edges")
    pixels = page_gtex[20:]
    row_bytes = (width // side) * unit_bytes
    rows = []
    for unit_row in range(top // side, bottom // side):
        start = unit_row * row_bytes + (left // side) * unit_bytes
        rows.append(pixels[start:start + ((right - left) // side) * unit_bytes])
    return asset_writers.gtex_bytes(right - left, bottom - top, texture_format, b"".join(rows))


def table_text(texture_results):
    lines = []
    for result in texture_results:
        page = result.get("page")
        if page is None:
            continue
        draw = page["draw"]
        lines.append("tile %s %d %d %s %d %d %d %d %d %d %d %d" % (page["name"], page["width"], page["height"], result["name"], page["left"], page["top"], result["width"], result["height"], draw[0], draw[1], draw[2], draw[3]))
    return "\n".join(lines) + ("\n" if lines else "")
