import argparse
import os
import struct
import sys
import zlib

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
    palette = bytes(channel for colour in PALETTE for channel in colour)
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


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("output_folder")
    arguments = parser.parse_args()
    write_file(os.path.join(arguments.output_folder, "icon0.png"), make_image(128, 128, 10))
    write_file(os.path.join(arguments.output_folder, "livearea/contents/bg.png"), make_image(840, 500, 40))
    write_file(os.path.join(arguments.output_folder, "livearea/contents/startup.png"), make_image(280, 158, 12))
    return 0


if __name__ == "__main__":
    sys.exit(main())
