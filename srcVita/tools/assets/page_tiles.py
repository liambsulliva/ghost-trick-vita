import asset_writers
import gtex_pack

TILE_SIDE_MOST = 2048
TILE_OVERLAP = 4
TABLE_PATH = "scripts/tiles.txt"
SPLIT_SIDE = 1024
TILES_PER_PAGE_MOST = 4
SMALLEST_WORTHWHILE_SAVING = 0.25


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


def padded_side(length):
    side = 4
    while side < length:
        side = side * 2
    return side


def split_once(length, first_end):
    second_start = first_end - TILE_OVERLAP
    return [(0, first_end, 0, first_end - TILE_OVERLAP // 2), (second_start, length, second_start + TILE_OVERLAP // 2, length)]


def span_choices(length):
    if length > TILE_SIDE_MOST:
        return [spans_along(length)]
    choices = [spans_along(length)]
    if length > SPLIT_SIDE + TILE_OVERLAP:
        choices.append(split_once(length, SPLIT_SIDE))
    return choices


def padded_area(spans_x, spans_y):
    return sum(padded_side(x[1] - x[0]) * padded_side(y[1] - y[0]) for x in spans_x for y in spans_y)


def page_spans(width, height):
    whole = (span_choices(width)[0], span_choices(height)[0])
    best = whole
    for spans_x in span_choices(width):
        for spans_y in span_choices(height):
            if len(spans_x) * len(spans_y) <= TILES_PER_PAGE_MOST and padded_area(spans_x, spans_y) < padded_area(*best):
                best = (spans_x, spans_y)
    if best != whole and len(whole[0]) * len(whole[1]) == 1 and padded_area(*best) > (1.0 - SMALLEST_WORTHWHILE_SAVING) * padded_area(*whole):
        return whole
    return best


def needs_tiles(width, height):
    spans_x, spans_y = page_spans(width, height)
    return len(spans_x) * len(spans_y) > 1


def tile_name(page_name, column, row):
    name = "%s_t%d%d" % (page_name, column, row)
    asset_writers.check_name(name)
    return name


def tile_layout(page_name, width, height):
    tiles = []
    spans_x, spans_y = page_spans(width, height)
    for row, span_y in enumerate(spans_y):
        for column, span_x in enumerate(spans_x):
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
