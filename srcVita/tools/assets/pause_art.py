import os
import subprocess

import numpy as np

import asset_writers
import bake_record
import frame_layout
import hud_art
import narration
import pause_clock
import pause_gallery
import pause_people
import texture_encoder

CAT_SCREEN_PIXELS_PER_DS_PIXEL = 2 * frame_layout.SCREEN_PIXELS_PER_DS_PIXEL
LINE_TIGHTENING = 8
TWO_LINE_POINTS_LESS = 4
GRID_STEP = 32
SOLID = 128
PAW_ROWS = 5
BODY_BAND = (50, 6)
BODY_WIDTH = 45
BODY_SEARCH = 4
PAGE_SIZE = 1024
CELL_GAP = 2
CAT_TABLE_PATH = "pause/cat.txt"
CAT_TRACK_PATH = "pause/cat_track.txt"
CAT_PAGE_PREFIX = "pause_cat_"
SPOTLIGHT_NAME = "pause_spotlight"
SPOTLIGHT_SCALE = 0.5
SPOTLIGHT_FADE_FROM = 0.78
LABEL_SHEET_NAME = "pause_labels"
LABEL_TABLE_PATH = "pause/labels.txt"
LABEL_POINTS = 36
LABEL_OUTLINE = 4
LABEL_CELL = (320, 64)
ARROW_CELL = 40
CREAM = "#efe7cc"
NAVY = "#1b2a3a"
GHOST_BLUE = "#78e8ff"
WIDGET_SHEET_NAME = "pause_widgets"
WIDGET_TABLE_PATH = "pause/widgets.txt"
WIDGET_SHEET_WIDTH = 1024
WIDGET_SUPERSAMPLE = 4
WHITE = "#ffffff"
HEADER_ORANGE = "#d3763a"
PILL_GREY = "#3e424a"
PILL_RED = "#c41c1c"
BACK_RED = "#c4221c"
BACK_EDGE = "#c8ccd2"
FLAME_LIT = "#78c8f0"
FLAME_DARK = "#343840"
NUMBER_DARK = "#9696a0"
METER_STEPS = 10
FLAME_CELL = (40, 52)
FLAME_SIZE = 34
ARROW_DOWN_CELL = (20, 14)
SLICES = [
    ("row", 70, 62, 31, "black", PILL_GREY, 4),
    ("row_selected", 70, 62, 31, "black", PILL_RED, 4),
    ("value", 56, 48, 24, "black", PILL_GREY, 3),
    ("back", 42, 34, 17, BACK_RED, BACK_EDGE, 4),
    ("choice", 42, 34, 17, "none", PILL_RED, 3),
    ("box", 56, 56, 22, "black", PILL_GREY, 4),
    ("button", 64, 56, 28, "black", PILL_GREY, 4),
    ("button_selected", 64, 56, 28, "black", PILL_RED, 4),
    ("tab", 44, 40, 20, "black", PILL_GREY, 3),
    ("tab_selected", 44, 40, 20, PILL_RED, BACK_EDGE, 3),
    ("sprocket", 11, 7, 2, "#46464c", "none", 0),
    ("note_box", 24, 24, 10, "#faf8f2", "#968c82", 2),
    ("save_panel", 40, 40, 12, "#2a0808", PILL_GREY, 3),
    ("save_panel_selected", 40, 40, 12, "#2a0808", "#d3763a", 5),
]


def remaster_page(packs, tool_path, work_folder, pack_path, crop):
    data = packs.read(pack_path)
    width, height = texture_encoder.read_tex_size(data)
    tex_path = os.path.join(work_folder, "pause_page.tex")
    rgba_path = os.path.join(work_folder, "pause_page.rgba")
    with open(tex_path, "wb") as output:
        output.write(data)
    texture_encoder.run_texture_tool(tool_path, tex_path, rgba_path, width, height)
    with open(rgba_path, "rb") as source:
        pixels = np.frombuffer(source.read()[:width * height * 4], dtype=np.uint8).reshape(height, width, 4)
    os.remove(tex_path)
    os.remove(rgba_path)
    x, y, w, h = crop
    return pixels[y:y + h, x:x + w].copy()


