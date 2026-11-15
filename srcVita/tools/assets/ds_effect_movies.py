import json
import struct

import numpy

import asset_writers
import bake_record
import chapter_assets
import ds_effects
import ds_textures
import effect_file
import effect_movies

MAGIC = b"bws."
FIXED_ONE = 4096.0
FRAME_HEADER = "<HH"
FRAME_HEADER_OFFSET = 8
SECTIONS_BASE = 0x40
SECTION_FIELDS = (0x30, 0x34, 0x38, 0x3c)
ITEM_SIZE = 44
FILL_SIZE = 28
SHAPE_HEADER_SIZE = 20
ACTION_ARG_SIZE = 8
COLOR_LEVELS = 31.0
ALPHA_FULL = 100.0
NO_LABEL = -1
ROOT_SPRITE = 0
TEXTURE_ITEM_AFTER_SCRIPT = 1
ARG_KIND_NAME = 4
NAME_TABLE_THIS = 1
NAME_TABLE_ROOT = 2
NAME_TABLE_BUILT_IN = 5
NAME_TABLE_OWN = 6
BUILT_IN_NAMES = {0x8000: "gotoAndPlay", 0x8003: "fscommand2", 0x8005: "removeMovieClip", 0x8006: "setVertex"}
FIXED_NAMES = {NAME_TABLE_THIS: "this", NAME_TABLE_ROOT: "_root"}
VERTEX_COUNT = 4
MOVIE_NAME = "ds_effect_%03d"
TEXTURE_FOLDER = "textures"
PAGE_WIDTH = 1024
PAGE_HEIGHT_MOST = 2048
CELL_GUTTER = 2
MOVIE_MEMORY_MOST = 16 * 1024 * 1024
RGBA_BYTES = 4
DEGENERATE = 1e-6
CODE_FILES = ["ds_effect_movies.py", "ds_effects.py", "ds_textures.py", "effect_file.py", "effect_movies.py", "asset_writers.py"]


class EffectMovieRefused(ValueError):
    pass


class Reader:
    def __init__(self, data):
        self.data = data

    def word(self, offset):
        if offset < 0 or offset + 4 > len(self.data):
            raise EffectMovieRefused("a DS effect movie reads past its end at 0x%x" % offset)
        return struct.unpack_from("<i", self.data, offset)[0]

    def halves(self, offset):
        if offset < 0 or offset + 4 > len(self.data):
            raise EffectMovieRefused("a DS effect movie reads past its end at 0x%x" % offset)
        return struct.unpack_from("<HH", self.data, offset)

    def table(self, offset):
        count = self.word(offset)
        if count < 0 or offset + 4 + 4 * count > len(self.data):
            raise EffectMovieRefused("a DS effect movie's table at 0x%x runs past its end" % offset)
        after = offset + 4 + 4 * count
        return [after + self.word(offset + 4 + 4 * index) for index in range(count)]

    def matrix(self, offset):
        return tuple(self.word(offset + 4 * index) / FIXED_ONE for index in range(6))

    def name(self, offset):
        end = self.data.find(b"\0", offset)
        if offset < 0 or end < 0:
            raise EffectMovieRefused("a DS effect movie's name at 0x%x has no end" % offset)
        return self.data[offset:end].decode("latin-1")


def read_shape(reader, offset):
    shape_id, fill_count = reader.halves(offset)
    rect = tuple(reader.word(offset + 4 + 4 * index) for index in range(4))
    fills = []
    for index in range(fill_count):
        place = offset + SHAPE_HEADER_SIZE + FILL_SIZE * index
        fills.append({"bitmap": reader.word(place), "matrix": reader.matrix(place + 4)})
    return shape_id, {"rect": rect, "fills": fills}


