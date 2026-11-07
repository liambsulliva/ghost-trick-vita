import struct

import numpy as np

TYPE_BITMAP = 0x615284D0
TYPE_SHEET = 0xC657A2FC
TYPE_MATRIX = 0x18841315
TYPE_COLOR = 0xEDFDC600
TYPE_PLACE = 0xE112D823
TYPE_SNAPSHOT = 0x399F1F5E
TYPE_FRAME = 0x96F7D38F
TYPE_SPRITE = 0xDC2F7D89
TYPE_ARG = 0x286B67D9
TYPE_ACTION = 0x8B2A055D
TYPE_DO_ACTION = 0xC54CBD5F
TYPE_RECT = 0x79A8148F
TYPE_FILL = 0xDBC1BFDD
TYPE_SHAPE = 0xEB46A06B
TYPE_STAGE = 0xF2E8CC44
TYPE_ROOT = 0x26F3F3E7
TYPE_PLAIN_ROOT = 0xE4FFB789

USR_RSZ_POSITION_OFFSET = 32
RSZ_INSTANCE_COUNT_OFFSET = 12
RSZ_TABLE_OFFSETS_OFFSET = 24

STAGE_ORIGIN = (128.0, 96.0)
BAKED_TEXELS_PER_DS_PIXEL = 2.0
TICKS_PER_SECOND = 60.0
LAST_SHAPE_PIVOT = {"st01_onpu": (16.3, 28.8)}

OP_END = 0x00
OP_PLAY = 0x06
OP_STOP = 0x07
OP_POP = 0x17
OP_GET_VARIABLE = 0x1C
OP_CALL_FUNCTION = 0x3D
OP_CALL_METHOD = 0x52
OP_GOTO_LABEL = 0x8C
OP_PUSH = 0x96


class EffectFormatError(ValueError):
    pass


class Reader:
    def __init__(self, data, position):
        self.data = data
        self.position = position

    def align(self, size):
        self.position = (self.position + size - 1) & ~(size - 1)

    def i32(self):
        self.align(4)
        value = struct.unpack_from("<i", self.data, self.position)[0]
        self.position += 4
        return value

    def f32(self):
        self.align(4)
        value = struct.unpack_from("<f", self.data, self.position)[0]
        self.position += 4
        return value

    def u16(self):
        self.align(2)
        value = struct.unpack_from("<H", self.data, self.position)[0]
        self.position += 2
        return value

    def s16(self):
        self.align(2)
        value = struct.unpack_from("<h", self.data, self.position)[0]
        self.position += 2
        return value

    def u8(self):
        value = self.data[self.position]
        self.position += 1
        return value

    def string(self):
        count = self.i32()
        if count < 0 or self.position + 2 * count > len(self.data):
            raise EffectFormatError("bad string length at " + hex(self.position))
        text = self.data[self.position:self.position + 2 * count].decode("utf-16-le")
        self.position += 2 * count
        return text.rstrip("\0")

    def i32_list(self):
        count = self.i32()
        if count < 0 or count > 1000000:
            raise EffectFormatError("bad list length at " + hex(self.position))
        return [self.i32() for _ in range(count)]

    def vec4(self):
        self.align(16)
        value = struct.unpack_from("<4f", self.data, self.position)
        self.position += 16
        return value

    def ivec4(self):
        self.align(16)
        value = struct.unpack_from("<4i", self.data, self.position)
        self.position += 16
        return value


def read_bitmap(r):
    return {"type": "bitmap", "ds_format": r.i32(), "width": r.i32(), "height": r.i32()}


def read_sheet(r):
    sheet = {"type": "sheet"}
    sheet["pages"] = [r.string() for _ in range(r.i32())]
    sheet["page_size"] = (r.i32(), r.i32())
    sheet["cell_size"] = (r.i32(), r.i32())
    sheet["cells_per_page"] = r.i32_list()
    grid = r.vec4()
    sheet["grid"] = (int(grid[0]), int(grid[1]))
    sheet["cell_count"] = r.i32()
    r.u8()
    r.u8()
    r.align(4)
    sheet["bitmap_refs"] = r.i32_list()
    sheet["unknown_one"] = r.i32()
    offset_count = r.i32()
    sheet["offsets"] = [r.vec4()[:2] for _ in range(offset_count)]
    sheet["frame_cells"] = r.i32_list()
    sheet["stage"] = (r.i32(), r.i32())
    sheet["flag"] = r.i32()
    sheet["unknown_f"] = r.f32()
    sheet["shift"] = r.vec4()[:2]
    sheet["mode"] = r.ivec4()[0]
    sheet["unknown_v2"] = r.vec4()[:2]
    return sheet


