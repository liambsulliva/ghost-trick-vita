import os

import ds_rom
import ds_set
import ds_textures
import languages
import script_assets

ASSET_TOOLS_FOLDER = os.path.dirname(os.path.abspath(__file__))


class DsLanguageError(ValueError):
    pass


def every_language_of(set_description):
    if "languages" not in set_description:
        return languages.load_languages()[:1]
    return languages.load_languages(os.path.join(ASSET_TOOLS_FOLDER, set_description["languages"]))


def has_own_text(rom, script_paths, base_language, language):
    if language["ds"] == base_language["ds"]:
        return False
    return any(rom.file_id_of(script_assets.language_path_of(path, language["ds"])) is not None for path in script_paths)


def carried_languages(every_language, rom_path, script_paths):
    with ds_rom.DsRom(rom_path) as rom:
        return [every_language[0]] + [language for language in languages.other_languages(every_language) if has_own_text(rom, script_paths, every_language[0], language)]


def check_listed_languages(files, carried):
    for language in carried:
        if language["ds"] not in ds_textures.LANGUAGE_NUMBERS:
            raise DsLanguageError("the cartridge's texture groups have no number for language " + language["ds"])
    listed = files[languages.LIST_PATH].decode("ascii").split()
    if listed != [language["code"] for language in carried]:
        raise DsLanguageError("the language list names %s, but the set baked %s" % (listed, [language["code"] for language in carried]))
    missing = [code for code in listed[1:] if languages.names_path({"code": code}) not in files]
    if missing:
        raise DsLanguageError("the set lists languages with no overlay: " + ", ".join(missing))


def bake_overlay(set_description, base_language, language, scripts, arm9, cartridge, rom_path, output_folder, record, work_folder, base):
    localized = languages.localized_set(set_description, base_language, language)
    language_record = languages.LanguageRecord(record, language)
    art = ds_set.bake_text_and_menus({"texture_paths": [], "texture_sizes": {}, "animation_texts": base["animation_texts"]}, localized, scripts, arm9, cartridge, rom_path, language_record, work_folder)
    script_assets.bake_language_cues(localized, rom_path, language_record)
    texture_sizes = dict(base["texture_sizes"], **art["texture_sizes"])
    files = dict(art["files"])
    files.update(script_assets.table_files(localized, output_folder, base["motion_lines"], art["narration_lines"], texture_sizes, texture_sizes, list(base["animation_texts"]), art["talk_textures"]))
    overlay = languages.overlay_files(language, files, base["files"])
    overlay_paths = set(overlay) | language_record.overlay_paths
    overlay[languages.names_path(language)] = languages.names_text(overlay_paths, language).encode("utf-8")
    print("ds language %s: %d files differ from English" % (language["code"], len(overlay_paths)))
    return overlay, overlay_paths | {languages.names_path(language)} | language_record.same_paths


def bake_overlays(set_description, scripts, arm9, cartridge, rom_path, output_folder, record, work_folder, base):
    carried = carried_languages(every_language_of(set_description), rom_path, list(scripts))
    files = languages.list_files(carried)
    paths = set(files)
    for language in languages.other_languages(carried):
        overlay, overlay_paths = bake_overlay(set_description, carried[0], language, scripts, arm9, cartridge, rom_path, output_folder, record, work_folder, base)
        files.update(overlay)
        paths |= overlay_paths
    check_listed_languages(files, carried)
    return files, paths
