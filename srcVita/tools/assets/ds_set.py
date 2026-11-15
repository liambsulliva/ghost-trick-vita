import collections

import asset_writers
import bake_record
import chapter_assets
import ds_rom
import ds_rooms
import ds_set_text
import ds_sounds
import ds_sprites
import ds_textures
import ds_ui
import script_assets

PLATE_NAME = "ds_plate_%d"
SPRITE_PAGE_NAME = "ds_sprite_%d"
SDAT_PATH = "sound_data.sdat"
TEXTURE_FOLDER = "textures"
PAGE_WIDTH = 1024
PAGE_HEIGHT = 2048
SPRITE_LATER_PAGE_NAME = "ds_sprite_%d_%d"
FRAME_GAP = 1
DS_TICKS_PER_FRAME = 4
SPRITE_CODE_FILES = ["asset_writers.py", "ds_set.py", "ds_sprites.py"]


class Cartridge:
    def __init__(self, rom_path):
        with ds_rom.DsRom(rom_path) as rom:
            self.pack_bytes = rom.read_file(ds_rooms.PACK_PATH)
            self.sdat = rom.read_file(SDAT_PATH)
            self.textures = ds_textures.TexturePack(rom.read_file(ds_textures.PACK_PATH))
        self.sprites = ds_sprites.SpritePack(self.pack_bytes)


def plate_rows(scripts, arm9, plate_entries, soul_view):

    def cartridge_plate(plate_id):
        if plate_id not in plate_entries:
            return None
        return {"name": PLATE_NAME % plate_id}

    return chapter_assets.room_rows(scripts, arm9, soul_view.get("ramp"), soul_view.get("ghost_ramps"), cartridge_plate)


def plate_sizes(plates, plate_entries):
    sizes = {}
    for plate in plates:
        size = ds_rooms.plate_size(plate_entries[plate["ds_plate"]])
        for name in [plate["name"], plate.get("soul_plate")] + list(plate.get("ghost_plates", {}).values()):
            if name is not None:
                sizes[name] = size
    return sizes


def music_ids(scripts):
    return {str(music_id): chapter_assets.MUSIC_NAME % music_id for music_id in chapter_assets.music_ids_used(scripts)}


def expand(set_description, rom_path, cartridge):
    expanded = dict(set_description)
    section, scripts, arm9, languages, system_line_texts = chapter_assets.read_chain(set_description, rom_path)
    soul_view = set_description.get("soul_view", {})
    plate_entries = ds_rooms.read_plate_entries(cartridge.pack_bytes)
    plates, section["room_plates"], section["room_sizes"] = plate_rows(scripts, arm9, plate_entries, soul_view)
    expanded["plates"] = plates
    section["room_bands"] = {str(room): chapter_assets.read_room_bands(arm9, room) for room in chapter_assets.rooms_used(scripts)}
    if "soul_view" in set_description:
        expanded["soul_view"] = dict(soul_view, trickless_core_infos=chapter_assets.trickless_core_infos(arm9, scripts))
    section["music_ids"] = music_ids(scripts)
    section["motion_tables"] = {path: {} for path in scripts}
    sounds, section["sound_kinds"] = ds_sounds.sound_rows_of_scripts(cartridge.sdat, scripts, arm9)
    expanded["sounds"] = sounds
    expanded["scripts"] = section
    return expanded, scripts, arm9, plate_entries, languages, system_line_texts


def table_rows(scripts):
    rows = []
    for path, script in scripts.items():
        for table in script.tables:
            stem = script.table_name_at(table[0])
            resource = chapter_assets.begin_id_of(table[2])
            if stem is None or not resource:
                continue
            rows.append((path, stem, table[0], resource, script_assets.walk_table_frames(script.plain, table[0])))
    return rows


def animation_names(rows):
    resources_of_stem = collections.defaultdict(set)
    for _, stem, _, resource, _ in rows:
        resources_of_stem[stem.lower()].add(resource)
    names = {}
    for path, stem, offset, resource, _ in rows:
        name = script_assets.TABLE_ANIMATION_PREFIX + stem.lower()
        if len(resources_of_stem[stem.lower()]) > 1:
            name = name + "_%d" % resource
        names[(path, offset)] = name
    return names


def place_frames(frames):
    places = []
    page_sizes = []
    page = 0
    x = 0
    y = 0
    row_height = 0
    page_width = 1
    page_height = 1
    for width, height, _, _, _ in frames:
        if x > 0 and x + width > PAGE_WIDTH:
            x = 0
            y = y + row_height + FRAME_GAP
            row_height = 0
        if y + height > PAGE_HEIGHT and (x > 0 or y > 0):
            page_sizes.append((page_width, page_height))
            page = page + 1
            x = 0
            y = 0
            row_height = 0
            page_width = 1
            page_height = 1
        places.append((page, x, y))
        page_width = max(page_width, x + width)
        page_height = max(page_height, y + height)
        x = x + width + FRAME_GAP
        row_height = max(row_height, height)
    page_sizes.append((page_width, page_height))
    return places, page_sizes


def page_rgba(frames, places, page, page_width, page_height):
    rgba = bytearray(page_width * page_height * 4)
    for (width, height, pixels, _, _), (frame_page, left, top) in zip(frames, places):
        if frame_page != page:
            continue
        for row in range(height):
            start = ((top + row) * page_width + left) * 4
            rgba[start:start + width * 4] = pixels[row * width * 4:(row + 1) * width * 4]
    return bytes(rgba)


