import os
import sys

import asset_writers
import bake_record
import ds_rom

ASSET_TOOLS_FOLDER = os.path.dirname(os.path.abspath(__file__))
SRC_VITA_FOLDER = os.path.dirname(os.path.dirname(ASSET_TOOLS_FOLDER))
sys.path.append(os.path.join(os.path.dirname(SRC_VITA_FOLDER), "tools"))

import lmg_decode
import lz

SCRIPTS_FOLDER = "scripts"
SCRIPT_EXTENSION = ".lmg"
XML_SUFFIX = ".xml"
ROM_SUFFIX = ".lz"
SCRIPT_CHANGE_OP = 0x24
ACT_OP = 0x19
ACT_END_LOW_OPERAND = 1
ACT_END_HIGH_OPERAND = 2
ACT_NAME_OPERAND = 3
SCRIPT_CODE_FILES = ["ds_rom.py", "script_assets.py", "../../../tools/lz.py"]


class ScriptBakeError(ValueError):
    pass


def script_name(path):
    flat = path.replace("/", "-")
    if flat.endswith(XML_SUFFIX):
        return flat[:-len(XML_SUFFIX)]
    return flat


def baked_path(path):
    name = script_name(path)
    try:
        asset_writers.check_name(name)
    except ValueError as error:
        raise ScriptBakeError(path + ": " + str(error)) from None
    return SCRIPTS_FOLDER + "/" + name + SCRIPT_EXTENSION


def read_one_script(rom, record, code, path):
    relative_path = baked_path(path)
    packed = rom.read_file(path + ROM_SUFFIX)
    key = record.key_for(code, {"path": path}, packed)
    current = record.read_if_current(relative_path, key)
    if current is not None:
        return current, None
    plain = lz.try_decompress(packed)
    if plain is None:
        raise ScriptBakeError(path + " in the DS ROM could not be decompressed")
    return plain, key


def store_new_scripts(record, scripts):
    for path, (plain, key) in scripts.items():
        if key is not None:
            record.store(baked_path(path), key, plain)


def decoded_script(path, plain):
    try:
        _, strings, _, items, errors = lmg_decode.decode(plain)
    except ValueError as error:
        raise ScriptBakeError(path + ": " + str(error)) from None
    if errors:
        raise ScriptBakeError(path + ": " + errors[0])
    return strings, [item for item in items if item[1] == "op"]


def act_span(strings, operations, act_name):
    for position, _, op, operands in operations:
        if op == ACT_OP and strings.get(operands[ACT_NAME_OPERAND], "") == act_name:
            return position, operands[ACT_END_LOW_OPERAND] | (operands[ACT_END_HIGH_OPERAND] << 16)
    return None


def script_changes_nothing_explains(section, path, plain):
    strings, operations = decoded_script(path, plain)
    known = set(section["files"]) | set(section.get("never_taken", []))
    stop_span = act_span(strings, operations, section.get("stop_at_act"))
    unexplained = []
    for position, _, op, operands in operations:
        if op != SCRIPT_CHANGE_OP:
            continue
        target = strings.get(operands[0])
        stopped_first = stop_span is not None and stop_span[0] <= position < stop_span[1]
        if target not in known and not stopped_first:
            unexplained.append(path + " can change to " + str(target))
    return unexplained


def check_script_changes(section, baked):
    unexplained = []
    for path, plain in baked.items():
        unexplained.extend(script_changes_nothing_explains(section, path, plain))
    if unexplained:
        raise ScriptBakeError("; ".join(unexplained) + ", which the set's scripts section does not list")


def scripts_already_baked(output_folder):
    return [SCRIPTS_FOLDER + "/" + name for name in os.listdir(os.path.join(output_folder, SCRIPTS_FOLDER))]


def bake_or_keep_scripts(set_description, rom_path, output_folder, record):
    section = set_description.get("scripts")
    if section is None:
        return []
    if not os.path.isfile(rom_path):
        print("warning: no DS ROM at " + rom_path + ", so no scripts were baked", file=sys.stderr)
        return scripts_already_baked(output_folder)
    code = bake_record.code_fingerprint(SCRIPT_CODE_FILES)
    with ds_rom.DsRom(rom_path) as rom:
        scripts = {path: read_one_script(rom, record, code, path) for path in section["files"]}
    check_script_changes(section, {path: plain for path, (plain, _) in scripts.items()})
    store_new_scripts(record, scripts)
    return [baked_path(path) for path in section["files"]]
