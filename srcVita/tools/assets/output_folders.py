import zlib

LANGUAGE_FOLDER = "lang"
NAMES = ("textures", "animations", "scenes", "music", "music_ds", "sounds", "scripts", "pause")
TEXTURE_FOLDER = "textures"
TEXTURE_SUFFIX = ".gtex"
TEXTURE_FOLDER_BUCKETS = 64


def texture_bucket_of(name):
    return "%02x" % (zlib.crc32(name.encode("utf-8")) % TEXTURE_FOLDER_BUCKETS)


def is_texture_file_name(file_name):
    return file_name.endswith(TEXTURE_SUFFIX) and not file_name.startswith(".")


def split_texture_folder(parts):
    if len(parts) >= 2 and parts[0] == TEXTURE_FOLDER:
        return [], parts[1:]
    if len(parts) >= 4 and parts[0] == LANGUAGE_FOLDER and parts[2] == TEXTURE_FOLDER:
        return parts[:2], parts[3:]
    return None, None


def disk_path_of(relative_path):
    parts = relative_path.split("/")
    prefix, rest = split_texture_folder(parts)
    if prefix is None or len(rest) != 1 or not is_texture_file_name(rest[0]):
        return relative_path
    return "/".join(prefix + [TEXTURE_FOLDER, texture_bucket_of(rest[0][:-len(TEXTURE_SUFFIX)]), rest[0]])


def relative_path_of(disk_path):
    parts = disk_path.split("/")
    prefix, rest = split_texture_folder(parts)
    if prefix is not None and len(rest) == 2 and is_texture_file_name(rest[1]) and rest[0] == texture_bucket_of(rest[1][:-len(TEXTURE_SUFFIX)]):
        return "/".join(prefix + [TEXTURE_FOLDER, rest[1]])
    return disk_path
