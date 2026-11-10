import struct

import asset_writers
import effect_file

USER_PATH = "natives/android/charfiles/3d/05_effect/%s/23_%s.user.2"
AFTER_LAST_CODES = {"end": 0, "loop": 1, "hold": 2}
JUMP_CONDITION_CODES = {effect_file.JUMP_ALWAYS: 0, effect_file.JUMP_WHEN_EQUAL: 1, effect_file.JUMP_ONE_TIME_IN: 2}


def page_texture_name(effect_name, page):
    return "fx_%s_%02d" % (effect_name.lower().replace("-", "_"), page)


def frame_row(quads, page_size):
    if not quads:
        return "fxframe -1 0 0 0 0 0 0 0 0 0 0 0 0"
    quad = quads[0]
    x, y, width, height = quad["source"]
    numbers = [x / page_size[0], y / page_size[1], (x + width) / page_size[0], (y + height) / page_size[1]]
    for corner in quad["corners"]:
        numbers.extend(corner)
    return "fxframe %d %s" % (quad["page"], " ".join(asset_writers.format_number(round(number, 4)) for number in numbers))


def movie_rows(row, effect):
    lines = ["fxmovie %s %s %d %d %d %d" % (row["effect"], asset_writers.format_number(effect["ticks_per_frame"]), AFTER_LAST_CODES[effect["after_last"]], effect["loop_start"] or 0, row.get("sound", -1), len(effect["page_paths"]))]
    lines.extend("fxjump %d %d %d %d" % (jump["frame"], jump["target"], JUMP_CONDITION_CODES[jump["condition"]], jump["value"]) for jump in effect["jumps"])
    lines.extend("fxstatus %d %d" % (status, frame) for status, frame in sorted(effect["statuses"].items()))
    lines.extend(frame_row(frame["quads"], effect["page_size"]) for frame in effect["frames"])
    return lines


def add_to_set(set_description, packs):
    rows = set_description.get("effect_movies", [])
    if not rows:
        return set_description
    expanded = dict(set_description)
    textures = list(expanded.get("screen_textures", []))
    effects = list(expanded.get("effects", []))
    lines = []
    movies_read = set()
    for row in rows:
        if row["effect"] not in movies_read:
            try:
                effect = effect_file.read_effect(packs.read(USER_PATH % (row["effect"], row["effect"])))
            except (ValueError, LookupError, StopIteration, struct.error) as error:
                if not row.get("found_in_scripts"):
                    raise
                print("effect movie %s for effect %d could not be read, so it is not drawn: %s" % (row["effect"], row["id"], error))
                continue
            movies_read.add(row["effect"])
            for page, path in enumerate(effect["page_paths"]):
                textures.append({"name": page_texture_name(row["effect"], page), "source": path, "width": effect["page_size"][0], "height": effect["page_size"][1]})
            lines.extend(movie_rows(row, effect))
        if "id" in row:
            effects.append({"id": row["id"], "kind": "movie", "texture": row["effect"], "ticks": 0, "end_zoom": 1, "width": 0, "height": 0, "only_in_soul_view": row.get("only_in_soul_view", 0)})
    expanded["screen_textures"] = textures
    expanded["effects"] = effects
    expanded["effect_movie_text"] = "\n".join(lines) + "\n"
    return expanded