def source_of_parts(parts):
    return b"".join(len(part).to_bytes(4, "little") + part for part in parts)


def page_name(resource, page):
    return SPRITE_PAGE_NAME % resource if page == 0 else SPRITE_LATER_PAGE_NAME % (resource, page)


def page_path(resource, page):
    return TEXTURE_FOLDER + "/" + page_name(resource, page) + ".gtex"


def pages_of_resource(resource, cartridge, arm9, record, code):
    first_path = page_path(resource, 0)
    parts = ds_sprites.resource_parts(cartridge.sprites, ds_sprites.resolve(arm9, resource))
    key = record.key_for(code, {"resource": resource}, source_of_parts(parts))
    details = record.details_of(first_path)
    if "page_sizes" in details and all(record.read_if_current(page_path(resource, page), key) is not None for page in range(len(details["page_sizes"]))):
        return [tuple(layout) for layout in details["layouts"]], [tuple(place) for place in details["places"]], [tuple(size) for size in details["page_sizes"]]
    frames = ds_sprites.frames_of_parts(*parts)
    if not frames:
        raise ds_sprites.SpriteRefused("no frames")
    places, page_sizes = place_frames(frames)
    layouts = [(width, height, None, left, top) for width, height, _, left, top in frames]
    saved = {"layouts": [list(layout) for layout in layouts], "places": [list(place) for place in places], "page_sizes": [list(size) for size in page_sizes]}
    for page, (page_width, page_height) in enumerate(page_sizes):
        record.store(page_path(resource, page), key, asset_writers.gtex_bytes(page_width, page_height, asset_writers.FORMAT_RGBA8888, page_rgba(frames, places, page, page_width, page_height)), saved if page == 0 else {})
    return layouts, places, page_sizes


def anim_frame(resource, frame, place, page_sizes):
    width, height, _, left, top = frame
    if width == 0 or height == 0:
        return {"texture": "-", "ticks": DS_TICKS_PER_FRAME, "x": 0, "y": 0, "width": 0, "height": 0, "u": 0, "v": 0, "u_size": 0, "v_size": 0}
    page, x, y = place
    page_width, page_height = page_sizes[page]
    return {"texture": page_name(resource, page), "ticks": DS_TICKS_PER_FRAME, "x": left, "y": top, "width": width, "height": height,
            "u": x / page_width, "v": y / page_height, "u_size": width / page_width, "v_size": height / page_height}


def bake_motion_tables(set_description, scripts, arm9, cartridge, record):
    code = bake_record.code_fingerprint(SPRITE_CODE_FILES)
    rows = table_rows(scripts)
    names = animation_names(rows)
    pages = {}
    refused = {}
    texture_paths = []
    texture_sizes = {}
    animation_texts = {}
    motion_lines = collections.defaultdict(list)
    for path, stem, offset, resource, ds_frames in rows:
        if resource in refused or not ds_frames:
            continue
        if resource not in pages:
            try:
                pages[resource] = pages_of_resource(resource, cartridge, arm9, record, code)
            except ds_sprites.SpriteRefused as error:
                refused[resource] = str(error)
                continue
            for page, size in enumerate(pages[resource][2]):
                texture_paths.append(page_path(resource, page))
                texture_sizes[page_name(resource, page)] = size
        frames, places, page_sizes = pages[resource]
        name = names[(path, offset)]
        if name not in animation_texts:
            picked = [min(frame, len(frames) - 1) for frame in ds_frames]
            animation_texts[name] = asset_writers.anim_text(name, 0, [anim_frame(resource, frames[index], places[index], page_sizes) for index in picked])
        motion_lines[script_assets.SCRIPTS_FOLDER + "/" + script_assets.script_name(path) + script_assets.MOTIONS_EXTENSION].append("motion %x %s 1" % (offset, name))
        scripts_tables = set_description["scripts"]["motion_tables"].setdefault(path, {})
        scripts_tables[stem] = name
    if refused:
        print("sprite resources with no decoded DS art: %d (%s)" % (len(refused), ", ".join("%d" % resource for resource in sorted(refused)[:12])))
    return {"texture_paths": texture_paths, "texture_sizes": texture_sizes, "animation_texts": animation_texts,
            "motion_lines": {path: "\n".join(lines) + "\n" for path, lines in motion_lines.items()}}


def bake_art(set_description, scripts, arm9, cartridge, rom_path, record, work_folder):
    art = bake_motion_tables(set_description, scripts, arm9, cartridge, record)
    text = ds_set_text.bake_text(set_description, scripts, rom_path, record, work_folder)
    art["texture_paths"] = art["texture_paths"] + text["texture_paths"]
    art["texture_sizes"] = dict(art["texture_sizes"], **text["texture_sizes"])
    art["narration_lines"] = text["narration_lines"]
    art["talk_textures"] = text["talk_textures"]
    art["files"] = dict(text["talk_files"])
    language = ds_textures.LANGUAGE_NUMBERS.get(set_description.get("scripts", {}).get("language", "en"), 1)
    interface_paths, interface_sizes = ds_ui.bake(set_description.get("ds_textures", []), cartridge.textures, language, record)
    art["texture_paths"] = art["texture_paths"] + interface_paths
    art["texture_sizes"] = dict(art["texture_sizes"], **interface_sizes)
    return art