def read_item(reader, offset):
    slot, character = reader.halves(offset)
    depth = reader.halves(offset + 4)[0]
    first = reader.word(offset + 8)
    matrix = reader.matrix(offset + 12)
    red, green, blue = reader.data[offset + 36:offset + 39]
    alpha = reader.word(offset + 40) / FIXED_ONE / ALPHA_FULL
    return {"slot": slot, "character": character, "depth": depth, "first": first, "matrix": matrix, "rgba": (red / COLOR_LEVELS, green / COLOR_LEVELS, blue / COLOR_LEVELS, alpha)}


def read_frame(reader, offset, names):
    label, count = reader.word(offset), reader.word(offset + 4)
    if count < 0:
        raise EffectMovieRefused("a DS effect movie frame has a negative item count")
    if label != NO_LABEL and not 0 <= label < len(names):
        raise EffectMovieRefused("a DS effect movie frame names label %d past its name table" % label)
    items = [read_item(reader, offset + 8 + ITEM_SIZE * index) for index in range(count)]
    return {"label": names[label] if label != NO_LABEL else "", "items": items}


def read_actions(reader, offset, names):
    actions = []
    while True:
        if offset + 4 > len(reader.data):
            raise EffectMovieRefused("a DS effect movie's actions run past its end")
        op, _, arg_count, _ = reader.data[offset:offset + 4]
        args = []
        for index in range(arg_count):
            place = offset + 4 + ACTION_ARG_SIZE * index
            kind, table = reader.data[place], reader.data[place + 1]
            args.append({"type": "arg", "kind": kind, "index": table, "value": reader.word(place + 4), "text": ""})
        for arg in args:
            arg["text"] = arg_text(op, arg, names)
        actions.append({"type": "action", "op": op, "push_count": arg_count if op == effect_file.OP_PUSH else 0, "arg_count": arg_count, "args": args})
        offset = offset + 4 + ACTION_ARG_SIZE * arg_count
        if op == effect_file.OP_END:
            return actions, offset


def arg_text(op, arg, names):
    if op == effect_file.OP_GOTO_LABEL:
        return names[arg["value"]] if 0 <= arg["value"] < len(names) else ""
    if arg["kind"] != ARG_KIND_NAME:
        return ""
    if arg["index"] == NAME_TABLE_OWN:
        return names[arg["value"]] if 0 <= arg["value"] < len(names) else ""
    if arg["index"] == NAME_TABLE_BUILT_IN:
        return BUILT_IN_NAMES.get(arg["value"], "built_in_%x" % arg["value"])
    return FIXED_NAMES.get(arg["index"], "")


def script_records(actions):
    records = [None]
    refs = []
    for action in actions:
        arg_refs = []
        for arg in action["args"]:
            records.append(arg)
            arg_refs.append(len(records) - 1)
        records.append(dict(action, args=arg_refs))
        refs.append(len(records) - 1)
    return records, {"type": "do_action", "actions": refs}


def script_of(frame, actions):
    records, do_action = script_records(actions)
    ops = set(action["op"] for action in actions)
    rows = effect_file.UNRESOLVED if ops & effect_file.BRANCH_OPS else effect_file.frame_script_rows(records, do_action)
    return {"frame": frame, "calls": effect_file.run_script(records, do_action), "rows": rows}


def read_sprite(reader, offset, names, fps):
    frame_count, sprite_id = reader.halves(offset)
    base = offset + 12
    scripts_at = base + reader.word(offset + 8)
    frames = [read_frame(reader, frame_offset, names) for frame_offset in reader.table(base)]
    if len(frames) != frame_count:
        raise EffectMovieRefused("a DS effect movie sprite lists %d frames but holds %d" % (frame_count, len(frames)))
    scripts = []
    for script_offset in reader.table(scripts_at):
        actions, _ = read_actions(reader, script_offset + 12, names)
        scripts.append(script_of(reader.word(script_offset), actions))
    return {"sprite_id": sprite_id, "fps": fps, "frames": frames, "scripts": scripts}


