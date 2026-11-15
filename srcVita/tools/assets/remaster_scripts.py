import collections
import json
import os
import re
import struct

import message_file
import pack_files
import resource_list

ASSET_TOOLS_FOLDER = os.path.dirname(os.path.abspath(__file__))
CLASSES_FILE = os.path.join(ASSET_TOOLS_FOLDER, "remaster_script_classes.json")
SCRIPT_FOLDER = "natives/android/scriptcode/data/script/"
SCRIPT_SUFFIX = ".user.2"
DS_SCRIPT_SUFFIX = ".xml"
CODE_START = 0x34
RSZ_OFFSET_FIELD = 16
RSZ_INSTANCE_COUNT_OFFSET = 12
RSZ_TABLES_OFFSET = 24
INSTANCE_ENTRY_SIZE = 8
STRING_LIMIT = 0x4000
GUID_SIZE = 16
WORD_MASK = 0xffff
SIGN_BIT = 0x8000
EMPTY_TEXT = ""
PLACEHOLDER_TEXT = "*"
MOTION_LABEL_SUFFIX = ".mtbl.xml"
LOCAL_LABEL_FORMAT = "_L$%04x"
TABLE_VARIANT_FORMAT = "%s_%02d"
NUMBERED_TABLE = re.compile(r"^st\d\d_.+_\d{3}$")
ENTRY_PAGE = re.compile(r"^(.*)_(\d\d)$")
MOTION_TAG_LONG_FRAME = 0xfffb
MOTION_TAG_CORE_VALUE = 0xfff6
CORE_VALUE_FLAG_SHIFT = 12
CORE_VALUE_MASK = 0xfff
MOVIE_START_CLASS = "SC_MOVIE_PLAY"
BLOCK_NAME_CLASSES = ("SC_SCRIPT", "SC_OBJECT", "SC_ACTION")
JUMP_CLASS = "SC_JUMP"
MOTION_START_CLASS = "SC_MTBL"
MESSAGE_CLASS = "SC_MESSAGE"
ANIM_CLASS = "SC_MTBL_ANIM"
CORE_VALUE_CLASS = "SC_MTBL_GMKINFO"
CHARA_LIST_KIND = "chara"
EXPANDED_SUFFIX = "_expand"
NO_ART_RESOURCE = 0
NO_ART_TABLE = "no_art"
TEXT_FOLDER = "natives/android/gamedesign/text/"
MESSAGE_SUFFIX = ".msg.22"
NUMBERED_MESSAGE_FORMAT = "%s/%s_%04d"
MESSAGE_NUMBER_LIMIT = 10000
SCENE_MESSAGE_FORMAT = "%s/%s_%s%03d"
SCENE_MESSAGE_KINDS = ("game", "demo")
SCENE_NUMBER_LIMIT = 1000
STAGE_MESSAGE_NAMES = ("id",)
SHARED_MESSAGE_FILES = ("database/database",)


class RemasterScriptError(ValueError):
    pass


def load_classes(path=CLASSES_FILE):
    with open(path) as handle:
        table = json.load(handle)
    classes = table["classes"]
    by_type = {pack_files.murmur3_hash((table["type_prefix"] + name).encode()): name for name in classes}
    return classes, by_type


def read_u16(data, offset):
    return struct.unpack_from("<H", data, offset)[0]


def read_u32(data, offset):
    return struct.unpack_from("<I", data, offset)[0]


def read_fields(data, position, fields):
    position = position + (position & 1)
    code = read_u16(data, position)
    position = position + 2
    values = []
    for kind in fields:
        if kind == "b":
            values.append(data[position])
            position = position + 1
        elif kind == "h":
            position = position + (position & 1)
            values.append(read_u16(data, position))
            position = position + 2
        elif kind == "w":
            position = (position + 3) & ~3
            values.append(read_u32(data, position))
            position = position + 4
        elif kind == "x":
            position = (position + 3) & ~3
            count = read_u32(data, position)
            if count > STRING_LIMIT or position + 4 + 2 * count > len(data):
                raise RemasterScriptError("a string runs past the end of the script")
            values.append(data[position + 4:position + 4 + 2 * count].decode("utf-16-le", "replace").rstrip("\0"))
            position = position + 4 + 2 * count
        elif kind == "g":
            position = (position + 7) & ~7
            values.append(data[position:position + GUID_SIZE].hex())
            position = position + GUID_SIZE
        else:
            raise RemasterScriptError("unknown field kind " + kind)
    if position > len(data):
        raise RemasterScriptError("a record runs past the end of the script")
    return code, values, position


