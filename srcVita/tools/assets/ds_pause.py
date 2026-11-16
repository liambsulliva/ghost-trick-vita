import numpy as np

import asset_writers
import bake_record
import ds_rom
import ds_sprites
import ds_text
import frame_layout
import pause_art

CODE_FILES = ["ds_pause.py", "ds_text.py", "ds_textures.py", "ds_sprites.py", "pause_art.py", "frame_layout.py", "asset_writers.py"]
TEXTURE_FOLDER = "textures"
INK_COLOR = 3
INK_ALPHAS = np.array([0, 102, 178, 255], dtype=np.uint8)
LINE_TOP = 10
LINE_ROWS = 16
LINE_PITCH = 13
TEXT_SCALE = 2
LABEL_SCALE = 3
NUMBER_SCALE = 1
PIXEL_STEP = 2
SOLID = 128
CODE_OF_CHARACTER = ds_text.CODE_OF_CHARACTER
BUTTON_CORNER = 8
BUTTON_MIDDLE = 8
BUTTON_SCALE = 2
TITLE_BUTTONS = [("title_button", "resting"), ("title_button_selected", "selected"), ("title_button_pressed", "pressed"), ("title_icon", "resting"), ("title_icon_selected", "selected"), ("title_icon_pressed", "pressed")]
FLAME_HEIGHT = 48
FLAME_NUMBER_MIDDLE = 0.7
FLAME_DARKENING = 0.35


class DsPauseError(ValueError):
    pass


def color_of(text):
    return tuple(int(text[place:place + 2], 16) for place in (1, 3, 5))


def items_of(text):
    missing = sorted(set(character for character in text if character not in CODE_OF_CHARACTER))
    if missing:
        raise DsPauseError("the DS font has no %s" % " ".join(repr(character) for character in missing))
    return [("char", CODE_OF_CHARACTER[character]) for character in text]


def ink_of_line(font, text):
    pixels, cursor, _, _ = ds_text.draw_items(font, items_of(text), INK_COLOR)
    left = min([0] + [x for x, _, _ in pixels])
    right = max([cursor] + [x + 1 for x, _, _ in pixels])
    ink = np.zeros((LINE_ROWS, max(right - left, 1)), dtype=np.uint8)
    for x, y, value in pixels:
        if 0 <= y - LINE_TOP < LINE_ROWS:
            ink[y - LINE_TOP, x - left] = value
    return ink


def ink_of_text(font, text):
    lines = [ink_of_line(font, line) for line in text.split("\n")]
    width = max(line.shape[1] for line in lines)
    ink = np.zeros((LINE_ROWS + (len(lines) - 1) * LINE_PITCH, width), dtype=np.uint8)
    for row, line in enumerate(lines):
        left = (width - line.shape[1]) // 2
        top = row * LINE_PITCH
        ink[top:top + LINE_ROWS, left:left + line.shape[1]] = np.maximum(ink[top:top + LINE_ROWS, left:left + line.shape[1]], line)
    if len(lines) > 1:
        rows = np.flatnonzero(ink.any(axis=1))
        if rows.size:
            ink = ink[rows[0]:rows[-1] + 1]
    return ink


def enlarged(pixels, scale):
    return np.repeat(np.repeat(pixels, scale, axis=0), scale, axis=1)


def text_pixels(font, text, color, scale):
    if not text.strip():
        return np.zeros((1, 1, 4), dtype=np.uint8)
    ink = ink_of_text(font, text)
    pixels = np.zeros(ink.shape + (4,), dtype=np.uint8)
    pixels[..., :3] = color_of(color)
    pixels[..., 3] = INK_ALPHAS[ink]
    return enlarged(pixels, scale)


def grown(mask):
    padded = np.pad(mask, 1)
    result = np.zeros_like(mask)
    for dy in range(3):
        for dx in range(3):
            result = result | padded[dy:dy + mask.shape[0], dx:dx + mask.shape[1]]
    return result