def read_matrix(r):
    return {"type": "matrix", "values": tuple(r.f32() for _ in range(6))}


def read_color(r):
    red, green, blue, zero_a, alpha, zero_b = [r.s16() for _ in range(6)]
    return {"type": "color", "rgba": (red / 256.0, green / 256.0, blue / 256.0, alpha / 256.0), "unknown": (zero_a, zero_b)}


def read_place(r):
    tag, has_character, has_matrix, has_color, always_one, depth_word = [r.i32() for _ in range(6)]
    character, frame = r.i32(), r.i32()
    for _ in range(3):
        r.i32()
    r.i32()
    r.i32()
    matrix_ref, color_ref = r.i32(), r.i32()
    r.i32()
    return {"type": "place", "tag": tag, "has_character": has_character, "has_matrix": has_matrix, "has_color": has_color, "depth": (depth_word >> 16) & 0xFFFF, "character": character, "frame": frame, "matrix_ref": matrix_ref, "color_ref": color_ref}


def read_snapshot(r):
    slot, character = r.u16(), r.u16()
    depth, _ = r.s16(), r.s16()
    first = r.i32()
    for _ in range(6):
        r.i32()
    matrix_ref, color_ref = r.i32(), r.i32()
    return {"type": "snapshot", "slot": slot, "character": character, "depth": depth, "first": first, "matrix_ref": matrix_ref, "color_ref": color_ref}


def read_frame(r):
    label = r.string()
    label_id = r.i32()
    return {"type": "frame", "label": label, "label_id": label_id, "places": r.i32_list(), "snapshots": r.i32_list()}


def read_sprite(r):
    fps, frame_count, sprite_id = r.i32(), r.i32(), r.i32()
    frames = r.i32_list()
    r.i32()
    return {"type": "sprite", "fps": fps, "frame_count": frame_count, "sprite_id": sprite_id, "frames": frames, "do_actions": r.i32_list()}


def read_arg(r):
    kind, index, _, _, value = [r.i32() for _ in range(5)]
    return {"type": "arg", "kind": kind, "index": index, "value": value, "text": r.string()}


def read_action(r):
    op, _, push_count, arg_count = [r.i32() for _ in range(4)]
    return {"type": "action", "op": op, "push_count": push_count, "arg_count": arg_count, "args": r.i32_list()}


def read_do_action(r):
    frame = r.i32()
    for _ in range(8):
        r.i32()
    actions = r.i32_list()
    r.i32()
    return {"type": "do_action", "frame": frame, "actions": actions}


def read_rect(r):
    return {"type": "rect", "x_min": r.i32(), "x_max": r.i32(), "y_min": r.i32(), "y_max": r.i32()}


def read_fill(r):
    return {"type": "fill", "bitmap": r.i32(), "matrix_ref": r.i32()}


def read_shape(r):
    shape = {"type": "shape", "id": r.i32(), "rect_ref": r.i32(), "fills": r.i32_list()}
    count = r.i32()
    shape["unknown_u16"] = [r.u16() for _ in range(count)]
    r.align(4)
    shape["last_fill_ref"] = r.i32()
    return shape


def read_stage(r):
    r.i32()
    r.i32()
    stage = {"type": "stage", "size": (r.i32(), r.i32()), "max_depth": r.i32(), "sprites": r.i32_list(), "shapes": r.i32_list()}
    r.i32()
    stage["bitmap_ids"] = r.i32_list()
    stage["names"] = [r.string() for _ in range(r.i32())]
    return stage


def read_root(r):
    return {"type": "root", "unknown": r.i32(), "stage_ref": r.i32()}


RECORD_READERS = {
    TYPE_BITMAP: read_bitmap,
    TYPE_SHEET: read_sheet,
    TYPE_MATRIX: read_matrix,
    TYPE_COLOR: read_color,
    TYPE_PLACE: read_place,
    TYPE_SNAPSHOT: read_snapshot,
    TYPE_FRAME: read_frame,
    TYPE_SPRITE: read_sprite,
    TYPE_ARG: read_arg,
    TYPE_ACTION: read_action,
    TYPE_DO_ACTION: read_do_action,
    TYPE_RECT: read_rect,
    TYPE_FILL: read_fill,
    TYPE_SHAPE: read_shape,
    TYPE_STAGE: read_stage,
    TYPE_ROOT: read_root,
}


