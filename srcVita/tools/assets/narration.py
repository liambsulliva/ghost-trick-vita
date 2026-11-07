import os

import asset_writers
import bake_record
import font_file
import frame_layout
import line_renderer
import message_file
import texture_encoder

SCREEN_WIDTH = 960
SCREEN_CENTER_X = SCREEN_WIDTH / 2
GAME_PICTURE_TOP_ON_SCREEN = 2
CLIP_PIXELS_PER_DS_PIXEL = 3.75
SCREEN_PIXELS_PER_CLIP_PIXEL = frame_layout.SCREEN_PIXELS_PER_DS_PIXEL / CLIP_PIXELS_PER_DS_PIXEL
CLIP_SIZED_PIXELS_PER_EM = 38.5
NARRATION_PIXELS_PER_EM = 29.8
BUBBLE_CHARACTERS_PER_LINE = 30
NARRATION_SCALE_FROM_CLIP_SIZE = NARRATION_PIXELS_PER_EM / CLIP_SIZED_PIXELS_PER_EM
NARRATION_MAX_INK_WIDTH = SCREEN_WIDTH * 2 / 3
NARRATION_COLOR = (255, 255, 255)
LINE_PITCH_IN_CLIP = 67
DEFAULT_POS_Y = 16
PARAGRAPH_LINE_HEIGHT_IN_CLIP = 79
FIRST_LINE_TOP_BASE_IN_CLIP = 8.0
FIRST_LINE_TOP_PER_POS_Y_IN_CLIP = 3.77
TALLEST_INK_ABOVE_BASELINE_EM = 0.833
RUN_THE_KEY_TOOL = "run python3 -I srcVita/tools/assets/extract_text_key.py with your own copy of the Android package"


class NarrationError(ValueError):
    pass


def set_uses_text(set_description):
    return bool(set_description.get("chapter") or set_description.get("narration") or set_description.get("bubble_text"))


def load_text_key(set_description, apk_folder):
    if not set_uses_text(set_description):
        return None
    path = os.path.join(apk_folder, message_file.TEXT_KEY_FILE_NAME)
    try:
        with open(path, "rb") as source:
            key = source.read(message_file.TEXT_KEY_SIZE + 1)
    except FileNotFoundError:
        raise NarrationError("no text key at %s: %s" % (path, RUN_THE_KEY_TOOL)) from None
    except OSError as error:
        raise NarrationError("could not read %s: %s: %s" % (path, error.strerror, RUN_THE_KEY_TOOL)) from None
    if len(key) != message_file.TEXT_KEY_SIZE:
        raise NarrationError("%s is the wrong size: %s" % (path, RUN_THE_KEY_TOOL))
    return key


def first_line_ink_top(pos_y):
    return GAME_PICTURE_TOP_ON_SCREEN + (FIRST_LINE_TOP_BASE_IN_CLIP + FIRST_LINE_TOP_PER_POS_Y_IN_CLIP * pos_y) * SCREEN_PIXELS_PER_CLIP_PIXEL


def clip_sized_ink_tops(paragraphs, pos_y):
    tops = []
    paragraph_top = first_line_ink_top(pos_y)
    for paragraph in paragraphs:
        tops.append([paragraph_top + line_index * LINE_PITCH_IN_CLIP * SCREEN_PIXELS_PER_CLIP_PIXEL for line_index in range(len(paragraph))])
        paragraph_top = paragraph_top + (len(paragraph) * PARAGRAPH_LINE_HEIGHT_IN_CLIP + LINE_PITCH_IN_CLIP) * SCREEN_PIXELS_PER_CLIP_PIXEL
    return tops


def ink_tops_by_paragraph(paragraphs, pos_y):
    clip_sized_tops = clip_sized_ink_tops(paragraphs, pos_y)
    line_tops = [top for tops in clip_sized_tops for top in tops]
    if not line_tops:
        return clip_sized_tops
    message_middle = (line_tops[0] + line_tops[-1] + TALLEST_INK_ABOVE_BASELINE_EM * CLIP_SIZED_PIXELS_PER_EM) / 2
    shrunk_tops = [[message_middle + (top - message_middle) * NARRATION_SCALE_FROM_CLIP_SIZE for top in tops] for tops in clip_sized_tops]
    return shrunk_tops


