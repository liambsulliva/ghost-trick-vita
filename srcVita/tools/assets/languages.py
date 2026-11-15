import copy
import json
import os

import bake_record
import gtex_pack
import message_file
import output_folders

ASSET_TOOLS_FOLDER = os.path.dirname(os.path.abspath(__file__))
LANGUAGES_FILE = os.path.join(ASSET_TOOLS_FOLDER, "languages.json")
OVERLAY_FOLDER = output_folders.LANGUAGE_FOLDER
NAMES_FILE = "names.txt"
ART_PATH_MARK = ".tex."
DS_TEXT_MARK = ".%s.msg.xml"
KEEPS_ITS_FONTS = ("languages", "key_font")


class LanguageError(ValueError):
    pass


def load_languages(path=LANGUAGES_FILE):
    with open(path, "r", encoding="utf-8") as source:
        languages = json.load(source)["languages"]
    if not languages or languages[0]["slot"] != message_file.ENGLISH_SLOT:
        raise LanguageError("the first language must be the base set's English")
    return languages


def other_languages(languages):
    return languages[1:]


def overlay_prefix(language):
    return "%s/%s/" % (OVERLAY_FOLDER, language["code"])


def names_path(language):
    return overlay_prefix(language) + NAMES_FILE


def language_key(text_key, language):
    return message_file.LanguageKey(text_key, language["slot"])


def merged(base, change):
    if isinstance(base, dict) and isinstance(change, dict):
        result = dict(base)
        for key, value in change.items():
            if key not in base:
                raise LanguageError("the set has no text named " + key)
            result[key] = merged(base[key], value)
        return result
    if isinstance(base, list) and isinstance(change, list) and len(base) == len(change) and all(isinstance(item, dict) for item in base):
        return [merged(item, other) for item, other in zip(base, change)]
    if type(base) is not type(change):
        raise LanguageError("a translation replaces %r with a different kind of value" % (base,))
    return change


def swapped_art_suffix(text, base_art, art):
    suffix = "." + base_art
    if ART_PATH_MARK in text and text.endswith(suffix):
        return text[:-len(suffix)] + "." + art
    return text


def with_paths_swapped(value, fonts, base_art, art):
    if isinstance(value, dict):
        return {key: item if key in KEEPS_ITS_FONTS else with_paths_swapped(item, fonts, base_art, art) for key, item in value.items()}
    if isinstance(value, list):
        return [with_paths_swapped(item, fonts, base_art, art) for item in value]
    if isinstance(value, str):
        return swapped_art_suffix(fonts.get(value, value), base_art, art)
    return value


def localized_set(set_description, base_language, language):
    localized = with_paths_swapped(copy.deepcopy(set_description), language.get("fonts", {}), base_language["art"], language["art"])
    localized = merged(localized, language.get("text", {}))
    if language.get("font_fallbacks"):
        localized["font_fallbacks"] = language["font_fallbacks"]
    if "scripts" in localized:
        localized["scripts"] = dict(localized["scripts"], language=language["ds"])
    talk = localized.get("talk")
    if talk is not None and "system_lines" in talk:
        talk["system_lines"] = {label: dict(line, file=line["file"].replace(DS_TEXT_MARK % base_language["ds"], DS_TEXT_MARK % language["ds"])) for label, line in talk["system_lines"].items()}
    return localized


def is_overlay_path(relative_path):
    return relative_path.startswith(OVERLAY_FOLDER + "/")


def base_path_of(relative_path):
    if not is_overlay_path(relative_path):
        return relative_path
    return relative_path.split("/", 2)[2]


class LanguageRecord:
    def __init__(self, base, language):
        self.base = base
        self.prefix = overlay_prefix(language)
        self.overlay_paths = set()
        self.same_paths = set()

    def key_for(self, code, settings, source_bytes):
        return self.base.key_for(code, settings, source_bytes)

    def read_if_current(self, relative_path, key):
        data = self.base.read_if_current(relative_path, key)
        if data is not None:
            return data
        entry = self.base.entries.get(self.prefix + relative_path)
        if entry is not None and entry.get("same_as_base") and entry["key"] == key:
            data = self.base_bytes_if(relative_path, entry["output"])
            if data is not None:
                self.same_paths.add(self.prefix + relative_path)
            return data
        data = self.base.read_if_current(self.prefix + relative_path, key)
        if data is not None:
            self.overlay_paths.add(self.prefix + relative_path)
        return data

    def base_bytes_if(self, relative_path, output_hash):
        try:
            with open(bake_record.disk_path(self.base.output_folder, relative_path), "rb") as source:
                data = source.read()
        except OSError:
            return None
        return data if bake_record.sha256_of_bytes(data) == output_hash else None

    def details_of(self, relative_path):
        overlay = self.base.details_of(self.prefix + relative_path)
        return overlay if overlay else self.base.details_of(relative_path)

    def store(self, relative_path, key, data, details=None):
        stored = gtex_pack.pack_gtex(data) if bake_record.is_texture_path(relative_path) else data
        output_hash = bake_record.sha256_of_bytes(stored)
        if self.base_bytes_if(relative_path, output_hash) is not None:
            entry = {"key": key, "output": output_hash, "same_as_base": True}
            if details is not None:
                entry["details"] = details
            with self.base.lock:
                self.base.entries[self.prefix + relative_path] = entry
            self.same_paths.add(self.prefix + relative_path)
            return
        self.base.store(self.prefix + relative_path, key, data, details)
        self.overlay_paths.add(self.prefix + relative_path)

    def reused_in(self, folder_name):
        return self.base.reused_in(folder_name)

    def baked_in(self, folder_name):
        return self.base.baked_in(folder_name)


def overlay_files(language, files, base_files):
    prefix = overlay_prefix(language)
    return {prefix + path: data for path, data in files.items() if base_files.get(path) != data}


def names_text(overlay_paths, language):
    prefix = overlay_prefix(language)
    return "".join(path[len(prefix):] + "\n" for path in sorted(overlay_paths) if path.startswith(prefix) and path != names_path(language))


def remove_overlay_files_not_listed(output_folder, wanted_paths):
    removed = []
    root = os.path.join(output_folder, OVERLAY_FOLDER)
    if not os.path.isdir(root):
        return removed
    for folder, _, file_names in os.walk(root, topdown=False):
        for file_name in file_names:
            on_disk = os.path.relpath(os.path.join(folder, file_name), output_folder).replace(os.sep, "/")
            relative_path = output_folders.relative_path_of(on_disk)
            if relative_path not in wanted_paths or output_folders.disk_path_of(relative_path) != on_disk:
                os.remove(os.path.join(folder, file_name))
                removed.append(on_disk)
        if folder != root and not os.listdir(folder):
            os.rmdir(folder)
    return removed