def read_movie(movie_bytes):
    if movie_bytes[:4] != MAGIC:
        raise EffectMovieRefused("a DS effect movie does not begin with its magic")
    reader = Reader(movie_bytes)
    _, fps = struct.unpack_from(FRAME_HEADER, movie_bytes, FRAME_HEADER_OFFSET)
    if fps <= 0:
        raise EffectMovieRefused("a DS effect movie has no frame rate")
    sections = [SECTIONS_BASE] + [SECTIONS_BASE + reader.word(field) for field in SECTION_FIELDS]
    bitmap_ids = [reader.word(place) for place in reader.table(sections[0])]
    shapes = dict(read_shape(reader, place) for place in reader.table(sections[1]))
    names = [reader.name(place) for place in reader.table(sections[4])]
    sprites = [read_sprite(reader, place, names, fps) for place in reader.table(sections[3])]
    if not any(sprite["sprite_id"] == ROOT_SPRITE for sprite in sprites):
        raise EffectMovieRefused("a DS effect movie has no root sprite")
    return {"fps": fps, "names": names, "bitmap_ids": bitmap_ids, "shapes": shapes, "sprites": sprites}


def movie_textures(effect_bytes, bitmap_ids, language):
    textures = {}
    for index, bitmap_id in enumerate(bitmap_ids):
        try:
            texture_bytes = ds_effects.texture_item(effect_bytes, index + TEXTURE_ITEM_AFTER_SCRIPT)
            width, height, rgba = ds_textures.decode(texture_bytes)
        except (ds_effects.EffectRefused, ds_textures.TextureRefused) as error:
            raise EffectMovieRefused(str(error))
        textures[bitmap_id] = numpy.frombuffer(rgba, dtype=numpy.uint8).reshape(height, width, RGBA_BYTES).astype(numpy.float32) / 255.0
    return textures


def matrix_array(matrix):
    scale_x, scale_y, skew_a, skew_b, move_x, move_y = matrix
    return numpy.array([[scale_x, skew_b, move_x], [skew_a, scale_y, move_y], [0.0, 0.0, 1.0]])


def calls_of(sprite, name):
    return sorted(script["frame"] for script in sprite["scripts"] for call in script["calls"] if call["name"] == name)


def loop_target(sprite):
    labels = {frame["label"]: index for index, frame in enumerate(sprite["frames"]) if frame["label"]}
    for script in sprite["scripts"]:
        for call in script["calls"]:
            if call["name"] in ("gotoAndPlay", "gotoLabel") and call["args"] and call["args"][0] in labels:
                return labels[call["args"][0]]
    return 0


def frame_at_age(sprite, age):
    stops = calls_of(sprite, "stop") + calls_of(sprite, "removeMovieClip")
    if stops and age >= stops[0]:
        return stops[0]
    length = len(sprite["frames"])
    if age < length:
        return age
    start = min(loop_target(sprite), length - 1)
    return start + (age - start) % (length - start)


def draws_of_frame(movie, sprite, frame_index, entered, path, transform, tint, state, depth_limit=8):
    if depth_limit <= 0:
        raise EffectMovieRefused("a DS effect movie nests sprites too deeply")
    sprites = {each["sprite_id"]: each for each in movie["sprites"]}
    draws = []
    for item in sorted(sprite["frames"][frame_index]["items"], key=lambda item: item["depth"]):
        key = path + (item["slot"],)
        held = state.get(key)
        character = item["character"]
        if held is None or held[0] != character or (entered and item["first"]):
            age = 0
        else:
            age = held[1] + 1
        state[key] = (character, age)
        placed = transform @ matrix_array(item["matrix"])
        color = tuple(own * outer for own, outer in zip(item["rgba"], tint))
        if character == ROOT_SPRITE:
            continue
        if character in movie["shapes"]:
            shape = movie["shapes"][character]
            for fill in shape["fills"]:
                draws.append({"bitmap": fill["bitmap"], "shape_to_stage": placed, "bitmap_to_shape": matrix_array(fill["matrix"]), "rect": shape["rect"], "rgba": color})
        elif character in sprites:
            inner = sprites[character]
            if inner["frames"]:
                inner_frame = frame_at_age(inner, age)
                inner_entered = age == 0 or frame_at_age(inner, age - 1) != inner_frame
                draws.extend(draws_of_frame(movie, inner, inner_frame, inner_entered, key, placed, color, state, depth_limit - 1))
        elif character in movie["bitmap_ids"]:
            draws.append({"bitmap": character, "shape_to_stage": placed, "bitmap_to_shape": numpy.identity(3), "rect": None, "rgba": color})
        else:
            raise EffectMovieRefused("a DS effect movie places character %d, which it does not hold" % character)
    return [draw for draw in draws if draw["rgba"][3] > 0.0 and abs(numpy.linalg.det(draw["shape_to_stage"])) > DEGENERATE and abs(numpy.linalg.det(draw["bitmap_to_shape"])) > DEGENERATE]