def read_records(data):
    if data[:4] != b"USR\0":
        raise EffectFormatError("not a USR file")
    rsz_offset = struct.unpack_from("<Q", data, USR_RSZ_POSITION_OFFSET)[0]
    if data[rsz_offset:rsz_offset + 4] != b"RSZ\0":
        raise EffectFormatError("no RSZ block")
    instance_count = struct.unpack_from("<I", data, rsz_offset + RSZ_INSTANCE_COUNT_OFFSET)[0]
    instance_offset, data_offset = struct.unpack_from("<QQ", data, rsz_offset + RSZ_TABLE_OFFSETS_OFFSET)
    type_ids = [struct.unpack_from("<I", data, rsz_offset + instance_offset + 8 * index)[0] for index in range(instance_count)]
    if TYPE_PLAIN_ROOT in type_ids:
        raise EffectFormatError("plain texture file, not a movie (23_smoke_a2, timeloop)")
    reader = Reader(data, rsz_offset + data_offset)
    records = [None]
    for type_id in type_ids[1:]:
        read_one = RECORD_READERS.get(type_id)
        if read_one is None:
            raise EffectFormatError("unknown record type " + hex(type_id))
        records.append(read_one(reader))
    if reader.position != len(data):
        raise EffectFormatError(str(len(data) - reader.position) + " unread bytes")
    return records


def first_of(records, type_name):
    for record in records[1:]:
        if record["type"] == type_name:
            return record
    raise EffectFormatError("no " + type_name + " record")


def run_script(records, do_action):
    """Small stack machine: enough to name the calls a frame script makes."""
    stack = []
    calls = []
    previous_text = ""
    for action_ref in do_action["actions"]:
        action = records[action_ref]
        op = action["op"]
        if op == OP_PUSH:
            for arg_ref in action["args"]:
                arg = records[arg_ref]
                if arg["kind"] == 4:
                    stack.append(arg["text"])
                    previous_text = arg["text"]
                elif arg["text"] and arg["text"] != previous_text:
                    stack.append(arg["text"])
                    previous_text = arg["text"]
                else:
                    stack.append(arg["value"])
        elif op == OP_GET_VARIABLE:
            name = stack.pop() if stack else None
            stack.append(("variable", name))
        elif op == OP_CALL_FUNCTION:
            name = stack.pop() if stack else None
            count = stack.pop() if stack else 0
            count = count if isinstance(count, int) else 0
            args = [stack.pop() for _ in range(min(count, len(stack)))]
            calls.append({"name": name, "args": args})
            stack.append(None)
        elif op == OP_CALL_METHOD:
            name = stack.pop() if stack else None
            target = stack.pop() if stack else None
            count = stack.pop() if stack else 0
            count = count if isinstance(count, int) else 0
            args = [stack.pop() for _ in range(min(count, len(stack)))]
            target_name = target[1] if isinstance(target, tuple) else target
            calls.append({"name": name, "target": target_name, "args": args})
            stack.append(None)
        elif op == OP_POP:
            if stack:
                stack.pop()
        elif op == OP_GOTO_LABEL:
            for arg_ref in action["args"]:
                calls.append({"name": "gotoLabel", "args": [records[arg_ref]["text"]]})
        elif op == OP_PLAY:
            calls.append({"name": "play", "args": []})
        elif op == OP_STOP:
            calls.append({"name": "stop", "args": []})
        elif op != OP_END:
            calls.append({"name": "op_" + hex(op), "args": []})
    return calls


def resolve_character(records, character, sprites_by_id, shapes_by_id):
    if character in sprites_by_id:
        return "sprite"
    if character in shapes_by_id:
        return "shape"
    return "bitmap_or_unknown"


