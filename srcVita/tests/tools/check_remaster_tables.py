import argparse
import os
import re
import struct
import sys

SRC_VITA = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(SRC_VITA, "tools", "assets"))
sys.path.insert(0, os.path.join(SRC_VITA, "..", "tools"))

import chapter_assets
import ds_rom
import pack_files
import remaster_tables
import resource_list

REPOSITORY = os.path.dirname(SRC_VITA)
DEFAULT_ROM = os.path.join(REPOSITORY, "extracted", "ds_rom", "game.nds")
DEFAULT_APK_FOLDER = os.path.join(REPOSITORY, "extracted", "remaster_apk")
LANGUAGE_FILE = re.compile(r"\.(en|fr|gr|it|sp|jp)\.")
SILENT_SOUND_KIND = 0
REMASTER_EFFECT_CHANGES = {1, 2, 3, 7, 8, 10, 11, 12, 13, 61, 63, 209}
ROW_END = 0xffff


def script_paths(rom, directory=0, prefix=""):
    for name, _, child in rom.directory_entries(directory):
        if child is not None:
            yield from script_paths(rom, child, prefix + name + "/")
        elif name.endswith(".xml.lz") and not LANGUAGE_FILE.search(name):
            yield prefix + name[:-len(".lz")]


def rooms_the_scripts_enter(rom):
    rooms = set()
    for path in script_paths(rom):
        try:
            script = chapter_assets.read_script(rom, path)
        except (chapter_assets.ChapterBakeError, ValueError):
            continue
        rooms.update(chapter_assets.rooms_used({path: script}))
    return rooms


def ds_room_rows(arm9, room):
    _, _, rows_address = struct.unpack_from("<HHI", arm9, chapter_assets.ROOM_TABLE_ADDRESS - chapter_assets.ARM9_LOAD_ADDRESS + chapter_assets.ROOM_ROW_SIZE * room)
    rows = []
    position = rows_address - chapter_assets.ARM9_LOAD_ADDRESS
    while rows_address and 0 <= position < len(arm9) - 8:
        row = list(struct.unpack_from("<4H", arm9, position))
        position += 8
        rows.append(row)
        if row[0] == ROW_END:
            break
    return rows


def check_rooms(tables, arm9, entered):
    failures = []
    for room in sorted(entered):
        remaster = tables["rooms"].get(str(room))
        width, height, _ = struct.unpack_from("<HHI", arm9, chapter_assets.ROOM_TABLE_ADDRESS - chapter_assets.ARM9_LOAD_ADDRESS + chapter_assets.ROOM_ROW_SIZE * room)
        if remaster is None or remaster["size"] != [width, height] or remaster["rows"] != ds_room_rows(arm9, room):
            failures.append("room %d differs" % room)
    return failures


def check_sounds(tables, arm9):
    failures = []
    for kind, row in enumerate(tables["sound_rows"]):
        index, group, _, _ = struct.unpack_from("<hhHH", arm9, chapter_assets.SOUND_TABLE_ADDRESS - chapter_assets.ARM9_LOAD_ADDRESS + chapter_assets.SOUND_ROW_SIZE * kind)
        if row is None:
            if kind != SILENT_SOUND_KIND:
                failures.append("sound kind %d has no name the converter reads" % kind)
        elif row != [index, group]:
            failures.append("sound kind %d is %s, the DS has %s" % (kind, row, [index, group]))
    return failures


def check_trickless(tables, arm9):
    ds = set()
    remaster = set()
    for info, row in enumerate(tables["core_infos"]):
        fields = struct.unpack_from("<4H", arm9, chapter_assets.CORE_INFO_TABLE_ADDRESS - chapter_assets.ARM9_LOAD_ADDRESS + chapter_assets.CORE_INFO_ROW_SIZE * info)
        if fields[chapter_assets.CORE_INFO_ACTION_FIELD] in chapter_assets.CORE_INFO_NO_ACTIONS:
            ds.add(info)
        if row is not None and row[chapter_assets.CORE_INFO_ACTION_FIELD] == tables["no_action_label"]:
            remaster.add(info)
    if ds != remaster:
        return ["trickless cores differ: DS only %s, remaster only %s" % (sorted(ds - remaster)[:10], sorted(remaster - ds)[:10])]
    return []