def corners_in_stage(draw, textures):
    height, width = textures[draw["bitmap"]].shape[:2]
    if draw["rect"] is None:
        x_min, x_max, y_min, y_max = 0, width, 0, height
    else:
        x_min, x_max, y_min, y_max = draw["rect"]
    points = numpy.array([[x_min, y_min, 1.0], [x_max, y_min, 1.0], [x_max, y_max, 1.0], [x_min, y_max, 1.0]]).T
    return (draw["shape_to_stage"] @ points)[:2].T


def apply(matrix, points):
    return numpy.stack([matrix[0, 0] * points[0] + matrix[0, 1] * points[1] + matrix[0, 2], matrix[1, 0] * points[0] + matrix[1, 1] * points[1] + matrix[1, 2], numpy.ones(points.shape[1])])


def paint(canvas, left, top, draw, textures):
    bitmap = textures[draw["bitmap"]]
    rows, columns = canvas.shape[:2]
    ys, xs = numpy.mgrid[0:rows, 0:columns]
    points = numpy.stack([xs.ravel() + left + 0.5, ys.ravel() + top + 0.5, numpy.ones(xs.size)])
    try:
        to_shape = numpy.linalg.inv(draw["shape_to_stage"])
        to_bitmap = numpy.linalg.inv(draw["bitmap_to_shape"])
    except numpy.linalg.LinAlgError:
        return
    in_shape = apply(to_shape, points)
    inside = numpy.ones(xs.size, dtype=bool)
    if draw["rect"] is not None:
        x_min, x_max, y_min, y_max = draw["rect"]
        inside = (in_shape[0] >= x_min) & (in_shape[0] < x_max) & (in_shape[1] >= y_min) & (in_shape[1] < y_max)
    in_bitmap = apply(to_bitmap, in_shape)
    u = numpy.floor(in_bitmap[0]).astype(numpy.int64)
    v = numpy.floor(in_bitmap[1]).astype(numpy.int64)
    inside &= (u >= 0) & (u < bitmap.shape[1]) & (v >= 0) & (v < bitmap.shape[0])
    source = numpy.zeros((xs.size, RGBA_BYTES), dtype=numpy.float32)
    source[inside] = bitmap[v[inside], u[inside]]
    source = source * numpy.array(draw["rgba"], dtype=numpy.float32)
    source = source.reshape(rows, columns, RGBA_BYTES)
    alpha = source[..., 3:4]
    canvas[..., :3] = source[..., :3] * alpha + canvas[..., :3] * (1.0 - alpha)
    canvas[..., 3:4] = alpha + canvas[..., 3:4] * (1.0 - alpha)


def render_frame(draws, textures, bounds):
    left, top, right, bottom = bounds
    canvas = numpy.zeros((bottom - top, right - left, RGBA_BYTES), dtype=numpy.float32)
    for draw in draws:
        paint(canvas, left, top, draw, textures)
    alpha = canvas[..., 3:4]
    canvas[..., :3] = numpy.where(alpha > 0.0, canvas[..., :3] / numpy.maximum(alpha, 1e-6), 0.0)
    return (numpy.clip(canvas, 0.0, 1.0) * 255.0 + 0.5).astype(numpy.uint8)


