import collections
import os
import sys

import asset_writers
import bake_record
import ds_rom
import frame_layout
import ui_clips

ASSET_TOOLS_FOLDER = os.path.dirname(os.path.abspath(__file__))
SRC_VITA_FOLDER = os.path.dirname(os.path.dirname(ASSET_TOOLS_FOLDER))
sys.path.append(os.path.join(os.path.dirname(SRC_VITA_FOLDER), "tools"))

import lmg_decode
import lz
import message_cues

SCRIPTS_FOLDER = "scripts"
SCRIPT_EXTENSION = ".lmg"
XML_SUFFIX = ".xml"
ROM_SUFFIX = ".lz"
SCRIPT_CHANGE_OP = 0x24
ACT_OP = 0x19
ACT_END_LOW_OPERAND = 1
ACT_END_HIGH_OPERAND = 2
ACT_NAME_OPERAND = 3
SCRIPT_CODE_FILES = ["ds_rom.py", "message_cues.py", "script_assets.py", "ui_clips.py", "../../../tools/lz.py"]
CUES_EXTENSION = ".cues"
MOTIONS_EXTENSION = ".motions"
CHAIN_FILE = SCRIPTS_FOLDER + "/chain.txt"
PRELOAD_FILE = SCRIPTS_FOLDER + "/chain.preload"
ROOMS_FILE = SCRIPTS_FOLDER + "/rooms.txt"
AUDIO_FILE = SCRIPTS_FOLDER + "/audio_names.txt"
TEXT_LINES_FILE = SCRIPTS_FOLDER + "/text_lines.txt"
BALLOONS_FILE = SCRIPTS_FOLDER + "/balloons.txt"
SOUL_FILE = SCRIPTS_FOLDER + "/soul.txt"
CORE_LABELS_FILE = SCRIPTS_FOLDER + "/core_labels.txt"
ROUTE_FILE = SCRIPTS_FOLDER + "/route_steps.txt"
UI_CLIPS_FILE = SCRIPTS_FOLDER + "/ui_clips.txt"
EFFECTS_FILE = SCRIPTS_FOLDER + "/effects.txt"
CHAPTER_PRELOADED_MESSAGES = 6
CHAPTER_PRELOADED_ROOMS = 2
NARRATION_OP = 0x5d
BUBBLE_MARGIN_WIDTH = 30
TICKS_PER_SECOND = 60
BUBBLE_MARGIN_HEIGHT = 21
THORN_TEXT_FILL_WIDTH = 0.86
THORN_TEXT_FILL_HEIGHT = 0.8
MOTION_LABEL_SUFFIX = ".mtbl.xml"
FLAGS_SCRIPT = "flags.xml"
LANGUAGE_SUFFIX_FORMAT = ".%s.xml.lz"
SCREEN_CENTER_X = 480
NO_LABEL = "-"
NO_KIND = -1
WORD_FRAME_LIMIT = 0xfff0
TABLE_ANIMATION_PREFIX = "t_"


class ScriptBakeError(ValueError):
    pass


def script_name(path):
    flat = path.replace("/", "-")
    if flat.endswith(XML_SUFFIX):
        return flat[:-len(XML_SUFFIX)]
    return flat


def baked_path(path):
    name = script_name(path)
    try:
        asset_writers.check_name(name)
    except ValueError as error:
        raise ScriptBakeError(path + ": " + str(error)) from None
    return SCRIPTS_FOLDER + "/" + name + SCRIPT_EXTENSION


def read_one_script(rom, record, code, path):
    relative_path = baked_path(path)
    packed = rom.read_file(path + ROM_SUFFIX)
    key = record.key_for(code, {"path": path}, packed)
    current = record.read_if_current(relative_path, key)
    if current is not None:
        return current, None
    plain = lz.try_decompress(packed)
    if plain is None:
        raise ScriptBakeError(path + " in the DS ROM could not be decompressed")
    return plain, key


def store_new_scripts(record, scripts):
    for path, (plain, key) in scripts.items():
        if key is not None:
            record.store(baked_path(path), key, plain)


def language_path_of(path, language):
    stem = path[:-len(XML_SUFFIX)] if path.endswith(XML_SUFFIX) else path
    return stem + LANGUAGE_SUFFIX_FORMAT % language


def narration_labels(path, plain):
    strings, operations, _ = decoded_script_with_errors(path, plain)
    labels = []
    for _, _, op, operands in operations:
        if op == NARRATION_OP and strings.get(operands[0]) not in labels:
            labels.append(strings.get(operands[0]))
    return labels


