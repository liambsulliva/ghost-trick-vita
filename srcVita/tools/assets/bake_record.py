import hashlib
import json
import os
import threading

RECORD_FILE_NAME = ".bake_record.json"
ASSET_TOOLS_FOLDER = os.path.dirname(os.path.abspath(__file__))


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
    temporary_path = path + ".new"
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


def remove_files_not_listed(output_folder, folder_names, relative_paths_to_keep):
    removed = []
    for folder_name in folder_names:
        folder = os.path.join(output_folder, folder_name)
        for file_name in sorted(os.listdir(folder)):
            relative_path = folder_name + "/" + file_name
            if relative_path not in relative_paths_to_keep:
                os.remove(os.path.join(folder, file_name))
                removed.append(relative_path)
    return removed


class BakeRecord:
    def __init__(self, output_folder):
        self.output_folder = output_folder
        self.path = os.path.join(output_folder, RECORD_FILE_NAME)
        self.lock = threading.Lock()
        self.entries = self.load_entries()
        self.reused_count = 0
        self.baked_count = 0

    def load_entries(self):
        try:
            with open(self.path, "r", encoding="ascii") as source:
                return json.load(source)
        except (OSError, ValueError):
            return {}

    def key_for(self, code, settings, source_bytes):
        settings_text = json.dumps(settings, sort_keys=True)
        return sha256_of_bytes((code + settings_text + sha256_of_bytes(source_bytes)).encode("utf-8"))

    def read_if_current(self, relative_path, key):
        entry = self.entries.get(relative_path)
        if entry is None or entry["key"] != key:
            return None
        try:
            with open(os.path.join(self.output_folder, relative_path), "rb") as source:
                data = source.read()
        except OSError:
            return None
        if sha256_of_bytes(data) != entry["output"]:
            return None
        with self.lock:
            self.reused_count = self.reused_count + 1
        return data

    def store(self, relative_path, key, data):
        write_file_atomically(os.path.join(self.output_folder, relative_path), data)
        with self.lock:
            self.entries[relative_path] = {"key": key, "output": sha256_of_bytes(data)}
            self.baked_count = self.baked_count + 1

    def forget_all_except(self, relative_paths):
        self.entries = {path: entry for path, entry in self.entries.items() if path in relative_paths}

    def save(self):
        write_file_atomically(self.path, json.dumps(self.entries, indent=0, sort_keys=True).encode("ascii"))
