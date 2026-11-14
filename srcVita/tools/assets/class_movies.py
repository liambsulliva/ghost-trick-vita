import os

import script_assets

MOVIE_OP = 0xec
HASH_SEED = 0x7d4
RANDOM_REST = (0x159a55e5, 0x1f123bb5, 0x05491333)
WORD_MASK = 0xFFFFFFFF
HASH_MODULUS = 0x7FFFFFFF
MOVIE_FOLDER = "natives/android/streaming/gui/ui010500/tex/"
MOVIE_SUFFIX = ".mov.1.android"


class ClassMovieError(ValueError):
    pass


def next_random(state):
    mixed = (state[0] ^ (state[0] << 11)) & WORD_MASK
    state[0], state[1], state[2] = state[1], state[2], state[3]
    state[3] = (state[3] ^ (state[3] >> 19) ^ mixed ^ (mixed >> 8)) & WORD_MASK
    return state[3]


def signed_byte(value):
    return value - 256 if value > 127 else value


def native_class_hash(name):
    text = name.encode("ascii") + b"\0"
    total = 0
    state = [HASH_SEED, *RANDOM_REST]
    for index in range(len(text) - 1):
        pair = ((signed_byte(text[index]) - 0x20) << 6) | ((signed_byte(text[index + 1]) - 0x20) & WORD_MASK)
        state = [pair & WORD_MASK, *RANDOM_REST]
        steps = ((text[index] & 0xf) + next_random(state) + 1) & 0xf
        for _ in range(steps + 1):
            next_random(state)
        total = total ^ next_random(state)
    return total % HASH_MODULUS


def movie_path(file_name):
    return MOVIE_FOLDER + file_name + MOVIE_SUFFIX


def add_to_set(set_description):
    movies = []
    for movie in set_description.get("movies", []):
        row = dict(movie)
        if "class" in row:
            row["class_id"] = "%08x" % native_class_hash(row["class"])
        movies.append(row)
    return dict(set_description, movies=movies)


def movie_class_ids(plain):
    _, _, _, items, _ = script_assets.lmg_decode.decode(plain)
    for item in items:
        if item[1] == "op" and item[2] == MOVIE_OP:
            operands = item[3]
            yield operands[0] | (operands[1] << 16)


def classes_with_no_movie(movies, class_ids_by_script):
    baked = set(int(movie["class_id"], 16) for movie in movies if "class_id" in movie)
    missing = {}
    for script, class_ids in class_ids_by_script:
        for class_id in class_ids:
            if class_id not in baked:
                missing.setdefault(class_id, script)
    return missing


def class_ids_of_baked_scripts(output_folder, script_paths):
    for path in script_paths:
        if path.endswith(script_assets.SCRIPT_EXTENSION):
            with open(os.path.join(output_folder, path), "rb") as source:
                yield os.path.basename(path), list(movie_class_ids(source.read()))


def check_every_class_has_a_movie(set_description, output_folder, script_paths):
    missing = classes_with_no_movie(set_description.get("movies", []), class_ids_of_baked_scripts(output_folder, script_paths))
    if missing:
        raise ClassMovieError("movie classes with no movie row: " + ", ".join("%08x (%s)" % (class_id, script) for class_id, script in sorted(missing.items())))
