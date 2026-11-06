import struct

TYPE_SHEET = 147581786
TYPE_RECORD = 4048237731
TYPE_ROOT_ANIMATION = 240123514
TYPE_ROOT_BACKGROUND = 2353218137
USR_RSZ_POSITION_OFFSET = 32
RSZ_INSTANCE_COUNT_OFFSET = 12
RSZ_TABLE_OFFSETS_OFFSET = 24
RECORD_SHORT_COUNT = 20
SHORT_BLANK_MARKER = 0
SHORT_ANCHOR_X = 1
SHORT_ANCHOR_Y = 2
SHORT_POINT_X = 6
SHORT_POINT_Y = 7
UNUSED_RECORD_TAIL_VALUES = 3


class TimelineError(ValueError):
    pass


class ByteReader:
    def __init__(self, data, position):
        self.data = data
        self.position = position

    def align(self, size):
        self.position = (self.position + size - 1) & ~(size - 1)

    def read_i32(self):
        self.align(4)
        value = struct.unpack_from("<i", self.data, self.position)[0]
        self.position = self.position + 4
        return value

    def read_i16(self):
        self.align(2)
        value = struct.unpack_from("<h", self.data, self.position)[0]
        self.position = self.position + 2
        return value

    def read_u8(self):
        value = self.data[self.position]
        self.position = self.position + 1
        return value

    def read_string(self):
        length = self.read_i32()
        if length < 0 or self.position + 2 * length > len(self.data):
            raise TimelineError("bad string length")
        text = self.data[self.position:self.position + 2 * length].decode("utf-16-le")
        self.position = self.position + 2 * length
        return text.rstrip("\0")

    def read_vector(self):
        self.align(16)
        value = struct.unpack_from("<4f", self.data, self.position)
        self.position = self.position + 16
        return value


def read_sheet(reader):
    sheet = {"kind": "sheet"}
    page_path_count = reader.read_i32()
    sheet["pages"] = [reader.read_string() for _ in range(page_path_count)]
    sheet["atlas_size"] = (reader.read_i32(), reader.read_i32())
    sheet["cell_size"] = (reader.read_i32(), reader.read_i32())
    page_count = reader.read_i32()
    sheet["cells_per_page"] = [reader.read_i32() for _ in range(page_count)]
    reader.read_vector()
    reader.read_i32()
    reader.read_u8()
    reader.read_u8()
    return sheet


def read_record(reader):
    short_count = reader.read_i32()
    if short_count != RECORD_SHORT_COUNT:
        raise TimelineError("record short array has the wrong length")
    shorts = [reader.read_i16() for _ in range(short_count)]
    reader.read_i32()
    position = reader.read_vector()
    cell_index = reader.read_i32()
    for _ in range(UNUSED_RECORD_TAIL_VALUES):
        reader.read_i32()
    return {"kind": "record", "shorts": shorts, "position": (position[0], position[1]), "cell_index": cell_index}


def read_animation_root(reader):
    sheet_index = reader.read_i32()
    record_count = reader.read_i32()
    record_indices = [reader.read_i32() for _ in range(record_count)]
    _, ds_frame_count, _, shift_x, shift_y = [reader.read_i32() for _ in range(5)]
    return {"kind": "root", "sheet_index": sheet_index, "record_indices": record_indices, "ds_frame_count": ds_frame_count, "placement_shift": (shift_x, shift_y)}


def read_background_root(reader):
    reader.read_i32()
    return {"kind": "background_root"}


def read_objects(data):
    if data[:4] != b"USR\0":
        raise TimelineError("not a USR file")
    rsz_offset = struct.unpack_from("<Q", data, USR_RSZ_POSITION_OFFSET)[0]
    if data[rsz_offset:rsz_offset + 4] != b"RSZ\0":
        raise TimelineError("no RSZ block")
    instance_count = struct.unpack_from("<I", data, rsz_offset + RSZ_INSTANCE_COUNT_OFFSET)[0]
    instance_offset, data_offset = struct.unpack_from("<QQ", data, rsz_offset + RSZ_TABLE_OFFSETS_OFFSET)
    types = []
    for index in range(instance_count):
        types.append(struct.unpack_from("<I", data, rsz_offset + instance_offset + 8 * index)[0])
    reader = ByteReader(data, rsz_offset + data_offset)
    objects = [None]
    for type_id in types[1:]:
        if type_id == TYPE_SHEET:
            objects.append(read_sheet(reader))
        elif type_id == TYPE_RECORD:
            objects.append(read_record(reader))
        elif type_id == TYPE_ROOT_ANIMATION:
            objects.append(read_animation_root(reader))
        elif type_id == TYPE_ROOT_BACKGROUND:
            objects.append(read_background_root(reader))
        else:
            raise TimelineError("unknown object type " + str(type_id))
    if reader.position != len(data):
        raise TimelineError("file has unread bytes")
    return objects


def page_starts(cells_per_page):
    starts = []
    first_cell = 0
    for cell_count in cells_per_page:
        starts.append((first_cell, cell_count))
        first_cell = first_cell + cell_count
    return starts


def find_page(starts, cell_index):
    for page_index, (first_cell, cell_count) in enumerate(starts):
        if first_cell <= cell_index < first_cell + cell_count:
            return page_index, cell_index - first_cell
    raise TimelineError("cell " + str(cell_index) + " is on no page")


def build_timeline(objects):
    root = objects[-1]
    if root["kind"] != "root":
        raise TimelineError("file is not an animation timeline")
    sheet = objects[root["sheet_index"]]
    record_indices = root["record_indices"]
    ds_frame_count = root["ds_frame_count"]
    if ds_frame_count <= 0 or len(record_indices) % ds_frame_count != 0:
        raise TimelineError("record count is not a multiple of the DS frame count")
    starts = page_starts(sheet["cells_per_page"])
    records = []
    for record_index in record_indices:
        source = objects[record_index]
        shorts = source["shorts"]
        is_blank = shorts[SHORT_BLANK_MARKER] == 0 and source["position"] == (0.0, 0.0)
        page = None
        cell_in_page = None
        if not is_blank:
            page, cell_in_page = find_page(starts, source["cell_index"])
        records.append({
            "page": page,
            "cell_in_page": cell_in_page,
            "position": source["position"],
            "anchor": (shorts[SHORT_ANCHOR_X], shorts[SHORT_ANCHOR_Y]),
            "point": (shorts[SHORT_POINT_X], shorts[SHORT_POINT_Y]),
            "is_blank": is_blank,
        })
    return {
        "pages": sheet["pages"],
        "atlas_size": sheet["atlas_size"],
        "cell_size": sheet["cell_size"],
        "records_per_ds_frame": len(record_indices) // ds_frame_count,
        "placement_shift": root["placement_shift"],
        "records": records,
    }


def decode_timeline(data):
    return build_timeline(read_objects(data))