def bake_one_cue_file(rom, record, code, section, path, plain):
    labels = narration_labels(path, plain)
    language_path = language_path_of(path, section.get("language", "en"))
    if not labels or rom.file_id_of(language_path) is None:
        return None
    relative_path = SCRIPTS_FOLDER + "/" + script_name(path) + CUES_EXTENSION
    packed = rom.read_file(language_path)
    key = record.key_for(code, {"path": language_path, "labels": labels}, packed)
    if record.read_if_current(relative_path, key) is None:
        language_plain = lz.try_decompress(packed)
        if language_plain is None:
            raise ScriptBakeError(language_path + " in the DS ROM could not be decompressed")
        known_labels = lmg_decode.read_labels(language_plain, lmg_decode.read_header(language_plain))
        known_names = set(name for names in known_labels.values() for name in names)
        try:
            text = message_cues.cue_text(language_plain, known_labels, [label for label in labels if label in known_names])
        except (ValueError, message_cues.CueError) as error:
            raise ScriptBakeError(language_path + ": " + str(error)) from None
        record.store(relative_path, key, text.encode("ascii"))
    return relative_path


def bake_cue_files(rom, record, code, section, scripts):
    relative_paths = []
    for path, (plain, _) in scripts.items():
        relative_path = bake_one_cue_file(rom, record, code, section, path, plain)
        if relative_path is not None:
            relative_paths.append(relative_path)
    return relative_paths


def decoded_script_with_errors(path, plain):
    try:
        _, strings, _, items, errors = lmg_decode.decode(plain)
    except ValueError as error:
        raise ScriptBakeError(path + ": " + str(error)) from None
    return strings, [item for item in items if item[1] == "op"], errors


def decoded_script(path, plain):
    strings, operations, errors = decoded_script_with_errors(path, plain)
    if errors:
        raise ScriptBakeError(path + ": " + errors[0])
    return strings, operations


def act_span(strings, operations, act_name):
    for position, _, op, operands in operations:
        if op == ACT_OP and strings.get(operands[ACT_NAME_OPERAND], "") == act_name:
            return position, operands[ACT_END_LOW_OPERAND] | (operands[ACT_END_HIGH_OPERAND] << 16)
    return None


def script_changes_nothing_explains(section, path, plain):
    strings, operations = decoded_script(path, plain)
    known = set(section["files"]) | set(section.get("never_taken", []))
    stop_span = act_span(strings, operations, section.get("stop_at_act"))
    unexplained = []
    for position, _, op, operands in operations:
        if op != SCRIPT_CHANGE_OP:
            continue
        target = strings.get(operands[0])
        stopped_first = stop_span is not None and stop_span[0] <= position < stop_span[1]
        if target not in known and not stopped_first:
            unexplained.append(path + " can change to " + str(target))
    return unexplained


def check_script_changes(section, baked):
    unexplained = []
    for path, plain in baked.items():
        unexplained.extend(script_changes_nothing_explains(section, path, plain))
    if unexplained:
        raise ScriptBakeError("; ".join(unexplained) + ", which the set's scripts section does not list")


def scripts_already_baked(output_folder):
    return [SCRIPTS_FOLDER + "/" + name for name in os.listdir(os.path.join(output_folder, SCRIPTS_FOLDER))]


def bake_or_keep_scripts(set_description, rom_path, output_folder, record):
    section = set_description.get("scripts")
    if section is None:
        return []
    if not os.path.isfile(rom_path):
        print("warning: no DS ROM at " + rom_path + ", so no scripts were baked", file=sys.stderr)
        return scripts_already_baked(output_folder)
    code = bake_record.code_fingerprint(SCRIPT_CODE_FILES)
    with ds_rom.DsRom(rom_path) as rom:
        scripts = {path: read_one_script(rom, record, code, path) for path in section["files"]}
        if "chapter" not in set_description:
            check_script_changes(section, {path: plain for path, (plain, _) in scripts.items()})
        store_new_scripts(record, scripts)
        cue_paths = bake_cue_files(rom, record, code, section, scripts)
    return [baked_path(path) for path in section["files"]] + cue_paths


def read_baked_script(output_folder, path):
    baked = os.path.join(output_folder, baked_path(path))
    if not os.path.isfile(baked):
        return None
    with open(baked, "rb") as source:
        return source.read()


