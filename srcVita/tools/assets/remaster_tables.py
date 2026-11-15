import argparse
import hashlib
import json
import os
import re
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import numpy

import arm64_runner

ASSET_TOOLS_FOLDER = os.path.dirname(os.path.abspath(__file__))
REPOSITORY_FOLDER = os.path.dirname(os.path.dirname(os.path.dirname(ASSET_TOOLS_FOLDER)))
DEFAULT_APK_FOLDER = os.path.join(REPOSITORY_FOLDER, "extracted", "remaster_apk")
TABLES_FILE_NAME = "remaster_tables.json"
TABLES_FORMAT = 1
IL2CPP = "_ZN6il2cpp"
GHOST = "app_assets_Script_Classes_GhostSrc_"
GHOST_EX = "app_assets_Script_Classes_GhostSrcEx_"
ARRAY_COUNT_OFFSET = 0x1c
ARRAY_DATA_OFFSET = 0x20
ARRAY_SLACK = 0x40
OBJECT_SIZE = 0x400
FIELDS_OFFSET = 0x10
POINTER = 8
ROOM_ENTRY_SIZE = 16
ROOM_ROWS_OFFSET = 8
ROOM_ROW_WORDS = 4
FACE_ROW_WORDS = 7
SOUND_KIND_COUNT = 518
SOUND_GROUPS = {"SYS": 0, "GIM": 1}
SOUND_NAME = re.compile(r"^SE_(SYS|GIM)_(\d+)[A-Z]?$")
CORE_ACTION_OFFSET = 0x14
CLASS_KIND_BASE = 0x8000
FACTORY_SLOTS = 60
INTERN_RUN_SLACK = 64
INTERN_SIGNATURE = 12
INTERN_RUN_GAPS = 16
INTERN_NAME_LIMIT = 64
SIGNATURE_MULTIPLIER = 1000003
INTERN_SAMPLE = 16
PRINTABLE_STRING = re.compile(rb"[\x20-\x7e]+\0")
CHAPTER_CARD_EFFECT = "iEffect::CHAPTER_START_01"
STATIC_FIELDS = "13StaticField__E"
CONSTRUCTOR = "11_cctor"
CONTEXT_SIZE = 0x100


class TablesError(ValueError):
    pass


def mangled_prefix(class_path):
    return IL2CPP + str(len(class_path)) + class_path


def symbol_starting(library, prefix, containing=""):
    found = [name for name in library.address_of if name.startswith(prefix) and containing in name]
    if len(found) != 1:
        raise TablesError("expected one symbol starting %s, found %d" % (prefix, len(found)))
    return found[0]


def element_type_of(name):
    match = re.search(r"getElementTblI(.*)EEPT_v$", name)
    return match.group(1) if match else None


