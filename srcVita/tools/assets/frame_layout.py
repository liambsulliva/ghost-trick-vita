import asset_writers

DS_TICKS_PER_FRAME = 4
ART_PER_DS_PIXEL = 4
SCREEN_PIXELS_PER_DS_PIXEL = 720 / 256
TARGET_SCALE = SCREEN_PIXELS_PER_DS_PIXEL / ART_PER_DS_PIXEL
ANCHOR_UNITS_PER_DS_PIXEL = 2
ANCHOR_NOISE_UNITS = 1
TEX_VERSION_SUFFIX = ".691230823"
PLACEHOLDER_PAGE_NAME = "nullwhite"


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


class WholeFrames(list):
    pass


def select_records(timeline, ds_frames):
    if ds_frames is None:
        return list(timeline["records"])
    step = timeline["records_per_ds_frame"]
    selected = []
    if isinstance(ds_frames, WholeFrames):
        for ds_frame in ds_frames:
            if ds_frame * step < len(timeline["records"]):
                selected.extend(timeline["records"][ds_frame * step:(ds_frame + 1) * step])
        return selected
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
    origin_ds = origin / ART_PER_DS_PIXEL
    offset_ds = anchor_offset(anchor, point)
    shift_ds = shift / ART_PER_DS_PIXEL
    return position_ds - origin_ds - offset_ds + shift_ds


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


def record_draws_art(timeline, record):
    return not record["is_blank"] and record_texture_name(timeline, record) != PLACEHOLDER_PAGE_NAME


def add_used_cells_to_crops(crops, timeline, ds_frames):
    for record in select_records(timeline, ds_frames):
        if not record_draws_art(timeline, record):
            continue
        name = record_texture_name(timeline, record)
        rectangle = cell_rectangle(timeline, record)
        if name in crops:
            rectangle = merge_rectangles(crops[name], rectangle)
        crops[name] = rectangle


def inset_away_from_neighbours(cell_start, cell_length, crop_start, crop_length, baked_length):
    half_baked_texel = 0.5 * crop_length / baked_length
    before = half_baked_texel if cell_start > crop_start else 0
    after = half_baked_texel if cell_start + cell_length < crop_start + crop_length else 0
    return before, after


def frame_for_record(timeline, record, ticks, crops):
    if not record_draws_art(timeline, record):
        return asset_writers.blank_frame(ticks)
    name = record_texture_name(timeline, record)
    cell_x, cell_y, cell_width, cell_height = cell_rectangle(timeline, record)
    crop_x, crop_y, crop_width, crop_height = crops[name]
    shift_x, shift_y = timeline["placement_shift"]
    left = placement_in_ds_pixels(record["position"][0], record["origin"][0], record["anchor"][0], record["point"][0], shift_x)
    top = placement_in_ds_pixels(record["position"][1], record["origin"][1], record["anchor"][1], record["point"][1], shift_y)
    baked_width, baked_height = target_size(crop_width, crop_height)
    inset_left, inset_right = inset_away_from_neighbours(cell_x, cell_width, crop_x, crop_width, baked_width)
    inset_top, inset_bottom = inset_away_from_neighbours(cell_y, cell_height, crop_y, crop_height, baked_height)
    drawn_width = cell_width - inset_left - inset_right
    drawn_height = cell_height - inset_top - inset_bottom
    return {
        "texture": name,
        "ticks": ticks,
        "x": left + inset_left / ART_PER_DS_PIXEL,
        "y": top + inset_top / ART_PER_DS_PIXEL,
        "width": drawn_width / ART_PER_DS_PIXEL,
        "height": drawn_height / ART_PER_DS_PIXEL,
        "u": (cell_x + inset_left - crop_x) / crop_width,
        "v": (cell_y + inset_top - crop_y) / crop_height,
        "u_size": drawn_width / crop_width,
        "v_size": drawn_height / crop_height,
    }


def frames_step_down(ds_frames):
    return len(ds_frames) > 1 and all(later < earlier for earlier, later in zip(ds_frames, ds_frames[1:]))


def records_for_table(timeline, ds_frames):
    step = timeline["records_per_ds_frame"]
    if len(ds_frames) == 1:
        return [timeline["records"][ds_frames[0] * step]], 1
    selected = []
    for ds_frame in ds_frames:
        records = timeline["records"][ds_frame * step:(ds_frame + 1) * step]
        selected.extend(reversed(records) if frames_step_down(ds_frames) else records)
    return selected, step


def frames_for_table(timeline, ds_frames, crops):
    records, records_per_frame = records_for_table(timeline, ds_frames)
    ticks = ticks_per_record(records_per_frame)
    return [frame_for_record(timeline, record, ticks, crops) for record in records], records_per_frame


def frames_for_animation(timeline, ds_frames, crops):
    if ds_frames is None:
        ticks = ticks_per_record(timeline["records_per_ds_frame"])
    else:
        ticks = DS_TICKS_PER_FRAME
    frames = []
    for record in select_records(timeline, ds_frames):
        frames.append(frame_for_record(timeline, record, ticks, crops))
    return frames