def bounds_of(draws, textures):
    if not draws:
        return None
    corners = numpy.concatenate([corners_in_stage(draw, textures) for draw in draws])
    return (int(numpy.floor(corners[:, 0].min())), int(numpy.floor(corners[:, 1].min())), int(numpy.ceil(corners[:, 0].max())), int(numpy.ceil(corners[:, 1].max())))


def vertex_quad(draws, vertices):
    if len(draws) != 1:
        raise EffectMovieRefused("a DS effect movie moves its corners while a frame shows %d pieces" % len(draws))
    placed = numpy.array([[x, y, 1.0] for x, y in vertices]).T
    return [tuple(point) for point in (draws[0]["shape_to_stage"] @ placed)[:2].T]


def ink_bounds(image):
    rows = numpy.flatnonzero(image[..., 3].any(axis=1))
    columns = numpy.flatnonzero(image[..., 3].any(axis=0))
    return int(columns[0]), int(rows[0]), int(columns[-1]) + 1, int(rows[-1]) + 1


def root_frame_at(root, time, length):
    return time if length == len(root["frames"]) else min(time, len(root["frames"]) - 1)


def frames_of(movie, textures, vertices, length):
    root = next(sprite for sprite in movie["sprites"] if sprite["sprite_id"] == ROOT_SPRITE)
    state = {}
    cells = []
    cell_index = {}
    frames = []
    for time in range(length):
        frame_index = root_frame_at(root, time, length)
        entered = time == 0 or root_frame_at(root, time - 1, length) != frame_index
        draws = draws_of_frame(movie, root, frame_index, entered, (), numpy.identity(3), (1.0, 1.0, 1.0, 1.0), state)
        if not draws:
            frames.append(None)
            continue
        if vertices is not None:
            corners_stage = vertex_quad(draws, vertices)
            draw = dict(draws[0], shape_to_stage=numpy.identity(3))
            bounds = bounds_of([draw], textures)
            image = render_frame([draw], textures, bounds)
        else:
            left, top, right, bottom = bounds_of(draws, textures)
            image = render_frame(draws, textures, (left, top, right, bottom))
            if image[..., 3].any():
                ink_left, ink_top, ink_right, ink_bottom = ink_bounds(image)
                image = image[ink_top:ink_bottom, ink_left:ink_right]
                left, top = left + ink_left, top + ink_top
            corners_stage = effect_file.corners_of_rect(left, top, image.shape[1], image.shape[0])
        if not image[..., 3].any():
            frames.append(None)
            continue
        key = (image.shape, image.tobytes())
        if key not in cell_index:
            cell_index[key] = len(cells)
            cells.append(image)
        origin_x, origin_y = effect_file.STAGE_ORIGIN
        frames.append({"cell": cell_index[key], "corners": [(float(x) - origin_x, float(y) - origin_y) for x, y in corners_stage]})
    return cells, frames


def nested_playback(movie, root, vertices):
    nested = [sprite for sprite in movie["sprites"] if sprite["sprite_id"] != ROOT_SPRITE and sprite["frames"]]
    length = max(len(sprite["frames"]) for sprite in nested)
    removals = [frame for sprite in nested for frame in calls_of(sprite, "removeMovieClip")]
    playback = {"labels": {}, "statuses": {}, "jumps": [], "sounds": [], "vertices": vertices, "loop_start": None, "after_last": "loop", "frame_count": length}
    if removals:
        playback["frame_count"] = min(length, min(removals) + 1)
        playback["after_last"] = "end"
    elif all(calls_of(sprite, "stop") for sprite in nested):
        playback["after_last"] = "hold"
    else:
        playback["loop_start"] = 0
    return playback


