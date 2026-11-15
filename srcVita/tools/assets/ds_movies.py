import concurrent.futures
import os
import struct
import subprocess

import numpy

import asset_writers
import bake_record
import chapter_assets
import class_movies
import ds_rom
import nds_unpack
import script_assets
import texture_encoder

CODE_FILES = ["ds_movies.py", "texture_encoder.py", "asset_writers.py"]
MODS_MAGIC = b"MODS"
MODS_HEADER = "<4s4sIIII II"
MODS_HEADER_SIZE = struct.calcsize(MODS_HEADER)
MODS_FPS_ONE = 1 << 24
MODS_SUFFIX = ".mods"
ARM9_BASE = 0x02000000
OVERLAY_TABLE_FIELD = 0x50
OVERLAY_ENTRY = "<8I"
OVERLAY_ENTRY_SIZE = struct.calcsize(OVERLAY_ENTRY)
OVERLAY_COMPRESSED_BIT = 1 << 24
OVERLAY_SIZE_MASK = 0xFFFFFF
MOVIE_OVERLAY = 4
START_SLOT = 11
WORD = 4
THUMB_BIT = 1
LITERAL_LOAD_MASK = 0xF800
LITERAL_LOAD = 0x4800
LITERAL_REACH = 1024
CALL_FIRST = 0xF000
CALL_SECOND = (0xF800, 0xE800)
CALL_HALF_MASK = 0x7FF
CALL_SIGN = 0x400000
CALL_RANGE = 0x800000
REGISTER_CALL_REACH = 16
REGISTER_ARGUMENT = 1
FUNCTION_REACH = 0x40
FACTORY_CONSTRUCTOR_CALL = 1
TEXTURE_FOLDER = "textures"
FRAME_NAME = "movie_%s_%03d"
TICKS_PER_SECOND = script_assets.TICKS_PER_SECOND
CHROMA_MIDDLE = 128
FRAME_ENCODERS = 8


class DsMovieRefused(ValueError):
    pass


def read_overlay(rom, overlay_id):
    header = rom.read_range(0, OVERLAY_TABLE_FIELD + 8)
    table_offset, table_size = struct.unpack_from("<II", header, OVERLAY_TABLE_FIELD)
    table = rom.read_range(table_offset, table_size)
    for offset in range(0, table_size - OVERLAY_ENTRY_SIZE + 1, OVERLAY_ENTRY_SIZE):
        entry = struct.unpack_from(OVERLAY_ENTRY, table, offset)
        if entry[0] != overlay_id:
            continue
        start, end = struct.unpack("<II", rom.read_range(rom.allocation_offset + entry[6] * ds_rom.ALLOCATION_ENTRY_SIZE, ds_rom.ALLOCATION_ENTRY_SIZE))
        data = rom.read_range(start, end - start)
        if entry[7] & OVERLAY_COMPRESSED_BIT:
            data = nds_unpack.blz_decompress(data[:entry[7] & OVERLAY_SIZE_MASK])
        return entry[1], data
    raise DsMovieRefused("the cartridge has no overlay %d" % overlay_id)


class Memory:
    def __init__(self, regions):
        self.regions = regions

    def region_of(self, address, size):
        for base, data in self.regions:
            if base <= address and address + size <= base + len(data):
                return base, data
        return None, None

    def holds(self, address):
        return self.region_of(address, WORD)[0] is not None

    def half(self, address):
        base, data = self.region_of(address, 2)
        if base is None:
            raise DsMovieRefused("address %08x is outside the cartridge code" % address)
        return struct.unpack_from("<H", data, address - base)[0]

    def word(self, address):
        base, data = self.region_of(address, WORD)
        if base is None:
            raise DsMovieRefused("address %08x is outside the cartridge code" % address)
        return struct.unpack_from("<I", data, address - base)[0]

    def text_at(self, address):
        base, data = self.region_of(address, 1)
        if base is None:
            return None
        end = data.find(b"\0", address - base)
        return data[address - base:end].decode("latin-1") if end >= 0 else None

    def addresses_of_text(self, text):
        found = []
        needle = b"\0" + text.encode("ascii") + b"\0"
        for base, data in self.regions:
            position = data.find(needle)
            while position >= 0:
                found.append(base + position + 1)
                position = data.find(needle, position + 1)
        return found

    def words_equal_to(self, value):
        found = []
        packed = struct.pack("<I", value)
        for base, data in self.regions:
            position = data.find(packed)
            while position >= 0:
                if position % WORD == 0:
                    found.append(base + position)
                position = data.find(packed, position + 1)
        return found


def literal_load(memory, address):
    half = memory.half(address)
    if half & LITERAL_LOAD_MASK != LITERAL_LOAD:
        return None
    return (half >> 8) & 7, ((address + 4) & ~3) + (half & 0xFF) * WORD


