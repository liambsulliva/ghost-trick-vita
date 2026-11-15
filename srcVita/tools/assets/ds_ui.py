import json

import asset_writers
import bake_record
import ds_textures

TEXTURE_FOLDER = "textures"
CODE_FILES = ["ds_ui.py", "ds_textures.py", "ds_effects.py", "asset_writers.py"]
RGBA_BYTES = 4
FULL_ALPHA = 255


class DsUiError(ValueError):
    pass


def cropped(image, crop):
    width, height, rgba = image
    left, top, crop_width, crop_height = crop
    if left < 0 or top < 0 or crop_width <= 0 or crop_height <= 0 or left + crop_width > width or top + crop_height > height:
        raise DsUiError("a crop of %s does not fit a %dx%d DS texture" % (crop, width, height))
    rows = [rgba[((top + row) * width + left) * RGBA_BYTES:((top + row) * width + left + crop_width) * RGBA_BYTES] for row in range(crop_height)]
    return crop_width, crop_height, b"".join(rows)


def scaled(image, width, height, flip_x=False, flip_y=False):
    source_width, source_height, rgba = image
    columns = [(column * source_width) // width for column in range(width)]
    if flip_x:
        columns.reverse()
    rows = []
    known = {}
    for row in range(height):
        source_row = (row * source_height) // height
        if flip_y:
            source_row = source_height - 1 - source_row
        if source_row not in known:
            start = source_row * source_width * RGBA_BYTES
            known[source_row] = b"".join(rgba[start + column * RGBA_BYTES:start + (column + 1) * RGBA_BYTES] for column in columns)
        rows.append(known[source_row])
    return width, height, b"".join(rows)


def paste(canvas, canvas_width, canvas_height, image, x, y):
    width, height, rgba = image
    if x < 0 or y < 0 or x + width > canvas_width or y + height > canvas_height:
        raise DsUiError("a %dx%d part at %d,%d does not fit its %dx%d texture" % (width, height, x, y, canvas_width, canvas_height))
    for row in range(height):
        start = ((y + row) * canvas_width + x) * RGBA_BYTES
        canvas[start:start + width * RGBA_BYTES] = rgba[row * width * RGBA_BYTES:(row + 1) * width * RGBA_BYTES]


def trimmed(image):
    width, height, rgba = image
    columns = [column for column in range(width) if any(rgba[(row * width + column) * RGBA_BYTES + 3] for row in range(height))]
    rows = [row for row in range(height) if any(rgba[(row * width + column) * RGBA_BYTES + 3] for column in range(width))]
    if not columns:
        raise DsUiError("a DS texture to trim is wholly clear")
    return cropped(image, [columns[0], rows[0], columns[-1] - columns[0] + 1, rows[-1] - rows[0] + 1])


def cut_out(image, rule):
    width, height, rgba = image
    other_limit = rule["other_channels_above"]
    red_limit = rule["red_above"]
    kept = [rgba[place + 1] > other_limit or rgba[place + 2] > other_limit or rgba[place] > red_limit for place in range(0, width * height * RGBA_BYTES, RGBA_BYTES)]
    for _ in range(rule.get("grow", 0)):
        wider = list(kept)
        for row in range(height):
            for column in range(width):
                if kept[row * width + column]:
                    for near_row in range(max(row - 1, 0), min(row + 2, height)):
                        for near_column in range(max(column - 1, 0), min(column + 2, width)):
                            wider[near_row * width + near_column] = True
        kept = wider
    result = bytearray(rgba)
    for place, keep in enumerate(kept):
        if not keep:
            result[place * RGBA_BYTES:(place + 1) * RGBA_BYTES] = bytes(RGBA_BYTES)
    return width, height, bytes(result)


def main_color(image):
    width, height, rgba = image
    counts = {}
    for place in range(0, width * height * RGBA_BYTES, RGBA_BYTES):
        if rgba[place + 3] == FULL_ALPHA:
            color = rgba[place:place + 3]
            counts[color] = counts.get(color, 0) + 1
    if not counts:
        raise DsUiError("a DS texture to take a colour from has no opaque pixel")
    return max(counts, key=lambda color: (counts[color], color))


def flat_colored(image, color):
    width, height, rgba = image
    result = bytearray(rgba)
    for place in range(0, width * height * RGBA_BYTES, RGBA_BYTES):
        result[place:place + 3] = color
    return width, height, bytes(result)


def place_in_cell(image, cell, align):
    cell_x, cell_y, cell_width, cell_height = cell
    if image[0] > cell_width or image[1] > cell_height:
        raise DsUiError("a %dx%d part does not fit its %dx%d cell" % (image[0], image[1], cell_width, cell_height))
    return cell_x + int((cell_width - image[0]) * align[0]), cell_y + int((cell_height - image[1]) * align[1])


def nine_slices(part):
    left, top, width, height = part["crop"]
    corner = part["corner"]
    target_corner = part["target_corner"]
    source_columns = [(left, corner), (left + corner, width - 2 * corner), (left + width - corner, corner)]
    source_rows = [(top, corner), (top + corner, height - 2 * corner), (top + height - corner, corner)]
    target_columns = [(part["x"], target_corner), (part["x"] + target_corner, part["width"] - 2 * target_corner), (part["x"] + part["width"] - target_corner, target_corner)]
    target_rows = [(part["y"], target_corner), (part["y"] + target_corner, part["height"] - 2 * target_corner), (part["y"] + part["height"] - target_corner, target_corner)]
    slices = []
    for (source_y, source_height), (target_y, target_height) in zip(source_rows, target_rows):
        for (source_x, source_width), (target_x, target_width) in zip(source_columns, target_columns):
            slices.append(dict(part, crop=[source_x, source_y, source_width, source_height], x=target_x, y=target_y, width=target_width, height=target_height, corner=None))
    return slices


def plain_parts(parts):
    plain = []
    for part in parts:
        if part.get("corner"):
            plain.extend(nine_slices(part))
        else:
            plain.append(part)
    return plain


def compose(description, texture_of):
    parts = plain_parts(description["parts"])
    width = description["width"]
    height = description["height"]
    canvas = bytearray(width * height * RGBA_BYTES)
    if "fill" in description:
        canvas = bytearray(bytes(description["fill"]) + bytes((FULL_ALPHA,))) * (width * height)
    for part in parts:
        image = texture_of(part["texture"], part.get("opaque", False))
        if "crop" in part:
            image = cropped(image, part["crop"])
        if "cut_out" in part:
            image = cut_out(image, part["cut_out"])
        if "flat_color_of" in part:
            image = flat_colored(image, main_color(texture_of(part["flat_color_of"], False)))
        if "cell" in part:
            image = trimmed(image)
            x, y = place_in_cell(image, part["cell"], part.get("align", [0.5, 0.5]))
            paste(canvas, width, height, image, x, y)
            continue
        target_width = part.get("width", image[0])
        target_height = part.get("height", image[1])
        if (target_width, target_height) != image[:2] or part.get("flip_x") or part.get("flip_y"):
            image = scaled(image, target_width, target_height, part.get("flip_x", False), part.get("flip_y", False))
        paste(canvas, width, height, image, part.get("x", 0), part.get("y", 0))
    return width, height, bytes(canvas)


def source_of(description, pack, language):
    sources = []
    for part in description["parts"]:
        for index in [part["texture"]] + ([part["flat_color_of"]] if "flat_color_of" in part else []):
            stored = pack.texture_bytes(index, language)
            sources.append(len(stored).to_bytes(4, "little") + stored)
    return b"".join(sources)


def bake(descriptions, pack, language, record):
    code = bake_record.code_fingerprint(CODE_FILES)
    paths = []
    sizes = {}
    refused = []
    for description in descriptions:
        name = description["name"]
        asset_writers.check_name(name)
        relative_path = TEXTURE_FOLDER + "/" + name + ".gtex"
        try:
            key = record.key_for(code, {"description": json.dumps(description, sort_keys=True), "language": language}, source_of(description, pack, language))
            gtex_bytes = record.read_if_current(relative_path, key)
            if gtex_bytes is None:
                width, height, rgba = compose(description, lambda index, opaque: pack.texture(index, language, not opaque))
                gtex_bytes = asset_writers.gtex_bytes(width, height, asset_writers.FORMAT_RGBA8888, rgba)
                record.store(relative_path, key, gtex_bytes)
        except (ds_textures.TextureRefused, DsUiError) as error:
            refused.append("%s (%s)" % (name, error))
            continue
        paths.append(relative_path)
        sizes[name] = asset_writers.read_gtex_header(gtex_bytes)[:2]
    if refused:
        print("ds interface textures left out: %d (%s)" % (len(refused), ", ".join(refused[:12])))
    return paths, sizes
