import zlib

NAMES = ("textures", "animations", "scenes", "music", "music_ds", "sounds", "scripts", "pause")
TEXTURE_FOLDER = "textures"
TEXTURE_SUFFIX = ".gtex"
TEXTURE_FOLDER_BUCKETS = 64


def texture_bucket_of(name):
    return "%02x" % (zlib.crc32(name.encode("utf-8")) % TEXTURE_FOLDER_BUCKETS)


def is_texture_file_name(file_name):
    return file_name.endswith(TEXTURE_SUFFIX) and not file_name.startswith(".")


def disk_path_of(relative_path):
    parts = relative_path.split("/")
    if len(parts) != 2 or parts[0] != TEXTURE_FOLDER or not is_texture_file_name(parts[1]):
        return relative_path
    return "/".join([TEXTURE_FOLDER, texture_bucket_of(parts[1][:-len(TEXTURE_SUFFIX)]), parts[1]])


def relative_path_of(disk_path):
    parts = disk_path.split("/")
    if len(parts) == 3 and parts[0] == TEXTURE_FOLDER and is_texture_file_name(parts[2]) and parts[1] == texture_bucket_of(parts[2][:-len(TEXTURE_SUFFIX)]):
        return parts[0] + "/" + parts[2]
    return disk_path