def walk_table_frames(data, offset):
    frames = []
    position = offset
    while True:
        word = lmg_decode.read_u16(data, position)
        if word < WORD_FRAME_LIMIT:
            if word >> 10:
                frames.append(word & 0x3ff)
            position = position + 2
            continue
        tag = word - 0x10000
        if tag == -2 or tag == -1:
            return frames
        if tag not in lmg_decode.MOTION_LENGTHS:
            raise ScriptBakeError("a motion table holds the unknown word " + hex(word))
        position = position + 2 * lmg_decode.MOTION_LENGTHS[tag]


def table_animation_name(table_name, folder, names_differ_by_stage):
    if table_name.lower() in names_differ_by_stage:
        return TABLE_ANIMATION_PREFIX + table_name.lower() + "_" + folder.lower()
    return TABLE_ANIMATION_PREFIX + table_name.lower()


def folders_played_by_tables(motion_tables):
    return set(folder for tables in motion_tables.values() for folder in tables.values())


def table_names_with_several_folders(motion_tables):
    folders_of_name = collections.defaultdict(set)
    for tables in motion_tables.values():
        for table_name, folder in tables.items():
            folders_of_name[table_name.lower()].add(folder.lower())
    return set(name for name, folders in folders_of_name.items() if len(folders) > 1)


def table_offsets_of(data):
    labels = lmg_decode.read_labels(data, lmg_decode.read_header(data))
    offsets = {}
    for target, names in labels.items():
        for name in names:
            if name.endswith(MOTION_LABEL_SUFFIX):
                offsets[name[:-len(MOTION_LABEL_SUFFIX)]] = target
    return offsets


def timeline_of_hand_animation(set_description, timelines, animation_name):
    for animation in set_description.get("animations", []):
        if animation["name"] == animation_name:
            return timelines[animation["timeline"]]
    raise ScriptBakeError("a motion table names an unknown animation: " + animation_name)


def table_animations(set_description, output_folder, timelines, crops):
    section = set_description.get("scripts")
    if section is None or "motion_tables" not in section:
        return {}, {}
    animation_texts = {}
    motion_lines = {}
    tables_without_records = set()
    names_differ_by_stage = table_names_with_several_folders(section["motion_tables"])
    for path in section["files"]:
        plain = read_baked_script(output_folder, path)
        if plain is None:
            continue
        offsets = table_offsets_of(plain)
        lines = []
        for table_name, animation_name in section["motion_tables"].get(path, {}).items():
            if table_name not in offsets:
                continue
            timeline = timeline_of_hand_animation(set_description, timelines, animation_name)
            if timeline["records_per_ds_frame"] == 0:
                tables_without_records.add(table_name)
                continue
            ds_frames = walk_table_frames(plain, offsets[table_name])
            if not ds_frames:
                continue
            frame_count = len(timeline["records"]) // timeline["records_per_ds_frame"]
            ds_frames = [min(frame, frame_count - 1) for frame in ds_frames]
            frames, records_per_frame = frame_layout.frames_for_table(timeline, ds_frames, crops)
            name = table_animation_name(table_name, animation_name, names_differ_by_stage)
            animation_texts[name] = asset_writers.anim_text(name, 0, frames)
            lines.append("motion %x %s %d" % (offsets[table_name], name, records_per_frame))
        if lines:
            motion_lines[SCRIPTS_FOLDER + "/" + script_name(path) + MOTIONS_EXTENSION] = "\n".join(lines) + "\n"
    if tables_without_records:
        print("motion tables whose timeline has no records: %d (%s)" % (len(tables_without_records), ", ".join(sorted(tables_without_records)[:12])))
    return animation_texts, motion_lines


def chain_text(section):
    lines = ["timing " + section["timing"], "entry " + script_name(section["entry"])]
    if "chapter_mark" in section:
        lines.append("chapter_mark %d" % section["chapter_mark"])
    lines.extend("start_command %s %s" % (verb, arg) for verb, arg in section.get("start_commands", []))
    if "stop_at_act" in section:
        lines.insert(0, "stop " + section["stop_at_act"])
    if "next_chapter_card_texture" in section:
        lines.append("next_chapter_card " + section["next_chapter_card_texture"])
    lines.extend("file " + script_name(path) for path in section["files"] if path != FLAGS_SCRIPT)
    return "\n".join(lines) + "\n"


def soul_plate_names(set_description):
    return {plate["name"]: plate["soul_plate"] for plate in set_description.get("plates", []) if "soul_plate" in plate}