def texture_top_for_ink_top(ink_top, baseline_row, pixels_per_em):
    return int(round(ink_top - baseline_row + TALLEST_INK_ABOVE_BASELINE_EM * pixels_per_em))


def read_requested_texts(packs, requests, text_key):
    entry_names_by_file = {}
    for request in requests:
        entry_names_by_file.setdefault(request["file"], []).append(request["entry"])
        entry_names_by_file[request["file"]].extend(request.get("later_entries", []))
    texts = {}
    for file_path, entry_names in entry_names_by_file.items():
        try:
            texts[file_path] = message_file.read_english_entries(packs.read(file_path), entry_names, text_key)
        except message_file.MissingEntryError as error:
            raise NarrationError("%s: %s" % (file_path, error)) from None
        except message_file.MessageFileError:
            raise NarrationError("the saved text key did not decode %s: %s" % (file_path, RUN_THE_KEY_TOOL)) from None
    return texts


class TextFonts:
    def __init__(self, packs, work_folder):
        self.packs = packs
        self.work_folder = work_folder
        self.pack_bytes_by_path = {}
        self.decoded_paths = {}

    def pack_bytes(self, font_path):
        if font_path not in self.pack_bytes_by_path:
            self.pack_bytes_by_path[font_path] = self.packs.read(font_path)
        return self.pack_bytes_by_path[font_path]

    def decoded_path(self, font_path):
        if font_path not in self.decoded_paths:
            decoded_path = os.path.join(self.work_folder, "font_%d.otf" % len(self.decoded_paths))
            with open(decoded_path, "wb") as output:
                output.write(font_file.decode_font(self.pack_bytes(font_path)))
            self.decoded_paths[font_path] = decoded_path
        return self.decoded_paths[font_path]


def encode_line(work_folder, name, line):
    blocks = texture_encoder.encode_rgba(work_folder, name, line["width"], line["height"], line["pixels"], "dxt5")
    return asset_writers.gtex_bytes(line["width"], line["height"], asset_writers.FORMAT_DXT5, blocks)


def bake_line(fonts, work_folder, record, code, name, font_path, text, pixels_per_em, max_ink_width, color):
    relative_path = "textures/" + name + ".gtex"
    settings = {"font": bake_record.sha256_of_bytes(fonts.pack_bytes(font_path)), "pixels_per_em": pixels_per_em, "max_ink_width": max_ink_width, "color": list(color)}
    key = record.key_for(code, settings, text.encode("utf-8"))
    natural_width = record.details_of(relative_path).get("natural_width")
    gtex_bytes = None if natural_width is None else record.read_if_current(relative_path, key)
    if gtex_bytes is not None:
        return bake_record.result_of_own_size(name, gtex_bytes), natural_width
    line = line_renderer.render_line(work_folder, name, fonts.decoded_path(font_path), text, pixels_per_em, max_ink_width, color)
    gtex_bytes = encode_line(work_folder, name, line)
    record.store(relative_path, key, gtex_bytes, {"natural_width": line["natural_width"]})
    return bake_record.result_of_own_size(name, gtex_bytes), line["natural_width"]


def line_texture_name(message_name, line_index):
    return "text_%s_%d" % (message_name, line_index)


def parse_requested_message(request, raw_text, ignores_other_tags=False):
    try:
        return message_file.parse_message(raw_text, ignores_other_tags)
    except message_file.MessageFileError as error:
        raise NarrationError(request["name"] + ": " + str(error)) from None


def read_narration_message(request, raw_text):
    message = parse_requested_message(request, raw_text)
    if message["pos_y"] is None:
        message["pos_y"] = DEFAULT_POS_Y
    return message


def read_narration_with_later_parts(request, texts):
    message = read_narration_message(request, texts[request["file"]][request["entry"]])
    message["later_parts"] = [read_narration_message(request, texts[request["file"]][entry]) for entry in request.get("later_entries", [])]
    return message


def parts_of_message(message):
    return [message] + message.get("later_parts", [])


def wrapped_bubble_lines(lines):
    wrapped = []
    for line in lines:
        current = ""
        for word in line.split():
            if current and len(current) + 1 + len(word) > BUBBLE_CHARACTERS_PER_LINE:
                wrapped.append(current)
                current = word
            else:
                current = (current + " " + word).strip()
        if current:
            wrapped.append(current)
    return wrapped


