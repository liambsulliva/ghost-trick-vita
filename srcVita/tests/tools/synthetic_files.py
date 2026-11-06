import os
import struct
import zipfile
import zlib

import pack_files

FORMAT_RGBA8 = 28
FORMAT_ASTC_6X6 = 1038
FORMAT_ETC2_RGB = 1086
TEX_VERSION = 691230823
TEX_HEADER_AND_RECORD_SIZE = 56
MIP_RECORD_SIZE = 16
TYPE_SHEET = 147581786
TYPE_RECORD = 4048237731
TYPE_ROOT_ANIMATION = 240123514


def tex_file(width, height, format_code, payload, row_pitch):
    header = b"TEX\0" + struct.pack("<IHHHBBIiIIQ", TEX_VERSION, width, height, 1, 1, 16, format_code, -1, 0, 0, 0)
    record = struct.pack("<QII", TEX_HEADER_AND_RECORD_SIZE, row_pitch, len(payload))
    return header + record + payload


def tex_file_with_small_mip_stored_first(width, height, format_code, payload, row_pitch, small_mip_payload):
    header = b"TEX\0" + struct.pack("<IHHHBBIiIIQ", TEX_VERSION, width, height, 1, 1, 32, format_code, -1, 0, 0, 0)
    small_mip_offset = len(header) + 2 * MIP_RECORD_SIZE
    top_record = struct.pack("<QII", small_mip_offset + len(small_mip_payload), row_pitch, len(payload))
    small_record = struct.pack("<QII", small_mip_offset, row_pitch // 2, len(small_mip_payload))
    return header + top_record + small_record + small_mip_payload + payload


def astc_constant_block(red, green, blue, alpha):
    block = bytes([0xFC, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF])
    return block + struct.pack("<HHHH", red * 257, green * 257, blue * 257, alpha * 257)


def etc2_constant_block(red_5bit, green_5bit, blue_5bit):
    return bytes([red_5bit << 3, green_5bit << 3, blue_5bit << 3, 0x02, 0, 0, 0, 0])


def expand_5_bits(value):
    return (value << 3) | (value >> 2)


class ByteWriter:
    def __init__(self):
        self.data = bytearray()

    def align(self, size):
        while len(self.data) % size != 0:
            self.data.append(0)

    def i32(self, value):
        self.align(4)
        self.data.extend(struct.pack("<i", value))

    def i16(self, value):
        self.align(2)
        self.data.extend(struct.pack("<h", value))

    def u8(self, value):
        self.data.append(value)

    def string(self, text):
        encoded = (text + "\0").encode("utf-16-le")
        self.i32(len(encoded) // 2)
        self.data.extend(encoded)

    def vector(self, x, y):
        self.align(16)
        self.data.extend(struct.pack("<4f", x, y, 0.0, 0.0))


def write_sheet(writer, textures, atlas, cell, cells_per_texture):
    writer.i32(len(textures))
    for texture in textures:
        writer.string(texture)
    writer.i32(atlas[0])
    writer.i32(atlas[1])
    writer.i32(cell[0])
    writer.i32(cell[1])
    writer.i32(len(cells_per_texture))
    for count in cells_per_texture:
        writer.i32(count)
    writer.vector(atlas[0] // cell[0], atlas[1] // cell[1])
    writer.i32(sum(cells_per_texture))
    writer.u8(0)
    writer.u8(0)


def write_record(writer, shorts, position, cell, adjust):
    writer.i32(len(shorts))
    for value in shorts:
        writer.i16(value)
    writer.i32(1)
    writer.vector(position[0], position[1])
    writer.i32(cell)
    writer.i32(adjust[0])
    writer.i32(adjust[1])
    writer.i32(0)


def timeline_file(textures, atlas, cell, cells_per_texture, records, ds_frame_count, placement_shift=(0, 0)):
    rsz_offset = 64
    instance_count = 1 + 1 + len(records) + 1
    instance_table_offset = 48
    data_offset = instance_table_offset + 8 * instance_count
    data_offset = data_offset + (16 - data_offset % 16) % 16
    writer = ByteWriter()
    writer.data.extend(b"USR\0" + struct.pack("<III", 0, 0, 0))
    writer.data.extend(struct.pack("<QQQ", 0, 0, rsz_offset))
    writer.align(16)
    while len(writer.data) < rsz_offset:
        writer.data.append(0)
    writer.data.extend(b"RSZ\0" + struct.pack("<IIII", 16, 1, instance_count, 0))
    writer.align(8)
    writer.data.extend(struct.pack("<QQ", instance_table_offset, data_offset))
    types = [0, TYPE_SHEET] + [TYPE_RECORD] * len(records) + [TYPE_ROOT_ANIMATION]
    while len(writer.data) < rsz_offset + instance_table_offset:
        writer.data.append(0)
    for type_id in types:
        writer.data.extend(struct.pack("<II", type_id, 0))
    while len(writer.data) < rsz_offset + data_offset:
        writer.data.append(0)
    write_sheet(writer, textures, atlas, cell, cells_per_texture)
    for record in records:
        adjust = (0, 0)
        if len(record) > 3:
            adjust = record[3]
        write_record(writer, record[0], record[1], record[2], adjust)
    writer.i32(1)
    writer.i32(len(records))
    for index in range(len(records)):
        writer.i32(2 + index)
    for value in (1, ds_frame_count, 0, placement_shift[0], placement_shift[1]):
        writer.i32(value)
    return bytes(writer.data)


def pack_bytes(files):
    table = bytearray()
    payload = bytearray()
    table_size = len(files) * pack_files.PACK_ENTRY_SIZE
    data_start = pack_files.PACK_HEADER_SIZE + table_size
    for path, data, use_deflate in files:
        lower_hash, upper_hash = pack_files.path_hash_key(path)
        stored = data
        method = pack_files.METHOD_STORED
        if use_deflate:
            compressor = zlib.compressobj(wbits=-15)
            stored = compressor.compress(data) + compressor.flush()
            method = pack_files.METHOD_DEFLATE
        table.extend(struct.pack("<IIQQQQQ", lower_hash, upper_hash, data_start + len(payload), len(stored), len(data), method, 0))
        payload.extend(stored)
    header = b"KPKA" + struct.pack("<IIII", 4, len(files), 0, 0)
    return header[:pack_files.PACK_HEADER_SIZE] + bytes(table) + bytes(payload)


def write_apk_folder(folder, files):
    with open(os.path.join(folder, "re_chunk_000.pak"), "wb") as output:
        output.write(pack_bytes(files))
    for pack_name, file_name, member_name in pack_files.PACK_SOURCES[1:]:
        with zipfile.ZipFile(os.path.join(folder, file_name), "w", zipfile.ZIP_STORED) as archive:
            archive.writestr(member_name, pack_bytes([]))
