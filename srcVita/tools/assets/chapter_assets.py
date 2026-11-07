import collections
import os
import re
import struct
import sys

import ds_rom
import frame_layout
import pack_files
import resource_list

ASSET_TOOLS_FOLDER = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(ASSET_TOOLS_FOLDER))), "tools"))

import lmg_decode
import lz
import message_cues
import message_file
import nds_unpack

ROM_SUFFIX = ".lz"
MOTION_LABEL_SUFFIX = ".mtbl.xml"
TEX_SUFFIX = "_iml4.tex.691230823"
ARM9_OFFSET_FIELD = 0x20
ARM9_SIZE_FIELD = 0x2c
ARM9_LOAD_ADDRESS = 0x02000000
ROOM_TABLE_ADDRESS = 0x02083328
ROOM_ROW_SIZE = 8
ROOM_ROW_END = 0xffff
ROOM_ROW_TAGS_FROM = 0xfffa
ROOM_ID_LIMIT = 256
TILE_PIXELS = 16
SOUND_TABLE_ADDRESS = 0x02080e84
SOUND_ROW_SIZE = 8
SOUND_ROW_COUNT = 518
SOUND_GROUP_NAMES = {0: "sys", 1: "gim"}
SOUND_SOURCE = "natives/android/streaming/sound/se_resource/%s/%s.asrc.31"
MUSIC_SOURCE = "natives/android/streaming/sound/bgm_new/bgm%02d_new.asrc.31"
MUSIC_NAME = "bgm%02d_new"
NATIVE_SOUND_KINDS = (5, 6, 11, 12, 13, 14, 15, 18)
NATIVE_MUSIC_IDS = (9,)
FACE_TABLE_ADDRESS = 0x02085724
FACE_ROW_SIZE = 14
FACE_ROW_COUNT = 320
FIRST_FACE_RESOURCE = 204
NO_FACE_RESOURCE = 0x8000
FACE_LIST_NAME = "cmntexturedatalist"
FACE_FOLDER_MARK = "08_face"
HEAD_FACE_MARK = "cmn_do_"
HEAD_FACE_SIZE = 256
FACE_WIDTH = 360
FACE_HEIGHT = 540
MESSAGE_FOLDER = "natives/android/gamedesign/text/%s/%s_%04d.msg.22"
MESSAGE_FILE_NUMBER_LIMIT = 10000
ENTRY_PAGE_PATTERN = re.compile(r"^(.*)_(\d\d)$")
TABLE_VARIANT_PATTERN = re.compile(r"^(st\d\d_.+_\d{3})_(\d{2,3}[a-z]?)$")
NARRATION_OP = 0x5d
TALK_OPS = (0x37, 0x5c, 0xdb)
BALLOON_LABEL_OPS = (0x51, 0x45, 0x46, 0x4c, 0x4e)
ROOM_OP = 0xc1
MOTION_OP = 0x117
SCRIPT_CHANGE_OP = 0x24
SOUND_OPS = (0x10b, 0x10c, 0x10d)
MUSIC_OPS = (0x106, 0x107, 0x10a)
TAG_BEGIN = -3
TAG_SOUND = -9
PRELOADED_TABLES_PER_SCRIPT = 32


class ChapterBakeError(ValueError):
    pass


class Script:
    def __init__(self, path, plain):
        self.path = path
        self.plain = plain
        _, strings, labels, items, _ = lmg_decode.decode(plain)
        self.strings = strings
        self.labels = labels
        self.items = items
        self.operations = [item for item in items if item[1] == "op"]
        self.tables = [item for item in items if item[1] == "motion"]

    def list_name(self):
        return os.path.basename(self.path)[:-len(".xml")].lower()

    def text_of(self, word):
        return self.strings.get(word)

    def strings_of_ops(self, ops):
        found = []
        for _, _, op, operands in self.operations:
            if op in ops and operands and self.text_of(operands[0]) is not None:
                found.append(self.text_of(operands[0]))
        return list(dict.fromkeys(found))

    def table_name_at(self, offset):
        for name in self.labels.get(offset, []):
            if name.endswith(MOTION_LABEL_SUFFIX):
                return name[:-len(MOTION_LABEL_SUFFIX)]
        return None


def read_script(rom, path):
    plain = lz.try_decompress(rom.read_file(path + ROM_SUFFIX))
    if plain is None:
        raise ChapterBakeError(path + " could not be decompressed")
    return Script(path, plain)


def discover_chain(rom, entry, chapter):
    prefix = chapter + "/"
    scripts = collections.OrderedDict()
    waiting = collections.deque([entry])
    while waiting:
        path = waiting.popleft()
        if path in scripts or rom.file_id_of(path + ROM_SUFFIX) is None:
            continue
        scripts[path] = read_script(rom, path)
        for target in scripts[path].strings_of_ops((SCRIPT_CHANGE_OP,)):
            if target.startswith(prefix):
                waiting.append(target)
    return scripts