def outlined_text_pixels(font, text, color, outline_color, scale):
    ink = np.pad(ink_of_text(font, text), 1)
    cover = INK_ALPHAS[ink].astype(np.uint16)[..., None]
    fill = np.array(color_of(color), dtype=np.uint16)
    edge = np.array(color_of(outline_color), dtype=np.uint16)
    pixels = np.zeros(ink.shape + (4,), dtype=np.uint8)
    pixels[..., :3] = (fill * cover + edge * (255 - cover)) // 255
    rim = grown(ink > 0)
    pixels[..., 3] = np.where(rim, 255, 0)
    pixels[~rim] = 0
    return enlarged(pixels, scale)


def pixelated(pixels):
    height, width = pixels.shape[:2]
    small = pixels[PIXEL_STEP // 2::PIXEL_STEP, PIXEL_STEP // 2::PIXEL_STEP].copy()
    small[..., 3] = np.where(small[..., 3] >= SOLID, 255, 0)
    large = enlarged(small, PIXEL_STEP)
    large = np.pad(large, ((0, max(height - large.shape[0], 0)), (0, max(width - large.shape[1], 0)), (0, 0)), mode="edge")
    return np.ascontiguousarray(large[:height, :width])


def texture_pixels(pack, index, language):
    width, height, rgba = pack.texture(index, language)
    return np.frombuffer(rgba, dtype=np.uint8).reshape(height, width, 4)


def ink_box(pixels):
    rows = np.flatnonzero(pixels[..., 3].any(axis=1))
    columns = np.flatnonzero(pixels[..., 3].any(axis=0))
    if rows.size == 0:
        raise DsPauseError("a DS texture for the menus is wholly clear")
    return pixels[rows[0]:rows[-1] + 1, columns[0]:columns[-1] + 1]


def button_pixels(pixels):
    button = ink_box(pixels)
    if button.shape[1] < 2 * BUTTON_CORNER + BUTTON_MIDDLE or button.shape[0] < 2 * BUTTON_CORNER:
        raise DsPauseError("a DS button is smaller than its corners")
    middle = button.shape[1] // 2
    columns = np.concatenate([button[:, :BUTTON_CORNER], button[:, middle:middle + BUTTON_MIDDLE], button[:, -BUTTON_CORNER:]], axis=1)
    return enlarged(columns, BUTTON_SCALE)


def fitted(pixels, most_width, most_height):
    source = ink_box(pixels)
    height = min(most_height, max(source.shape[0] * most_width // source.shape[1], 1))
    width = min(most_width, max(source.shape[1] * height // source.shape[0], 1))
    rows = np.arange(height) * source.shape[0] // height
    columns = np.arange(width) * source.shape[1] // width
    fitted_pixels = source[rows][:, columns].copy()
    fitted_pixels[..., 3] = np.where(fitted_pixels[..., 3] >= SOLID, 255, 0)
    return fitted_pixels


def darkened(pixels):
    result = pixels.copy()
    grey = pixels[..., :3].astype(np.float32).mean(axis=2) * FLAME_DARKENING
    result[..., :3] = grey[..., None].astype(np.uint8)
    return result


def pasted_over(base, top, left, upper):
    region = base[upper:upper + top.shape[0], left:left + top.shape[1]]
    cover = top[..., 3:4].astype(np.uint16)
    region[..., :3] = (top[..., :3].astype(np.uint16) * cover + region[..., :3].astype(np.uint16) * (255 - cover)) // 255
    region[..., 3] = np.maximum(region[..., 3], top[..., 3])


def flame_pixels(font, flame, number, lit):
    cell = np.zeros((pause_art.FLAME_CELL[1], pause_art.FLAME_CELL[0], 4), dtype=np.uint8)
    body = flame if lit else darkened(flame)
    left = (cell.shape[1] - body.shape[1]) // 2
    upper = (cell.shape[0] - body.shape[0]) // 2
    cell[upper:upper + body.shape[0], left:left + body.shape[1]] = body
    digits = ink_box(text_pixels(font, str(number), pause_art.NAVY if lit else pause_art.NUMBER_DARK, NUMBER_SCALE))
    digit_left = (cell.shape[1] - digits.shape[1]) // 2
    digit_upper = int(cell.shape[0] * FLAME_NUMBER_MIDDLE) - digits.shape[0] // 2
    pasted_over(cell, digits, digit_left, digit_upper)
    return cell


def text_cells(font, widgets, language):
    wanted = []
    for style, color in (("row", pause_art.WHITE), ("header", pause_art.HEADER_ORANGE), ("bar", pause_art.WHITE)):
        wanted += [(style + "." + key, text, color) for key, text in widgets[style].items()]
    wanted += [("row.chapter_%d" % number, widgets["chapter_format"] % number, pause_art.WHITE) for number in range(1, widgets["chapter_count"] + 1)]
    wanted += [("title." + key, text, pause_art.WHITE) for key, text in widgets.get("title", {}).items()]
    wanted += [("language." + entry["key"], entry["text"], pause_art.WHITE) for entry in widgets["languages"]]
    cells = []
    refused = []
    for name, text, color in wanted:
        try:
            cells.append(("text", name, text_pixels(font, text, color, TEXT_SCALE)))
        except DsPauseError as error:
            refused.append("%s (%s)" % (name, error))
    return cells, refused


def widget_cells(font, pack, language, work_folder, widgets, art):
    cells, refused = text_cells(font, widgets, language)
    for name, width, height, radius, fill, stroke, stroke_width in pause_art.SLICES:
        cells.append(("slice", "%s %d" % (name, radius), pixelated(pause_art.slice_pixels(work_folder, width, height, radius, fill, stroke, stroke_width))))
    for name, state in TITLE_BUTTONS:
        cells.append(("slice", "%s %d" % (name, BUTTON_CORNER * BUTTON_SCALE), button_pixels(texture_pixels(pack, art["title_buttons"][state], language))))
    flame = fitted(texture_pixels(pack, art["flame"], language), pause_art.FLAME_CELL[0], FLAME_HEIGHT)
    for number in range(1, pause_art.METER_STEPS + 1):
        for lit in (0, 1):
            cells.append(("flame", "%d %d" % (number, lit), flame_pixels(font, flame, number, lit)))
    cells.append(("cell", "arrow_down", pixelated(pause_art.arrow_down_pixels(work_folder))))
    cells.append(("cell", "trash", pixelated(pause_art.trash_pixels(work_folder))))
    return cells, refused


def label_sheet(font, work_folder, words):
    cell_width, cell_height = pause_art.LABEL_CELL
    arrow = pause_art.ARROW_CELL
    width = asset_writers.round_up_to_power_of_two(2 * cell_width)
    height = asset_writers.round_up_to_power_of_two(len(words) * cell_height + arrow)
    sheet = np.zeros((height, width, 4), dtype=np.uint8)
    lines = []
    for row, word in enumerate(words):
        for column, fill in enumerate((pause_art.CREAM, pause_art.GHOST_BLUE)):
            pixels = outlined_text_pixels(font, word, fill, pause_art.NAVY, LABEL_SCALE)[:cell_height, :cell_width]
            sheet[row * cell_height:row * cell_height + pixels.shape[0], column * cell_width:column * cell_width + pixels.shape[1]] = pixels
        lines.append("label %d %d %d %d %d" % (row, 0, row * cell_height, pixels.shape[1], pixels.shape[0]))
    arrow_y = len(words) * cell_height
    arrows = pixelated(pause_art.magick_rgba(work_folder, 2 * arrow, arrow, pause_art.arrow_arguments(0, 0, True) + pause_art.arrow_arguments(arrow, 0, False)))
    sheet[arrow_y:arrow_y + arrow, :2 * arrow] = arrows
    lines.append("arrows %d %d %d" % (0, arrow_y, arrow))
    lines.append("selected_column %d" % cell_width)
    return sheet, lines


def font_bytes(font):
    return b"".join((font.glyphs, font.widths, font.profiles))


def stored_sheet(record, key, name, table_path, build):
    relative_path = TEXTURE_FOLDER + "/" + name + ".gtex"
    gtex = record.read_if_current(relative_path, key)
    table = record.read_if_current(table_path, key)
    refused = []
    if gtex is None or table is None:
        sheet, lines, refused = build()
        gtex = asset_writers.gtex_bytes(sheet.shape[1], sheet.shape[0], asset_writers.FORMAT_RGBA8888, np.ascontiguousarray(sheet).tobytes())
        table = ("\n".join(lines) + "\n").encode("utf-8")
        record.store(relative_path, key, gtex)
        record.store(table_path, key, table)
    return relative_path, asset_writers.read_gtex_header(gtex)[:2], table, refused


def cat_animations(wanted, animation_texts):
    missing = [entry["name"] for entry in wanted if pause_art.TABLE_PREFIX + entry["name"] not in animation_texts]
    if missing:
        raise DsPauseError("the DS set has no animation %s" % ", ".join(missing))
    return [{"name": pause_art.TABLE_PREFIX + entry["name"], "mirror": entry["mirror"], "walk": entry.get("walk", False), "text": animation_texts[pause_art.TABLE_PREFIX + entry["name"]]} for entry in wanted]


def art_sized_page(sprite_pages, texture):
    width, height, rgba = sprite_pages.pixels(texture)
    return enlarged(np.frombuffer(rgba, dtype=np.uint8).reshape(height, width, 4), frame_layout.ART_PER_DS_PIXEL)


def bake_cat(pause, record, code, work_folder, animation_texts, sprite_pages):
    animations = cat_animations(pause["cat_animations"], animation_texts)
    textures = sorted(set(frame["texture"] for animation in animations for frame in pause_art.frames_from_text(animation["text"])))
    key = record.key_for(code, {"animations": animations, "fidget": pause["cat_fidget"]}, b"".join(sprite_pages.source(texture) for texture in textures))
    table, results = pause_art.bake_cat_pages(work_folder, record, key, animations, lambda animation, texture: art_sized_page(sprite_pages, texture), pause_art.GRID_FILTER)
    files = {pause_art.CAT_TABLE_PATH: table, pause_art.CAT_TRACK_PATH: pause_art.cat_track_text(table, animations, pause["cat_fidget"])}
    names = [result["name"] for result in results]
    return files, [TEXTURE_FOLDER + "/" + name + ".gtex" for name in names], {name: (pause_art.PAGE_SIZE, pause_art.PAGE_SIZE) for name in names}


def bake(pause, art, pack, rom_path, language, record, work_folder, animation_texts, sprite_pages):
    with ds_rom.DsRom(rom_path) as rom:
        font = ds_text.font_from_rom(rom, ds_text.MAIN_FONT)
    code = bake_record.code_fingerprint(CODE_FILES)
    source = font_bytes(font) + b"".join(pack.texture_bytes(index, language) for index in sorted(set(list(art["title_buttons"].values()) + [art["flame"]])))
    widgets = pause["widgets"]

    def build_widgets():
        cells, refused = widget_cells(font, pack, language, work_folder, widgets, art)
        sheet, lines = pause_art.pack_widget_cells(cells)
        return sheet, lines, refused

    def build_labels():
        sheet, lines = label_sheet(font, work_folder, pause["labels"])
        return sheet, lines, []

    widget_path, widget_size, widget_table, refused = stored_sheet(record, record.key_for(code, {"widgets": widgets, "art": art, "language": language}, source), pause_art.WIDGET_SHEET_NAME, pause_art.WIDGET_TABLE_PATH, build_widgets)
    label_path, label_size, label_table, _ = stored_sheet(record, record.key_for(code, {"labels": pause["labels"]}, font_bytes(font)), pause_art.LABEL_SHEET_NAME, pause_art.LABEL_TABLE_PATH, build_labels)
    if refused:
        print("ds menu texts left out: %d (%s)" % (len(refused), ", ".join(refused[:12])))
    files = {pause_art.WIDGET_TABLE_PATH: widget_table, pause_art.LABEL_TABLE_PATH: label_table}
    sizes = {pause_art.WIDGET_SHEET_NAME: widget_size, pause_art.LABEL_SHEET_NAME: label_size}
    paths = [widget_path, label_path]
    try:
        cat_files, cat_paths, cat_sizes = bake_cat(pause, record, code, work_folder, animation_texts, sprite_pages)
        files.update(cat_files)
        paths = paths + cat_paths
        sizes.update(cat_sizes)
    except (DsPauseError, ds_sprites.SpriteRefused) as error:
        print("ds menu cat left out (%s)" % error)
    return files, paths, sizes
