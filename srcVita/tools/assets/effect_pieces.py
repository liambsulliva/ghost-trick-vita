import concurrent.futures
import os

import numpy

import asset_writers
import bake_record
import gtex_pack

SQUARE_SIDE = 32
GUTTER = 4
PAGE_SIDE_MOST = 2048
RUN_SQUARES_MOST = (PAGE_SIDE_MOST - 4 * GUTTER) // SQUARE_SIDE
BLOCK_SIDE = 4
PARALLELOGRAM_SLACK = 0.01
NUMBER_DIGITS = 6
MOVIE_MEMORY_MOST = 16 * 1024 * 1024
LAYOUT_VERSION = 1


def power_of_two_at_least(length):
    side = BLOCK_SIDE
    while side < length:
        side = side * 2
    return side


def page_texture_name(effect_name, page):
    return "fx_%s_%02d" % (effect_name.lower().replace("-", "_"), page)


def blocks_of(page_gtex):
    page_gtex = gtex_pack.unpack_gtex(page_gtex)
    width, height, texture_format = asset_writers.read_gtex_header(page_gtex)
    if texture_format not in asset_writers.BLOCK_BYTES:
        raise ValueError("effect pages must be block compressed")
    unit_bytes = asset_writers.BLOCK_BYTES[texture_format]
    columns = (width + BLOCK_SIDE - 1) // BLOCK_SIDE
    rows = (height + BLOCK_SIDE - 1) // BLOCK_SIDE
    blocks = numpy.frombuffer(page_gtex[20:20 + columns * rows * unit_bytes], dtype=numpy.uint8).reshape(rows, columns, unit_bytes)
    return {"width": width, "height": height, "format": texture_format, "blocks": blocks, "shows": blocks_that_show(blocks, texture_format)}


