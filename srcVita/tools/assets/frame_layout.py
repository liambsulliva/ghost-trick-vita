import asset_writers

DS_TICKS_PER_FRAME = 4
ART_PER_DS_PIXEL = 4
SCREEN_PIXELS_PER_DS_PIXEL = 720 / 256
TARGET_SCALE = SCREEN_PIXELS_PER_DS_PIXEL / ART_PER_DS_PIXEL
OBJECT_ORIGIN_X_IN_TIMELINE = 256
OBJECT_ORIGIN_Y_IN_TIMELINE = 128
ANCHOR_UNITS_PER_DS_PIXEL = 2
ANCHOR_NOISE_UNITS = 1
TEX_VERSION_SUFFIX = ".691230823"


def round_to_multiple_of_four(value):
    rounded = int((value + 2) // 4) * 4
    if rounded < 4:
        return 4
    return rounded


def target_size(source_width, source_height):
    return round_to_multiple_of_four(source_width * TARGET_SCALE), round_to_multiple_of_four(source_height * TARGET_SCALE)


def ticks_per_record(records_per_ds_frame):
    if DS_TICKS_PER_FRAME % records_per_ds_frame != 0:
        raise ValueError("unsupported records per DS frame: " + str(records_per_ds_frame))
    return DS_TICKS_PER_FRAME // records_per_ds_frame


def texture_name_for(logical_path):
    file_name = logical_path.replace("\\", "/").split("/")[-1].lower()
    for ending in ("_iml4.tex", "_iml3.tex", ".tex"):
        if file_name.endswith(ending):
            return file_name[:-len(ending)]
    return file_name


def pack_path_for(logical_path):
    return "natives/android/" + logical_path.lower() + TEX_VERSION_SUFFIX


def select_records(timeline, ds_frames):
    if ds_frames is None:
        return list(timeline["records"])
    step = timeline["records_per_ds_frame"]
    selected = []
    for ds_frame in ds_frames:
        selected.append(timeline["records"][ds_frame * step])
    return selected


def anchor_offset(anchor_value, point_value):
    difference = anchor_value - point_value
    if abs(difference) <= ANCHOR_NOISE_UNITS:
        return 0
    return difference / ANCHOR_UNITS_PER_DS_PIXEL


def placement_in_ds_pixels(position, origin, anchor, point, shift):
    position_ds = position / ART_PER_DS_PIXEL
    offset_ds = anchor_offset(anchor, point)
    shift_ds = shift / ART_PER_DS_PIXEL
    return position_ds - origin - offset_ds + shift_ds


def cell_rectangle(timeline, record):
    atlas_width = timeline["atlas_size"][0]
    cell_width, cell_height = timeline["cell_size"]
    columns = atlas_width // cell_width
    return ((record["cell_in_page"] % columns) * cell_width, (record["cell_in_page"] // columns) * cell_height, cell_width, cell_height)


def merge_rectangles(first, second):
    left = min(first[0], second[0])
    top = min(first[1], second[1])
    right = max(first[0] + first[2], second[0] + second[2])
    bottom = max(first[1] + first[3], second[1] + second[3])
    return (left, top, right - left, bottom - top)


def record_texture_name(timeline, record):
    return texture_name_for(timeline["pages"][record["page"]])


def add_used_cells_to_crops(crops, timeline, ds_frames):
    for record in select_records(timeline, ds_frames):
        if record["is_blank"]:
            continue
        name = record_texture_name(timeline, record)
        rectangle = cell_rectangle(timeline, record)
        if name in crops:
            rectangle = merge_rectangles(crops[name], rectangle)
        crops[name] = rectangle


def frame_for_record(timeline, record, ticks, crops):
    if record["is_blank"]:
        return asset_writers.blank_frame(ticks)
    name = record_texture_name(timeline, record)
    cell_x, cell_y, cell_width, cell_height = cell_rectangle(timeline, record)
    crop_x, crop_y, crop_width, crop_height = crops[name]
    shift_x, shift_y = timeline["placement_shift"]
    left = placement_in_ds_pixels(record["position"][0], OBJECT_ORIGIN_X_IN_TIMELINE, record["anchor"][0], record["point"][0], shift_x)
    top = placement_in_ds_pixels(record["position"][1], OBJECT_ORIGIN_Y_IN_TIMELINE, record["anchor"][1], record["point"][1], shift_y)
    return {
        "texture": name,
        "ticks": ticks,
        "x": left,
        "y": top,
        "width": cell_width / ART_PER_DS_PIXEL,
        "height": cell_height / ART_PER_DS_PIXEL,
        "u": (cell_x - crop_x) / crop_width,
        "v": (cell_y - crop_y) / crop_height,
        "u_size": cell_width / crop_width,
        "v_size": cell_height / crop_height,
    }


def frames_for_animation(timeline, ds_frames, crops):
    if ds_frames is None:
        ticks = ticks_per_record(timeline["records_per_ds_frame"])
    else:
        ticks = DS_TICKS_PER_FRAME
    frames = []
    for record in select_records(timeline, ds_frames):
        frames.append(frame_for_record(timeline, record, ticks, crops))
    return frames