def call_target(memory, address):
    first = memory.half(address)
    second = memory.half(address + 2)
    if first & LITERAL_LOAD_MASK != CALL_FIRST or second & LITERAL_LOAD_MASK not in CALL_SECOND:
        return None
    offset = ((first & CALL_HALF_MASK) << 12) | ((second & CALL_HALF_MASK) << 1)
    if offset & CALL_SIGN:
        offset = offset - CALL_RANGE
    return address + 4 + offset


def calls_in(memory, start):
    return [target for target in (call_target(memory, address) for address in range(start, start + FUNCTION_REACH, 2)) if target is not None]


def first_literal_word(memory, start):
    for address in range(start, start + FUNCTION_REACH, 2):
        loaded = literal_load(memory, address)
        if loaded is not None:
            return memory.word(loaded[1])
    raise DsMovieRefused("no literal load in the function at %08x" % start)


def mods_file_of_start(memory, start):
    for address in range(start, start + FUNCTION_REACH, 2):
        loaded = literal_load(memory, address)
        if loaded is not None and memory.holds(loaded[1]):
            text = memory.text_at(memory.word(loaded[1]))
            if text is not None and text.endswith(MODS_SUFFIX):
                return text
    return None


def register_calls_naming(memory, name_address):
    for slot in memory.words_equal_to(name_address):
        for address in range(max(slot - LITERAL_REACH, 0), slot, 2):
            if not memory.holds(address):
                continue
            loaded = literal_load(memory, address)
            if loaded != (REGISTER_ARGUMENT, slot):
                continue
            for call in range(address + 2, address + REGISTER_CALL_REACH, 2):
                target = call_target(memory, call)
                if target is not None:
                    yield target
                    break


def movie_file_of_register(memory, register):
    methods = first_literal_word(memory, register)
    factory = memory.word(methods) & ~THUMB_BIT
    calls = calls_in(memory, factory)
    if len(calls) <= FACTORY_CONSTRUCTOR_CALL:
        return None
    vtable = first_literal_word(memory, calls[FACTORY_CONSTRUCTOR_CALL])
    start = memory.word(vtable + START_SLOT * WORD) & ~THUMB_BIT
    return mods_file_of_start(memory, start)


def native_movie_file(memory, class_name):
    for name_address in memory.addresses_of_text(class_name):
        for register in register_calls_naming(memory, name_address):
            try:
                found = movie_file_of_register(memory, register)
            except DsMovieRefused:
                continue
            if found is not None:
                return found
    return None


def cartridge_memory(arm9, rom):
    overlay_base, overlay = read_overlay(rom, MOVIE_OVERLAY)
    return Memory([(ARM9_BASE, arm9), (overlay_base, overlay)])


def native_class_of(row):
    return row.get("ds_class", row.get("class"))


def movie_plan(movies, memory):
    plan = {}
    for row in movies:
        class_name = native_class_of(row)
        if class_name is None:
            plan[row["name"]] = (None, "no native class names this movie")
            continue
        found = native_movie_file(memory, class_name)
        plan[row["name"]] = (found, None) if found is not None else (None, "%s starts no .mods file on the cartridge" % class_name)
    return plan


def read_header(mods_bytes):
    if len(mods_bytes) < MODS_HEADER_SIZE or mods_bytes[:4] != MODS_MAGIC:
        raise DsMovieRefused("not a Mobiclip .mods file")
    _, _, frames, width, height, fps, channels, frequency = struct.unpack_from(MODS_HEADER, mods_bytes)
    return {"frames": frames, "width": width, "height": height, "fps": fps / MODS_FPS_ONE, "audio_channels": channels, "audio_frequency": frequency}


