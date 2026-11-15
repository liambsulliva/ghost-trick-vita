import struct

import asset_writers
import bake_record
import chapter_assets
import ds_alert_balloons
import ds_rom
import ds_set_text
import ds_text
import ds_textures
import frame_layout

TEXTURE_FOLDER = "textures"
CODE_FILES = ["ds_balloons.py", "ds_text.py", "ds_textures.py", "asset_writers.py"]
ARM9_LOAD_ADDRESS = 0x02000000
SIZE_TABLE_ADDRESS = 0x02081f30
SIZE_ROW = "<HBxHH"
SPACE_TABLE_ADDRESS = 0x02081f14
SPACE_ROW = "<Bbbbb"
FRAME_TABLE_ADDRESS = 0x02084a48
FRAME_ROW = "<5B"
PIECE_TABLE_ADDRESS = 0x02084a6c
PIECE_ROW = "<4H"
FRAME_KINDS = 2
ONE_COLUMN_CANVAS = 0x6d
TWO_COLUMN_CANVAS = 0x73
PALETTE_BLOCK = 0
PALETTE_ENTRY = 7
OVAL_KINDS = (0, 1)
THORN_KINDS = (2, 3, 4)
SMALL_THORN = 4
MIDDLE_THORN = 2
LARGE_THORN = 3
TAIL_ON_THE_LEFT = 1
TAIL_ON_THE_RIGHT = 0
OVAL_LEFT = 0x12
SECOND_COLUMN_SHIFT = 0x40
ONE_LINE_TOP = 0xc
MORE_LINES_TOP = 4
SEGMENT_WIDTH = 6
SEGMENT_LIMIT = 0x18
SEGMENT_CENTER = 10
ONE_COLUMN_LIMIT = 0x60
TAIL_BACK_BYTES = 8
TALL_FRAME_LINES = (2, 3)
MANY_LINES_GAP = 1
LINE_LIMIT = 3
LINE_SLOT_WIDTH = 0x100
LINE_SLOT_HEIGHT = 0x14
CENTER_LINE_CODE = 3
INK_COLOR = 15
CLEAR_INDEX = 0
OPAQUE = 255
RGBA_BYTES = 4
GROW_TICKS = 8
OPEN_DELAY = 0
TEXTURE_SCALE = frame_layout.SCREEN_PIXELS_PER_DS_PIXEL
SHAPE_NAME = "ds_balloon_%s"
TEXT_NAME = "ds_balloon_text_%d_%s"


class DsBalloonError(ValueError):
    pass


def table_rows(arm9, address, row_format, count):
    size = struct.calcsize(row_format)
    return [struct.unpack_from(row_format, arm9, address - ARM9_LOAD_ADDRESS + size * index) for index in range(count)]


class Tables:
    def __init__(self, arm9):
        self.sizes = table_rows(arm9, SIZE_TABLE_ADDRESS, SIZE_ROW, max(THORN_KINDS) + 1)
        self.spaces = table_rows(arm9, SPACE_TABLE_ADDRESS, SPACE_ROW, max(THORN_KINDS) + 1)
        self.frames = table_rows(arm9, FRAME_TABLE_ADDRESS, FRAME_ROW, FRAME_KINDS)
        self.pieces = table_rows(arm9, PIECE_TABLE_ADDRESS, PIECE_ROW, FRAME_KINDS)


class Picture:
    def __init__(self, width, height, indexes=None):
        self.width = width
        self.height = height
        self.indexes = bytearray(indexes) if indexes is not None else bytearray(width * height)

    def put(self, x, y, index):
        if 0 <= x < self.width and 0 <= y < self.height:
            self.indexes[y * self.width + x] = index

    def at(self, x, y):
        return self.indexes[y * self.width + x]


def texture_indexes(texture_bytes):
    if len(texture_bytes) < ds_textures.HEADER_SIZE:
        raise DsBalloonError("a balloon texture is shorter than its header")
    magic, _version, texture_format, width_code, _height_code, data_offset, data_size, _palette_offset, _palette_size, width, height = struct.unpack_from(ds_textures.HEADER, texture_bytes, 0)
    if magic != ds_textures.MAGIC or texture_format != ds_textures.FORMAT_16_COLORS:
        raise DsBalloonError("a balloon texture is not a 16 colour DS texture")
    row_width = ds_textures.SMALLEST_ROW << width_code
    indexes = [index for index, _ in ds_textures.packed_indexes(texture_bytes[data_offset:data_offset + data_size], 4)]
    if len(indexes) < row_width * height:
        raise DsBalloonError("a balloon texture's size does not match its pixels")
    return Picture(width, height, bytes(indexes[row * row_width + column] for row in range(height) for column in range(width)))


def copy_rows(target, source, x, top, width, rows, mirrored=False):
    for row in range(rows):
        for column in range(width):
            source_column = width - 1 - column if mirrored else column
            target.put(x + column, top + row, source.at(source_column, row))