def read_timeline(user_bytes):
    records = read_records(user_bytes)
    stage = first_of(records, "stage")
    sprites = []
    sprites_by_id = {}
    for record in records[1:]:
        if record["type"] == "sprite":
            sprites.append(record)
            sprites_by_id[record["sprite_id"]] = record
    shapes_by_id = {records[ref]["id"]: records[ref] for ref in stage["shapes"]}
    out_sprites = []
    for sprite in sprites:
        frames = []
        for frame_ref in sprite["frames"]:
            frame = records[frame_ref]
            items = []
            for snapshot_ref in frame["snapshots"]:
                snapshot = records[snapshot_ref]
                items.append({
                    "slot": snapshot["slot"],
                    "character": snapshot["character"],
                    "character_kind": resolve_character(records, snapshot["character"], sprites_by_id, shapes_by_id),
                    "depth": snapshot["depth"],
                    "matrix": records[snapshot["matrix_ref"]]["values"],
                    "rgba": records[snapshot["color_ref"]]["rgba"],
                })
            frames.append({"label": frame["label"], "items": items})
        scripts = [{"frame": records[ref]["frame"], "calls": run_script(records, records[ref])} for ref in sprite["do_actions"]]
        out_sprites.append({"sprite_id": sprite["sprite_id"], "fps": sprite["fps"], "frames": frames, "scripts": scripts})
    out_shapes = {}
    for shape_id, shape in shapes_by_id.items():
        rect = records[shape["rect_ref"]]
        fills = [{"bitmap": records[ref]["bitmap"], "matrix": records[records[ref]["matrix_ref"]]["values"]} for ref in shape["fills"]]
        out_shapes[shape_id] = {"rect": (rect["x_min"], rect["x_max"], rect["y_min"], rect["y_max"]), "fills": fills}
    bitmaps = [record for record in records[1:] if record["type"] == "bitmap"]
    return {"stage": stage["size"], "max_depth": stage["max_depth"], "names": stage["names"], "sprites": out_sprites, "shapes": out_shapes, "bitmaps": bitmaps}


def ticks_per_baked_frame(fps, movie_frames, baked_frames):
    ticks_per_movie_frame = TICKS_PER_SECOND / fps
    if movie_frames <= 0 or baked_frames <= movie_frames:
        return ticks_per_movie_frame
    per_entry = ticks_per_movie_frame * movie_frames / baked_frames
    return max(1.0, per_entry)


def stem_from_page(page_name):
    file_name = page_name.replace("\\", "/").split("/")[-1]
    if file_name.lower().startswith("23_"):
        file_name = file_name[3:]
    lowered = file_name.lower()
    for suffix in ("_iml4.tex", ".tex"):
        if lowered.endswith(suffix):
            lowered = lowered[:-len(suffix)]
    pieces = lowered.rsplit("_", 1)
    if len(pieces) == 2 and pieces[1].isdigit():
        return pieces[0]
    return lowered


def pack_path_of_page(page_name):
    return "natives/android/" + page_name.lower() + ".691230823"