def instance_types(data):
    rsz = struct.unpack_from("<Q", data, RSZ_OFFSET_FIELD)[0]
    count = read_u32(data, rsz + RSZ_INSTANCE_COUNT_OFFSET)
    table, values = struct.unpack_from("<QQ", data, rsz + RSZ_TABLES_OFFSET)
    types = [read_u32(data, rsz + table + INSTANCE_ENTRY_SIZE * index) for index in range(count)]
    return types, data[rsz + values:]


def read_records(data, classes, by_type):
    types, values = instance_types(data)
    names = []
    for type_hash in types[1:]:
        if type_hash not in by_type:
            break
        names.append(by_type[type_hash])
    if any(type_hash in by_type for type_hash in types[1 + len(names):]):
        raise RemasterScriptError("record %d has a class the class table does not know" % len(names))
    records = []
    position = 0
    for name in names:
        row = classes[name]
        code, fields, position = read_fields(values, position, row["fields"])
        if code != row["code"]:
            raise RemasterScriptError("record %d is %s but holds code %d" % (len(records), name, code))
        records.append((name, fields))
    return records


def script_pack_path(ds_path):
    stem = ds_path[:-len(DS_SCRIPT_SUFFIX)] if ds_path.endswith(DS_SCRIPT_SUFFIX) else ds_path
    return SCRIPT_FOLDER + stem.lower() + SCRIPT_SUFFIX


def list_name_of(ds_path):
    return os.path.basename(ds_path)[:-len(DS_SCRIPT_SUFFIX)].lower()


def label_of_entry(path, name):
    prefix = os.path.basename(path).split(".")[0] + "_"
    if not name.startswith(prefix):
        return None
    label = name[len(prefix):]
    match = ENTRY_PAGE.match(label)
    return match.group(1) if match else label


def message_labels(packs, text_key, paths, log):
    labels = {}
    for path in paths:
        data = packs.read(path)
        try:
            entry_count, _, string_area_start = message_file.read_header(data)
            string_area = message_file.decode_string_area(data[string_area_start:], text_key)
            named = [message_file.read_entry_and_name(data, string_area, string_area_start, index) for index in range(entry_count)]
        except message_file.MessageFileError as error:
            log("message_file_not_read", path + ": " + str(error))
            continue
        for entry, name in named:
            label = label_of_entry(path, name)
            if label is not None:
                labels.setdefault(data[entry:entry + GUID_SIZE].hex(), label)
    return labels


def message_paths(packs, script_paths):
    stages = sorted({os.path.dirname(path) for path in script_paths if os.path.dirname(path)})
    names = list(SHARED_MESSAGE_FILES)
    for stage in stages:
        names.extend(NUMBERED_MESSAGE_FORMAT % (stage, stage, number) for number in range(MESSAGE_NUMBER_LIMIT))
        names.extend("%s/%s_%s" % (stage, stage, name) for name in STAGE_MESSAGE_NAMES)
        names.extend(SCENE_MESSAGE_FORMAT % (stage, stage, kind, number) for kind in SCENE_MESSAGE_KINDS for number in range(SCENE_NUMBER_LIMIT))
    names.extend(os.path.dirname(path) + "/" + script_label(path) for path in script_paths if os.path.dirname(path))
    paths = [TEXT_FOLDER + name + MESSAGE_SUFFIX for name in dict.fromkeys(names)]
    return [path for path in paths if pack_files.path_hash_key(path) in packs.entries]


