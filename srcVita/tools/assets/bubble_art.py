import os

import asset_writers
import bake_record
import texture_encoder

RGBA_BYTES = 4
TRANSPARENT_WHITE = bytes([255, 255, 255, 0])


def read_part_pixels(tool_path, work_folder, tex_path, name, part):
    rgba_path = os.path.join(work_folder, name + "_part.rgba")
    texture_encoder.run_texture_tool(tool_path, tex_path, rgba_path, part["width"], part["height"], part["crop"])
    with open(rgba_path, "rb") as source:
        pixels = source.read()
    os.remove(rgba_path)
    return pixels


def part_pixel_offset(part, x, y):
    source_x = part["width"] - 1 - x if part.get("flip_x") else x
    source_y = part["height"] - 1 - y if part.get("flip_y") else y
    return (source_y * part["width"] + source_x) * RGBA_BYTES


def blend_pixel_over(canvas, canvas_offset, part_pixels, part_offset):
    top_alpha = part_pixels[part_offset + 3]
    bottom_alpha = canvas[canvas_offset + 3]
    remaining = bottom_alpha * (255 - top_alpha) // 255
    result_alpha = top_alpha + remaining
    if result_alpha == 0:
        return
    for channel in range(3):
        canvas[canvas_offset + channel] = (part_pixels[part_offset + channel] * top_alpha + canvas[canvas_offset + channel] * remaining) // result_alpha
    canvas[canvas_offset + 3] = result_alpha


def paste_part(canvas, canvas_width, canvas_height, part, part_pixels):
    for y in range(part["height"]):
        for x in range(part["width"]):
            canvas_x = part["x"] + x
            canvas_y = part["y"] + y
            if 0 <= canvas_x < canvas_width and 0 <= canvas_y < canvas_height:
                blend_pixel_over(canvas, (canvas_y * canvas_width + canvas_x) * RGBA_BYTES, part_pixels, part_pixel_offset(part, x, y))


def compose_gtex(tool_path, work_folder, description, tex_bytes):
    name = description["name"]
    width = description["width"]
    height = description["height"]
    tex_path = os.path.join(work_folder, name + "_source.tex")
    with open(tex_path, "wb") as output:
        output.write(tex_bytes)
    canvas = bytearray(TRANSPARENT_WHITE * (width * height))
    for part in description["parts"]:
        paste_part(canvas, width, height, part, read_part_pixels(tool_path, work_folder, tex_path, name, part))
    os.remove(tex_path)
    blocks = texture_encoder.encode_rgba(work_folder, name, width, height, bytes(canvas), "dxt5")
    return asset_writers.gtex_bytes(width, height, asset_writers.FORMAT_DXT5, blocks)


def bake_composed_texture(packs, tool_path, work_folder, description, record, code):
    name = description["name"]
    tex_bytes = packs.read(description["source"])
    relative_path = "textures/" + name + ".gtex"
    key = record.key_for(code, description, tex_bytes)
    gtex_bytes = record.read_if_current(relative_path, key)
    if gtex_bytes is None:
        gtex_bytes = compose_gtex(tool_path, work_folder, description, tex_bytes)
        record.store(relative_path, key, gtex_bytes)
    return bake_record.result_of_own_size(name, gtex_bytes)
