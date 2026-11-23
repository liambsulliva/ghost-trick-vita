import re
import struct

import numpy as np

import asset_writers
import bake_record
import ds_rom
import ds_sprites
import ds_text
import ds_textures
import lmg_decode
import lz
import message_cues
import nds_unpack
import pause_people
import texture_encoder

CODE_FILES = ["ds_people.py", "ds_text.py", "ds_textures.py", "ds_sprites.py", "pause_people.py", "asset_writers.py"]
TEXTURE_FOLDER = "textures"
OVERLAY_ID = 6
ARM9_LOAD_ADDRESS = 0x02000000
OVERLAY_TABLE_FIELD = 0x50
FILE_TABLE_FIELD = 0x48
OVERLAY_ROW_SIZE = 32
OVERLAY_IS_PACKED = 1 << 24
PORTRAIT_TABLE_ADDRESS = 0x020dacdc
PORTRAIT_ROW_SIZE = 16
PLACE_PICTURE_TABLE_ADDRESS = 0x020d88fc
PLATE_TABLES_ADDRESS = 0x020d8a48
LEVEL_TABLES_ADDRESS = 0x020d8a50
PERSON_NAMES_ADDRESS = 0x020d8d34
PLACE_NAMES_ADDRESS = 0x020d8cd8
PERSON_COUNT = 45
PLACE_NAME_COUNT = 23
PLACE_PICTURE_COUNT = 25
MESSAGE_FILE = "system/database_0000_Expand.%s.xml.lz"
UNKNOWN_PLACE_PICTURE = 0x1b4d
UNKNOWN_PLACE_PLATE = 0xad
CARD_RESTING = 0x79
PORTRAIT_GROUND_TEXTURE = 0x51
NAME_BAND_TEXTURE = 0x4e
PHONE_BAND_TEXTURE = 0x4f
SCREEN_SIZE = (256, 192)
PORTRAIT_LEFT = 64
PLACE_PICTURE_TOP = 12
CELL_SIZE = (128, 96)
CELL_ORIGIN = (64, 48)
THUMB_COLUMNS = 8
NOTE_SIZE = (224, 152)
NOTE_SCALE = 2
BAND_HEIGHT = 24
BODY_LEFT = 0
INK_TOP_IN_A_LINE = 10
INK_ROWS_IN_A_LINE = 16
BODY_GAP_UNDER_THE_BANDS = 3
BODY_LINE_PITCH_MOST = 14
INK_COLOR = 3
INK_ALPHAS = np.array([0, 102, 178, 255], dtype=np.uint8)
BAND_INK = (255, 255, 255)
BODY_INK = (255, 255, 255)
NOTE_PAPER = (0, 0, 0, 255)
CARD_BODY_IS_DARKER_THAN = 40
SISSEL = 2
SISSEL_AS_A_SOUL = 6
SISSEL_SHOWS_HIS_SOUL_FROM_LEVEL = 150
LABEL_PATTERN = re.compile(r"^(.+?)(_sub2?)?_(\d{3})$")


class PeopleRefused(Exception):
    pass


