import struct

FORMAT_RGBA8888 = 0
FORMAT_DXT5 = 1
FORMAT_DXT1 = 2
BLOCK_BYTES = {FORMAT_DXT5: 16, FORMAT_DXT1: 8}
NAME_MAX_LENGTH = 95
ANIMATION_FRAME_MAX = 512
SCENE_STEP_MAX = 1024
BLANK_TEXTURE = "-"
TICKS_PER_SECOND = 60
ALLOWED_NAME_CHARACTERS = set("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-")


def check_name(name):
    if len(name) == 0 or len(name) > NAME_MAX_LENGTH:
        raise ValueError("bad name length: " + name)
    for character in name:
        if character not in ALLOWED_NAME_CHARACTERS:
            raise ValueError("bad character in name: " + name)


def format_number(value):
    text = "%.6f" % value
    text = text.rstrip("0").rstrip(".")
    if text == "-0" or text == "":
        return "0"
    return text


def expected_pixel_data_size(width, height, texture_format):
    if texture_format == FORMAT_RGBA8888:
        return width * height * 4
    if width % 4 != 0 or height % 4 != 0:
        raise ValueError("compressed textures need sides that are multiples of 4")
    return (width // 4) * (height // 4) * BLOCK_BYTES[texture_format]


def round_up_to_power_of_two(value):
    rounded = 1
    while rounded < value:
        rounded = rounded * 2
    return rounded


def memory_bytes(width, height, texture_format):
    if texture_format == FORMAT_RGBA8888:
        return width * height * 4
    return expected_pixel_data_size(round_up_to_power_of_two(width), round_up_to_power_of_two(height), texture_format)


def gtex_bytes(width, height, texture_format, pixel_data):
    if len(pixel_data) != expected_pixel_data_size(width, height, texture_format):
        raise ValueError("pixel data has the wrong size")
    header = b"GTEX" + struct.pack("<IIII", width, height, texture_format, len(pixel_data))
    return header + bytes(pixel_data)


def blank_frame(ticks):
    return {"texture": BLANK_TEXTURE, "ticks": ticks, "x": 0, "y": 0, "width": 0, "height": 0, "u": 0, "v": 0, "u_size": 0, "v_size": 0}


def anim_text(name, loops, frames):
    check_name(name)
    if len(frames) == 0 or len(frames) > ANIMATION_FRAME_MAX:
        raise ValueError("bad frame count in animation " + name)
    lines = ["animation " + name, "ticks_per_second " + str(TICKS_PER_SECOND), "loops " + str(loops)]
    for frame in frames:
        if frame["ticks"] <= 0:
            raise ValueError("frame has no duration in animation " + name)
        fields = [frame["texture"], str(frame["ticks"])]
        for key in ("x", "y", "width", "height", "u", "v", "u_size", "v_size"):
            fields.append(format_number(frame[key]))
        lines.append("frame " + " ".join(fields))
    return "\n".join(lines) + "\n"


def scene_step_line(step):
    fields = [step[0]]
    for value in step[1:]:
        if isinstance(value, str):
            check_name(value)
            fields.append(value)
        else:
            fields.append(format_number(value))
    return " ".join(fields)


def scene_text(name, steps):
    check_name(name)
    if len(steps) == 0 or len(steps) > SCENE_STEP_MAX:
        raise ValueError("bad step count in scene " + name)
    lines = ["scene " + name]
    for step in steps:
        lines.append(scene_step_line(step))
    return "\n".join(lines) + "\n"


def manifest_text(scene_names, background_names, animation_names):
    lines = []
    for name in scene_names:
        lines.append("scene " + name)
    for name in background_names:
        lines.append("background " + name)
    for name in animation_names:
        lines.append("animation " + name)
    return "\n".join(lines) + "\n"