def source_of_cell(sheet, cell):
    remaining = cell
    for page, count in enumerate(sheet["cells_per_page"]):
        if remaining < count:
            columns = sheet["grid"][0]
            cell_width, cell_height = sheet["cell_size"]
            return page, (remaining % columns * cell_width, remaining // columns * cell_height, cell_width, cell_height)
        remaining -= count
    raise EffectFormatError("cell " + str(cell) + " is on no page")


def corners_of_rect(x, y, width, height):
    return ((x, y), (x + width, y), (x + width, y + height), (x, y + height))


def last_shape_points(timeline, main_sprite, pivot):
    points = []
    for frame in main_sprite["frames"]:
        point = None
        for item in frame["items"]:
            if item["character_kind"] == "shape":
                scale_x, scale_y, skew_a, skew_b, move_x, move_y = item["matrix"]
                matrix = np.array([[scale_x, skew_b, move_x], [skew_a, scale_y, move_y]])
                point = matrix @ np.array([pivot[0], pivot[1], 1.0])
        points.append(point)
    return points


def read_effect(user_bytes):
    records = read_records(user_bytes)
    sheet = first_of(records, "sheet")
    timeline = read_timeline(user_bytes)
    main = next(sprite for sprite in timeline["sprites"] if sprite["sprite_id"] == 0)
    movie_frames = len(main["frames"])
    baked_frames = len(sheet["frame_cells"])
    ticks = ticks_per_baked_frame(main["fps"], movie_frames, baked_frames)
    baked_per_movie_frame = baked_frames / float(movie_frames) if movie_frames else 1.0

    labels = {}
    for index, frame in enumerate(main["frames"]):
        if frame["label"]:
            labels[frame["label"]] = int(round(index * baked_per_movie_frame))

    vertices = {}
    sounds = []
    loop_start = None
    loop_jump_frame = None
    removes_itself_at = None
    stops_at = None
    for sprite in timeline["sprites"]:
        for script in sprite["scripts"]:
            baked_index = int(round(script["frame"] * baked_per_movie_frame))
            baked_last_of_frame = int(round((script["frame"] + 1) * baked_per_movie_frame)) - 1
            for call in script["calls"]:
                name = call["name"]
                if name == "setVertex" and len(call["args"]) == 3:
                    vertices[int(call["args"][0])] = (float(call["args"][1]), float(call["args"][2]))
                elif name == "fscommand2" and call["args"] and call["args"][0] == "sound":
                    sounds.append({"frame": baked_index, "name": call["args"][1]})
                elif name in ("gotoAndPlay", "gotoLabel") and sprite["sprite_id"] == 0 and call["args"]:
                    target = call["args"][0]
                    if target in labels:
                        loop_start = labels[target]
                        loop_jump_frame = baked_last_of_frame
                elif name == "removeMovieClip":
                    removes_itself_at = baked_last_of_frame
                elif name == "stop" and removes_itself_at is None:
                    stops_at = baked_last_of_frame

    quad_vertices = None
    if len(vertices) == 4:
        quad_vertices = (vertices[0], vertices[3], vertices[2], vertices[1])

    stem = stem_from_page(sheet["pages"][0])
    pivot = LAST_SHAPE_PIVOT.get(stem)
    pivot_points = last_shape_points(timeline, main, pivot) if pivot else None
    pivot_start = next((p for p in pivot_points if p is not None), None) if pivot else None

    shift_x, shift_y = sheet["shift"]
    frames = []
    for index, cell in enumerate(sheet["frame_cells"]):
        offset_x, offset_y = sheet["offsets"][cell]
        page, source = source_of_cell(sheet, cell)
        blank = offset_x == 0.0 and offset_y == 0.0
        quads = []
        if not blank:
            width = source[2] / BAKED_TEXELS_PER_DS_PIXEL
            height = source[3] / BAKED_TEXELS_PER_DS_PIXEL
            left = (offset_x + shift_x) / BAKED_TEXELS_PER_DS_PIXEL - STAGE_ORIGIN[0]
            top = (offset_y + shift_y) / BAKED_TEXELS_PER_DS_PIXEL - STAGE_ORIGIN[1]
            if pivot_points is not None:
                movie_index = min(int(index / baked_per_movie_frame), movie_frames - 1)
                point = pivot_points[movie_index]
                if point is not None:
                    left += float(point[0] - pivot_start[0])
                    top += float(point[1] - pivot_start[1])
            if quad_vertices is not None:
                corners = quad_vertices
                matrix = None
            else:
                corners = corners_of_rect(left, top, width, height)
                matrix = (1.0 / BAKED_TEXELS_PER_DS_PIXEL, 0.0, 0.0, 1.0 / BAKED_TEXELS_PER_DS_PIXEL, left, top)
            quads.append({"page": page, "source": source, "corners": corners, "matrix": matrix, "rgba": (1.0, 1.0, 1.0, 1.0), "blend": "alpha"})
        frames.append({"index": index, "cell": cell, "quads": quads})

    if loop_start is not None:
        after_last = "loop"
    elif removes_itself_at is not None:
        after_last = "end"
    elif stops_at is not None or movie_frames == 1:
        after_last = "hold"
    else:
        after_last = "end"

    frame_count = baked_frames
    if loop_jump_frame is not None:
        frame_count = min(frame_count, loop_jump_frame + 1)
    if removes_itself_at is not None:
        frame_count = min(frame_count, removes_itself_at + 1)

    return {
        "name": stem,
        "pages": sheet["pages"],
        "page_paths": [pack_path_of_page(page) for page in sheet["pages"]],
        "page_size": sheet["page_size"],
        "cell_size": sheet["cell_size"],
        "grid": sheet["grid"],
        "cells_per_page": sheet["cells_per_page"],
        "stage": sheet["stage"],
        "origin_in_stage": STAGE_ORIGIN,
        "shift": sheet["shift"],
        "mode": sheet["mode"],
        "movie_fps": main["fps"],
        "movie_frames": movie_frames,
        "frame_count": frame_count,
        "ticks_per_frame": ticks,
        "frame_rate": TICKS_PER_SECOND / ticks,
        "loop_start": loop_start,
        "after_last": after_last,
        "labels": labels,
        "sounds": sounds,
        "vertices": quad_vertices,
        "last_shape_pivot": pivot,
        "frames": frames,
    }
