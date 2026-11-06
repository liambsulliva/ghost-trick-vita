import os
import struct
import zipfile
import zlib

PACK_SOURCES = (
    ("re_chunk_000.pak", None),
    ("data02", "files/assetpacks/asset_fast_follow/18/18/assets/re_chunk_001.pak"),
    ("data01", "files/assetpacks/asset_on_demand0/18/18/assets/re_chunk_002.pak"),
)
PACK_VERSION = 4
PACK_HEADER_SIZE = 16
PACK_ENTRY_COUNT_OFFSET = 8
PACK_METHOD_MASK = 0xF
PACK_ENTRY_SIZE = 48
METHOD_STORED = 0
METHOD_DEFLATE = 1


def rotate_left(value, amount):
    return ((value << amount) | (value >> (32 - amount))) & 0xFFFFFFFF


def scramble_word(word):
    word = (word * 0xCC9E2D51) & 0xFFFFFFFF
    word = rotate_left(word, 15)
    return (word * 0x1B873593) & 0xFFFFFFFF


def murmur3_hash(data):
    state = 0xFFFFFFFF
    block_count = len(data) // 4
    for block_index in range(block_count):
        word = struct.unpack_from("<I", data, block_index * 4)[0]
        state = rotate_left(state ^ scramble_word(word), 13)
        state = (state * 5 + 0xE6546B64) & 0xFFFFFFFF
    tail = data[block_count * 4:]
    if tail:
        state = state ^ scramble_word(int.from_bytes(tail, "little"))
    state = state ^ len(data)
    state = state ^ (state >> 16)
    state = (state * 0x85EBCA6B) & 0xFFFFFFFF
    state = state ^ (state >> 13)
    state = (state * 0xC2B2AE35) & 0xFFFFFFFF
    return state ^ (state >> 16)


def path_hash_key(path):
    lower_hash = murmur3_hash(path.lower().encode("utf-16-le"))
    upper_hash = murmur3_hash(path.upper().encode("utf-16-le"))
    return lower_hash, upper_hash


def zip_member_data_offset(archive_path, member_name):
    with zipfile.ZipFile(archive_path) as archive:
        info = archive.getinfo(member_name)
    if info.compress_type != zipfile.ZIP_STORED:
        raise ValueError(member_name + " is not stored uncompressed")
    with open(archive_path, "rb") as source:
        source.seek(info.header_offset)
        local_header = source.read(30)
    if local_header[:4] != b"PK\x03\x04":
        raise ValueError(member_name + " has no local file header")
    name_length, extra_length = struct.unpack_from("<HH", local_header, 26)
    return info.header_offset + 30 + name_length + extra_length


class PackEntry:
    def __init__(self, file_path, base_offset, offset, stored_size, plain_size, method):
        self.file_path = file_path
        self.base_offset = base_offset
        self.offset = offset
        self.stored_size = stored_size
        self.plain_size = plain_size
        self.method = method


def read_pack_entries(file_path, base_offset):
    with open(file_path, "rb") as source:
        source.seek(base_offset)
        header = source.read(PACK_HEADER_SIZE)
        if header[:4] != b"KPKA" or header[4] != PACK_VERSION:
            raise ValueError(file_path + ": not a KPKA version " + str(PACK_VERSION) + " pack")
        entry_count = struct.unpack_from("<I", header, PACK_ENTRY_COUNT_OFFSET)[0]
        table = source.read(entry_count * PACK_ENTRY_SIZE)
    if len(table) != entry_count * PACK_ENTRY_SIZE:
        raise ValueError(file_path + ": truncated entry table")
    entries = {}
    for index in range(entry_count):
        lower_hash, upper_hash, offset, stored_size, plain_size, attributes = struct.unpack_from("<IIQQQQ", table, index * PACK_ENTRY_SIZE)
        entries[(lower_hash, upper_hash)] = PackEntry(file_path, base_offset, offset, stored_size, plain_size, attributes & PACK_METHOD_MASK)
    return entries


class PackSet:
    def __init__(self, root):
        self.entries = {}
        for file_name, member_name in PACK_SOURCES:
            file_path = os.path.join(root, file_name)
            base_offset = 0
            if member_name is not None:
                base_offset = zip_member_data_offset(file_path, member_name)
            self.entries.update(read_pack_entries(file_path, base_offset))

    def read(self, path):
        entry = self.entries.get(path_hash_key(path))
        if entry is None:
            raise KeyError("not in the packs: " + path)
        with open(entry.file_path, "rb") as source:
            source.seek(entry.base_offset + entry.offset)
            stored = source.read(entry.stored_size)
        if len(stored) != entry.stored_size:
            raise ValueError("truncated pack entry: " + path)
        if entry.method == METHOD_STORED:
            plain = stored
        elif entry.method == METHOD_DEFLATE:
            plain = zlib.decompress(stored, -15)
        else:
            raise ValueError("unsupported pack compression for " + path)
        if len(plain) != entry.plain_size:
            raise ValueError("wrong decompressed size for " + path)
        return plain
