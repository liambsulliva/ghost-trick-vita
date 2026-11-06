import struct

LIST_FOLDER = "natives/android/gamedesign/resourcelistuserdata/"
PACK_PREFIX = "natives/android/"
PACK_SUFFIX = ".2"


class ResourceListError(ValueError):
    pass


def read_wide_text(data, offset):
    end = offset
    while data[end:end + 2] != b"\0\0":
        end = end + 2
    return data[offset:end].decode("utf-16-le")


def read_rows(data):
    data_offset = struct.unpack_from("<Q", data, 32)[0]
    object_count, instance_count, user_data_count = struct.unpack_from("<III", data, data_offset + 8)
    instance_table, values_offset, user_data_table = struct.unpack_from("<QQQ", data, data_offset + 24)
    object_indices = struct.unpack_from("<%di" % object_count, data, data_offset + 48)
    user_paths = {}
    for index in range(user_data_count):
        entry = data_offset + user_data_table + 16 * index
        instance = struct.unpack_from("<I", data, entry)[0]
        text_offset = struct.unpack_from("<Q", data, entry + 8)[0]
        user_paths[instance] = read_wide_text(data, data_offset + text_offset)
    rows = []
    position = data_offset + values_offset
    for instance in range(1, instance_count):
        if instance in user_paths:
            continue
        if instance == object_indices[0]:
            break
        resource_id, path_instance = struct.unpack_from("<ii", data, position)
        position = position + 8
        if path_instance in user_paths:
            rows.append((resource_id, user_paths[path_instance]))
    return rows


def pack_path_of(logical_path):
    return PACK_PREFIX + logical_path.lower() + PACK_SUFFIX


def read_list(packs, kind, list_name):
    return read_rows(packs.read(LIST_FOLDER + kind + "/" + list_name + ".user.2"))
