import argparse
import os
import sys
import tempfile

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "assets"))

import asset_writers
import texture_encoder

ROOM_WIDTH = 720
ROOM_HEIGHT = 540
ATLAS_SIZE = 512
CELL_SIZE = 256
CELL_COLUMNS = 2
CELL_COUNT = 4
BAR_WIDTH = 120
BAR_HEIGHT = 544
MARKER_SIZE = 90


def set_pixel(pixels, width, x, y, color):
    index = (y * width + x) * 4
    pixels[index] = color[0]
    pixels[index + 1] = color[1]
    pixels[index + 2] = color[2]
    pixels[index + 3] = color[3]


def fill_rectangle(pixels, width, left, top, right, bottom, color):
    for y in range(top, bottom):
        for x in range(left, right):
            set_pixel(pixels, width, x, y, color)


def room_pixels(tint):
    pixels = bytearray(ROOM_WIDTH * ROOM_HEIGHT * 4)
    for y in range(ROOM_HEIGHT):
        shade = 40 + 120 * y // ROOM_HEIGHT
        color = ((shade * tint[0]) // 255, (shade * tint[1]) // 255, (shade * tint[2]) // 255, 255)
        for x in range(ROOM_WIDTH):
            set_pixel(pixels, ROOM_WIDTH, x, y, color)
    fill_rectangle(pixels, ROOM_WIDTH, 0, 0, MARKER_SIZE, MARKER_SIZE, (255, 0, 0, 255))
    fill_rectangle(pixels, ROOM_WIDTH, ROOM_WIDTH - MARKER_SIZE, 0, ROOM_WIDTH, MARKER_SIZE, (0, 255, 0, 255))
    fill_rectangle(pixels, ROOM_WIDTH, 0, ROOM_HEIGHT - MARKER_SIZE, MARKER_SIZE, ROOM_HEIGHT, (0, 0, 255, 255))
    fill_rectangle(pixels, ROOM_WIDTH, ROOM_WIDTH - MARKER_SIZE, ROOM_HEIGHT - MARKER_SIZE, ROOM_WIDTH, ROOM_HEIGHT, (255, 255, 255, 255))
    return pixels


def atlas_pixels():
    pixels = bytearray(ATLAS_SIZE * ATLAS_SIZE * 4)
    colors = [(255, 80, 80, 255), (80, 255, 80, 255), (80, 80, 255, 255), (255, 255, 80, 255)]
    for cell in range(CELL_COUNT):
        cell_left = (cell % CELL_COLUMNS) * CELL_SIZE
        cell_top = (cell // CELL_COLUMNS) * CELL_SIZE
        center_x = cell_left + CELL_SIZE // 2
        center_y = cell_top + CELL_SIZE // 2
        radius = 40 + cell * 20
        for y in range(cell_top, cell_top + CELL_SIZE):
            for x in range(cell_left, cell_left + CELL_SIZE):
                if (x - center_x) ** 2 + (y - center_y) ** 2 <= radius * radius:
                    set_pixel(pixels, ATLAS_SIZE, x, y, colors[cell])
                else:
                    set_pixel(pixels, ATLAS_SIZE, x, y, (colors[cell][0], colors[cell][1], colors[cell][2], 0))
    return pixels


def bar_pixels(color):
    pixels = bytearray(BAR_WIDTH * BAR_HEIGHT * 4)
    fill_rectangle(pixels, BAR_WIDTH, 0, 0, BAR_WIDTH, BAR_HEIGHT, color)
    return pixels


def write_binary(path, data):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "wb") as output:
        output.write(data)


def write_text(path, text):
    write_binary(path, text.encode("ascii"))


def write_dxt_texture(folder, name, width, height, pixels, texture_format, compression):
    with tempfile.TemporaryDirectory() as work_folder:
        data = texture_encoder.encode_rgba(work_folder, name, width, height, pixels, compression)
    write_binary(os.path.join(folder, "textures", name + ".gtex"), asset_writers.gtex_bytes(width, height, texture_format, data))


def write_rgba_texture(folder, name, width, height, pixels):
    write_binary(os.path.join(folder, "textures", name + ".gtex"), asset_writers.gtex_bytes(width, height, asset_writers.FORMAT_RGBA8888, pixels))


def animation_text(name, texture_name):
    frames = []
    for cell in range(CELL_COUNT):
        frames.append({
            "texture": texture_name,
            "ticks": 30,
            "x": -45.5,
            "y": -91,
            "width": 91,
            "height": 91,
            "u": (cell % CELL_COLUMNS) / CELL_COLUMNS,
            "v": (cell // CELL_COLUMNS) / CELL_COLUMNS,
            "u_size": 0.5,
            "v_size": 0.5,
        })
    frames.append(asset_writers.blank_frame(30))
    return asset_writers.anim_text(name, 1, frames)


def scene_text():
    lines = [
        "scene test_opening",
        "side_bars test_left_bar test_right_bar",
        "card test_room_green",
        "wait 60",
        "hide_card",
        "room 1",
        "plate test_room_blue 0 0 256 192",
        "object orbit_right test_orbit 180 176 2 0",
        "object orbit_left test_orbit 76 176 1 1",
        "camera 0 0",
        "fade_out 30",
        "fade_in 30",
        "wait 6000",
    ]
    return "\n".join(lines) + "\n"


def main():
    parser = argparse.ArgumentParser(description="Writes a small asset set with drawn test images, for trying the app without game files.")
    parser.add_argument("output_folder", help="folder to write manifest.txt, textures, animations and scenes into")
    arguments = parser.parse_args()
    folder = arguments.output_folder

    write_dxt_texture(folder, "test_room_blue", ROOM_WIDTH, ROOM_HEIGHT, room_pixels((120, 170, 255)), asset_writers.FORMAT_DXT1, "dxt1")
    write_dxt_texture(folder, "test_room_green", ROOM_WIDTH, ROOM_HEIGHT, room_pixels((130, 255, 170)), asset_writers.FORMAT_DXT1, "dxt1")
    write_dxt_texture(folder, "test_atlas", ATLAS_SIZE, ATLAS_SIZE, atlas_pixels(), asset_writers.FORMAT_DXT5, "dxt5")
    write_rgba_texture(folder, "test_left_bar", BAR_WIDTH, BAR_HEIGHT, bar_pixels((20, 60, 70, 255)))
    write_rgba_texture(folder, "test_right_bar", BAR_WIDTH, BAR_HEIGHT, bar_pixels((70, 60, 20, 255)))
    write_text(os.path.join(folder, "animations", "test_orbit.anim"), animation_text("test_orbit", "test_atlas"))
    write_text(os.path.join(folder, "scenes", "test_opening.scene"), scene_text())
    write_text(os.path.join(folder, "manifest.txt"), "scene test_opening\nbackground test_room_blue\nbackground test_room_green\nanimation test_orbit\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