def ycgco_to_rgba(planes, width, height):
    luma = planes[:width * height].reshape(height, width).astype(numpy.int32)
    chroma_size = (width // 2) * (height // 2)
    green = planes[width * height:width * height + chroma_size].reshape(height // 2, width // 2).repeat(2, 0).repeat(2, 1).astype(numpy.int32) - CHROMA_MIDDLE
    orange = planes[width * height + chroma_size:width * height + 2 * chroma_size].reshape(height // 2, width // 2).repeat(2, 0).repeat(2, 1).astype(numpy.int32) - CHROMA_MIDDLE
    rest = luma - green
    rgba = numpy.full((height, width, 4), 255, dtype=numpy.uint8)
    rgba[..., 0] = numpy.clip(rest + orange, 0, 255)
    rgba[..., 1] = numpy.clip(luma + green, 0, 255)
    rgba[..., 2] = numpy.clip(rest - orange, 0, 255)
    return rgba.tobytes()


def decode_frames(mods_bytes, header, work_folder, name):
    source_path = os.path.join(work_folder, name + MODS_SUFFIX)
    with open(source_path, "wb") as output:
        output.write(mods_bytes)
    try:
        planes = subprocess.run(["ffmpeg", "-v", "error", "-i", source_path, "-vf", "setparams=colorspace=bt470bg", "-f", "rawvideo", "-pix_fmt", "yuv420p", "-"], capture_output=True, check=True).stdout
    finally:
        os.remove(source_path)
    width = header["width"]
    height = header["height"]
    frame_size = width * height * 3 // 2
    data = numpy.frombuffer(planes, dtype=numpy.uint8)
    return [ycgco_to_rgba(data[index * frame_size:(index + 1) * frame_size], width, height) for index in range(len(planes) // frame_size)]


def frame_path(name, index):
    return TEXTURE_FOLDER + "/" + FRAME_NAME % (name, index) + ".gtex"


def kept_frames(record, name, key):
    details = record.details_of(frame_path(name, 0))
    if details.get("key") != key or "frames" not in details:
        return None
    for index in range(details["frames"]):
        if record.read_if_current(frame_path(name, index), key) is None:
            return None
    return details["frames"]


def bake_movie(record, name, mods_bytes, header, key, work_folder):
    frames = decode_frames(mods_bytes, header, work_folder, name)
    if not frames:
        raise DsMovieRefused("ffmpeg decoded no frames")
    width = header["width"]
    height = header["height"]

    def encode(index):
        blocks = texture_encoder.encode_rgba(work_folder, FRAME_NAME % (name, index), width, height, frames[index], "dxt1")
        details = {"key": key, "frames": len(frames)} if index == 0 else None
        record.store(frame_path(name, index), key, asset_writers.gtex_bytes(width, height, asset_writers.FORMAT_DXT1, blocks), details)

    with concurrent.futures.ThreadPoolExecutor(FRAME_ENCODERS) as pool:
        list(pool.map(encode, range(len(frames))))
    return len(frames)


def requested_class_ids(scripts):
    for script in scripts.values():
        for item in script.operations:
            if item[2] == class_movies.MOVIE_OP:
                yield item[3][0] | (item[3][1] << 16), script.path


def classes_with_no_baked_movie(movies, scripts):
    baked = set(int(row["class_id"], 16) for row in movies if "class_id" in row and row.get("frames", 0) > 0)
    missing = {}
    for class_id, path in requested_class_ids(scripts):
        if class_id not in baked:
            missing.setdefault(class_id, path)
    return missing


def bake(movies, memory, rom, record, work_folder):
    code = bake_record.code_fingerprint(CODE_FILES)
    plan = movie_plan(movies, memory)
    rows = []
    paths = []
    gaps = {}
    for movie in movies:
        row = dict(movie, frames=0)
        file_name, reason = plan[movie["name"]]
        try:
            if file_name is None:
                raise DsMovieRefused(reason)
            mods_bytes = rom.read_file(file_name)
            header = read_header(mods_bytes)
            row.update(fps=max(1, int(round(header["fps"]))), width=header["width"], height=header["height"], ds_file=file_name)
            key = record.key_for(code, {"width": header["width"], "height": header["height"]}, mods_bytes)
            frame_count = kept_frames(record, movie["name"], key)
            if frame_count is None:
                frame_count = bake_movie(record, movie["name"], mods_bytes, header, key, work_folder)
            row["frames"] = frame_count
            paths.extend(frame_path(movie["name"], index) for index in range(frame_count))
        except (DsMovieRefused, ds_rom.RomRefused, subprocess.CalledProcessError, ValueError) as error:
            gaps[movie["name"]] = str(error)
        rows.append(row)
    return rows, paths, gaps


def add_to_set(set_description, scripts, arm9, rom_path, record, work_folder):
    with ds_rom.DsRom(rom_path) as rom:
        movies, paths, gaps = bake(set_description.get("movies", []), cartridge_memory(arm9, rom), rom, record, work_folder)
    expanded = class_movies.add_to_set(dict(set_description, movies=movies))
    if gaps:
        print("ds movies left out: %d (%s)" % (len(gaps), ", ".join("%s: %s" % item for item in sorted(gaps.items()))))
    missing = classes_with_no_baked_movie(expanded["movies"], scripts)
    if missing:
        print("ds movie classes the scripts start with no baked movie: " + ", ".join("%08x (%s)" % (class_id, os.path.basename(path)) for class_id, path in sorted(missing.items())))
    print("ds movies: %d baked, %d frames" % (sum(1 for row in movies if row["frames"] > 0), len(paths)))
    return expanded, paths