def ghost_plate_names(set_description):
    return {plate["name"]: plate.get("ghost_plates", {}) for plate in set_description.get("plates", [])}


def rooms_text(section, plate_sizes, soul_plates, ghost_plates=None):
    lines = ["room %s %d %d" % (room, size[0], size[1]) for room, size in section.get("room_sizes", {}).items()]
    for room, bands in section.get("room_bands", {}).items():
        lines.extend("band %s %d %d" % (room, key, value) for key, value in bands)
    for room, plates in section.get("room_plates", {}).items():
        for name, x, y in plates:
            width, height = plate_sizes[name]
            ghosts = (ghost_plates or {}).get(name, {})
            lines.append("plate %s %s %s %s %s %s %s %s %s" % (room, name, asset_writers.format_number(x), asset_writers.format_number(y), asset_writers.format_number(width), asset_writers.format_number(height), soul_plates.get(name, NO_LABEL), ghosts.get("missile", NO_LABEL), ghosts.get("boss", NO_LABEL)))
    return "\n".join(lines) + "\n"


def soul_text(soul_view):
    lines = ["owner " + soul_view["owner"], "ring " + soul_view["ring"]]
    lines.extend("core " + name for name in soul_view.get("cores", []))
    for class_id, style in soul_view.get("classes", {}).items():
        lines.append("class %s %s" % (class_id, " ".join(asset_writers.format_number(value) for value in style)))
    for object_name, style in soul_view.get("objects", {}).items():
        lines.append("object %s %s" % (object_name, " ".join(asset_writers.format_number(value) for value in style)))
    return "\n".join(lines) + "\n"


