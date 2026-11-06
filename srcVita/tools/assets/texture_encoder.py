import os
import struct
import subprocess

import asset_writers

DDS_HEADER_SIZE = 128
DDS_FOURCC_OFFSET = 84
TEX_HEADER_SIZE = 40
TEX_WIDTH_OFFSET = 8


def read_tex_size(tex_bytes):
    if tex_bytes[:4] != b"TEX\0" or len(tex_bytes) < TEX_HEADER_SIZE:
        raise ValueError("not a TEX file")
    return struct.unpack_from("<HH", tex_bytes, TEX_WIDTH_OFFSET)


def run_texture_tool(tool_path, tex_path, rgba_path, width, height, crop=None):
    command = [tool_path, tex_path, rgba_path, str(width), str(height)]
    if crop is not None:
        for value in crop:
            command.append(str(value))
    result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if result.returncode != 0:
        raise ValueError(result.stderr.decode("utf-8", "replace").strip())
    fields = result.stdout.decode("ascii").split()
    return fields[3] == "1"


def compress_with_magick(rgba_path, dds_path, width, height, compression):
    command = ["magick", "-size", "%dx%d" % (width, height), "-depth", "8", "rgba:" + rgba_path]
    if compression == "dxt1":
        command.extend(["-alpha", "off"])
    command.extend(["-define", "dds:compression=" + compression])
    command.extend(["-define", "dds:cluster-fit=true"])
    command.extend(["-define", "dds:mipmaps=0"])
    command.append("dds:" + dds_path)
    subprocess.run(command, check=True)
    with open(dds_path, "rb") as source:
        return source.read()


def block_data_from_dds(dds_bytes, width, height, compression):
    if dds_bytes[:4] != b"DDS " or len(dds_bytes) < DDS_HEADER_SIZE:
        raise ValueError("magick did not write a DDS file")
    stored_height, stored_width = struct.unpack_from("<II", dds_bytes, 12)
    if stored_width != width or stored_height != height:
        raise ValueError("magick wrote the wrong image size")
    fourcc = dds_bytes[DDS_FOURCC_OFFSET:DDS_FOURCC_OFFSET + 4]
    if fourcc != compression.upper().encode("ascii"):
        raise ValueError("magick wrote the wrong compression")
    return dds_bytes[DDS_HEADER_SIZE:]


def encode_rgba(work_folder, name, width, height, rgba_pixels, compression):
    rgba_path = os.path.join(work_folder, name + ".rgba")
    dds_path = os.path.join(work_folder, name + ".dds")
    with open(rgba_path, "wb") as output:
        output.write(rgba_pixels)
    dds_bytes = compress_with_magick(rgba_path, dds_path, width, height, compression)
    return block_data_from_dds(dds_bytes, width, height, compression)


def convert_tex_to_gtex(tool_path, work_folder, name, tex_bytes, width, height, crop=None):
    tex_path = os.path.join(work_folder, name + ".tex")
    rgba_path = os.path.join(work_folder, name + ".rgba")
    dds_path = os.path.join(work_folder, name + ".dds")
    with open(tex_path, "wb") as output:
        output.write(tex_bytes)
    is_opaque = run_texture_tool(tool_path, tex_path, rgba_path, width, height, crop)
    with open(rgba_path, "rb") as source:
        rgba_pixels = source.read()
    if is_opaque:
        texture_format = asset_writers.FORMAT_DXT1
        compression = "dxt1"
    else:
        texture_format = asset_writers.FORMAT_DXT5
        compression = "dxt5"
    block_data = encode_rgba(work_folder, name, width, height, rgba_pixels, compression)
    os.remove(tex_path)
    os.remove(rgba_path)
    os.remove(dds_path)
    return asset_writers.gtex_bytes(width, height, texture_format, block_data), texture_format
