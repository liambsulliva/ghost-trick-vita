import hashlib
import json
import os
import threading

import asset_writers
import gtex_pack
import output_folders

RECORD_FILE_NAME = ".bake_record.json"
TEXTURE_FOLDER_PREFIXES = ("textures/",)
TEXTURE_FILE_SUFFIX = ".gtex"
TEMPORARY_SUFFIX = ".new"
ASSET_TOOLS_FOLDER = os.path.dirname(os.path.abspath(__file__))


def texture_result(name, source_size, gtex_bytes):
    width, height, texture_format = asset_writers.read_gtex_header(gtex_bytes)
    return {
        "name": name,
        "source_size": source_size,
        "width": width,
        "height": height,
        "format": texture_format & ~gtex_pack.PACKED_FLAG,
        "byte_count": gtex_pack.unpacked_size(gtex_bytes),
    }


def top_folder(relative_path):
    return relative_path.split("/")[0]


def result_of_own_size(name, gtex_bytes):
    width, height, _ = asset_writers.read_gtex_header(gtex_bytes)
    return texture_result(name, (width, height), gtex_bytes)


def sha256_of_bytes(data):
    return hashlib.sha256(data).hexdigest()


def code_fingerprint(file_names, extra_text=""):
    digest = hashlib.sha256(extra_text.encode("utf-8"))
    for file_name in sorted(file_names):
        digest.update(file_name.encode("utf-8"))
        with open(os.path.join(ASSET_TOOLS_FOLDER, file_name), "rb") as source:
            digest.update(source.read())
    return digest.hexdigest()


def write_file_atomically(path, data):
    os.makedirs(os.path.dirname(path) or ".", exist_ok=True)
    temporary_path = path + TEMPORARY_SUFFIX
    with open(temporary_path, "wb") as output:
        output.write(data)
    os.replace(temporary_path, path)


def write_if_different(path, data):
    if os.path.exists(path):
        with open(path, "rb") as source:
            if source.read() == data:
                return False
    write_file_atomically(path, data)
    return True


def remove_leftover_temporary_file(path):
    try:
        os.remove(path + TEMPORARY_SUFFIX)
    except FileNotFoundError:
        pass


def disk_path(output_folder, relative_path):
    return os.path.join(output_folder, output_folders.disk_path_of(relative_path))


def move_flat_textures_into_buckets(output_folder):
    folder = os.path.join(output_folder, output_folders.TEXTURE_FOLDER)
    if not os.path.isdir(folder):
        return 0
    moved = 0
    for file_name in sorted(os.listdir(folder)):
        flat_path = os.path.join(folder, file_name)
        if not os.path.isfile(flat_path) or not output_folders.is_texture_file_name(file_name):
            continue
        target = disk_path(output_folder, output_folders.TEXTURE_FOLDER + "/" + file_name)
        os.makedirs(os.path.dirname(target), exist_ok=True)
        os.replace(flat_path, target)
        moved = moved + 1
    return moved


def remove_files_not_listed(output_folder, folder_names, relative_paths_to_keep):
    removed = []
    for folder_name in folder_names:
        folder = os.path.join(output_folder, folder_name)
        for parent, folder_names_inside, file_names in os.walk(folder, topdown=False):
            for file_name in sorted(file_names):
                on_disk = os.path.relpath(os.path.join(parent, file_name), output_folder).replace(os.sep, "/")
                relative_path = output_folders.relative_path_of(on_disk)
                if relative_path not in relative_paths_to_keep or output_folders.disk_path_of(relative_path) != on_disk:
                    os.remove(os.path.join(parent, file_name))
                    removed.append(on_disk)
            if parent != folder and not os.listdir(parent):
                os.rmdir(parent)
    return removed


def entry_is_usable(entry):
    if not isinstance(entry, dict):
        return False
    return isinstance(entry.get("key"), str) and isinstance(entry.get("output"), str) and isinstance(entry.get("details", {}), dict)


class BakeRecord:
    def __init__(self, output_folder):
        self.output_folder = output_folder
        move_flat_textures_into_buckets(output_folder)
        self.path = os.path.join(output_folder, RECORD_FILE_NAME)
        self.lock = threading.Lock()
        self.entries = self.load_entries()
        self.reused_counts = {}
        self.baked_counts = {}

    def load_entries(self):
        try:
            with open(self.path, "r", encoding="ascii") as source:
                loaded = json.load(source)
        except (OSError, ValueError):
            return {}
        if not isinstance(loaded, dict):
            return {}
        return {path: entry for path, entry in loaded.items() if entry_is_usable(entry)}

    def key_for(self, code, settings, source_bytes):
        settings_text = json.dumps(settings, sort_keys=True)
        return sha256_of_bytes((code + settings_text + sha256_of_bytes(source_bytes)).encode("utf-8"))

    def read_if_current(self, relative_path, key):
        entry = self.entries.get(relative_path)
        if entry is None or entry["key"] != key:
            return None
        try:
            with open(disk_path(self.output_folder, relative_path), "rb") as source:
                data = source.read()
        except OSError:
            return None
        if sha256_of_bytes(data) != entry["output"]:
            return None
        with self.lock:
            self.reused_counts[top_folder(relative_path)] = self.reused_in(top_folder(relative_path)) + 1
        return data

    def reused_in(self, folder_name):
        return self.reused_counts.get(folder_name, 0)

    def baked_in(self, folder_name):
        return self.baked_counts.get(folder_name, 0)

    def details_of(self, relative_path):
        return self.entries.get(relative_path, {}).get("details", {})

    def store(self, relative_path, key, data, details=None):
        if relative_path.startswith(TEXTURE_FOLDER_PREFIXES) and relative_path.endswith(TEXTURE_FILE_SUFFIX):
            data = gtex_pack.pack_gtex(data)
        path = disk_path(self.output_folder, relative_path)
        os.makedirs(os.path.dirname(path), exist_ok=True)
        write_file_atomically(path, data)
        entry = {"key": key, "output": sha256_of_bytes(data)}
        if details is not None:
            entry["details"] = details
        with self.lock:
            self.entries[relative_path] = entry
            self.baked_counts[top_folder(relative_path)] = self.baked_in(top_folder(relative_path)) + 1

    def forget_all_except(self, relative_paths):
        self.entries = {path: entry for path, entry in self.entries.items() if path in relative_paths}

    def save(self):
        write_file_atomically(self.path, json.dumps(self.entries, indent=0, sort_keys=True).encode("ascii"))
