import asset_writers
import bake_record
import ds_effect_movies
import ds_effects
import ds_textures
import effect_file
import frame_layout

TEXTURE_FOLDER = "textures"
CODE_FILES = ["ds_alert_balloons.py", "ds_effect_movies.py", "ds_effects.py", "ds_textures.py", "asset_writers.py"]
NO_TEXT_BALLOON_OP = 0x52
QUESTION_KIND = 4
QUESTION_EFFECT = 0x8c
ALERT_EFFECT = 0x85
MIRRORED_KINDS = (3, 6)
IDENTITY_FILL = (1.0, 1.0, 0.0, 0.0, 0.0, 0.0)
SAME_SCALE = 1e-3
TEXTURE_SCALE = frame_layout.SCREEN_PIXELS_PER_DS_PIXEL
TEXTURE_NAME = "ds_balloon_fx%d_%d"
PART_LIMIT = 4


class AlertRefused(ValueError):
    pass


def effect_of_kind(kind):
    if kind in MIRRORED_KINDS:
        raise AlertRefused("kind %d draws its effect mirrored, which no row can show" % kind)
    return QUESTION_EFFECT if kind == QUESTION_KIND else ALERT_EFFECT


def balloon_kinds(scripts):
    found = {}
    for path, script in scripts.items():
        for _, _, op, operands in script.operations:
            if op == NO_TEXT_BALLOON_OP and operands and operands[0] not in found:
                found[operands[0]] = path
    return found


def root_sprite(movie):
    return next(sprite for sprite in movie["sprites"] if sprite["sprite_id"] == ds_effect_movies.ROOT_SPRITE)


def slot_showings(movie):
    root = root_sprite(movie)
    if any(sprite["sprite_id"] != ds_effect_movies.ROOT_SPRITE and sprite["frames"] for sprite in movie["sprites"]):
        raise AlertRefused("a balloon effect nests movie clips")
    showings = {}
    for index, frame in enumerate(root["frames"]):
        for item in frame["items"]:
            showings.setdefault(item["slot"], []).append((index, item))
    return showings


def texture_item_of(movie, character):
    shape = movie["shapes"].get(character)
    if shape is None or len(shape["fills"]) != 1:
        raise AlertRefused("a balloon effect places character %d, which is not a one-picture shape" % character)
    fill = shape["fills"][0]
    if tuple(fill["matrix"]) != IDENTITY_FILL or fill["bitmap"] not in movie["bitmap_ids"]:
        raise AlertRefused("a balloon effect's shape %d moves its picture inside it" % character)
    return movie["bitmap_ids"].index(fill["bitmap"]) + ds_effect_movies.TEXTURE_ITEM_AFTER_SCRIPT, shape["rect"]


def part_of_slot(movie, showings):
    first_frame, first = showings[0]
    _, settled = showings[-1]
    if any(item["character"] != first["character"] for _, item in showings):
        raise AlertRefused("a balloon effect changes the picture a slot shows")
    scale_x, scale_y, skew_a, skew_b, move_x, move_y = settled["matrix"]
    if skew_a or skew_b or abs(scale_x - scale_y) > SAME_SCALE:
        raise AlertRefused("a balloon effect's slot %d settles turned or stretched" % first["slot"])
    item, rect = texture_item_of(movie, first["character"])
    x_min, x_max, y_min, y_max = rect
    origin_x, origin_y = effect_file.STAGE_ORIGIN
    grow = next(index for index, (_, shown) in enumerate(showings) if abs(shown["matrix"][0] - scale_x) <= SAME_SCALE)
    return {"item": item, "first": first_frame, "depth": first["depth"], "scale": scale_x, "dx": move_x + scale_x * (x_min + x_max) / 2 - origin_x, "dy": move_y + scale_y * (y_min + y_max) / 2 - origin_y, "grow": grow}


def parts_of_movie(movie):
    slots = [part_of_slot(movie, showings) for showings in slot_showings(movie).values()]
    if not slots:
        raise AlertRefused("a balloon effect draws nothing")
    if len(slots) > PART_LIMIT:
        raise AlertRefused("a balloon effect shows %d pictures, more than a row holds" % len(slots))
    delay = min(slot["first"] for slot in slots)
    return delay, [dict(slot, after=slot["first"] - delay) for slot in sorted(slots, key=lambda slot: slot["depth"])]


def row_of_kind(kind, effect, movie):
    delay, slots = parts_of_movie(movie)
    parts = [{"texture": TEXTURE_NAME % (effect, slot["item"]), "dx": round(slot["dx"], 4), "dy": round(slot["dy"], 4), "after": slot["after"], "grow": slot["grow"], "texture_scale": TEXTURE_SCALE * slot["scale"]} for slot in slots]
    return {"kind": kind, "delay": delay, "parts": parts}


def movie_of(effect_bytes):
    start, end = ds_effects.item_offsets(effect_bytes)[ds_effects.SCRIPT_ITEM]
    return ds_effect_movies.read_movie(effect_bytes[start + ds_effects.ITEM_HEADER_SIZE:end])


def compose(kinds, effect_bytes_of):
    rows = []
    textures = {}
    refused = []
    for kind in sorted(kinds):
        try:
            effect = effect_of_kind(kind)
            effect_bytes = effect_bytes_of(effect)
            row = row_of_kind(kind, effect, movie_of(effect_bytes))
            for part in row["parts"]:
                if part["texture"] not in textures:
                    item = int(part["texture"].rsplit("_", 1)[1])
                    textures[part["texture"]] = ds_textures.decode(ds_effects.texture_item(effect_bytes, item))
        except (AlertRefused, ds_effects.EffectRefused, ds_effect_movies.EffectMovieRefused, ds_textures.TextureRefused) as error:
            refused.append("kind %d (%s)" % (kind, error))
            continue
        rows.append(row)
    return rows, textures, refused


def store_textures(textures, record):
    code = bake_record.code_fingerprint(CODE_FILES)
    paths = []
    sizes = {}
    for name, (width, height, rgba) in textures.items():
        asset_writers.check_name(name)
        relative_path = TEXTURE_FOLDER + "/" + name + ".gtex"
        key = record.key_for(code, {"width": width, "height": height}, rgba)
        if record.read_if_current(relative_path, key) is None:
            record.store(relative_path, key, asset_writers.gtex_bytes(width, height, asset_writers.FORMAT_RGBA8888, rgba))
        paths.append(relative_path)
        sizes[name] = (width, height)
    return paths, sizes


def bake(set_description, scripts, cartridge, record):
    language = ds_textures.LANGUAGE_NUMBERS.get(set_description.get("scripts", {}).get("language", "en"), 1)
    pack = ds_effects.EffectPack(cartridge.textures.pack)
    rows, textures, refused = compose(balloon_kinds(scripts), lambda effect: pack.effect_bytes(effect, language))
    if refused:
        print("ds alert balloons left out: %d (%s)" % (len(refused), ", ".join(refused)))
    paths, sizes = store_textures(textures, record)
    return rows, paths, sizes