def stacked_lines(rendered):
    width = max(line["width"] for line in rendered)
    height = sum(line["height"] for line in rendered)
    pixels = bytearray(width * height * 4)
    top = 0
    for line in rendered:
        left = (width - line["width"]) // 2
        for row in range(line["height"]):
            start = (top + row) * width * 4 + left * 4
            pixels[start:start + line["width"] * 4] = line["pixels"][row * line["width"] * 4:(row + 1) * line["width"] * 4]
        top = top + line["height"]
    return {"width": width, "height": height, "pixels": bytes(pixels)}


def read_bubble_message(request, raw_text):
    message = parse_requested_message(request, raw_text, ignores_other_tags=True)
    all_lines = [text.strip() for paragraph in message["paragraphs"] for text in paragraph if text.strip()]
    message["lines"] = [" ".join(all_lines)] if request.get("single_line") else wrapped_bubble_lines(all_lines)
    if request.get("joins_lines"):
        message["paragraphs"] = [[" ".join(text.strip() for paragraph in message["paragraphs"] for text in paragraph if text.strip())]]
    if len(message["paragraphs"]) != 1 or len(message["paragraphs"][0]) != 1:
        raise NarrationError("bubble text is not one line: " + request["name"])
    return message


def can_be_read(reader, request, texts):
    try:
        reader(request, texts[request["file"]][request["entry"]])
    except NarrationError as error:
        print("text skipped: %s" % str(error).split(":")[0])
        return False
    return True


def literal_message(request):
    return {"paragraphs": [[request["literal"]]], "lines": [request["literal"]], "pos_y": None, "auto_pf": None, "mono_type": None, "waits": []}


def read_text_messages(packs, set_description, text_key):
    narration_requests = set_description.get("narration", [])
    bubble_requests = set_description.get("bubble_text", [])
    texts = read_requested_texts(packs, narration_requests + [request for request in bubble_requests if "literal" not in request], text_key)
    if set_description.get("chapter"):
        set_description["narration"] = [request for request in narration_requests if can_be_read(read_narration_message, request, texts)]
        set_description["bubble_text"] = [request for request in bubble_requests if "literal" in request or can_be_read(read_bubble_message, request, texts)]
        narration_requests = set_description["narration"]
        bubble_requests = set_description["bubble_text"]
    return {
        "narration": [read_narration_with_later_parts(request, texts) for request in narration_requests],
        "bubble_text": [literal_message(request) if "literal" in request else read_bubble_message(request, texts[request["file"]][request["entry"]]) for request in bubble_requests],
    }


def text_texture_names(set_description, messages):
    names = [request["name"] for request in set_description.get("bubble_text", [])]
    for request, message in zip(set_description.get("narration", []), messages["narration"]):
        line_count = sum(1 for part in parts_of_message(message) for paragraph in part["paragraphs"] for text in paragraph if text.strip())
        names.extend(line_texture_name(request["name"], line_index) for line_index in range(line_count))
    return names


def bake_narration_message(fonts, work_folder, record, code, font_path, request, message):
    paragraphs = []
    results = []
    failures = []
    line_index = 0
    laid_out = [pair for part in parts_of_message(message) for pair in zip(part["paragraphs"], ink_tops_by_paragraph(part["paragraphs"], part["pos_y"]))]
    for paragraph, ink_tops in laid_out:
        lines = []
        for text, ink_top in zip(paragraph, ink_tops):
            if not text.strip():
                continue
            name = line_texture_name(request["name"], line_index)
            line_index = line_index + 1
            try:
                result, natural_width = bake_line(fonts, work_folder, record, code, name, font_path, text, NARRATION_PIXELS_PER_EM, NARRATION_MAX_INK_WIDTH, NARRATION_COLOR)
            except Exception as error:
                failures.append((name, str(error)))
                continue
            results.append(result)
            baseline_row, _ = line_renderer.line_box(NARRATION_PIXELS_PER_EM)
            lines.append({
                "id": line_index,
                "texture": name,
                "top_y": texture_top_for_ink_top(ink_top, baseline_row, NARRATION_PIXELS_PER_EM),
                "natural_width": natural_width,
            })
        paragraphs.append(lines)
    return {"paragraphs": paragraphs, "pos_y": message["pos_y"], "mono_type": message["mono_type"], "auto_pf": message["auto_pf"]}, results, failures


