import argparse
import os
import struct
import subprocess
import sys
import tempfile
import zlib

TOOLS_FOLDER = os.path.dirname(os.path.abspath(__file__))
SRC_VITA_FOLDER = os.path.dirname(TOOLS_FOLDER)
ASSETS_FOLDER = os.path.join(os.path.dirname(SRC_VITA_FOLDER), "extracted", "remaster_apk")
TEXTURE_TOOL = os.path.join(SRC_VITA_FOLDER, "build", "tools", "texture_tool")
LOGO_PACK_PATH = "natives/android/charfiles/3d/06_id/11_title/title_up_iml4.tex.691230823.en"
FIELD_COLOR = "#0c1523"
LOGO_LAYOUTS = (
    ("icon0.png", 128, 128, 120, 120),
    ("livearea/contents/bg.png", 840, 500, 600, 300),
    ("livearea/contents/startup.png", 280, 158, 250, 140),
)

sys.path.insert(0, os.path.join(TOOLS_FOLDER, "assets"))

import pack_files
import texture_encoder

PALETTE = [(12, 21, 35), (235, 240, 250), (64, 220, 191)]
BACKGROUND = 0
LETTER = 1
ACCENT = 2

GLYPHS = {
    "G": ["01110", "10001", "10000", "10111", "10001", "10001", "01110"],
    "T": ["11111", "00100", "00100", "00100", "00100", "00100", "00100"],
}


def png_chunk(kind, payload):
    checksum = zlib.crc32(kind + payload)
    return struct.pack(">I", len(payload)) + kind + payload + struct.pack(">I", checksum)


def indexed_png(width, height, pixels):
    header = struct.pack(">IIBBBBB", width, height, 8, 3, 0, 0, 0)
    palette = bytes(channel for color in PALETTE for channel in color)
    rows = bytearray()
    for row in range(height):
        rows.append(0)
        rows.extend(pixels[row * width:(row + 1) * width])
    return (
        b"\x89PNG\r\n\x1a\n"
        + png_chunk(b"IHDR", header)
        + png_chunk(b"PLTE", palette)
        + png_chunk(b"IDAT", zlib.compress(bytes(rows), 9))
        + png_chunk(b"IEND", b"")
    )


def draw_letters(pixels, width, height, text, scale):
    letter_width = 5 * scale
    letter_height = 7 * scale
    gap = scale
    total_width = len(text) * letter_width + (len(text) - 1) * gap
    start_x = (width - total_width) // 2
    start_y = (height - letter_height) // 2
    for position, letter in enumerate(text):
        glyph = GLYPHS[letter]
        left = start_x + position * (letter_width + gap)
        for row, row_bits in enumerate(glyph):
            for column, bit in enumerate(row_bits):
                if bit != "1":
                    continue
                for y in range(start_y + row * scale, start_y + (row + 1) * scale):
                    for x in range(left + column * scale, left + (column + 1) * scale):
                        pixels[y * width + x] = LETTER


def draw_underline(pixels, width, height, scale):
    bar_height = max(2, scale // 2)
    top = (height + 7 * scale) // 2 + scale
    for y in range(top, min(height, top + bar_height)):
        for x in range(width // 4, width - width // 4):
            pixels[y * width + x] = ACCENT


def make_image(width, height, scale):
    pixels = bytearray([BACKGROUND]) * (width * height)
    draw_letters(pixels, width, height, "GT", scale)
    draw_underline(pixels, width, height, scale)
    return indexed_png(width, height, pixels)


def write_file(path, data):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "wb") as output:
        output.write(data)


def assets_available(assets_folder, texture_tool):
    return os.path.isdir(assets_folder) and os.path.isfile(texture_tool)


def export_logo(assets_folder, texture_tool, work_folder):
    tex_bytes = pack_files.PackSet(assets_folder).read(LOGO_PACK_PATH)
    width, height = texture_encoder.read_tex_size(tex_bytes)
    tex_path = os.path.join(work_folder, "logo.tex")
    rgba_path = os.path.join(work_folder, "logo.rgba")
    write_file(tex_path, tex_bytes)
    texture_encoder.run_texture_tool(texture_tool, tex_path, rgba_path, width, height)
    return rgba_path, width, height


def compose_command(logo_path, logo_width, logo_height, layout, output_path):
    _, canvas_width, canvas_height, box_width, box_height = layout
    return [
        "magick", "-size", "%dx%d" % (logo_width, logo_height), "-depth", "8", "rgba:" + logo_path,
        "-background", FIELD_COLOR, "-flatten", "-trim", "+repage",
        "-resize", "%dx%d" % (box_width, box_height),
        "-gravity", "center", "-extent", "%dx%d" % (canvas_width, canvas_height),
        "-colors", "256", "-define", "png:exclude-chunks=date,time", "PNG8:" + output_path,
    ]


def write_logo_images(output_folder, assets_folder, texture_tool):
    with tempfile.TemporaryDirectory() as work_folder:
        logo_path, logo_width, logo_height = export_logo(assets_folder, texture_tool, work_folder)
        for layout in LOGO_LAYOUTS:
            output_path = os.path.join(output_folder, layout[0])
            os.makedirs(os.path.dirname(output_path), exist_ok=True)
            subprocess.run(compose_command(logo_path, logo_width, logo_height, layout, output_path), check=True)


def write_procedural_images(output_folder):
    write_file(os.path.join(output_folder, "icon0.png"), make_image(128, 128, 10))
    write_file(os.path.join(output_folder, "livearea/contents/bg.png"), make_image(840, 500, 40))
    write_file(os.path.join(output_folder, "livearea/contents/startup.png"), make_image(280, 158, 12))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("output_folder")
    parser.add_argument("--assets-folder", default=ASSETS_FOLDER)
    parser.add_argument("--texture-tool", default=TEXTURE_TOOL)
    parser.add_argument("--logo", action="store_true", help="build the images from the title logo in the extracted assets instead of the monogram")
    arguments = parser.parse_args()
    if not arguments.logo:
        write_procedural_images(arguments.output_folder)
        return 0
    if assets_available(arguments.assets_folder, arguments.texture_tool):
        write_logo_images(arguments.output_folder, arguments.assets_folder, arguments.texture_tool)
    else:
        print("make_livearea: game assets or texture_tool missing, using procedural images", file=sys.stderr)
        write_procedural_images(arguments.output_folder)
    return 0


if __name__ == "__main__":
    sys.exit(main())