def cell_pixels(page, frame, mirror):
    height, width = page.shape[:2]
    x = round(frame["u"] * width)
    y = round(frame["v"] * height)
    w = round(frame["u_size"] * width)
    h = round(frame["v_size"] * height)
    cell = page[y:y + h, x:x + w]
    return cell[:, ::-1] if mirror else cell


def frame_key(frame):
    return (frame["texture"], frame["u"], frame["v"], frame["u_size"], frame["v_size"], frame["x"], frame["y"], frame["width"])


def source_left_top(frame, cell, mirror):
    pixels_per_ds = cell.shape[1] / frame["width"]
    left = -(frame["x"] + frame["width"]) * pixels_per_ds if mirror else frame["x"] * pixels_per_ds
    return round(left), round(frame["y"] * pixels_per_ds)


def paw_point(solid):
    bottom = int(np.nonzero(solid.max(1))[0].max())
    toe = int(np.nonzero(solid[bottom - PAW_ROWS + 1:bottom + 1].max(0))[0].min())
    return toe, bottom


def shifted(solid, dx, dy):
    out = np.zeros_like(solid)
    ys, xs = np.nonzero(solid)
    ys, xs = ys + dy, xs + dx
    keep = (ys >= 0) & (ys < solid.shape[0]) & (xs >= 0) & (xs < solid.shape[1])
    out[ys[keep], xs[keep]] = True
    return out


def body_band(solid, toe, bottom):
    return solid[bottom - BODY_BAND[0]:bottom - BODY_BAND[1], toe:toe + BODY_WIDTH]


def placed_mask(cell, left, top, origin, size):
    board = np.zeros(size, dtype=bool)
    x, y = left - origin[0], top - origin[1]
    board[y:y + cell.shape[0], x:x + cell.shape[1]] = cell[:, :, 3] >= SOLID
    return board


def pin_frames(cells):
    lefts = [c["left"] for c in cells]
    tops = [c["top"] for c in cells]
    origin = (min(lefts), min(tops))
    size = (max(c["top"] + c["pixels"].shape[0] for c in cells) - origin[1] + 2 * BODY_SEARCH + 2, max(c["left"] + c["pixels"].shape[1] for c in cells) - origin[0] + 2 * BODY_SEARCH + 2)
    masks = [placed_mask(c["pixels"], c["left"], c["top"], origin, size) for c in cells]
    reference = next(index for index, mask in enumerate(masks) if mask.any())
    ref_toe, ref_bottom = paw_point(masks[reference])
    target = body_band(masks[reference], ref_toe, ref_bottom)
    pins = []
    for mask in masks:
        if not mask.any():
            pins.append((0, 0))
            continue
        toe, bottom = paw_point(mask)
        dx, dy = ref_toe - toe, ref_bottom - bottom
        best = min(range(-BODY_SEARCH, BODY_SEARCH + 1), key=lambda k: (int(np.logical_xor(body_band(shifted(mask, dx, dy + k), ref_toe, ref_bottom), target).sum()), abs(k)))
        pins.append((dx, dy + best))
    ref_mask = masks[reference]
    lower = ref_mask[ref_bottom - BODY_BAND[0]:ref_bottom + 1]
    columns = lower.sum(0)
    anchor_x = round(float((np.arange(len(columns)) * columns).sum() / columns.sum())) + origin[0]
    anchor_y = ref_bottom + 1 + origin[1]
    return [(c["left"] + dx - anchor_x, c["top"] + dy - anchor_y) for c, (dx, dy) in zip(cells, pins)]