def read_overlay(rom, overlay_id):
    header = rom.read_range(0, 0x60)
    table_offset, table_size = struct.unpack_from("<II", header, OVERLAY_TABLE_FIELD)
    file_table = struct.unpack_from("<I", header, FILE_TABLE_FIELD)[0]
    rows = rom.read_range(table_offset, table_size)
    for row in range(table_size // OVERLAY_ROW_SIZE):
        found, address, _, _, _, _, file_id, flags = struct.unpack_from("<8I", rows, row * OVERLAY_ROW_SIZE)
        if found == overlay_id:
            start, end = struct.unpack("<II", rom.read_range(file_table + file_id * 8, 8))
            data = rom.read_range(start, end - start)
            return address, nds_unpack.blz_decompress(data) if flags & OVERLAY_IS_PACKED else data
    raise PeopleRefused("the DS ROM has no overlay %d" % overlay_id)


class Memory:
    def __init__(self, arm9, overlay_address, overlay):
        self.arm9 = arm9
        self.overlay_address = overlay_address
        self.overlay = overlay

    def read(self, address, size):
        if self.overlay_address <= address and address + size <= self.overlay_address + len(self.overlay):
            return self.overlay[address - self.overlay_address:address - self.overlay_address + size]
        offset = address - ARM9_LOAD_ADDRESS
        if offset < 0 or offset + size > len(self.arm9):
            raise PeopleRefused("the People tables point outside the program at %#x" % address)
        return self.arm9[offset:offset + size]

    def words(self, address, count, kind="I"):
        return struct.unpack("<%d%s" % (count, kind), self.read(address, count * struct.calcsize(kind)))

    def text(self, address):
        data = self.read(address, 64)
        return data[:data.index(b"\0")].decode("ascii")


def read_tables(memory):
    person_plates_address, place_plates_address = memory.words(PLATE_TABLES_ADDRESS, 2)
    person_levels_address, place_levels_address = memory.words(LEVEL_TABLES_ADDRESS, 2)

    def levels(address, count):
        found = []
        for index in range(count):
            level_count, levels_address = struct.unpack("<iI", memory.read(address + index * 8, 8))
            found.append(list(memory.words(levels_address, level_count, "H")) if level_count > 0 else [])
        return found

    return {
        "person_names": [memory.text(address) for address in memory.words(PERSON_NAMES_ADDRESS, PERSON_COUNT)],
        "place_names": [memory.text(address) for address in memory.words(PLACE_NAMES_ADDRESS, PLACE_NAME_COUNT)],
        "portraits": [struct.unpack_from("<h", memory.read(PORTRAIT_TABLE_ADDRESS + index * PORTRAIT_ROW_SIZE, 2))[0] for index in range(PERSON_COUNT)],
        "place_pictures": list(memory.words(PLACE_PICTURE_TABLE_ADDRESS, PLACE_PICTURE_COUNT, "H")),
        "person_plates": list(memory.words(person_plates_address, PERSON_COUNT)),
        "place_plates": list(memory.words(place_plates_address, PLACE_PICTURE_COUNT)),
        "person_levels": levels(person_levels_address, PERSON_COUNT),
        "place_levels": levels(place_levels_address, PLACE_NAME_COUNT),
    }


def image_of(width, height, rgba):
    return np.frombuffer(rgba, dtype=np.uint8).reshape(height, width, 4).copy()


def pasted(canvas, image, left, top):
    height, width = image.shape[:2]
    canvas_height, canvas_width = canvas.shape[:2]
    from_x = max(0, -left)
    from_y = max(0, -top)
    to_x = min(width, canvas_width - left)
    to_y = min(height, canvas_height - top)
    if from_x >= to_x or from_y >= to_y:
        return
    source = image[from_y:to_y, from_x:to_x].astype(np.uint16)
    target = canvas[top + from_y:top + to_y, left + from_x:left + to_x]
    cover = source[..., 3:4]
    target[..., :3] = (source[..., :3] * cover + target[..., :3].astype(np.uint16) * (255 - cover)) // 255
    target[..., 3] = np.maximum(target[..., 3], source[..., 3].astype(np.uint8))


def stretched(image, width, height):
    rows = np.arange(height) * image.shape[0] // height
    columns = np.arange(width) * image.shape[1] // width
    return image[rows][:, columns]


def black_canvas(size):
    canvas = np.zeros((size[1], size[0], 4), dtype=np.uint8)
    canvas[..., 3] = 255
    return canvas


def face_pixels(textures, portrait, language):
    canvas = black_canvas(SCREEN_SIZE)
    pasted(canvas, stretched(image_of(*textures.texture(PORTRAIT_GROUND_TEXTURE, language)), SCREEN_SIZE[0], SCREEN_SIZE[1]), 0, 0)
    pasted(canvas, image_of(*textures.texture(portrait, language)), PORTRAIT_LEFT, 0)
    return canvas


def face_cutout(face):
    return face[:CELL_SIZE[1], PORTRAIT_LEFT:PORTRAIT_LEFT + CELL_SIZE[0]]


def first_frame(sprites, arm9, resource, language):
    width, height, rgba, left, top = ds_sprites.frames_of_resource(sprites, arm9, resource, language)[0]
    return image_of(width, height, rgba), left, top


def place_pixels(sprites, arm9, resource, language):
    canvas = black_canvas(SCREEN_SIZE)
    image, _, _ = first_frame(sprites, arm9, resource, language)
    pasted(canvas, image, (SCREEN_SIZE[0] - image.shape[1]) // 2, PLACE_PICTURE_TOP)
    return canvas


def cell_pixels(sprites, arm9, resources, language):
    cell = np.zeros((CELL_SIZE[1], CELL_SIZE[0], 4), dtype=np.uint8)
    for resource in resources:
        image, left, top = first_frame(sprites, arm9, resource, language)
        pasted(cell, image, CELL_ORIGIN[0] + left, CELL_ORIGIN[1] + top)
    return cell


def frame_pixels(sprites, arm9, language):
    cell = cell_pixels(sprites, arm9, [CARD_RESTING], language)
    body = cell[..., :3].max(axis=2) < CARD_BODY_IS_DARKER_THAN
    cell[body] = 0
    return cell


def read_messages(rom, language_code):
    plain = lz.try_decompress(rom.read_file(MESSAGE_FILE % language_code))
    if plain is None:
        raise PeopleRefused("the DS database text for %s could not be decompressed" % language_code)
    targets = message_cues.label_targets(lmg_decode.read_labels(plain, lmg_decode.read_header(plain)))
    return plain, {label: target for label, target in targets.items() if LABEL_PATTERN.match(label)}


def lines_of(plain, targets, label):
    if label not in targets:
        return []
    pages = ds_text.pages_of_items(ds_text.message_items(plain, targets[label]))
    return [line for page in pages for group in page for line in group]


def line_pixels(font, line, color):
    drawn, cursor, _, _ = ds_text.draw_items(font, line["items"], INK_COLOR)
    left = min([0] + [x for x, _, _ in drawn])
    right = max([cursor] + [x + 1 for x, _, _ in drawn])
    top = min([0] + [y for _, y, _ in drawn])
    bottom = max([font.shape["cell_height"]] + [y + 1 for _, y, _ in drawn])
    pixels = np.zeros((bottom - top, max(right - left, 1), 4), dtype=np.uint8)
    pixels[..., :3] = color
    for x, y, value in drawn:
        pixels[y - top, x - left, 3] = INK_ALPHAS[min(value, len(INK_ALPHAS) - 1)]
    return pixels, -left, -top


def note_pixels(font, textures, language, title_lines, phone_lines, body_lines):
    card = np.zeros((NOTE_SIZE[1], NOTE_SIZE[0], 4), dtype=np.uint8)
    card[...] = NOTE_PAPER
    bands = [(NAME_BAND_TEXTURE, title_lines)] + ([(PHONE_BAND_TEXTURE, phone_lines)] if phone_lines is not None else [])
    for row, (texture, lines) in enumerate(bands):
        band = image_of(*textures.texture(texture, language))
        pasted(card, band, (NOTE_SIZE[0] - band.shape[1]) // 2, row * BAND_HEIGHT)
        if lines:
            pixels, _, cell_top = line_pixels(font, lines[0], BAND_INK)
            pasted(card, pixels, (NOTE_SIZE[0] - pixels.shape[1]) // 2, row * BAND_HEIGHT + (BAND_HEIGHT - INK_ROWS_IN_A_LINE) // 2 - INK_TOP_IN_A_LINE - cell_top)
    top = len(bands) * BAND_HEIGHT + BODY_GAP_UNDER_THE_BANDS
    room = NOTE_SIZE[1] - top - INK_ROWS_IN_A_LINE
    pitch = min(BODY_LINE_PITCH_MOST, room // (len(body_lines) - 1)) if len(body_lines) > 1 else BODY_LINE_PITCH_MOST
    for row, line in enumerate(body_lines):
        pixels, pen_x, cell_top = line_pixels(font, line, BODY_INK)
        pasted(card, pixels, BODY_LEFT - pen_x, top - INK_TOP_IN_A_LINE + row * pitch - cell_top)
    return np.repeat(np.repeat(card, NOTE_SCALE, axis=0), NOTE_SCALE, axis=1)


def rgba_gtex(pixels):
    return asset_writers.gtex_bytes(pixels.shape[1], pixels.shape[0], asset_writers.FORMAT_RGBA8888, np.ascontiguousarray(pixels).tobytes())


def ids_by_name(listed_names, native_names):
    return {name: listed_names.index(name) + 1 for name in native_names if name in listed_names}


def bake(people, rom_path, arm9, textures, sprites, language, language_code, record, work_folder):
    code = bake_record.code_fingerprint(CODE_FILES)
    with ds_rom.DsRom(rom_path) as rom:
        overlay_address, overlay = read_overlay(rom, OVERLAY_ID)
        plain, targets = read_messages(rom, language_code)
        font = ds_text.font_from_rom(rom, ds_text.MAIN_FONT, arm9)
    tables = read_tables(Memory(arm9, overlay_address, overlay))
    person_ids = ids_by_name(people["names"], tables["person_names"])
    place_ids = ids_by_name(people["places"], tables["place_names"])
    files = {}
    paths = []
    sizes = {}
    lines = ["order " + " ".join(str(value) for value in people["order"])]
    for key in ("skip_people", "skip_places", "first_people", "first_places"):
        lines.append(key + " " + " ".join(str(value) for value in people[key]))
    lines.append("place_count %d" % len(people["places"]))
    cells = []

    def stored(name, key, build):
        relative_path = TEXTURE_FOLDER + "/" + name + ".gtex"
        gtex = record.read_if_current(relative_path, key)
        if gtex is None:
            gtex = build()
            record.store(relative_path, key, gtex)
        paths.append(relative_path)
        sizes[name] = asset_writers.read_gtex_header(gtex)[:2]

    for index, name in enumerate(tables["person_names"]):
        if name not in person_ids:
            continue
        portrait = tables["portraits"][index]
        face = "%s%02d" % (pause_people.FACE_PREFIX, person_ids[name])
        source = textures.texture_bytes(portrait, language) + textures.texture_bytes(PORTRAIT_GROUND_TEXTURE, language)
        stored(face, record.key_for(code, {"face": portrait}, source), lambda portrait=portrait: rgba_gtex(face_pixels(textures, portrait, language)))
        lines.append("face %d %s" % (person_ids[name], face))
        cells.append((face, portrait))
    if SISSEL < len(tables["person_names"]) and tables["person_names"][SISSEL] in person_ids and tables["person_names"][SISSEL_AS_A_SOUL] in person_ids:
        sissel = person_ids[tables["person_names"][SISSEL]]
        if SISSEL_SHOWS_HIS_SOUL_FROM_LEVEL in tables["person_levels"][SISSEL]:
            lines.append("reveal 1 note 1 %d %d" % (sissel, SISSEL_SHOWS_HIS_SOUL_FROM_LEVEL))
            lines.append("reveal_face 1 %d %s%02d" % (sissel, pause_people.FACE_PREFIX, person_ids[tables["person_names"][SISSEL_AS_A_SOUL]]))
    for index, name in enumerate(tables["place_names"]):
        picture = tables["place_pictures"][index]
        if name not in place_ids or picture == UNKNOWN_PLACE_PICTURE:
            continue
        place = "%s%02d" % (pause_people.PLACE_PREFIX, place_ids[name])
        stored(place, record.key_for(code, {"place": picture, "language": language}, b""), lambda picture=picture: rgba_gtex(place_pixels(sprites, arm9, picture, language)))
        lines.append("place %d %s" % (place_ids[name], place))
        cells.append((place, [tables["place_plates"][index]]))
    cells.append(("unknown", [UNKNOWN_PLACE_PLATE]))
    cells.append(("frame", None))
    rows = (len(cells) + THUMB_COLUMNS - 1) // THUMB_COLUMNS
    sheet = np.zeros((rows * CELL_SIZE[1], THUMB_COLUMNS * CELL_SIZE[0], 4), dtype=np.uint8)
    for index, (name, resources) in enumerate(cells):
        left = index % THUMB_COLUMNS * CELL_SIZE[0]
        top = index // THUMB_COLUMNS * CELL_SIZE[1]
        if resources is None:
            cell = frame_pixels(sprites, arm9, language)
        elif isinstance(resources, int):
            cell = face_cutout(face_pixels(textures, resources, language))
        else:
            cell = cell_pixels(sprites, arm9, resources, language)
        sheet[top:top + CELL_SIZE[1], left:left + CELL_SIZE[0]] = cell
        lines.append("thumb %s %d %d %d %d" % (name, left, top, CELL_SIZE[0], CELL_SIZE[1]))
    sheet_path = TEXTURE_FOLDER + "/" + pause_people.THUMB_SHEET_NAME + ".gtex"
    sheet_gtex = rgba_gtex(sheet)
    record.store(sheet_path, record.key_for(code, {"cells": [[name, resources] for name, resources in cells], "language": language}, sheet_gtex), sheet_gtex)
    paths.append(sheet_path)
    sizes[pause_people.THUMB_SHEET_NAME] = (sheet.shape[1], sheet.shape[0])
    stored(pause_people.SPOT_NAME, record.key_for(code, {"spot": "black"}, b""), lambda: rgba_gtex(black_canvas((8, 8))))
    font_fingerprint = bake_record.sha256_of_bytes(font.glyphs if hasattr(font, "glyphs") else repr(font.shape).encode("ascii"))
    for list_number, names, ids, levels in ((1, tables["person_names"], person_ids, tables["person_levels"]), (2, tables["place_names"], place_ids, tables["place_levels"])):
        for index, name in enumerate(names):
            if name not in ids:
                continue
            for level in levels[index]:
                texture = pause_people.note_texture_name(list_number, ids[name], level)
                title = lines_of(plain, targets, pause_people.label_of(name, "place" if list_number == 2 else "title", level))
                phone = lines_of(plain, targets, pause_people.label_of(name, "title", level)) if list_number == 2 else None
                body = lines_of(plain, targets, pause_people.label_of(name, "body", level))
                text = repr([[line["codes"] for line in part] for part in (title, phone or [], body)]).encode("ascii")
                key = record.key_for(code, {"note": texture, "font": font_fingerprint, "language": language}, text)

                def build(title=title, phone=phone, body=body, texture=texture):
                    pixels = note_pixels(font, textures, language, title, phone, body)
                    return asset_writers.gtex_bytes(pixels.shape[1], pixels.shape[0], asset_writers.FORMAT_DXT5, texture_encoder.encode_rgba(work_folder, texture, pixels.shape[1], pixels.shape[0], pixels.tobytes(), "dxt5"))

                stored(texture, key, build)
                lines.append("note %d %d %d %s" % (list_number, ids[name], level, texture))
    files[pause_people.TABLE_PATH] = ("\n".join(lines) + "\n").encode("ascii")
    return files, paths, sizes