def chara_names(packs, list_name):
    try:
        rows = resource_list.read_list(packs, CHARA_LIST_KIND, list_name)
    except KeyError:
        return {}
    return {resource_id: os.path.basename(path)[:-len(".user")].lower() for resource_id, path in rows}


def chara_list_names(script_paths):
    return list(dict.fromkeys(list_name_of(path) for path in script_paths))


def shared_table_names(packs, list_names):
    shared = {}
    for list_name in list_names:
        for resource_id, name in chara_names(packs, list_name).items():
            shared.setdefault(resource_id, name)
    return shared


def table_names(packs, ds_path, shared):
    return collections.ChainMap(chara_names(packs, list_name_of(ds_path)), shared)


class StringTable:
    def __init__(self):
        self.offsets = {}
        self.data = bytearray()
        self.offset_of(PLACEHOLDER_TEXT)

    def offset_of(self, text):
        if text == EMPTY_TEXT:
            text = PLACEHOLDER_TEXT
        if text not in self.offsets:
            self.offsets[text] = len(self.data)
            self.data += text.encode("latin-1", "replace") + b"\0"
        return self.offsets[text]


class Compiler:
    def __init__(self, records, classes, symbols, messages, tables, log):
        self.records = records
        self.classes = classes
        self.symbols = symbols
        self.messages = messages
        self.tables = tables
        self.log = log
        self.words = []
        self.fixups = []
        self.starts = {}
        self.labels = []
        self.strings = StringTable()
        self.previous_motion = {}
        self.table_name_counts = collections.Counter()
        self.movie_frame = 0

    def message_text(self, index):
        record = self.records[index] if index < len(self.records) else None
        if record is None or record[0] != MESSAGE_CLASS:
            self.log("message_reference_not_a_message", str(index))
            return PLACEHOLDER_TEXT
        label = self.messages.get(record[1][2])
        if label is None:
            self.log("message_guid_has_no_entry", record[1][2])
            return PLACEHOLDER_TEXT
        return label

    def word(self, rule, values):
        if rule.startswith("c"):
            return int(rule[1:])
        if rule.startswith("sx"):
            return WORD_MASK if values[int(rule[2:])] & SIGN_BIT else 0
        if rule.startswith("lo"):
            return values[int(rule[2:])] & WORD_MASK
        if rule.startswith("hi"):
            return (values[int(rule[2:])] >> 16) & WORD_MASK
        if rule.startswith("f"):
            return values[int(rule[1:])] & WORD_MASK
        if rule.startswith("s"):
            choices = [values[int(index)] for index in rule[1:].split("|")]
            return self.strings.offset_of(next((text for text in choices if text != EMPTY_TEXT), EMPTY_TEXT))
        if rule.startswith("y"):
            return (self.symbols.get(values[int(rule[1:])]) or 0) & WORD_MASK
        if rule.startswith("m"):
            return self.strings.offset_of(self.message_text(values[int(rule[1:])]))
        if rule.startswith("g"):
            flag_word = values[0]
            flag = flag_word >> 16 if flag_word & WORD_MASK == MOTION_TAG_CORE_VALUE else flag_word & WORD_MASK
            return (flag << CORE_VALUE_FLAG_SHIFT | values[int(rule[1:])] & CORE_VALUE_MASK) & WORD_MASK
        if rule.startswith("d"):
            frame = values[int(rule[1:])]
            delay = max(frame - self.movie_frame, 0)
            self.movie_frame = frame
            return delay & WORD_MASK
        if rule.startswith("o") or rule.startswith("O"):
            self.fixups.append((len(self.words) + 1 + len(self.pending), rule[0], values[int(rule[1:])]))
            return 0
        raise RemasterScriptError("unknown word rule " + rule)

    def emit_op(self, name, row, values):
        self.pending = []
        for rule in row["words"]:
            self.pending.append(self.word(rule, values))
        if name in BLOCK_NAME_CLASSES:
            self.labels.append((values[0] if isinstance(values[0], str) else self.script_name, len(self.words)))
        self.words += [row["op"]] + self.pending

    def table_label(self, resource_id):
        name = self.tables.get(resource_id)
        if name is None and resource_id == NO_ART_RESOURCE:
            name = NO_ART_TABLE
        elif name is None:
            self.log("motion_table_has_no_chara_row", str(resource_id))
            name = "table_%d" % resource_id
        count = self.table_name_counts[name]
        self.table_name_counts[name] = count + 1
        if count and NUMBERED_TABLE.match(name):
            name = TABLE_VARIANT_FORMAT % (name, count)
        return name + MOTION_LABEL_SUFFIX

    def emit_motion(self, name, row, values):
        if name == MOTION_START_CLASS:
            self.previous_motion = {}
            self.labels.append((self.table_label(values[2]), len(self.words)))
        if name == ANIM_CLASS and values[0] == MOTION_TAG_LONG_FRAME:
            self.words += [MOTION_TAG_LONG_FRAME, values[2] & WORD_MASK, values[1] & WORD_MASK]
            return
        self.pending = []
        words = [self.word(rule, values) for rule in row["motion"]]
        if "repeat_key" in row:
            key = (name,) + tuple(values[index] for index in row["repeat_key"])
            if self.previous_motion.get(key) == words:
                return
            self.previous_motion[key] = words
        self.words += words

    def compile(self, script_name):
        self.script_name = script_name
        index = 0
        while index < len(self.records):
            name, values = self.records[index]
            self.starts[index] = len(self.words)
            row = self.classes[name]
            if name == MOVIE_START_CLASS:
                self.movie_frame = 0
            if row.get("skip"):
                index = index + 1
                continue
            if "motion" in row:
                self.emit_motion(name, row, values)
                index = index + 1
                continue
            if row.get("takes_next_jump"):
                if index + 1 >= len(self.records) or self.records[index + 1][0] != JUMP_CLASS:
                    raise RemasterScriptError("record %d is %s with no jump after it" % (index, name))
                self.starts[index + 1] = len(self.words)
                self.emit_op(name, row, values + self.records[index + 1][1])
                index = index + 2
                continue
            self.emit_op(name, row, values)
            index = index + 1
        return self.file_bytes()

    def file_bytes(self):
        end = len(self.words)
        targets = set()
        for position, half, record_index in self.fixups:
            if record_index in self.starts:
                word_index = self.starts[record_index]
            else:
                word_index = next((self.starts[later] for later in sorted(self.starts) if later >= record_index), end)
            offset = CODE_START + 2 * word_index
            self.words[position] = offset & WORD_MASK if half == "o" else offset >> 16
            targets.add(word_index)
        named = {word_index for _, word_index in self.labels}
        for number, word_index in enumerate(sorted(targets - named)):
            self.labels.append((LOCAL_LABEL_FORMAT % number, word_index))
        self.labels.sort(key=lambda label: label[1])
        code = struct.pack("<%dH" % len(self.words), *self.words)
        strings = bytes(self.strings.data) + b"\0" * (len(self.strings.data) & 1)
        names = bytearray()
        entries = []
        for name, word_index in self.labels:
            entries.append(struct.pack("<II", len(names), CODE_START + 2 * word_index))
            names += name.encode("latin-1", "replace") + b"\0"
        labels = struct.pack("<I", len(entries)) + b"".join(entries) + bytes(names)
        labels = labels + b"\0" * (-len(labels) % 4)
        header = b"1LMG" + struct.pack("<IIII", 0, len(code), len(strings), len(labels))
        header = header + b"\0" * (CODE_START - len(header))
        return header + code + strings + labels


def script_label(ds_path):
    name = list_name_of(ds_path)
    return name[:-len(EXPANDED_SUFFIX)] if name.endswith(EXPANDED_SUFFIX) else name


def compile_script(ds_path, data, classes, by_type, symbols, messages, tables, log):
    records = read_records(data, classes, by_type)
    return Compiler(records, classes, symbols, messages, tables, log).compile(script_label(ds_path))