def blocks_that_show(blocks, texture_format):
    if texture_format != asset_writers.FORMAT_DXT5:
        return numpy.ones(blocks.shape[:2], dtype=bool)
    first = blocks[..., 0].astype(numpy.int32)
    second = blocks[..., 1].astype(numpy.int32)
    indices = numpy.zeros(blocks.shape[:2], dtype=numpy.uint64)
    for byte in range(6):
        indices |= blocks[..., 2 + byte].astype(numpy.uint64) << numpy.uint64(8 * byte)
    eight_levels = [first, second] + [((7 - step) * first + step * second) // 7 for step in range(1, 7)]
    six_levels = [first, second] + [((5 - step) * first + step * second) // 5 for step in range(1, 5)] + [numpy.zeros_like(first), numpy.full_like(first, 255)]
    uses_eight = first > second
    shows = numpy.zeros(blocks.shape[:2], dtype=bool)
    for texel in range(16):
        index = ((indices >> numpy.uint64(3 * texel)) & numpy.uint64(7)).astype(numpy.int32)
        alpha = numpy.where(uses_eight, numpy.choose(index, eight_levels), numpy.choose(index, six_levels))
        shows |= alpha > 0
    return shows


def is_parallelogram(corners):
    (x0, y0), (x1, y1), (x2, y2), (x3, y3) = corners
    return abs(x0 + x2 - x1 - x3) <= PARALLELOGRAM_SLACK and abs(y0 + y2 - y1 - y3) <= PARALLELOGRAM_SLACK


def cells_in_order(effect):
    cells = []
    seen = {}
    for frame in effect["frames"]:
        if frame["quads"]:
            quad = frame["quads"][0]
            cell = (quad["page"], tuple(int(value) for value in quad["source"]))
            if cell not in seen:
                seen[cell] = len(cells)
                cells.append(cell)
    return cells


def runs_of_squares(shows, left, top, right, bottom):
    square = SQUARE_SIDE // BLOCK_SIDE
    first_column = left // BLOCK_SIDE
    first_row = top // BLOCK_SIDE
    end_column = (right + BLOCK_SIDE - 1) // BLOCK_SIDE
    end_row = (bottom + BLOCK_SIDE - 1) // BLOCK_SIDE
    runs = []
    for row in range(first_row, end_row, square):
        row_end = min(row + square, end_row)
        run_start = None
        run_length = 0
        for column in range(first_column, end_column, square):
            column_end = min(column + square, end_column)
            if shows[row:row_end, column:column_end].any():
                if run_start is None:
                    run_start = column
                    run_length = 0
                run_length = run_length + 1
                run_end = column_end
                if run_length == RUN_SQUARES_MOST:
                    runs.append((run_start, row, run_end, row_end))
                    run_start = None
            elif run_start is not None:
                runs.append((run_start, row, run_end, row_end))
                run_start = None
        if run_start is not None:
            runs.append((run_start, row, run_end, row_end))
    return [(max(left, a * BLOCK_SIDE), max(top, b * BLOCK_SIDE), min(right, c * BLOCK_SIDE), min(bottom, d * BLOCK_SIDE)) for a, b, c, d in runs]


def pieces_of_cell(page, cell, keeps_whole_cell):
    x, y, width, height = cell
    right = x + width
    bottom = y + height
    if keeps_whole_cell:
        shown = page["shows"][y // BLOCK_SIDE:(bottom + BLOCK_SIDE - 1) // BLOCK_SIDE, x // BLOCK_SIDE:(right + BLOCK_SIDE - 1) // BLOCK_SIDE].any()
        inner_rects = [(x, y, right, bottom)] if shown else []
    else:
        inner_rects = runs_of_squares(page["shows"], x, y, right, bottom)
    pieces = []
    for left, top, inner_right, inner_bottom in inner_rects:
        stored = (max(0, (left - GUTTER) // BLOCK_SIDE), max(0, (top - GUTTER) // BLOCK_SIDE), min(page["blocks"].shape[1], (inner_right + GUTTER + BLOCK_SIDE - 1) // BLOCK_SIDE), min(page["blocks"].shape[0], (inner_bottom + GUTTER + BLOCK_SIDE - 1) // BLOCK_SIDE))
        if (stored[2] - stored[0]) * BLOCK_SIDE > PAGE_SIDE_MOST or (stored[3] - stored[1]) * BLOCK_SIDE > PAGE_SIDE_MOST:
            raise ValueError("an effect cell kept whole is larger than a page")
        pieces.append({"inner": (left, top, inner_right, inner_bottom), "stored": stored, "fractions": ((left - x) / width, (top - y) / height, (inner_right - x) / width, (inner_bottom - y) / height)})
    return pieces


def shelf_pack(pieces, side):
    side_blocks = side // BLOCK_SIDE
    places = []
    page = -1
    shelf_x = shelf_y = shelf_height = 0
    used_heights = []
    for piece in pieces:
        width = piece["stored"][2] - piece["stored"][0]
        height = piece["stored"][3] - piece["stored"][1]
        if page < 0 or shelf_x + width > side_blocks:
            shelf_y = shelf_y + shelf_height
            shelf_x = 0
            shelf_height = 0
        if page < 0 or shelf_y + height > side_blocks:
            page = page + 1
            used_heights.append(0)
            shelf_x = shelf_y = shelf_height = 0
        places.append((page, shelf_x, shelf_y))
        shelf_x = shelf_x + width
        shelf_height = max(shelf_height, height)
        used_heights[page] = max(used_heights[page], shelf_y + height)
    return places, used_heights


def pack_pieces(pieces):
    widest = max((piece["stored"][2] - piece["stored"][0]) * BLOCK_SIDE for piece in pieces)
    side = power_of_two_at_least(widest)
    places, used_heights = shelf_pack(pieces, side)
    while len(used_heights) > 1 and side < PAGE_SIDE_MOST:
        side = side * 2
        places, used_heights = shelf_pack(pieces, side)
    for piece, (page, x, y) in zip(pieces, places):
        piece["new_page"] = page
        piece["placed"] = (x, y)
    return [{"width": side, "height": power_of_two_at_least(height * BLOCK_SIDE)} for height in used_heights]


def number(value):
    return asset_writers.format_number(round(value, NUMBER_DIGITS))


def piece_row(piece, page):
    left, top, right, bottom = piece["inner"]
    placed_x = piece["placed"][0] * BLOCK_SIDE + left - piece["stored"][0] * BLOCK_SIDE
    placed_y = piece["placed"][1] * BLOCK_SIDE + top - piece["stored"][1] * BLOCK_SIDE
    numbers = [placed_x / page["width"], placed_y / page["height"], (placed_x + right - left) / page["width"], (placed_y + bottom - top) / page["height"]]
    numbers.extend(piece["fractions"])
    return "fxpiece %d %s" % (piece["new_page"], " ".join(number(value) for value in numbers))


def frame_row(frame, spans):
    if not frame["quads"]:
        return "fxframe 0 0 0 0 0 0 0 0 0 0"
    quad = frame["quads"][0]
    first, count = spans[(quad["page"], tuple(int(value) for value in quad["source"]))]
    corners = []
    for corner in quad["corners"]:
        corners.extend(corner)
    return "fxframe %d %d %s" % (first, count, " ".join(asset_writers.format_number(round(value, 4)) for value in corners))


def page_gtex_bytes(page, pieces, source_pages, texture_format, page_index):
    unit_bytes = asset_writers.BLOCK_BYTES[texture_format]
    blocks = numpy.zeros((page["height"] // BLOCK_SIDE, page["width"] // BLOCK_SIDE, unit_bytes), dtype=numpy.uint8)
    for piece in pieces:
        if piece["new_page"] != page_index:
            continue
        left, top, right, bottom = piece["stored"]
        placed_x, placed_y = piece["placed"]
        blocks[placed_y:placed_y + bottom - top, placed_x:placed_x + right - left] = source_pages[piece["source_page"]]["blocks"][top:bottom, left:right]
    return asset_writers.gtex_bytes(page["width"], page["height"], texture_format, blocks.tobytes())


def layout_of(effect, source_pages):
    formats = set(page["format"] for page in source_pages.values())
    if len(formats) > 1:
        raise ValueError("an effect's pages use more than one texture format")
    keeps_whole_cells = not all(is_parallelogram(frame["quads"][0]["corners"]) for frame in effect["frames"] if frame["quads"])
    pieces = []
    spans = {}
    for cell in cells_in_order(effect):
        page_number, rect = cell
        cell_pieces = pieces_of_cell(source_pages[page_number], rect, keeps_whole_cells)
        for piece in cell_pieces:
            piece["source_page"] = page_number
        spans[cell] = (len(pieces), len(cell_pieces))
        pieces.extend(cell_pieces)
    pages = pack_pieces(pieces) if pieces else []
    return pieces, spans, pages


def movie_lines(header_lines, page_count, body_lines):
    fields = header_lines[0].split()
    fields[6] = str(page_count)
    return [" ".join(fields)] + header_lines[1:] + body_lines


def settings_of(effect):
    return {"version": LAYOUT_VERSION, "page_paths": effect["page_paths"], "page_size": list(effect["page_size"]), "frames": [[[quad["page"], list(quad["source"]), [list(corner) for corner in quad["corners"]]] for quad in frame["quads"]] for frame in effect["frames"]]}


def page_path(name, page):
    return "textures/" + page_texture_name(name, page) + ".gtex"


def stored_bake(record, name, key):
    details = record.details_of(page_path(name, 0))
    if details.get("key") != key or "pages" not in details or "lines" not in details:
        return None
    stored = []
    for page in range(details["pages"]):
        data = record.read_if_current(page_path(name, page), key)
        if data is None:
            return None
        stored.append(data)
    return details["lines"], stored


def bake_movie(packs, name, effect, work_folder, record, code, encode_page):
    source_bytes = [packs.read(path) for path in effect["page_paths"]]
    key = record.key_for(code, settings_of(effect), b"".join(source_bytes))
    kept = stored_bake(record, name, key)
    if kept is not None:
        return kept
    used_pages = sorted(set(cell[0] for cell in cells_in_order(effect)))
    source_pages = {page: blocks_of(encode_page(work_folder, "%s_source_%02d" % (name, page), source_bytes[page], effect["page_size"][0], effect["page_size"][1])) for page in used_pages}
    pieces, spans, pages = layout_of(effect, source_pages)
    lines = [piece_row(piece, pages[piece["new_page"]]) for piece in pieces]
    lines.extend(frame_row(frame, spans) for frame in effect["frames"])
    texture_format = next(iter(source_pages.values()))["format"] if source_pages else asset_writers.FORMAT_DXT5
    stored = []
    for index, page in enumerate(pages):
        data = page_gtex_bytes(page, pieces, source_pages, texture_format, index)
        details = {"key": key, "pages": len(pages), "lines": lines} if index == 0 else None
        record.store(page_path(name, index), key, data, details)
        stored.append(gtex_pack.pack_gtex(data))
    return lines, stored


def check_movie_memory(name, stored):
    memory = 0
    for data in stored:
        width, height, texture_format = asset_writers.read_gtex_header(data)
        memory = memory + asset_writers.memory_bytes(width, height, texture_format & ~gtex_pack.PACKED_FLAG)
    if memory > MOVIE_MEMORY_MOST:
        raise ValueError("effect movie %s keeps %d bytes of pages loaded while it plays, more than %d" % (name, memory, MOVIE_MEMORY_MOST))


def bake_all(packs, movies, work_folder, record, code, encode_page):
    results = []
    texts = []
    with concurrent.futures.ThreadPoolExecutor(max_workers=os.cpu_count()) as pool:
        futures = [pool.submit(bake_movie, packs, movie["name"], movie["effect"], work_folder, record, code, encode_page) for movie in movies]
        for movie, future in zip(movies, futures):
            lines, stored = future.result()
            check_movie_memory(movie["name"], stored)
            texts.extend(movie_lines(movie["header"], len(stored), lines))
            for page, data in enumerate(stored):
                results.append(bake_record.result_of_own_size(page_texture_name(movie["name"], page), data))
    return results, "\n".join(texts) + ("\n" if texts else "")
