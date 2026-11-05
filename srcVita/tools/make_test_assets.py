import argparse
import math
import os
import struct
import sys

FRAME_COUNT = 8
FRAME_SIZE = 256
BACKGROUND_WIDTH = 960
BACKGROUND_HEIGHT = 540


def texture_file_bytes(width, height, rgba_pixels):
    header = b"GTEX" + struct.pack("<IIII", width, height, 0, len(rgba_pixels))
    return header + bytes(rgba_pixels)


def background_pixels(width, height, tint):
    pixels = bytearray(width * height * 4)
    for y in range(height):
        row_shade = 40 + 120 * y // height
        for x in range(width):
            index = (y * width + x) * 4
            pixels[index] = (row_shade * tint[0]) // 255
            pixels[index + 1] = (row_shade * tint[1]) // 255
            pixels[index + 2] = (row_shade * tint[2]) // 255
            pixels[index + 3] = 255
    return pixels


def frame_pixels(size, frame):
    pixels = bytearray(size * size * 4)
    angle = 2 * math.pi * frame / FRAME_COUNT
    center_x = size / 2 + size / 4 * math.cos(angle)
    center_y = size / 2 + size / 4 * math.sin(angle)
    radius = size / 6
    for y in range(size):
        for x in range(size):
            if (x - center_x) ** 2 + (y - center_y) ** 2 <= radius * radius:
                index = (y * size + x) * 4
                pixels[index] = 255
                pixels[index + 1] = 255 - frame * 30
                pixels[index + 2] = frame * 30
                pixels[index + 3] = 255
    return pixels


def write_binary(path, data):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "wb") as output:
        output.write(data)


def write_text(path, text):
    write_binary(path, text.encode("ascii"))


def animation_text(name, frame_count):
    lines = ["animation " + name, "ticks_per_second 30", "loops 1"]
    for frame in range(frame_count):
        lines.append("frame %s_%02d 2 700 300 512 512" % (name, frame))
    return "\n".join(lines) + "\n"


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("output_folder")
    arguments = parser.parse_args()
    folder = arguments.output_folder

    backgrounds = [("test_room_blue", (120, 170, 255)), ("test_room_green", (130, 255, 170))]
    manifest = []
    for name, tint in backgrounds:
        pixels = background_pixels(BACKGROUND_WIDTH, BACKGROUND_HEIGHT, tint)
        write_binary(os.path.join(folder, "textures", name + ".gtex"), texture_file_bytes(BACKGROUND_WIDTH, BACKGROUND_HEIGHT, pixels))
        manifest.append("background " + name)

    animation_name = "test_orbit"
    for frame in range(FRAME_COUNT):
        pixels = frame_pixels(FRAME_SIZE, frame)
        texture_name = "%s_%02d" % (animation_name, frame)
        write_binary(os.path.join(folder, "textures", texture_name + ".gtex"), texture_file_bytes(FRAME_SIZE, FRAME_SIZE, pixels))
    write_text(os.path.join(folder, "animations", animation_name + ".anim"), animation_text(animation_name, FRAME_COUNT))
    manifest.append("animation " + animation_name)

    write_text(os.path.join(folder, "manifest.txt"), "\n".join(manifest) + "\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