def grid_for(placed):
    left = min(l for _, l, _ in placed)
    top = min(t for _, _, t in placed)
    right = max(l + p.shape[1] for p, l, _ in placed)
    bottom = max(t + p.shape[0] for p, _, t in placed)
    anchor_x = -(left // GRID_STEP) * GRID_STEP
    anchor_y = -(top // GRID_STEP) * GRID_STEP
    width = -(-(anchor_x + right) // GRID_STEP) * GRID_STEP
    height = -(-(anchor_y + bottom) // GRID_STEP) * GRID_STEP
    return width, height, anchor_x, anchor_y


def scaled_grid(work_folder, grid_shape, pixels, left, top):
    grid_width, grid_height, anchor_x, anchor_y = grid_shape
    grid = np.zeros((grid_height, grid_width, 4), dtype=np.uint8)
    x, y = anchor_x + left, anchor_y + top
    h, w = pixels.shape[:2]
    grid[y:y + h, x:x + w] = pixels
    factor = CAT_SCREEN_PIXELS_PER_DS_PIXEL / frame_layout.ART_PER_DS_PIXEL
    out_w, out_h = round(grid_width * factor), round(grid_height * factor)
    in_path = os.path.join(work_folder, "pause_grid.rgba")
    out_path = os.path.join(work_folder, "pause_grid_out.rgba")
    with open(in_path, "wb") as output:
        output.write(grid.tobytes())
    subprocess.run(["magick", "-size", "%dx%d" % (grid_width, grid_height), "-depth", "8", "rgba:" + in_path, "-filter", "Lanczos", "-resize", "%dx%d!" % (out_w, out_h), "-depth", "8", "rgba:" + out_path], check=True, capture_output=True)
    with open(out_path, "rb") as source:
        scaled = np.frombuffer(source.read(), dtype=np.uint8).reshape(out_h, out_w, 4)
    os.remove(in_path)
    os.remove(out_path)
    ys, xs = np.nonzero(scaled[:, :, 3])
    x0, y0, x1, y1 = xs.min(), ys.min(), xs.max() + 1, ys.max() + 1
    anchor = (round(anchor_x * factor), round(anchor_y * factor))
    return scaled[y0:y1, x0:x1].copy(), int(x0 - anchor[0]), int(y0 - anchor[1])


def pack_cells(images):
    pages, placements = [], []
    x = y = shelf = 0
    page = 0
    for image in images:
        h, w = image.shape[:2]
        if x + w > PAGE_SIZE:
            x, y, shelf = 0, y + shelf + CELL_GAP, 0
        if y + h > PAGE_SIZE:
            page, x, y, shelf = page + 1, 0, 0, 0
        while len(pages) <= page:
            pages.append(np.zeros((PAGE_SIZE, PAGE_SIZE, 4), dtype=np.uint8))
        pages[page][y:y + h, x:x + w] = image
        placements.append((page, x, y, w, h))
        x += w + CELL_GAP
        shelf = max(shelf, h)
    return pages, placements


FRAME_FIELDS = ("x", "y", "width", "height", "u", "v", "u_size", "v_size")


def frames_from_text(text):
    frames = []
    for line in text.splitlines():
        parts = line.split()
        if parts and parts[0] == "frame":
            frame = {"texture": parts[1], "ticks": int(parts[2])}
            frame.update({key: float(value) for key, value in zip(FRAME_FIELDS, parts[3:])})
            frames.append(frame)
    return frames


def cat_frames(packs, tool_path, work_folder, timelines, crops, animations):
    pages = {}
    per_animation = []
    for animation in animations:
        timeline = timelines[animation["timeline"]]
        frames = frames_from_text(animation["text"])
        paths = {frame_layout.texture_name_for(path): path for path in timeline["pages"]}
        cells, keys = [], {}
        for index, frame in enumerate(frames):
            if frame["texture"] not in pages:
                pages[frame["texture"]] = remaster_page(packs, tool_path, work_folder, frame_layout.pack_path_for(paths[frame["texture"]]), crops[frame["texture"]])
            pixels = cell_pixels(pages[frame["texture"]], frame, animation["mirror"])
            left, top = source_left_top(frame, pixels, animation["mirror"])
            cells.append({"pixels": pixels, "left": left, "top": top, "ticks": frame["ticks"], "key": frame_key(frame)})
        per_animation.append((animation["name"], cells, pin_frames(cells)))
    return per_animation


def bake_cat(packs, tool_path, work_folder, record, code, timelines, crops, animations):
    settings = {"animations": animations, "crops": {name: list(crop) for name, crop in sorted(crops.items()) if name.startswith(tuple(a["name"][len(TABLE_PREFIX):] for a in animations))}, "grid_step": GRID_STEP, "scale": CAT_SCREEN_PIXELS_PER_DS_PIXEL}
    source = b"".join(packs.read(frame_layout.pack_path_for(path)) for animation in animations for path in timelines[animation["timeline"]]["pages"])
    key = record.key_for(code, settings, source)
    table = record.read_if_current(CAT_TABLE_PATH, key)
    if table is not None:
        page_count = int(table.decode("ascii").splitlines()[0].split()[1])
        return table, [bake_record.result_of_own_size(CAT_PAGE_PREFIX + "%02d" % n, record.read_if_current("textures/" + CAT_PAGE_PREFIX + "%02d.gtex" % n, key)) for n in range(page_count)]
    images, rows, seen = [], [], {}
    baked = cat_frames(packs, tool_path, work_folder, timelines, crops, animations)
    grid_shape = grid_for([(cell["pixels"], left, top) for _, cells, pins in baked for cell, (left, top) in zip(cells, pins)])
    for name, cells, pins in baked:
        for index, (cell, (left, top)) in enumerate(zip(cells, pins)):
            unique = (cell["key"], left, top)
            if unique not in seen:
                image, offset_x, offset_y = scaled_grid(work_folder, grid_shape, cell["pixels"], left, top)
                seen[unique] = (len(images), offset_x, offset_y)
                images.append(image)
            rows.append((name, index, cell["ticks"]) + seen[unique])
    pages, placements = pack_cells(images)
    lines = ["pages %d" % len(pages)]
    for name, index, ticks, image_index, offset_x, offset_y in rows:
        page, x, y, w, h = placements[image_index]
        lines.append("frame %s %d %d %d %d %d %d %d %d %d" % (name, index, ticks, page, x, y, w, h, offset_x, offset_y))
    table = ("\n".join(lines) + "\n").encode("ascii")
    results = []
    for number, page in enumerate(pages):
        name = CAT_PAGE_PREFIX + "%02d" % number
        blocks = texture_encoder.encode_rgba(work_folder, name, PAGE_SIZE, PAGE_SIZE, page.tobytes(), "dxt5")
        gtex = asset_writers.gtex_bytes(PAGE_SIZE, PAGE_SIZE, asset_writers.FORMAT_DXT5, blocks)
        record.store("textures/" + name + ".gtex", key, gtex)
        results.append(bake_record.result_of_own_size(name, gtex))
    record.store(CAT_TABLE_PATH, key, table)
    return table, results


def spotlight_pixels(art):
    row = art[art.shape[0] // 2].astype(float)
    first = int(np.argmax(row[:, 3] > 0))
    profile = row.copy()
    profile[:first] = row[first]
    edge = len(profile)
    half = int(edge * SPOTLIGHT_SCALE)
    ys, xs = np.mgrid[-half:half, -half:half]
    radius = np.sqrt(xs ** 2 + ys ** 2) / SPOTLIGHT_SCALE
    out = profile[np.clip(radius.astype(int), 0, edge - 1)]
    out[..., 3] *= np.clip((edge - radius) / (edge * (1 - SPOTLIGHT_FADE_FROM)), 0, 1)
    return out.clip(0, 255).astype(np.uint8)


def bake_spotlight(packs, tool_path, work_folder, record, code, source_path):
    data = packs.read(source_path)
    key = record.key_for(code, {"scale": SPOTLIGHT_SCALE, "fade_from": SPOTLIGHT_FADE_FROM}, data)
    relative_path = "textures/" + SPOTLIGHT_NAME + ".gtex"
    gtex = record.read_if_current(relative_path, key)
    if gtex is None:
        width, height = texture_encoder.read_tex_size(data)
        art = remaster_page(packs, tool_path, work_folder, source_path, (0, 0, width, height))
        pixels = spotlight_pixels(art)
        gtex = asset_writers.gtex_bytes(pixels.shape[1], pixels.shape[0], asset_writers.FORMAT_RGBA8888, pixels.tobytes())
        record.store(relative_path, key, gtex)
    return bake_record.result_of_own_size(SPOTLIGHT_NAME, gtex)


def label_arguments(font_path, word, fill, x, y):
    centre = "+%d+%d" % (x, y)
    return ["-gravity", "NorthWest", "-font", font_path, "-pointsize", str(LABEL_POINTS), "-fill", NAVY, "-stroke", NAVY, "-strokewidth", str(2 * LABEL_OUTLINE), "-annotate", centre, word, "-fill", fill, "-stroke", "none", "-annotate", centre, word]


def arrow_arguments(x, y, points_right):
    m = ARROW_CELL // 2
    tip = (x + m + 9, y + m) if points_right else (x + m - 9, y + m)
    back = x + m - 7 if points_right else x + m + 7
    polygon = "polygon %d,%d %d,%d %d,%d" % (back, y + m - 9, tip[0], tip[1], back, y + m + 9)
    return ["-fill", GHOST_BLUE, "-stroke", NAVY, "-strokewidth", "2", "-draw", polygon]


def word_widths(font_path, words):
    widths = []
    for word in words:
        result = subprocess.run(["magick", "-font", font_path, "-pointsize", str(LABEL_POINTS), "-strokewidth", str(2 * LABEL_OUTLINE), "-stroke", NAVY, "label:" + word, "-format", "%w %h", "info:"], check=True, capture_output=True, text=True)
        widths.append(tuple(int(value) for value in result.stdout.split()))
    return widths


def bake_labels(packs, work_folder, record, code, font, words):
    fonts = narration.TextFonts(packs, work_folder)
    key = record.key_for(code, {"words": words, "points": LABEL_POINTS, "outline": LABEL_OUTLINE, "font": bake_record.sha256_of_bytes(fonts.pack_bytes(font))}, b"")
    relative_path = "textures/" + LABEL_SHEET_NAME + ".gtex"
    gtex = record.read_if_current(relative_path, key)
    table = record.read_if_current(LABEL_TABLE_PATH, key)
    if gtex is None or table is None:
        font_path = fonts.decoded_path(font)
        sizes = word_widths(font_path, words)
        width, height = 2 * LABEL_CELL[0], len(words) * LABEL_CELL[1] + ARROW_CELL
        sheet_width, sheet_height = asset_writers.round_up_to_power_of_two(width), asset_writers.round_up_to_power_of_two(height)
        arguments = ["magick", "-size", "%dx%d" % (sheet_width, sheet_height), "xc:none"]
        lines = []
        for row, (word, (w, h)) in enumerate(zip(words, sizes)):
            for column, fill in enumerate((CREAM, GHOST_BLUE)):
                x, y = column * LABEL_CELL[0] + LABEL_OUTLINE, row * LABEL_CELL[1] + LABEL_OUTLINE
                arguments += label_arguments(font_path, word, fill, x, y)
            lines.append("label %d %d %d %d %d" % (row, 0, row * LABEL_CELL[1], min(w + 2 * LABEL_OUTLINE, LABEL_CELL[0]), min(h + 2 * LABEL_OUTLINE, LABEL_CELL[1])))
        arrow_y = len(words) * LABEL_CELL[1]
        arguments += arrow_arguments(0, arrow_y, True) + arrow_arguments(ARROW_CELL, arrow_y, False)
        lines.append("arrows %d %d %d" % (0, arrow_y, ARROW_CELL))
        lines.append("selected_column %d" % LABEL_CELL[0])
        rgba_path = os.path.join(work_folder, LABEL_SHEET_NAME + ".rgba")
        subprocess.run(arguments + ["-depth", "8", "rgba:" + rgba_path], check=True, capture_output=True)
        with open(rgba_path, "rb") as source:
            pixels = source.read()
        os.remove(rgba_path)
        gtex = asset_writers.gtex_bytes(sheet_width, sheet_height, asset_writers.FORMAT_RGBA8888, pixels)
        table = ("\n".join(lines) + "\n").encode("ascii")
        record.store(relative_path, key, gtex)
        record.store(LABEL_TABLE_PATH, key, table)
    return table, bake_record.result_of_own_size(LABEL_SHEET_NAME, gtex)


TABLE_PREFIX = "t_"


def cat_animation_entries(set_animations, table_texts, wanted):
    by_name = {animation["name"]: animation for animation in set_animations}
    missing = [entry["name"] for entry in wanted if entry["name"] not in by_name or TABLE_PREFIX + entry["name"] not in table_texts]
    if missing:
        raise ValueError("pause cat animations not in the set: " + ", ".join(missing))
    return [{"name": TABLE_PREFIX + entry["name"], "timeline": by_name[entry["name"]]["timeline"], "mirror": entry["mirror"], "text": table_texts[TABLE_PREFIX + entry["name"]]} for entry in wanted]


def tick_rows(rows, name):
    out = []
    for row, (anim, ticks) in enumerate(rows):
        if anim == name:
            out += [row] * max(1, ticks)
    return out


def played_ticks(ticks, play):
    out = []
    for first, last in play:
        step = 1 if last >= first else -1
        out += ticks[first:last + step:step] if last + step >= 0 else ticks[first::step]
    return out


def cat_track_text(cat_table, animations, fidget):
    rows = [(parts[1], int(parts[3])) for parts in (line.split() for line in cat_table.decode("ascii").splitlines()) if parts and parts[0] == "frame"]
    idle = tick_rows(rows, animations[0]["name"])
    turn = played_ticks(tick_rows(rows, animations[fidget["animation"]]["name"]), fidget["play"])
    lines = ["every %d" % fidget["every"], "link %d" % fidget["link"], "dissolve %d" % fidget["dissolve"]]
    lines += ["idle %d" % row for row in idle] + ["fidget %d" % row for row in turn]
    return ("\n".join(lines) + "\n").encode("ascii")


def magick_rgba(work_folder, width, height, arguments, out_width=None, out_height=None):
    out_width, out_height = out_width or width, out_height or height
    rgba_path = os.path.join(work_folder, "pause_widget.rgba")
    subprocess.run(["magick", "-size", "%dx%d" % (width, height), "xc:none"] + arguments + ["-depth", "8", "rgba:" + rgba_path], check=True, capture_output=True)
    with open(rgba_path, "rb") as source:
        pixels = np.frombuffer(source.read(), dtype=np.uint8).reshape(out_height, out_width, 4).copy()
    os.remove(rgba_path)
    return pixels


def supersampled(work_folder, width, height, arguments):
    k = WIDGET_SUPERSAMPLE
    return magick_rgba(work_folder, width * k, height * k, arguments + ["-filter", "Box", "-resize", "%dx%d!" % (width, height)], width, height)


def slice_pixels(work_folder, width, height, radius, fill, stroke, stroke_width):
    k = WIDGET_SUPERSAMPLE
    inset = stroke_width * k // 2
    inner = radius * k - inset
    shape = "roundrectangle %d,%d %d,%d %d,%d" % (inset, inset, width * k - inset - 1, height * k - inset - 1, inner, inner)
    return supersampled(work_folder, width, height, ["-fill", fill, "-stroke", stroke, "-strokewidth", str(stroke_width * k), "-draw", shape])


def flame_pixels(work_folder, font_path, number, lit, points):
    k = WIDGET_SUPERSAMPLE
    cx, cy = FLAME_CELL[0] * k / 2, FLAME_CELL[1] * k / 2
    size = FLAME_SIZE * k
    r = size * 0.42
    ellipse = "ellipse %d,%d %d,%d 0,360" % (cx, cy + size * 0.18, r, r)
    flame_tip = "polygon %d,%d %d,%d %d,%d" % (cx - r * 0.95, cy + size * 0.1, cx + r * 0.95, cy + size * 0.1, cx + r * 0.25, cy - size * 0.62)
    color = FLAME_LIT if lit else FLAME_DARK
    number_color = WHITE if lit else NUMBER_DARK
    return supersampled(work_folder, FLAME_CELL[0], FLAME_CELL[1], ["-fill", color, "-stroke", "none", "-draw", ellipse, "-draw", flame_tip, "-font", font_path, "-pointsize", str(points * k), "-fill", number_color, "-gravity", "center", "-annotate", "+0+%d" % round(size * 0.2), str(number)])


def arrow_down_pixels(work_folder):
    w, h = ARROW_DOWN_CELL
    return supersampled(work_folder, w, h, ["-fill", WHITE, "-draw", "polygon %d,%d %d,%d %d,%d" % (WIDGET_SUPERSAMPLE, WIDGET_SUPERSAMPLE, (w - 1) * WIDGET_SUPERSAMPLE, WIDGET_SUPERSAMPLE, w * WIDGET_SUPERSAMPLE // 2, (h - 1) * WIDGET_SUPERSAMPLE)])


def line_layout(points, text):
    if "\n" not in text:
        return []
    return ["-gravity", "center", "-interline-spacing", str(-points // LINE_TIGHTENING)]


def text_pixels(work_folder, font_path, points, color, text):
    layout = line_layout(points, text)
    size = subprocess.run(["magick", "-background", "none", "-font", font_path, "-pointsize", str(points)] + layout + ["label:" + text, "-format", "%w %h", "info:"], check=True, capture_output=True, text=True).stdout.split()
    width, height = int(size[0]), int(size[1])
    rgba_path = os.path.join(work_folder, "pause_text.rgba")
    subprocess.run(["magick", "-background", "none", "-fill", color, "-font", font_path, "-pointsize", str(points)] + layout + ["label:" + text, "-depth", "8", "rgba:" + rgba_path], check=True, capture_output=True)
    with open(rgba_path, "rb") as source:
        pixels = np.frombuffer(source.read(), dtype=np.uint8).reshape(height, width, 4).copy()
    os.remove(rgba_path)
    return pixels


def widget_cells(work_folder, fonts, widgets):
    font = fonts.decoded_path(widgets["font"])
    cells = []
    for style, color, points in (("row", WHITE, widgets["row_points"]), ("header", HEADER_ORANGE, widgets["row_points"]), ("bar", WHITE, widgets["bar_points"])):
        for key, text in widgets[style].items():
            cells.append(("text", style + "." + key, text_pixels(work_folder, font, points - TWO_LINE_POINTS_LESS if "\n" in text else points, color, text)))
    for number in range(1, widgets["chapter_count"] + 1):
        cells.append(("text", "row.chapter_%d" % number, text_pixels(work_folder, font, widgets["row_points"], WHITE, "%s %d" % (widgets["chapter_word"], number))))
    for language in widgets["languages"]:
        cells.append(("text", "language." + language["key"], text_pixels(work_folder, fonts.decoded_path(language["font"]), widgets["bar_points"], WHITE, language["text"])))
    for name, width, height, radius, fill, stroke, stroke_width in SLICES:
        cells.append(("slice", "%s %d" % (name, radius), slice_pixels(work_folder, width, height, radius, fill, stroke, stroke_width)))
    for number in range(1, METER_STEPS + 1):
        for lit in (0, 1):
            cells.append(("flame", "%d %d" % (number, lit), flame_pixels(work_folder, font, number, lit, widgets["number_points"])))
    cells.append(("cell", "arrow_down", arrow_down_pixels(work_folder)))
    return cells


def pack_widget_cells(cells):
    order = sorted(range(len(cells)), key=lambda index: -cells[index][2].shape[0])
    places = [None] * len(cells)
    x = y = shelf = 0
    for index in order:
        h, w = cells[index][2].shape[:2]
        if x + w > WIDGET_SHEET_WIDTH:
            x, y, shelf = 0, y + shelf + CELL_GAP, 0
        places[index] = (x, y)
        x += w + CELL_GAP
        shelf = max(shelf, h)
    height = asset_writers.round_up_to_power_of_two(y + shelf)
    sheet = np.zeros((height, WIDGET_SHEET_WIDTH, 4), dtype=np.uint8)
    lines = []
    for (kind, name, pixels), (x, y) in zip(cells, places):
        h, w = pixels.shape[:2]
        sheet[y:y + h, x:x + w] = pixels
        lines.append("%s %s %d %d %d %d" % (kind, name, x, y, w, h))
    return sheet, lines


def glyph_lines():
    return ["glyph %s %d %d %d %d" % (kind, x, y, width, hud_art.GLYPH_CELL) for kind, x, y, width in hud_art.glyph_cells()]


def preview_lines(widgets):
    preview = widgets.get("preview")
    if preview is None:
        return []
    return ["preview %s %d %d %d %d" % tuple([preview["plate"]] + list(preview["crop"]))]


def bake_widgets(packs, work_folder, record, code, widgets):
    fonts = narration.TextFonts(packs, work_folder)
    font_bytes = b"".join(fonts.pack_bytes(path) for path in sorted(set([widgets["font"]] + [language["font"] for language in widgets["languages"]])))
    key = record.key_for(code, {"widgets": widgets, "slices": SLICES, "flame": [FLAME_CELL, FLAME_SIZE], "glyphs": glyph_lines()}, font_bytes)
    relative_path = "textures/" + WIDGET_SHEET_NAME + ".gtex"
    gtex = record.read_if_current(relative_path, key)
    table = record.read_if_current(WIDGET_TABLE_PATH, key)
    if gtex is None or table is None:
        sheet, lines = pack_widget_cells(widget_cells(work_folder, fonts, widgets))
        table = ("\n".join(lines + glyph_lines() + preview_lines(widgets)) + "\n").encode("utf-8")
        gtex = asset_writers.gtex_bytes(sheet.shape[1], sheet.shape[0], asset_writers.FORMAT_RGBA8888, sheet.tobytes())
        record.store(relative_path, key, gtex)
        record.store(WIDGET_TABLE_PATH, key, table)
    return table, bake_record.result_of_own_size(WIDGET_SHEET_NAME, gtex)


def bake_pause_art(packs, tool_path, work_folder, record, code, pause, set_animations, table_texts, timelines, crops, text_key):
    files = {}
    animations = cat_animation_entries(set_animations, table_texts, pause["cat_animations"])
    cat_table, cat_results = bake_cat(packs, tool_path, work_folder, record, code, timelines, crops, animations)
    files[CAT_TABLE_PATH] = cat_table
    files[CAT_TRACK_PATH] = cat_track_text(cat_table, animations, pause["cat_fidget"])
    label_table, label_result = bake_labels(packs, work_folder, record, code, pause["label_font"], pause["labels"])
    files[LABEL_TABLE_PATH] = label_table
    spotlight = bake_spotlight(packs, tool_path, work_folder, record, code, pause["spotlight_source"])
    widget_table, widget_result = bake_widgets(packs, work_folder, record, code, pause["widgets"])
    files[WIDGET_TABLE_PATH] = widget_table
    people_table, people_results = pause_people.bake_people(packs, tool_path, work_folder, record, code, pause["people"], pause["widgets"]["font"], text_key)
    gallery_lines, gallery_results = pause_gallery.bake_gallery(packs, tool_path, work_folder, record, code, pause["people"]["illustrations"]) if "illustrations" in pause["people"] else ([], [])
    files[pause_people.TABLE_PATH] = people_table + "".join(line + "\n" for line in gallery_lines).encode("ascii")
    clock_results = []
    if "clock" in pause:
        files[pause_clock.CLOCK_TABLE_PATH], clock_result = pause_clock.bake_clock(packs, work_folder, record, code, pause["clock"], pause["label_font"], pause["people"], text_key)
        clock_results = [clock_result]
    return files, cat_results + [label_result, spotlight, widget_result] + people_results + gallery_results + clock_results