class Runtime:
    def __init__(self, library):
        self.library = library
        self.arrays = {}
        self.strings = {}
        self.constructed = {}
        self.dictionaries = {}
        self.created_arrays = []
        stubs = self.runtime_stubs()
        for name in set(library.slot_names.values()):
            element = element_type_of(name)
            if element is not None:
                stubs[name] = self.element_table(element)
            elif "memory4copyI" in name:
                stubs[name] = self.copy
            elif name.startswith("_ZN3via3clr13ManagedObject") and ("addRef" in name or "release" in name):
                stubs[name] = lambda machine: 0
        for name in library.address_of:
            if not name.startswith(IL2CPP) or name in stubs:
                continue
            if "cRUID" in name or "CREATE_RUID" in name:
                stubs[name] = self.new_object
            elif name.startswith(IL2CPP + "16System_UInt") and "I_I5Set_1" in name:
                stubs[name] = lambda machine: 0
            elif "Dictionary_2_" in name and "_7Add" in name:
                stubs[name] = self.dictionary_add
            elif GHOST not in name and GHOST_EX not in name:
                stubs[name] = self.new_object
        stubs[symbol_starting(library, mangled_prefix(GHOST + "Define") + "12COREDEF")] = self.core_definition
        stubs[symbol_starting(library, mangled_prefix(GHOST + "Define_SE_INFO_TBL") + "10_ctor", "System_String")] = self.sound_row
        self.machine = arm64_runner.Machine(library, stubs, lambda name: name.startswith(IL2CPP))
        self.context = self.machine.allocate(CONTEXT_SIZE)

    def runtime_stubs(self):
        def thrown(machine):
            raise arm64_runner.RunError("the game code threw exception %d" % machine.x[1])
        return {
            "_ZN3via3clr13ManagedObject7asArrayEv": lambda machine: machine.x[0],
            "_ZN3via3clr9VMContext20throwSystemExceptionENS0_10SystemTypeEj": thrown,
            "__stack_chk_fail": thrown,
            "_ZNK3via3clr9VMContext12hasExceptionEv": lambda machine: 0,
            "_ZN3via3clr17RuntimeTypeHandlecvRKNS0_10VMTypeInfoEEv": lambda machine: machine.x[0],
            "_ZN3via14atomic_replaceINS_3clr13ManagedObjectES2_EEvRPT_PT0_": self.replace_pointer,
            "_ZNK3via3clr13ManagedObject7isLocalEv": lambda machine: 1,
            "_ZN3via3clr9VMContext9newObjectERKNS0_10VMTypeInfoEb": self.new_object,
            "_ZN3via3clr9VMContext12createObjectERKNS0_10VMTypeInfoEb": self.new_object,
            "_ZN3via3clr9VMContext8newArrayERKNS0_10VMTypeInfoEmjb": self.new_array,
            "_ZN3via3clr9VMContext11createArrayERKNS0_10VMTypeInfoEmjb": self.new_array,
            "_ZNK3via3clr7VMArray15getDimensionTblEv": self.dimensions,
            "_ZN3via3clr6TypeDB15getInternStringEj": self.intern_string,
            IL2CPP + "7via_str12makeHash2884ERN3via3clr9VMContextEPNS_13System_StringE": lambda machine: self.strings.get(machine.x[1], 0),
            "_ZN3via3str8makeHashEPKw": lambda machine: 0,
            arm64_runner.UNBOUND_SLOT: self.copy,
        }

    def new_object(self, machine):
        return machine.allocate(OBJECT_SIZE)

    def new_array(self, machine):
        count = machine.x[2]
        address = machine.allocate(ARRAY_DATA_OFFSET + count * ARRAY_SLACK + ARRAY_SLACK)
        machine.memory.put(address + ARRAY_COUNT_OFFSET, 4, count)
        self.arrays[address] = {"count": count, "type": None}
        self.created_arrays.append(address)
        return address

    def element_table(self, element):
        def table(machine):
            array = machine.x[0]
            if array in self.arrays and self.arrays[array]["type"] is None:
                self.arrays[array]["type"] = element
            return array + ARRAY_DATA_OFFSET
        return table

    def dimensions(self, machine):
        info = self.arrays.setdefault(machine.x[0], {"count": 0, "type": None})
        if "dimensions" not in info:
            info["dimensions"] = machine.allocate(0x20)
        return info["dimensions"]

    def intern_string(self, machine):
        address = machine.allocate(0x40)
        self.strings[address] = machine.x[0] & arm64_runner.MASK32
        return address

    def copy(self, machine):
        machine.memory.write(machine.x[0], machine.memory.read(machine.x[1], machine.x[2]))
        return machine.x[0]

    def replace_pointer(self, machine):
        machine.memory.put(machine.x[0], POINTER, machine.x[1])
        return 0

    def dictionary_add(self, machine):
        self.dictionaries.setdefault(machine.x[1], []).append((machine.x[2] & 0xffff, machine.x[3]))
        return 0

    def core_definition(self, machine):
        address = self.new_object(machine)
        for index in range(3):
            machine.memory.put(address + FIELDS_OFFSET + 2 * index, 2, machine.x[2 + index])
        machine.memory.put(address + FIELDS_OFFSET + 6, 2, 0)
        self.constructed[address] = "core"
        return address

    def sound_row(self, machine):
        target = machine.x[1]
        self.constructed[target] = ("sound", machine.x[3] & 0xffff, self.strings.get(machine.x[2]))
        return 0

    def run_constructor(self, class_path):
        constructor = symbol_starting(self.library, mangled_prefix(class_path) + CONSTRUCTOR)
        self.machine.call(self.library.address_of[constructor], [self.context])
        statics = mangled_prefix(class_path) + STATIC_FIELDS
        return self.library.address_of[statics], self.library.size_of[statics]

    def u(self, address, size):
        return self.machine.memory.u(address, size)

    def static_arrays(self, statics):
        base, size = statics
        found = []
        for offset in range(0, size, POINTER):
            pointer = self.u(base + offset, POINTER)
            if pointer in self.arrays:
                found.append((offset, pointer))
        return found

    def count_of(self, array):
        return self.u(array + ARRAY_COUNT_OFFSET, 4)

    def only_array(self, statics, accept, what):
        found = [array for _, array in self.static_arrays(statics) if accept(array)]
        if len(found) != 1:
            raise TablesError("expected one %s table, found %d" % (what, len(found)))
        return found[0]

    def pointers_of(self, array):
        return [self.u(array + ARRAY_DATA_OFFSET + POINTER * index, POINTER) for index in range(self.count_of(array))]

    def typed(self, ending):
        return lambda array: (self.arrays[array]["type"] or "").endswith(ending)