def read_arm9(rom):
    header = rom.read_range(0, 0x40)
    offset = struct.unpack_from("<I", header, ARM9_OFFSET_FIELD)[0]
    size = struct.unpack_from("<I", header, ARM9_SIZE_FIELD)[0]
    return nds_unpack.blz_decompress(rom.read_range(offset, size))


def signed(value):
    return value - 0x10000 if value >= 0x8000 else value


def read_room(arm9, room):
    width, height, rows_address = struct.unpack_from("<HHI", arm9, ROOM_TABLE_ADDRESS - ARM9_LOAD_ADDRESS + ROOM_ROW_SIZE * room)
    plates = []
    position = rows_address - ARM9_LOAD_ADDRESS
    while 0 <= position < len(arm9) - 8:
        plate_id, x, y, _ = struct.unpack_from("<4H", arm9, position)
        position = position + 8
        if plate_id == ROOM_ROW_END:
            break
        if plate_id >= ROOM_ROW_TAGS_FROM:
            continue
        plates.append((plate_id & 0x7fff, signed(x) * TILE_PIXELS, signed(y) * TILE_PIXELS))
    return width, height, plates


def rooms_used(scripts):
    rooms = []
    for script in scripts.values():
        for _, _, op, operands in script.operations:
            if op == ROOM_OP and operands[0] < ROOM_ID_LIMIT:
                rooms.append(operands[0])
    return sorted(set(rooms))


def plate_source(logical_path):
    return pack_files_path_stem(logical_path) + TEX_SUFFIX


def pack_files_path_stem(logical_path):
    stem = logical_path[:-len(".user")] if logical_path.endswith(".user") else logical_path
    return resource_list.PACK_PREFIX + stem.lower()


def read_list_or_nothing(packs, kind, name):
    try:
        return resource_list.read_list(packs, kind, name)
    except KeyError:
        return []


def plates_and_rooms(packs, scripts, arm9, soul_ramp):
    plate_paths = {}
    for script in scripts.values():
        for plate_id, path in read_list_or_nothing(packs, "bg", script.list_name()):
            plate_paths.setdefault(plate_id, path)
    plates = collections.OrderedDict()
    room_plates = {}
    room_sizes = {}
    for room in rooms_used(scripts):
        width, height, rows = read_room(arm9, room)
        room_sizes[str(room)] = [width, height]
        placed = []
        for plate_id, x, y in rows:
            if plate_id not in plate_paths:
                continue
            name = os.path.basename(plate_paths[plate_id])[:-len(".user")].lower()
            if name not in plates:
                plates[name] = {"name": name, "source": plate_source(plate_paths[plate_id])}
                if soul_ramp is not None:
                    plates[name]["soul_plate"] = name + "-soul"
            placed.append([name, x, y])
        if placed:
            room_plates[str(room)] = placed
    return list(plates.values()), room_plates, room_sizes


def strip_table_variant(stem):
    match = TABLE_VARIANT_PATTERN.match(stem)
    return match.group(1) if match else None


def begin_id_of(records):
    for record in records:
        if record[1] == TAG_BEGIN:
            return record[2][0] & 0xffff
    return None


def frames_of(records):
    return [record[2][0] for record in records if record[1] == "frame"]


def folder_of_stem(stem, folders):
    lowered = stem.lower()
    if lowered in folders:
        return lowered
    variant = strip_table_variant(lowered)
    if variant in folders:
        return variant
    if lowered.endswith("_wait"):
        wanted = re.compile(r"^st\d\d_" + re.escape(lowered[:-len("_wait")]) + r"_\d{3}$")
        matches = sorted(name for name in folders if wanted.match(name))
        if matches:
            return matches[0]
    return None


def resolve_tables(packs, scripts):
    motion_tables = {}
    frames_by_folder = collections.defaultdict(set)
    paths_by_folder = {}
    unresolved = []
    for script in scripts.values():
        folders = {}
        for _, path in read_list_or_nothing(packs, "chara", script.list_name()):
            name = os.path.basename(path)[:-len(".user")].lower()
            folders[name] = path
        stems_by_begin = collections.defaultdict(list)
        table_rows = []
        for table in script.tables:
            stem = script.table_name_at(table[0])
            if stem is None:
                continue
            begin = begin_id_of(table[2])
            stems_by_begin[begin].append(stem)
            table_rows.append((stem, begin, frames_of(table[2])))
        for stem, begin, frames in table_rows:
            folder = next((peer.lower() for peer in stems_by_begin[begin] if peer.lower() in folders), None)
            if folder is None:
                folder = folder_of_stem(stem, folders)
            if folder is None:
                for peer in stems_by_begin[begin]:
                    folder = folder_of_stem(peer, folders)
                    if folder is not None:
                        break
            if folder is None:
                unresolved.append(stem)
                continue
            motion_tables[stem] = folder
            paths_by_folder[folder] = folders[folder]
            frames_by_folder[folder].update(frames)
    animations = []
    for folder, path in paths_by_folder.items():
        animations.append({"name": folder, "timeline": resource_list.pack_path_of(path), "ds_frames": frame_layout.WholeFrames(sorted(frames_by_folder[folder])), "table_only": True})
    return animations, motion_tables, sorted(set(unresolved))


