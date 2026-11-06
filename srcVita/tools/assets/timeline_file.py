import struct

TYPE_SHEET = 147581786
TYPE_RECORD = 4048237731
TYPE_ROOT_ANIMATION = 240123514
TYPE_ROOT_BACKGROUND = 2353218137
RECORD_SHORT_COUNT = 20
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
    sheet = {}
    texture_count = reader.read_i32()
    sheet["textures"] = [reader.read_string() for _ in range(texture_count)]
    sheet["atlas"] = (reader.read_i32(), reader.read_i32())
    sheet["cell"] = (reader.read_i32(), reader.read_i32())
    page_count = reader.read_i32()
    sheet["cells_per_texture"] = [reader.read_i32() for _ in range(page_count)]
    grid = reader.read_vector()
    sheet["grid"] = (int(grid[0]), int(grid[1]))
    sheet["total_cells"] = reader.read_i32()
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
    cell = reader.read_i32()
    for _ in range(UNUSED_RECORD_TAIL_VALUES):
        reader.read_i32()
    return {"shorts": shorts, "position": (position[0], position[1]), "cell": cell}


def read_objects(data):
    if data[:4] != b"USR\0":
        raise TimelineError("not a USR file")
    rsz_offset = struct.unpack_from("<Q", data, 32)[0]
    if data[rsz_offset:rsz_offset + 4] != b"RSZ\0":
        raise TimelineError("no RSZ block")
    instance_count = struct.unpack_from("<I", data, rsz_offset + 12)[0]
    instance_offset, data_offset = struct.unpack_from("<QQ", data, rsz_offset + 24)
    types = []
    for index in range(instance_count):
        types.append(struct.unpack_from("<I", data, rsz_offset + instance_offset + 8 * index)[0])
    reader = ByteReader(data, rsz_offset + data_offset)
    objects = [None]
    for type_id in types[1:]:
        if type_id == TYPE_SHEET:
            objects.append(("sheet", read_sheet(reader)))
        elif type_id == TYPE_RECORD:
            objects.append(("record", read_record(reader)))
        elif type_id == TYPE_ROOT_ANIMATION:
            sheet_index = reader.read_i32()
            reference_count = reader.read_i32()
            references = [reader.read_i32() for _ in range(reference_count)]
            tail = [reader.read_i32() for _ in range(5)]
            objects.append(("root", sheet_index, references, tail))
        elif type_id == TYPE_ROOT_BACKGROUND:
            objects.append(("background_root", reader.read_i32()))
        else:
            raise TimelineError("unknown object type " + str(type_id))
    if reader.position != len(data):
        raise TimelineError("file has unread bytes")
    return objects


def page_starts(cells_per_texture):
    starts = []
    first_cell = 0
    for cell_count in cells_per_texture:
        starts.append((first_cell, cell_count))
        first_cell = first_cell + cell_count
    return starts


def find_page(starts, cell):
    for page_index, (first_cell, cell_count) in enumerate(starts):
        if first_cell <= cell < first_cell + cell_count:
            return page_index, cell - first_cell
    raise TimelineError("cell " + str(cell) + " is on no page")


def build_timeline(objects):
    root = objects[-1]
    if root[0] != "root":
        raise TimelineError("file is not an animation timeline")
    sheet = objects[root[1]][1]
    references = root[2]
    ds_frame_count = root[3][1]
    if ds_frame_count <= 0 or len(references) % ds_frame_count != 0:
        raise TimelineError("record count is not a multiple of the DS frame count")
    starts = page_starts(sheet["cells_per_texture"])
    records = []
    for reference in references:
        source = objects[reference][1]
        shorts = source["shorts"]
        is_blank = shorts[0] == 0 and source["position"] == (0.0, 0.0)
        page = None
        local_cell = None
        if not is_blank:
            page, local_cell = find_page(starts, source["cell"])
        records.append({
            "cell": source["cell"],
            "page": page,
            "local_cell": local_cell,
            "position": source["position"],
            "anchor": (shorts[1], shorts[2]),
            "point": (shorts[6], shorts[7]),
            "is_blank": is_blank,
        })
    return {
        "textures": sheet["textures"],
        "atlas": sheet["atlas"],
        "cell": sheet["cell"],
        "records_per_ds_frame": len(references) // ds_frame_count,
        "ds_frame_count": ds_frame_count,
        "placement_shift": (root[3][3], root[3][4]),
        "records": records,
    }


def decode_timeline(data):
    return build_timeline(read_objects(data))