def bake_bubble_text(fonts, work_folder, record, code, request, message):
    color = tuple(request["color"])
    lines = message.get("lines") or [message["paragraphs"][0][0]]
    if len(lines) == 1:
        result, _ = bake_line(fonts, work_folder, record, code, request["name"], request["font"], lines[0], request["pixels_per_em"], SCREEN_WIDTH, color)
        return result
    relative_path = "textures/" + request["name"] + ".gtex"
    settings = {"font": bake_record.sha256_of_bytes(fonts.pack_bytes(request["font"])), "pixels_per_em": request["pixels_per_em"], "color": list(color)}
    key = record.key_for(code, settings, "\n".join(lines).encode("utf-8"))
    gtex_bytes = record.read_if_current(relative_path, key)
    if gtex_bytes is None:
        rendered = [line_renderer.render_line(work_folder, "%s_%d" % (request["name"], index), fonts.decoded_path(request["font"]), line, request["pixels_per_em"], SCREEN_WIDTH, color) for index, line in enumerate(lines)]
        gtex_bytes = encode_line(work_folder, request["name"], stacked_lines(rendered))
        record.store(relative_path, key, gtex_bytes, {})
    return bake_record.result_of_own_size(request["name"], gtex_bytes)


def bake_text_textures(packs, set_description, messages, work_folder, record, code):
    fonts = TextFonts(packs, work_folder)
    narration = {}
    results = []
    failures = []
    for request, message in zip(set_description.get("narration", []), messages["narration"]):
        narration[request["name"]], message_results, message_failures = bake_narration_message(fonts, work_folder, record, code, set_description["narration_font"], request, message)
        results.extend(message_results)
        failures.extend(message_failures)
    for request, message in zip(set_description.get("bubble_text", []), messages["bubble_text"]):
        try:
            results.append(bake_bubble_text(fonts, work_folder, record, code, request, message))
        except Exception as error:
            failures.append((request["name"], str(error)))
    return narration, results, failures


def message_for_step(narration, name):
    if name not in narration:
        raise NarrationError("step names unknown narration: " + name)
    return narration[name]


def show_steps(narration, name, paragraph_index, fade_ticks):
    paragraphs = message_for_step(narration, name)["paragraphs"]
    if paragraph_index < 0 or paragraph_index >= len(paragraphs):
        raise NarrationError("step names a paragraph that does not exist in: " + name)
    return [["text_show", line["id"], line["texture"], SCREEN_CENTER_X, line["top_y"], fade_ticks] for line in paragraphs[paragraph_index]]


def hide_steps(narration, name, fade_ticks):
    steps = []
    for paragraph in message_for_step(narration, name)["paragraphs"]:
        for line in paragraph:
            steps.append(["text_hide", line["id"], fade_ticks])
    return steps


def overlay_step(texture_sizes, step):
    _, overlay_id, texture_name, x, y, grow_ticks = step
    if texture_name not in texture_sizes:
        raise NarrationError("overlay names an unknown texture: " + texture_name)
    width, height = texture_sizes[texture_name]
    return ["overlay", overlay_id, texture_name, x, y, width / frame_layout.SCREEN_PIXELS_PER_DS_PIXEL, height / frame_layout.SCREEN_PIXELS_PER_DS_PIXEL, grow_ticks]


def expand_text_steps(steps, narration, texture_sizes):
    expanded = []
    for step in steps:
        kind = step[0]
        if kind == "narration_show":
            expanded.extend(show_steps(narration, step[1], step[2], step[3]))
        elif kind == "narration_hide":
            expanded.extend(hide_steps(narration, step[1], step[2]))
        elif kind == "overlay":
            expanded.append(overlay_step(texture_sizes, step))
        else:
            expanded.append(step)
    return expanded


def print_narration_summary(narration):
    if not narration:
        return
    widths = []
    for name, message in narration.items():
        for paragraph in message["paragraphs"]:
            for line in paragraph:
                widths.append((line["natural_width"], line["texture"]))
    squeezed = ["%s %.1f px" % (texture, width) for width, texture in widths if width > NARRATION_MAX_INK_WIDTH]
    print("narration: %d messages, %d lines, widest %.1f px" % (len(narration), len(widths), max(widths)[0]))
    print("narration lines squeezed: %s" % (", ".join(squeezed) if squeezed else "none"))