def sound_kinds_used(scripts):
    kinds = set(NATIVE_SOUND_KINDS)
    for script in scripts.values():
        for _, _, op, operands in script.operations:
            if op in SOUND_OPS:
                kinds.add(operands[0])
        for table in script.tables:
            for record in table[2]:
                if record[1] == TAG_SOUND:
                    kinds.add(record[2][0] & 0xffff)
    return sorted(kinds)


def music_ids_used(scripts):
    ids = set(NATIVE_MUSIC_IDS)
    for script in scripts.values():
        for _, _, op, operands in script.operations:
            if op in MUSIC_OPS:
                ids.add(operands[0])
    return sorted(ids)


def path_exists(packs, path):
    return pack_files.path_hash_key(path) in packs.entries


def audio_rows(packs, scripts, arm9):
    sounds = {}
    sound_kinds = {}
    for kind in sound_kinds_used(scripts):
        if kind >= SOUND_ROW_COUNT:
            continue
        index, group, _, _ = struct.unpack_from("<hhHH", arm9, SOUND_TABLE_ADDRESS - ARM9_LOAD_ADDRESS + SOUND_ROW_SIZE * kind)
        if group not in SOUND_GROUP_NAMES:
            continue
        name = "%s_%03d" % (SOUND_GROUP_NAMES[group], index)
        source = SOUND_SOURCE % (SOUND_GROUP_NAMES[group], name)
        if path_exists(packs, source):
            sounds[name] = {"name": name, "source": source}
            sound_kinds[str(kind)] = name
    music = {}
    music_ids = {}
    for music_id in music_ids_used(scripts):
        source = MUSIC_SOURCE % music_id
        if path_exists(packs, source):
            name = MUSIC_NAME % music_id
            music[name] = {"name": name, "source": source}
            music_ids[str(music_id)] = name
    return list(music.values()), music_ids, list(sounds.values()), sound_kinds


def message_files_of(packs, chapter):
    paths = []
    for number in range(MESSAGE_FILE_NUMBER_LIMIT):
        path = MESSAGE_FOLDER % (chapter, chapter, number)
        if path_exists(packs, path):
            paths.append(path)
    return paths


def entries_by_label(packs, chapter, text_key):
    pages = collections.defaultdict(dict)
    for path in message_files_of(packs, chapter):
        prefix = os.path.basename(path).split(".")[0] + "_"
        for name in message_file.read_entry_names(packs.read(path), text_key):
            if not name.startswith(prefix):
                continue
            label = name[len(prefix):]
            match = ENTRY_PAGE_PATTERN.match(label)
            if match:
                pages[match.group(1)][int(match.group(2))] = (path, name)
            else:
                pages[label][0] = (path, name)
    return {label: [found[index] for index in sorted(found)] for label, found in pages.items()}


def read_face_rows(arm9):
    rows = {}
    for face_id in range(1, FACE_ROW_COUNT):
        resource, _, flip, _, _, _, _ = struct.unpack_from("<7H", arm9, FACE_TABLE_ADDRESS - ARM9_LOAD_ADDRESS + FACE_ROW_SIZE * face_id)
        rows[face_id] = (resource, flip)
    return rows


def face_paths(packs):
    paths = [path for _, path in resource_list.read_list(packs, "texture", FACE_LIST_NAME) if FACE_FOLDER_MARK in path.lower()]
    return sorted(paths, key=lambda path: path.lower())


def face_texture_of(paths, resource):
    index = resource - FIRST_FACE_RESOURCE
    if resource == NO_FACE_RESOURCE or index < 0 or index >= len(paths):
        return None
    return paths[index]


def language_path_of(path, language):
    return path[:-len(".xml")] + ".%s.xml.lz" % language