def movies_text(movies):
    return "".join("movie %s %d %d %s %d\n" % (movie["name"], movie.get("frames", 0), TICKS_PER_SECOND // movie["fps"], movie.get("class_id", "0"), movie.get("sound", -1)) for movie in movies)


ROUTE_FIELD_WIDTHS = {"script": 95, "object": 47, "wait": 159}


def check_route_field_widths(step):
    for field, width in ROUTE_FIELD_WIDTHS.items():
        if len(step.get(field, "")) > width:
            raise ValueError("route step %s %r is longer than the player reads (%d)" % (field, step[field], width))


def route_steps_text(route):
    lines = ["scene_most_ticks %d\n" % route["scene_most_ticks"]] if "scene_most_ticks" in route else []
    for step in route["steps"]:
        check_route_field_widths(step)
        lines.append("step %s %s %s %d %d %s\n" % (step["script"], step["action"], step.get("object", "-"), step.get("place", step.get("slot", 0)), route["dwell_ticks"][step.get("pace", "normal")], step.get("wait", "-").replace(" & ", "&")))
    return "".join(lines)


def flipbooks_text(flipbooks):
    return "".join("flipbook %s %d %d %d %d %s %s %s\n" % (book["name"], book["ticks_per_frame"], book["frames"], book["columns"], book["rows"], asset_writers.format_number(book["cell_width"]), asset_writers.format_number(book["cell_height"]), " ".join(book["pages"])) for book in flipbooks)


def core_labels_text(core_labels, texture_sizes):
    lines = []
    for object_name in core_labels["objects"]:
        name_texture = "core_name_" + object_name
        action_texture = "core_action_" + object_name
        if name_texture not in texture_sizes:
            continue
        action = action_texture if action_texture in texture_sizes else NO_LABEL
        action_size = texture_sizes.get(action_texture, (0, 0))
        lines.append("label %s %s %s %s %s %s %s" % (object_name, name_texture, asset_writers.format_number(texture_sizes[name_texture][0]), asset_writers.format_number(texture_sizes[name_texture][1]), action, asset_writers.format_number(action_size[0]), asset_writers.format_number(action_size[1])))
    if "core_unknown" in texture_sizes:
        lines.append("label ? core_unknown %s %s %s 0 0" % (asset_writers.format_number(texture_sizes["core_unknown"][0]), asset_writers.format_number(texture_sizes["core_unknown"][1]), NO_LABEL))
    return "\n".join(lines) + "\n"


def effects_text(effects):
    return "".join("effect %d %s %s %d %s %s %s %d\n" % (row["id"], row["kind"], row["texture"], row["ticks"], asset_writers.format_number(row["end_zoom"]), asset_writers.format_number(row["width"]), asset_writers.format_number(row["height"]), row.get("only_in_soul_view", 0)) for row in effects)


def chapter_preload_text(set_description, section, narration_lines, extra_textures):
    textures = list(extra_textures)
    textures.append(section["card_texture"])
    for label in list(narration_lines)[:CHAPTER_PRELOADED_MESSAGES]:
        for paragraph in narration_lines[label]["paragraphs"]:
            textures.extend(line["texture"] for line in paragraph)
    for room_plates in list(section.get("room_plates", {}).values())[:CHAPTER_PRELOADED_ROOMS]:
        textures.extend(plate[0] for plate in room_plates)
    lines = ["texture " + name for name in dict.fromkeys(textures)]
    lines.extend("sound " + name for name in dict.fromkeys(section.get("sound_kinds", {}).values()))
    return "\n".join(lines) + "\n"


def audio_text(section):
    lines = ["music %s %s" % (id_text, name) for id_text, name in section.get("music_ids", {}).items()]
    lines.extend("sound %s %s" % (kind_text, name) for kind_text, name in section.get("sound_kinds", {}).items())
    return "\n".join(lines) + "\n"


def check_group_counts(cues_text, narration_lines):
    groups_by_label = {}
    label = None
    for line in cues_text.split("\n"):
        fields = line.split()
        if fields and fields[0] == "message":
            label = fields[1]
            groups_by_label[label] = 0
        elif fields and fields[0] == "page":
            groups_by_label[label] = groups_by_label[label] + int(fields[3])
    for label, groups in groups_by_label.items():
        if label in narration_lines and len(narration_lines[label]["paragraphs"]) != groups:
            raise ScriptBakeError("%s has %d groups in the script's language file and %d paragraphs in the narration" % (label, groups, len(narration_lines[label]["paragraphs"])))


def text_lines_text(narration_lines):
    lines = []
    for label, message in narration_lines.items():
        for group, paragraph in enumerate(message["paragraphs"]):
            for line in paragraph:
                lines.append("line %s %d %d %s %d %s" % (label, group, line["id"], line["texture"], SCREEN_CENTER_X, asset_writers.format_number(line["top_y"])))
    return "\n".join(lines) + "\n"


def chosen_thorn_size(part, texture_sizes):
    text_width = texture_sizes[part["fits"]][0] * part.get("room_per_text", 1.0)
    return next(size for size in part["thorn_sizes"] if text_width <= size["widest_text"])


def with_thorn_chosen_by_text(part, texture_sizes):
    if "thorn_sizes" not in part or part.get("fits") not in texture_sizes:
        return part
    return dict(part, texture=chosen_thorn_size(part, texture_sizes)["texture"])


def thorn_art_of(balloon, texture_sizes):
    for part in balloon["parts"]:
        if "thorn_sizes" in part and part.get("fits") in texture_sizes:
            return part
    return None


def text_placed_in_thorn(art, text_width, text_height, texture_sizes):
    size = chosen_thorn_size(art, texture_sizes)
    box_x, box_y, box_width, box_height = size["text_box"]
    art_width, art_height = texture_sizes[size["texture"]]
    scale = art.get("least_text_scale", 1.0)
    if size["texture"] == art["thorn_sizes"][0]["texture"] and text_height <= art.get("text_line_height", 0):
        scale = max(scale, min(1.0, box_width * THORN_TEXT_FILL_WIDTH / text_width, box_height * THORN_TEXT_FILL_HEIGHT / text_height))
    dx = art["dx"] + (box_x + box_width / 2.0 - art_width / 2.0) / frame_layout.SCREEN_PIXELS_PER_DS_PIXEL
    dy = art["dy"] + (box_y + box_height / 2.0 - art_height / 2.0) / frame_layout.SCREEN_PIXELS_PER_DS_PIXEL
    return dx, dy, text_width * scale, text_height * scale


def balloons_text(balloons, texture_sizes):
    lines = []
    for balloon in balloons:
        lines.append("balloon %d %s %d" % (balloon.get("kind", NO_KIND), balloon.get("label", NO_LABEL), balloon["delay"]))
        thorn_art = thorn_art_of(balloon, texture_sizes)
        for part in balloon["parts"]:
            part = with_thorn_chosen_by_text(part, texture_sizes)
            if part["texture"] not in texture_sizes:
                continue
            width, height = texture_sizes[part["texture"]]
            dx, dy = part["dx"], part["dy"]
            if part.get("fits") in texture_sizes and "thorn_sizes" not in part:
                text_width, text_height = texture_sizes[part["fits"]]
                room = part.get("room_per_text", 1.0)
                width = max(width, text_width * room + BUBBLE_MARGIN_WIDTH)
                height = max(height, text_height * room + BUBBLE_MARGIN_HEIGHT)
            if thorn_art is not None and part["texture"] == thorn_art["fits"] and "text_box" in chosen_thorn_size(thorn_art, texture_sizes):
                dx, dy, width, height = text_placed_in_thorn(thorn_art, width, height, texture_sizes)
            lines.append("part %s %s %s %s %s %d %d" % (part["texture"], asset_writers.format_number(dx), asset_writers.format_number(dy), asset_writers.format_number(width / frame_layout.SCREEN_PIXELS_PER_DS_PIXEL), asset_writers.format_number(height / frame_layout.SCREEN_PIXELS_PER_DS_PIXEL), part["after"], part["grow"]))
    return "\n".join(lines) + "\n"


def preload_text(section, balloons, narration_lines, animation_names, soul_plates, extra_textures):
    lines = ["animation " + name for name in animation_names]
    textures = []
    for plates in section.get("room_plates", {}).values():
        textures.extend(plate[0] for plate in plates)
        textures.extend(soul_plates[plate[0]] for plate in plates if plate[0] in soul_plates)
    textures.extend(extra_textures)
    if "card_texture" in section:
        textures.append(section["card_texture"])
    for message in narration_lines.values():
        for paragraph in message["paragraphs"]:
            textures.extend(line["texture"] for line in paragraph)
    for balloon in balloons:
        textures.extend(part["texture"] for part in balloon["parts"])
        textures.extend(size["texture"] for part in balloon["parts"] for size in part.get("thorn_sizes", []))
    lines.extend("texture " + name for name in dict.fromkeys(textures))
    music_names = list(dict.fromkeys(section.get("music_ids", {}).values()))
    lines.extend("music " + name for name in music_names[:1])
    lines.extend("sound " + name for name in dict.fromkeys(section.get("sound_kinds", {}).values()))
    return "\n".join(lines) + "\n"


def read_baked_text(output_folder, relative_path):
    path = os.path.join(output_folder, relative_path)
    if not os.path.isfile(path):
        return None
    with open(path, "r", encoding="ascii") as source:
        return source.read()


def table_files(set_description, output_folder, motion_lines, narration_lines, texture_sizes, plate_sizes, animation_names, talk_textures=()):
    section = set_description.get("scripts")
    if section is None or "motion_tables" not in section or not motion_lines:
        return {}
    balloons = set_description.get("script_balloons", [])
    if "chapter" not in set_description:
        for path in section["files"]:
            cues_text = read_baked_text(output_folder, SCRIPTS_FOLDER + "/" + script_name(path) + CUES_EXTENSION)
            if cues_text is not None:
                check_group_counts(cues_text, narration_lines)
    texts = dict(motion_lines)
    texts[CHAIN_FILE] = chain_text(section)
    soul_plates = soul_plate_names(set_description)
    texts[ROOMS_FILE] = rooms_text(section, plate_sizes, soul_plates, ghost_plate_names(set_description))
    texts[AUDIO_FILE] = audio_text(section)
    texts[TEXT_LINES_FILE] = text_lines_text(narration_lines)
    texts[BALLOONS_FILE] = balloons_text(balloons, texture_sizes)
    extra_textures = list(talk_textures) + set_description.get("talk", {}).get("window_textures", [])
    if "soul_view" in set_description:
        texts[SOUL_FILE] = soul_text(set_description["soul_view"])
    if "core_labels" in set_description:
        texts[CORE_LABELS_FILE] = core_labels_text(set_description["core_labels"], texture_sizes)
        extra_textures.append(set_description["soul_view"]["ring"])
    if "route" in set_description:
        texts[ROUTE_FILE] = route_steps_text(set_description["route"])
    if set_description.get("ui_clips"):
        texts[UI_CLIPS_FILE] = ui_clips.clips_text()
    texts[EFFECTS_FILE] = movies_text(set_description.get("movies", [])) + flipbooks_text(set_description.get("flipbooks", [])) + effects_text(set_description.get("effects", [])) + set_description.get("effect_movie_text", "")
    if "chapter" in set_description:
        texts[PRELOAD_FILE] = chapter_preload_text(set_description, section, narration_lines, extra_textures)
    else:
        texts[PRELOAD_FILE] = preload_text(section, balloons, narration_lines, animation_names, soul_plates, extra_textures)
    return {relative_path: text.encode("ascii") for relative_path, text in texts.items()}