def playback_of_movie(movie):
    root = next(sprite for sprite in movie["sprites"] if sprite["sprite_id"] == ROOT_SPRITE)
    playback = effect_file.playback_of({"sprites": movie["sprites"]}, root, len(root["frames"]))
    nested_lengths = [len(sprite["frames"]) for sprite in movie["sprites"] if sprite["sprite_id"] != ROOT_SPRITE]
    if len(root["frames"]) == 1 and nested_lengths and max(nested_lengths) > 1:
        return nested_playback(movie, root, playback["vertices"])
    return playback


def pack_cells(cells):
    places = []
    page = 0
    shelf_x = shelf_y = shelf_height = 0
    heights = [0]
    for cell in cells:
        height, width = cell.shape[:2]
        if width + 2 * CELL_GUTTER > PAGE_WIDTH or height + 2 * CELL_GUTTER > PAGE_HEIGHT_MOST:
            raise EffectMovieRefused("a DS effect movie frame of %dx%d is larger than a page" % (width, height))
        if shelf_x + width + 2 * CELL_GUTTER > PAGE_WIDTH:
            shelf_y, shelf_x, shelf_height = shelf_y + shelf_height, 0, 0
        if shelf_y + height + 2 * CELL_GUTTER > PAGE_HEIGHT_MOST:
            page, shelf_x, shelf_y, shelf_height = page + 1, 0, 0, 0
            heights.append(0)
        places.append((page, shelf_x + CELL_GUTTER, shelf_y + CELL_GUTTER))
        shelf_x = shelf_x + width + 2 * CELL_GUTTER
        shelf_height = max(shelf_height, height + 2 * CELL_GUTTER)
        heights[page] = max(heights[page], shelf_y + shelf_height)
    pages = []
    for height in heights:
        rounded = max(4, (height + 3) // 4 * 4)
        pages.append(numpy.zeros((rounded, PAGE_WIDTH, RGBA_BYTES), dtype=numpy.uint8))
    for cell, (page, x, y) in zip(cells, places):
        pages[page][y:y + cell.shape[0], x:x + cell.shape[1]] = cell
    return places, pages


def number(value):
    return asset_writers.format_number(round(value, 6))


def movie_lines(row, playback, ticks, cells, places, pages, frames):
    effect = {"ticks_per_frame": ticks, "after_last": playback["after_last"], "loop_start": playback["loop_start"], "page_paths": pages, "jumps": playback["jumps"], "statuses": playback["statuses"]}
    lines = effect_movies.header_rows(row, effect)
    for cell, (page, x, y) in zip(cells, places):
        height, width = cell.shape[:2]
        page_height, page_width = pages[page].shape[:2]
        lines.append("fxpiece %d %s 0 0 1 1" % (page, " ".join(number(value) for value in (x / page_width, y / page_height, (x + width) / page_width, (y + height) / page_height))))
    for frame in frames[:playback["frame_count"]]:
        if frame is None:
            lines.append("fxframe 0 0 0 0 0 0 0 0 0 0")
        else:
            corners = " ".join(asset_writers.format_number(round(value, 4)) for corner in frame["corners"] for value in corner)
            lines.append("fxframe %d 1 %s" % (frame["cell"], corners))
    return lines


def bake_movie(row, effect_bytes, language):
    items = ds_effects.item_offsets(effect_bytes)
    start, end = items[ds_effects.SCRIPT_ITEM]
    movie = read_movie(effect_bytes[start + ds_effects.ITEM_HEADER_SIZE:end])
    textures = movie_textures(effect_bytes, movie["bitmap_ids"], language)
    playback = playback_of_movie(movie)
    cells, frames = frames_of(movie, textures, playback["vertices"], playback["frame_count"])
    if not cells:
        raise EffectMovieRefused("a DS effect movie draws nothing")
    places, pages = pack_cells(cells)
    memory = sum(page.size for page in pages)
    if memory > MOVIE_MEMORY_MOST:
        raise EffectMovieRefused("a DS effect movie keeps %d bytes of pages loaded, more than %d" % (memory, MOVIE_MEMORY_MOST))
    ticks = effect_file.TICKS_PER_SECOND / movie["fps"]
    lines = movie_lines(row, playback, ticks, cells, places, pages, frames)
    return lines, [asset_writers.gtex_bytes(page.shape[1], page.shape[0], asset_writers.FORMAT_RGBA8888, page.tobytes()) for page in pages]


def page_path(name, page):
    return TEXTURE_FOLDER + "/" + effect_pieces_page_name(name, page) + ".gtex"


def effect_pieces_page_name(name, page):
    return "fx_%s_%02d" % (name, page)


def movie_rows(set_description, scripts, arm9):
    hand_rows = set(row["id"] for row in set_description.get("effects", []))
    movie_flags = {row["id"]: row for row in set_description.get("effect_movies", []) if "id" in row}
    ids = set()
    for script in scripts.values():
        ids.update(chapter_assets.spawned_effect_ids(script))
    rows = []
    for effect_id in sorted(ids - hand_rows):
        kind = chapter_assets.effect_kind_of(arm9, effect_id)
        if effect_id == 0 or kind >= chapter_assets.FIRST_CLASS_EFFECT_KIND:
            continue
        row = {"id": effect_id, "kind": kind, "effect": MOVIE_NAME % kind}
        for field in ("only_in_soul_view", "sound"):
            if field in movie_flags.get(effect_id, {}):
                row[field] = movie_flags[effect_id][field]
        rows.append(row)
    return rows


def bake(rows, effect_pack, language, record):
    code = bake_record.code_fingerprint(CODE_FILES)
    paths = []
    texts = []
    effects = []
    refused = []
    baked = {}
    for row in rows:
        name = row["effect"]
        if name not in baked:
            try:
                effect_bytes = effect_pack.effect_bytes(row["kind"], language)
                key = record.key_for(code, {"row": json.dumps({field: row[field] for field in ("effect", "sound") if field in row}, sort_keys=True), "language": language}, effect_bytes)
                kept = stored_bake(record, name, key)
                if kept is None:
                    lines, stored = bake_movie(row, effect_bytes, language)
                    for page, data in enumerate(stored):
                        record.store(page_path(name, page), key, data, {"key": key, "pages": len(stored), "lines": lines} if page == 0 else None)
                    kept = (lines, len(stored))
            except (EffectMovieRefused, ds_effects.EffectRefused, effect_file.EffectFormatError) as error:
                refused.append("%d (%s)" % (row["id"], error))
                baked[name] = None
                continue
            baked[name] = kept
            texts.extend(kept[0])
            paths.extend(page_path(name, page) for page in range(kept[1]))
        if baked[name] is not None:
            effects.append({"id": row["id"], "kind": "movie", "texture": name, "ticks": 0, "end_zoom": 1, "width": 0, "height": 0, "only_in_soul_view": row.get("only_in_soul_view", 0)})
    if refused:
        print("ds effect movies left out: %d (%s)" % (len(refused), ", ".join(refused[:12])))
    return effects, "\n".join(texts) + ("\n" if texts else ""), paths


def stored_bake(record, name, key):
    details = record.details_of(page_path(name, 0))
    if details.get("key") != key or "pages" not in details or "lines" not in details:
        return None
    for page in range(details["pages"]):
        if record.read_if_current(page_path(name, page), key) is None:
            return None
    return details["lines"], details["pages"]


def add_to_set(set_description, scripts, arm9, cartridge, record):
    language = ds_textures.LANGUAGE_NUMBERS.get(set_description.get("scripts", {}).get("language", "en"), 1)
    effects, text, paths = bake(movie_rows(set_description, scripts, arm9), ds_effects.EffectPack(cartridge.textures.pack), language, record)
    expanded = dict(set_description)
    expanded["effects"] = list(set_description.get("effects", [])) + effects
    expanded["effect_movie_text"] = text
    return expanded, paths