def read_rooms(runtime, define):
    array = runtime.only_array(define, runtime.typed("Define_BGInfoTbl_T_ValueTypeE"), "room")
    rooms = {}
    for room in range(runtime.count_of(array)):
        entry = array + ARRAY_DATA_OFFSET + ROOM_ENTRY_SIZE * room
        rows_array = runtime.u(entry + ROOM_ROWS_OFFSET, POINTER)
        rows = []
        if rows_array:
            for row in range(runtime.count_of(rows_array)):
                rows.append([runtime.u(rows_array + ARRAY_DATA_OFFSET + 2 * (ROOM_ROW_WORDS * row + word), 2) for word in range(ROOM_ROW_WORDS)])
        rooms[str(room)] = {"size": [runtime.u(entry, 2), runtime.u(entry + 2, 2)], "rows": rows}
    return rooms


def read_sound_name_ids(runtime, define):
    def holds_sound_rows(array):
        pointers = runtime.pointers_of(array)
        return bool(pointers) and all(isinstance(runtime.constructed.get(pointer), tuple) for pointer in pointers)
    array = runtime.only_array(define, holds_sound_rows, "sound")
    rows = [runtime.constructed[pointer] for pointer in runtime.pointers_of(array)]
    if len(rows) != SOUND_KIND_COUNT:
        raise TablesError("expected %d sound rows, found %d" % (SOUND_KIND_COUNT, len(rows)))
    return [name_id for _, _, name_id in rows]


def read_core_infos(runtime, statics):
    def holds_core_rows(array):
        pointers = runtime.pointers_of(array)
        return bool(pointers) and all(runtime.constructed.get(pointer) == "core" for pointer in pointers if pointer)
    array = runtime.only_array(statics, holds_core_rows, "core info")
    return [[runtime.u(pointer + FIELDS_OFFSET + 2 * word, 2) for word in range(4)] if pointer else None for pointer in runtime.pointers_of(array)]


def read_face_rows(runtime, statics):
    array = runtime.only_array(statics, runtime.typed("cKaiwaCtrl_CutinInfo_T_ValueTypeE"), "face")
    return [[runtime.u(array + ARRAY_DATA_OFFSET + 2 * (FACE_ROW_WORDS * row + word), 2) for word in range(FACE_ROW_WORDS)] for row in range(runtime.count_of(array))]


def read_symbol_items(runtime, define):
    def holds_items(array):
        pointers = runtime.pointers_of(array)
        return len(pointers) > 1000 and runtime.arrays[array]["type"] and "ManagedObject" in runtime.arrays[array]["type"]
    array = runtime.only_array(define, holds_items, "symbol")
    return [(runtime.u(pointer + FIELDS_OFFSET, 4), runtime.u(pointer + FIELDS_OFFSET + 4, 4)) for pointer in runtime.pointers_of(array)]


def decode_words(library, start, count):
    words = []
    for index in range(count):
        word = library.instruction(start + 4 * index)
        if word is None:
            break
        words.append(word)
    return words


