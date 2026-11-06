import math
import os
import subprocess

SUPERSAMPLE = 4
SIDE_PADDING = 4
LINE_BOX_ASCENT_EM = 1.0
LINE_BOX_DESCENT_EM = 0.3
CANVAS_SPARE_CHARACTERS = 4
RGBA_BYTES = 4


class LineRenderError(ValueError):
    pass


def round_up_to_multiple_of_four(value):
    return (int(math.ceil(value)) + 3) // 4 * 4


def line_box(pixels_per_em):
    baseline_row = int(math.ceil(LINE_BOX_ASCENT_EM * pixels_per_em))
    height = round_up_to_multiple_of_four(baseline_row + LINE_BOX_DESCENT_EM * pixels_per_em)
    return baseline_row, height


def run_magick(command):
    result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if result.returncode != 0:
        raise LineRenderError("magick could not draw a line with its font")
    return result.stdout


def annotate_arguments(font_path, text_path, pixels_per_em, canvas_width, canvas_height, baseline_row):
    return [
        "magick",
        "-size", "%dx%d" % (canvas_width, canvas_height),
        "xc:none",
        "-font", font_path,
        "-pointsize", "%g" % (pixels_per_em * SUPERSAMPLE),
        "-fill", "white",
        "-annotate", "+%d+%d" % (canvas_width // 4, baseline_row * SUPERSAMPLE),
        "@" + text_path,
    ]


def read_ink_box(arguments):
    output = run_magick(arguments + ["-format", "%@", "info:"]).decode("ascii")
    size, left, top = output.split("+")
    width, height = size.split("x")
    return int(left), int(top), int(width), int(height)


def set_every_color(pixels, color):
    pixel_count = len(pixels) // RGBA_BYTES
    for channel in range(3):
        pixels[channel::RGBA_BYTES] = bytes([color[channel]]) * pixel_count


def render_line(work_folder, name, font_path, text, pixels_per_em, max_ink_width, color):
    text_path = os.path.join(work_folder, name + ".txt")
    rgba_path = os.path.join(work_folder, name + "_line.rgba")
    with open(text_path, "w", encoding="utf-8") as output:
        output.write(text)
    baseline_row, height = line_box(pixels_per_em)
    canvas_width = (len(text) + CANVAS_SPARE_CHARACTERS) * 2 * int(math.ceil(pixels_per_em)) * SUPERSAMPLE
    canvas_height = height * SUPERSAMPLE
    arguments = annotate_arguments(font_path, text_path, pixels_per_em, canvas_width, canvas_height, baseline_row)
    ink_left, ink_top, ink_width, ink_height = read_ink_box(arguments)
    if ink_top <= 0 or ink_top + ink_height >= canvas_height:
        raise LineRenderError("ink leaves the line box in " + name)
    natural_width = ink_width / SUPERSAMPLE
    drawn_ink_width = min(natural_width, max_ink_width)
    texture_width = round_up_to_multiple_of_four(drawn_ink_width + 2 * SIDE_PADDING)
    source_width = int(round(texture_width * SUPERSAMPLE * natural_width / drawn_ink_width))
    source_left = ink_left - (source_width - ink_width) // 2
    run_magick(arguments + [
        "-crop", "%dx%d+%d+0" % (source_width, canvas_height, source_left),
        "+repage",
        "-filter", "Box",
        "-resize", "%dx%d!" % (texture_width, height),
        "-depth", "8",
        "rgba:" + rgba_path,
    ])
    with open(rgba_path, "rb") as source:
        pixels = bytearray(source.read())
    os.remove(rgba_path)
    os.remove(text_path)
    if len(pixels) != texture_width * height * RGBA_BYTES:
        raise LineRenderError("magick wrote the wrong image size for " + name)
    set_every_color(pixels, color)
    return {
        "width": texture_width,
        "height": height,
        "pixels": bytes(pixels),
        "natural_width": natural_width,
        "drawn_ink_width": drawn_ink_width,
        "baseline_row": baseline_row,
    }