def segment_count(width):
    return min(width // SEGMENT_WIDTH + 1, SEGMENT_LIMIT)


def oval_shape(kind, line_count, widths, tables, pictures):
    segments = segment_count(max(widths))
    two_columns = segments * SEGMENT_WIDTH > ONE_COLUMN_LIMIT
    canvas = pictures(TWO_COLUMN_CANVAS if two_columns else ONE_COLUMN_CANVAS)
    shape = Picture(canvas.width, canvas.height)
    left = OVAL_LEFT + (SECOND_COLUMN_SHIFT if two_columns else 0) - int((segments - SEGMENT_CENTER) * SEGMENT_WIDTH / 2)
    top = ONE_LINE_TOP if line_count == 1 else MORE_LINES_TOP
    frame = 1 if line_count in TALL_FRAME_LINES else 0
    edge_bytes, rows, middle_bytes, tail_bytes, tail_rows = tables.frames[frame]
    left_piece, middle_piece, right_piece, tail_piece = (pictures(entry) for entry in tables.pieces[frame])
    side = TAIL_ON_THE_LEFT if kind == 0 else TAIL_ON_THE_RIGHT
    x = int(left / 2)
    copy_rows(shape, left_piece, 2 * x, top, 2 * edge_bytes, rows)
    x = x + edge_bytes
    tail_x = x - TAIL_BACK_BYTES
    for _ in range(segments):
        copy_rows(shape, middle_piece, 2 * x, top, 2 * middle_bytes, rows)
        x = x + middle_bytes
    if side == TAIL_ON_THE_RIGHT:
        tail_x = x - TAIL_BACK_BYTES
    copy_rows(shape, right_piece, 2 * x, top, 2 * edge_bytes, rows)
    half = middle_bytes * (segments - 1) // 2
    tail_x = tail_x + half if side == TAIL_ON_THE_LEFT else tail_x - half
    copy_rows(shape, tail_piece, 2 * tail_x, top + rows, 2 * tail_bytes, tail_rows, side == TAIL_ON_THE_RIGHT)
    space = tables.spaces[kind]
    return {"name": SHAPE_NAME % ("oval%d_%dl_%d" % (side, frame + 1, segments)), "picture": shape, "left": left, "top": top, "text_left": space[1], "span": segments * SEGMENT_WIDTH, "space": space}


def thorn_size(widths, tables):
    widest = max(widths)
    if tables.sizes[SMALL_THORN][2] > widest:
        return SMALL_THORN
    if tables.sizes[MIDDLE_THORN][2] > widest:
        return MIDDLE_THORN
    return LARGE_THORN


def thorn_shape(widths, tables, pictures):
    size = thorn_size(widths, tables)
    entry, _columns, box_width, box_height = tables.sizes[size]
    canvas = pictures(entry)
    space = tables.spaces[size]
    return {"name": SHAPE_NAME % ("thorn_%d" % entry), "picture": Picture(canvas.width, canvas.height, canvas.indexes), "left": int((canvas.width - box_width) / 2), "top": int((canvas.height - box_height) / 2), "text_left": space[1], "span": box_width, "space": space}


def balloon_lines(message):
    lines = [line for paragraph in message["pages"][0] for line in paragraph]
    return lines[:LINE_LIMIT]


def line_is_centered(line):
    return any(item[0] == "control" and item[1] == CENTER_LINE_CODE for item in line["items"])


def shape_for(kind, widths, tables, pictures):
    if kind in OVAL_KINDS:
        return oval_shape(kind, len(widths), widths, tables, pictures)
    if kind in THORN_KINDS:
        return thorn_shape(widths, tables, pictures)
    raise DsBalloonError("balloon kind %d draws no shape natively" % kind)


def text_picture(font, lines, shape):
    picture = Picture(shape["picture"].width, shape["picture"].height)
    margin, _text_left, shift_one, shift_two, shift_many = shape["space"]
    shift = (shift_one, shift_two)[len(lines) - 1] if len(lines) <= 2 else shift_many
    if len(lines) > 2:
        margin = MANY_LINES_GAP
    color = INK_COLOR
    for row_index, line in enumerate(lines):
        pixels, width, _icons, color = ds_text.draw_items(font, line["items"], color)
        offset = (int((shape["span"] - (width + 1)) / 2) & 0xff) if line_is_centered(line) else 0
        left = shape["left"] + shape["text_left"] + offset
        top = shape["top"] + shift + row_index * (font.shape["line_height"] + margin)
        for x, y, index in pixels:
            if 0 <= x < LINE_SLOT_WIDTH and 0 <= y < LINE_SLOT_HEIGHT:
                picture.put(left + x, top + y, index)
    return picture


def rgba_of(picture, palette):
    rgba = bytearray(picture.width * picture.height * RGBA_BYTES)
    for place, index in enumerate(picture.indexes):
        if index != CLEAR_INDEX:
            rgba[place * RGBA_BYTES:(place + 1) * RGBA_BYTES] = bytes(palette[index][:3]) + bytes((OPAQUE,))
    return bytes(rgba)


def ink_box(picture):
    columns = [column for column in range(picture.width) if any(picture.at(column, row) for row in range(picture.height))]
    rows = [row for row in range(picture.height) if any(picture.at(column, row) for column in range(picture.width))]
    if not columns:
        return None
    return columns[0], rows[0], columns[-1] + 1 - columns[0], rows[-1] + 1 - rows[0]


def cropped(picture, box):
    left, top, width, height = box
    return Picture(width, height, bytes(picture.at(left + column, top + row) for row in range(height) for column in range(width)))


def balloon_labels(scripts):
    found = {}
    for path, script in scripts.items():
        for _, _, op, operands in script.operations:
            if op != chapter_assets.WORDED_BALLOON_OP or len(operands) < 3:
                continue
            label = script.text_of(operands[0])
            if label is not None and (label, operands[2]) not in found:
                found[(label, operands[2])] = path
    return found


def part(texture, dx, dy, after, grow):
    return {"texture": texture, "dx": round(dx, 4), "dy": round(dy, 4), "after": after, "grow": grow, "texture_scale": TEXTURE_SCALE}


def balloon_art(label, kind, message, font, tables, pictures):
    lines = balloon_lines(message)
    if not lines:
        raise DsBalloonError("%s has no line" % label)
    widths = [ds_text.draw_items(font, line["items"], INK_COLOR)[1] for line in lines]
    shape = shape_for(kind, widths, tables, pictures)
    text = text_picture(font, lines, shape)
    box = ink_box(text)
    parts = [part(shape["name"], 0, 0, 0, GROW_TICKS)]
    text_image = None
    if box is not None:
        text_image = cropped(text, box)
        dx = box[0] + box[2] / 2 - text.width / 2
        dy = box[1] + box[3] / 2 - text.height / 2
        parts.append(part(TEXT_NAME % (kind, label), dx, dy, GROW_TICKS, 0))
    return {"kind": kind, "label": label, "delay": OPEN_DELAY, "parts": parts}, shape, text_image


def compose(balloons, messages, font, tables, pictures):
    rows = []
    images = {}
    refused = []
    for (label, kind), path in balloons.items():
        message = messages(label, path)
        if message is None:
            refused.append("%s (no DS text)" % label)
            continue
        try:
            row, shape, text_image = balloon_art(label, kind, message, font, tables, pictures)
        except DsBalloonError as error:
            refused.append("%s (%s)" % (label, error))
            continue
        images.setdefault(shape["name"], shape["picture"])
        if text_image is not None:
            images[TEXT_NAME % (kind, label)] = text_image
        rows.append(row)
    return rows, images, refused


def store_images(images, palette, record):
    code = bake_record.code_fingerprint(CODE_FILES)
    paths = []
    sizes = {}
    for name, picture in images.items():
        asset_writers.check_name(name)
        relative_path = TEXTURE_FOLDER + "/" + name + ".gtex"
        rgba = rgba_of(picture, palette)
        key = record.key_for(code, {"width": picture.width, "height": picture.height}, rgba)
        if record.read_if_current(relative_path, key) is None:
            record.store(relative_path, key, asset_writers.gtex_bytes(picture.width, picture.height, asset_writers.FORMAT_RGBA8888, rgba))
        paths.append(relative_path)
        sizes[name] = (picture.width, picture.height)
    return paths, sizes


def bake(set_description, scripts, arm9, cartridge, rom_path, record):
    language = set_description.get("scripts", {}).get("language", "en")
    with ds_rom.DsRom(rom_path) as rom:
        font = ds_text.font_from_rom(rom, ds_text.BALLOON_FONT, arm9)
        book = ds_set_text.MessageBook(rom, scripts, language)
    palette = ds_text.palette_of_bgr555(struct.unpack_from("<16H", cartridge.sprites.entry(PALETTE_BLOCK, PALETTE_ENTRY)))
    known = {}

    def pictures(entry):
        if entry not in known:
            known[entry] = texture_indexes(cartridge.textures.texture_bytes(entry))
        return known[entry]

    rows, images, refused = compose(balloon_labels(scripts), book.find, font, Tables(arm9), pictures)
    if refused:
        print("ds balloons left out: %d (%s)" % (len(refused), ", ".join(refused[:12])))
    paths, sizes = store_images(images, palette, record)
    alert_rows, alert_paths, alert_sizes = ds_alert_balloons.bake(set_description, scripts, cartridge, record)
    return rows + alert_rows, paths + alert_paths, dict(sizes, **alert_sizes)