def static_load_offsets(library, function, statics_address):
    pages = {}
    holders = set()
    offsets = []
    address = library.address_of[function]
    for index, word in enumerate(decode_words(library, address, library.size_of[function] // 4)):
        pc = address + 4 * index
        if (word & 0x9f000000) == 0x90000000:
            pages[word & 0x1f] = arm64_runner.adrp_target(pc, word)
            holders.discard(word & 0x1f)
            continue
        if (word & 0xffc00000) == 0xf9400000:
            rn = (word >> 5) & 0x1f
            rt = word & 0x1f
            offset = ((word >> 10) & 0xfff) * 8
            if rn in holders:
                offsets.append(offset)
            elif rn in pages and library.relocated.get(pages[rn] + offset) == statics_address:
                pages.pop(rt, None)
                holders.add(rt)
                continue
            pages.pop(rt, None)
            holders.discard(rt)
    return offsets


def read_effect_kinds(runtime, define, create_effect):
    loads = static_load_offsets(runtime.library, create_effect, define[0])
    if len(loads) < 2:
        raise TablesError("could not find the effect kind table that createGameEffect reads")
    kinds_array = runtime.u(define[0] + loads[0], POINTER)
    if kinds_array not in runtime.arrays:
        raise TablesError("createGameEffect's first table is not an array")
    kinds = [runtime.u(kinds_array + ARRAY_DATA_OFFSET + 2 * index, 2) for index in range(runtime.count_of(kinds_array))]
    classes = runtime.u(define[0] + loads[1], POINTER)
    return kinds, sorted(set(key for key, _ in runtime.dictionaries.get(classes, [])))


def read_class_factories(runtime, create_effect, effect_id):
    library = runtime.library
    memory = runtime.machine.memory
    bss_start, _, bss_size = library.sections[".bss"]
    for slot, target in library.relocated.items():
        if bss_start <= target < bss_start + bss_size and memory.u(target, POINTER) == 0:
            memory.put(target, POINTER, target)
    before = len(runtime.created_arrays)

    def stop(machine, target):
        raise arm64_runner.StopRun()
    runtime.machine.on_indirect_call = stop
    runtime.machine.x[31] = 0
    runtime.machine.call(library.address_of[create_effect], [runtime.context, effect_id])
    runtime.machine.on_indirect_call = None
    tables = [array for array in runtime.created_arrays[before:] if runtime.count_of(array) == FACTORY_SLOTS]
    if len(tables) != 1:
        raise TablesError("createGameEffect did not build its %d factory slots" % FACTORY_SLOTS)
    return [format(pointer, "x") for pointer in runtime.pointers_of(tables[0])]


def pool_signatures(data, start, end):
    signatures = {}
    lengths = []
    starts = []

    def add_run():
        for at in range(len(lengths) - INTERN_SIGNATURE + 1):
            signatures.setdefault(tuple(lengths[at:at + INTERN_SIGNATURE]), []).append(starts[at])
    for found in PRINTABLE_STRING.finditer(data, start, end):
        if starts and found.start() != starts[-1] + lengths[-1] + 1:
            add_run()
            lengths, starts = [], []
        starts.append(found.start())
        lengths.append(found.end() - found.start() - 1)
    add_run()
    return signatures


def signature_hashes(rows):
    hashes = numpy.zeros(len(rows[0]), dtype=numpy.uint64)
    for row in rows:
        hashes = hashes * numpy.uint64(SIGNATURE_MULTIPLIER) + row.astype(numpy.uint64)
    return hashes


def c_string(data, start):
    end = data.find(b"\0", start, start + INTERN_NAME_LIMIT)
    return data[start:end].decode("latin-1") if end >= 0 else ""


def locate_intern_strings(library, consecutive_ids, other_ids):
    data = library.data
    _, offset, size = library.section(".data")
    words = numpy.frombuffer(data, dtype="<u4", count=size // 4, offset=offset).astype(numpy.int64)
    deltas = words[1:] - words[:-1]
    small = ((deltas > 0) & (deltas <= INTERN_RUN_SLACK)).astype(numpy.int64)
    running = numpy.concatenate([[0], numpy.cumsum(small)])
    needed = len(consecutive_ids) - 1
    starts = numpy.nonzero(running[needed:] - running[:-needed] >= needed - INTERN_RUN_GAPS)[0]
    signatures = pool_signatures(data, offset, offset + size)
    known = numpy.array(sorted(set(int(value) for value in signature_hashes([numpy.array([signature[step] for signature in signatures], dtype=numpy.int64) for step in range(INTERN_SIGNATURE)]))), dtype=numpy.uint64) if signatures else numpy.zeros(0, dtype=numpy.uint64)
    window_hashes = signature_hashes([deltas[starts + step] - 1 for step in range(INTERN_SIGNATURE)])
    candidates = starts[numpy.isin(window_hashes, known)]
    found = set()
    others = numpy.array(sorted(set(other_ids)), dtype=numpy.int64)
    bytes_view = numpy.frombuffer(data, dtype=numpy.uint8)
    for start in candidates.tolist():
        signature = tuple(int(deltas[start + step]) - 1 for step in range(INTERN_SIGNATURE))
        table = start - consecutive_ids[0]
        if table < 0 or table + int(others[-1]) >= len(words):
            continue
        sample = [int(words[table + name_id]) for name_id in others[::max(1, len(others) // INTERN_SAMPLE)].tolist()]
        for string_start in signatures.get(signature, ()):
            base = string_start - int(words[start])
            if not all(0 < base + value < len(data) and data[base + value - 1] == 0 for value in sample):
                continue
            if not all(SOUND_NAME.match(c_string(data, base + int(words[table + name_id]))) for name_id in consecutive_ids[1:]):
                continue
            before = base + words[table + others] - 1
            if before.min() >= 0 and before.max() < len(data) and not bytes_view[before].any():
                found.add((table, base))
    if len(found) != 1:
        raise TablesError("expected the intern string table in one place, found %d" % len(found))
    table, base = found.pop()

    def text_of(name_id):
        string_start = base + int(words[table + name_id])
        return data[string_start:data.index(b"\0", string_start)].decode("latin-1")
    return text_of


def sound_rows_from_names(names):
    rows = []
    for name in names:
        match = SOUND_NAME.match(name)
        rows.append([int(match.group(2)), SOUND_GROUPS[match.group(1)]] if match else None)
    return rows


def no_action_label(library):
    function = symbol_starting(library, mangled_prefix(GHOST + "uSubStatusNormal") + "15createItem")
    address = library.address_of[function]
    words = decode_words(library, address, library.size_of[function] // 4)
    action_registers = set()
    for index, word in enumerate(words):
        if (word & 0xffc00000) == 0x79400000 and ((word >> 10) & 0xfff) * 2 == CORE_ACTION_OFFSET:
            action_registers.add(word & 0x1f)
        if (word & 0xff800000) == 0x52800000 and index + 1 < len(words):
            following = words[index + 1]
            if (following & 0x7fe00c00) == 0x1a800000 and (following & 0x1f) == (word & 0x1f) and ((following >> 5) & 0x1f) in action_registers and ((following >> 16) & 0x1f) == (word & 0x1f):
                return (word >> 5) & 0xffff
    raise TablesError("could not find the label createItem shows for a core with no action")


def read_tables(library_bytes):
    library = arm64_runner.Library(library_bytes)
    runtime = Runtime(library)
    define = runtime.run_constructor(GHOST + "Define")
    create_effect = symbol_starting(library, mangled_prefix(GHOST + "uGameEffect") + "21createGameEffect")
    kinds, remaster_classes = read_effect_kinds(runtime, define, create_effect)
    sound_ids = read_sound_name_ids(runtime, define)
    items = read_symbol_items(runtime, define)
    text_of = locate_intern_strings(library, sound_ids, [name_id for name_id, _ in items])
    symbols = {text_of(name_id): value for name_id, value in items}
    rooms = read_rooms(runtime, define)
    cores = read_core_infos(runtime, runtime.run_constructor(GHOST + "uGmkCommon"))
    faces = read_face_rows(runtime, runtime.run_constructor(GHOST + "CutinTbl"))
    factories = read_class_factories(runtime, create_effect, symbols[CHAPTER_CARD_EFFECT])
    return {
        "format": TABLES_FORMAT,
        "library_sha256": hashlib.sha256(library_bytes).hexdigest(),
        "rooms": rooms,
        "sound_rows": sound_rows_from_names([text_of(name_id) for name_id in sound_ids]),
        "core_infos": cores,
        "no_action_label": no_action_label(library),
        "face_rows": faces,
        "effect_kinds": kinds,
        "remaster_class_kinds": remaster_classes,
        "class_factories": factories,
        "symbols": symbols,
    }


def tables_path(apk_folder):
    return os.path.join(apk_folder, TABLES_FILE_NAME)


def save_tables(apk_folder, tables):
    path = tables_path(apk_folder)
    temporary = path + ".partial"
    with open(temporary, "w") as handle:
        json.dump(tables, handle, sort_keys=True, separators=(",", ":"))
    os.replace(temporary, path)
    return path


def load_tables(apk_folder):
    path = tables_path(apk_folder)
    try:
        with open(path) as handle:
            tables = json.load(handle)
    except FileNotFoundError:
        raise TablesError("no " + TABLES_FILE_NAME + " in " + apk_folder + ": run remaster_tables.py on your Android package first") from None
    except (OSError, ValueError) as error:
        raise TablesError("could not read " + path + ": " + str(error)) from None
    if tables.get("format") != TABLES_FORMAT:
        raise TablesError(path + " was written by another version of remaster_tables.py: run it again")
    return tables


def main():
    import extract_text_key
    parser = argparse.ArgumentParser(description="Reads the game tables the remaster keeps in its code (rooms, sounds, cores, faces, effects and script constants) out of your own Android package, so a build needs no DS cartridge.")
    parser.add_argument("--package", required=True, help="your own copy of the Android package (.apk)")
    parser.add_argument("--apk-folder", default=DEFAULT_APK_FOLDER, help="the folder the packs were copied into; the tables are saved there")
    arguments = parser.parse_args()
    try:
        library_bytes = extract_text_key.read_library(arguments.package)
        path = save_tables(arguments.apk_folder, read_tables(library_bytes))
    except (extract_text_key.ExtractionError, TablesError, arm64_runner.RunError, OSError) as error:
        print("error: " + str(error), file=sys.stderr)
        return 1
    print("saved " + path)
    return 0


if __name__ == "__main__":
    sys.exit(main())