def expand_set(set_description, packs, rom_path, text_key):
    if "chapter" not in set_description:
        return set_description
    expanded = dict(set_description)
    section = dict(set_description["scripts"])
    chapter = set_description["chapter"]
    soul_ramp = set_description.get("soul_view", {}).get("ramp")
    with ds_rom.DsRom(rom_path) as rom:
        scripts = discover_chain(rom, section["entry"], chapter)
        arm9 = read_arm9(rom)
        languages = {}
        for path in scripts:
            language_path = language_path_of(path, section.get("language", "en"))
            if rom.file_id_of(language_path) is not None:
                languages[path] = lz.try_decompress(rom.read_file(language_path))
    section["files"] = list(scripts)
    plates, room_plates, room_sizes = plates_and_rooms(packs, scripts, arm9, soul_ramp)
    expanded["plates"] = plates
    section["room_plates"] = room_plates
    section["room_sizes"] = room_sizes
    animations, motion_tables, unresolved = resolve_tables(packs, scripts)
    expanded["animations"] = animations
    section["motion_tables"] = motion_tables
    if unresolved:
        print("tables with no art: %d (%s)" % (len(unresolved), ", ".join(unresolved[:12])))
    expanded["music"], section["music_ids"], expanded["sounds"], section["sound_kinds"] = audio_rows(packs, scripts, arm9)
    entries = entries_by_label(packs, chapter, text_key)
    narration_rows = []
    talk_labels = collections.OrderedDict()
    bubble_rows = []
    balloon_rows = list(set_description.get("balloon_kinds", {}).get("no_label", []))
    balloon_style = set_description.get("balloon_kinds", {}).get("with_label")
    for path, script in scripts.items():
        for label in script.strings_of_ops((NARRATION_OP,)):
            if label in entries:
                narration_rows.append({"name": label, "file": entries[label][0][0], "entry": entries[label][0][1]})
        for label in script.strings_of_ops(TALK_OPS):
            if label in entries:
                talk_labels[label] = path
        if balloon_style is not None:
            for label in script.strings_of_ops(BALLOON_LABEL_OPS):
                if label in entries and all(row["name"] != "bubble_" + label for row in bubble_rows):
                    bubble_rows.append(dict(balloon_style["text"], joins_lines=True, name="bubble_" + label, file=entries[label][0][0], entry=entries[label][0][1]))
                    balloon_rows.append({"label": label, "delay": balloon_style["delay"], "parts": [dict(balloon_style["art"], fits="bubble_" + label), dict(balloon_style["text_part"], texture="bubble_" + label)]})
    core_labels = set_description.get("core_labels")
    if core_labels is not None:
        for object_name, (name_id, action_id) in core_labels["objects"].items():
            style = {"font": core_labels["font"], "pixels_per_em": core_labels["pixels_per_em"], "color": core_labels["color"], "joins_lines": True, "single_line": True, "file": core_labels["file"]}
            bubble_rows.append(dict(style, name="core_name_" + object_name, entry=core_labels["name_entry"] + name_id))
            if action_id is not None:
                bubble_rows.append(dict(style, name="core_action_" + object_name, entry=core_labels["action_entry"] + action_id + "_0"))
    if core_labels is not None and "unknown_text" in core_labels:
        bubble_rows.append({"name": "core_unknown", "literal": core_labels["unknown_text"], "font": core_labels["font"], "pixels_per_em": core_labels["pixels_per_em"], "color": core_labels["color"]})
    expanded["narration"] = narration_rows
    expanded["bubble_text"] = bubble_rows
    expanded["script_balloons"] = balloon_rows
    paths = face_paths(packs)
    face_rows = read_face_rows(arm9)
    talk = dict(set_description.get("talk", {}))
    talk["entries"] = {label: entries[label] for label in talk_labels}
    talk["language_files"] = {label: languages.get(path) for label, path in talk_labels.items()}
    talk["face_rows"] = face_rows
    talk["face_paths"] = paths
    expanded["talk"] = talk
    expanded["scripts"] = section
    expanded["script_table_names"] = {path: [script.table_name_at(operands[0] | (operands[1] << 16)) for _, _, op, operands in script.operations if op == MOTION_OP] for path, script in scripts.items()}
    return expanded


def add_face_textures(expanded, face_logical_paths):
    textures = list(expanded.get("screen_textures", []))
    known = set(texture["name"] for texture in textures)
    for path in face_logical_paths:
        name = face_texture_name(path)
        if name not in known:
            known.add(name)
            is_head = HEAD_FACE_MARK in path.lower()
            textures.append({"name": name, "source": pack_files_path_stem(path) + TEX_SUFFIX, "width": HEAD_FACE_SIZE if is_head else FACE_WIDTH, "height": HEAD_FACE_SIZE if is_head else FACE_HEIGHT})
    expanded["screen_textures"] = textures


def face_texture_name(logical_path):
    return "face_" + os.path.basename(logical_path)[:-len(".user")].lower()