def check_faces(tables, arm9, packs):
    failures = []
    listed = dict(resource_list.read_list(packs, "texture", chapter_assets.FACE_LIST_NAME))
    paths = chapter_assets.face_paths(packs)
    for face_id, row in enumerate(tables["face_rows"]):
        resource, _, flip, _, _, _, _ = struct.unpack_from("<7H", arm9, chapter_assets.FACE_TABLE_ADDRESS - chapter_assets.ARM9_LOAD_ADDRESS + chapter_assets.FACE_ROW_SIZE * face_id)
        if face_id == 0:
            continue
        if listed.get(row[0]) != chapter_assets.face_texture_of(paths, resource) or row[2] != flip:
            failures.append("face %d: remaster %s flip %d, DS %s flip %d" % (face_id, listed.get(row[0]), row[2], chapter_assets.face_texture_of(paths, resource), flip))
    return failures


def check_effects(tables, arm9):
    failures = []
    for effect_id, kind in enumerate(tables["effect_kinds"]):
        ds_kind = chapter_assets.effect_kind_of(arm9, effect_id)
        if ds_kind >= chapter_assets.FIRST_CLASS_EFFECT_KIND:
            if kind != ds_kind:
                failures.append("class effect %d is kind %d, the DS has %d" % (effect_id, kind, ds_kind))
        elif kind != chapter_assets.effect_list_row_of_kind(ds_kind) and effect_id not in REMASTER_EFFECT_CHANGES:
            failures.append("effect %d lists row %d, the DS row %d" % (effect_id, kind, chapter_assets.effect_list_row_of_kind(ds_kind)))
    first_card = tables["symbols"][remaster_tables.CHAPTER_CARD_EFFECT]
    first_kind = chapter_assets.effect_kind_of(arm9, first_card)
    ds_cards = set(effect_id for effect_id in range(len(tables["effect_kinds"])) if chapter_assets.effect_kind_of(arm9, effect_id) >= chapter_assets.FIRST_CLASS_EFFECT_KIND and chapter_assets.effect_factory_of(arm9, chapter_assets.effect_kind_of(arm9, effect_id)) == chapter_assets.effect_factory_of(arm9, first_kind))
    factories = tables["class_factories"]
    card_factory = factories[tables["effect_kinds"][first_card] - chapter_assets.FIRST_CLASS_EFFECT_KIND]
    remaster_cards = set(effect_id for effect_id, kind in enumerate(tables["effect_kinds"]) if kind >= chapter_assets.FIRST_CLASS_EFFECT_KIND and factories[kind - chapter_assets.FIRST_CLASS_EFFECT_KIND] == card_factory)
    if ds_cards != remaster_cards:
        failures.append("chapter card effects differ: DS %s, remaster %s" % (sorted(ds_cards), sorted(remaster_cards)))
    return failures


def main():
    parser = argparse.ArgumentParser(description="Checks the tables remaster_tables.py reads from the Android package against the DS cartridge's own tables.")
    parser.add_argument("--ds-rom", default=DEFAULT_ROM)
    parser.add_argument("--apk-folder", default=DEFAULT_APK_FOLDER)
    arguments = parser.parse_args()
    tables = remaster_tables.load_tables(arguments.apk_folder)
    packs = pack_files.PackSet(arguments.apk_folder)
    with ds_rom.DsRom(arguments.ds_rom) as rom:
        arm9 = chapter_assets.read_arm9(rom)
        entered = rooms_the_scripts_enter(rom)
    failures = check_rooms(tables, arm9, entered) + check_sounds(tables, arm9) + check_trickless(tables, arm9) + check_faces(tables, arm9, packs) + check_effects(tables, arm9)
    for failure in failures:
        print("FAIL " + failure)
    print("remaster tables: %d rooms the scripts enter, %d sounds, %d cores, %d faces, %d effects checked, %d failures" % (len(entered), len(tables["sound_rows"]), len(tables["core_infos"]), len(tables["face_rows"]), len(tables["effect_kinds"]), len(failures)))
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
