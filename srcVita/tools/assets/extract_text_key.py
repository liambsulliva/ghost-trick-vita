import argparse
import collections
import os
import re
import signal
import struct
import sys
import tempfile
import zipfile
import zlib

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

import message_file
import pack_files

ASSET_TOOLS_FOLDER = os.path.dirname(os.path.abspath(__file__))
REPOSITORY_FOLDER = os.path.dirname(os.path.dirname(os.path.dirname(ASSET_TOOLS_FOLDER)))
DEFAULT_APK_FOLDER = os.path.join(REPOSITORY_FOLDER, "extracted", "remaster_apk")
LIBRARY_MEMBER = "lib/arm64-v8a/libruntime.so"
LIBRARY_MAGIC = b"\x7fELF"
CHECK_MESSAGE_PATH = "natives/android/gamedesign/text/st01/st01_0000.msg.22"
MAY_BE_ANOTHER_BUILD = "this copy may be a different build of the game"
EXIT_CODE_BASE_FOR_SIGNALS = 128
STOP_SIGNALS = (signal.SIGINT, signal.SIGTERM, signal.SIGHUP)

stop_signal_number = None


class ExtractionError(Exception):
    pass


def failure_reason(error):
    return error.strerror or type(error).__name__


def read_library(package_path):
    if not os.path.isfile(package_path):
        raise ExtractionError("package not found: " + package_path)
    with open(package_path, "rb") as source:
        first_bytes = source.read(len(LIBRARY_MAGIC))
        if first_bytes == LIBRARY_MAGIC:
            return first_bytes + source.read()
    if not zipfile.is_zipfile(package_path):
        raise ExtractionError("not a package or a library: " + package_path)
    try:
        with zipfile.ZipFile(package_path) as package:
            return package.read(LIBRARY_MEMBER)
    except KeyError:
        raise ExtractionError("the package has no " + LIBRARY_MEMBER + ": " + package_path) from None
    except (zipfile.BadZipFile, zlib.error, EOFError, NotImplementedError, RuntimeError):
        raise ExtractionError("could not read " + LIBRARY_MEMBER + " from the package: " + package_path) from None


def read_check_message(apk_folder):
    try:
        packs = pack_files.PackSet(apk_folder)
    except (OSError, ValueError, KeyError, IndexError, struct.error, zipfile.BadZipFile):
        raise ExtractionError("the packs are missing or damaged in " + apk_folder + ": copy them out of the package first") from None
    try:
        return packs.read(CHECK_MESSAGE_PATH)
    except (OSError, ValueError, KeyError, zlib.error):
        raise ExtractionError("could not read " + CHECK_MESSAGE_PATH + " from the packs: " + MAY_BE_ANOTHER_BUILD) from None


def read_string_layout(data):
    entry_count, language_count, area_start = message_file.read_header(data)
    name_pointers = []
    all_pointers = set()
    for index in range(entry_count):
        entry = message_file.read_u64(data, message_file.ENTRY_POINTERS_OFFSET + 8 * index)
        name_pointer = message_file.read_u64(data, entry + message_file.ENTRY_NAME_POINTER_OFFSET)
        name_pointers.append(name_pointer)
        all_pointers.add(name_pointer)
        for slot in range(language_count):
            all_pointers.add(message_file.read_u64(data, entry + message_file.ENTRY_TEXT_POINTERS_OFFSET + 8 * slot))
    in_order = sorted(all_pointers)
    if in_order and (in_order[0] < area_start or in_order[-1] >= len(data)):
        raise message_file.MessageFileError("string pointer outside the string area")
    return {
        "area_start": area_start,
        "stored_area": data[area_start:],
        "name_pointers": name_pointers,
        "next_pointers": dict(zip(in_order, in_order[1:] + [len(data)])),
    }


def key_bytes_from_entry_names(layout):
    area_start = layout["area_start"]
    stored_area = layout["stored_area"]
    votes = [collections.Counter() for _ in range(message_file.TEXT_KEY_SIZE)]
    for pointer in layout["name_pointers"]:
        for position in range(pointer - area_start + 1, layout["next_pointers"][pointer] - area_start, 2):
            votes[position & 15][stored_area[position] ^ stored_area[position - 1]] += 1
    return [vote.most_common(1)[0][0] if vote else None for vote in votes]


def candidate_windows(library, known_key_bytes):
    parts = []
    for value in known_key_bytes:
        if value is None:
            parts.append(b".")
        else:
            parts.append(re.escape(bytes([value])))
    pattern = re.compile(b"(?=(" + b"".join(parts) + b"))", re.DOTALL)
    return set(match.group(1) for match in pattern.finditer(library))


def key_decodes_every_string(layout, candidate):
    area_start = layout["area_start"]
    decoded_area = message_file.decode_string_area(layout["stored_area"], candidate)
    try:
        for pointer, next_pointer in layout["next_pointers"].items():
            text = message_file.read_string(decoded_area, area_start, pointer)
            if pointer + len(text.encode("utf-16-le")) + 2 > next_pointer:
                return False
        for pointer in layout["name_pointers"]:
            if not message_file.ENTRY_NAME_PATTERN.fullmatch(message_file.read_string(decoded_area, area_start, pointer)):
                return False
    except message_file.MessageFileError:
        return False
    return True


def find_keys(library, check_message):
    try:
        layout = read_string_layout(check_message)
    except message_file.MessageFileError:
        raise ExtractionError("could not read the layout of " + CHECK_MESSAGE_PATH + ": " + MAY_BE_ANOTHER_BUILD) from None
    known_key_bytes = key_bytes_from_entry_names(layout)
    if all(value is None for value in known_key_bytes):
        return []
    return [window for window in candidate_windows(library, known_key_bytes) if key_decodes_every_string(layout, window)]


def delete_temporary_key(temporary_path):
    try:
        os.unlink(temporary_path)
    except FileNotFoundError:
        pass
    except SystemExit:
        if os.path.exists(temporary_path):
            os.unlink(temporary_path)
        raise


def save_key(apk_folder, key):
    final_path = os.path.join(apk_folder, message_file.TEXT_KEY_FILE_NAME)
    descriptor, temporary_path = tempfile.mkstemp(dir=apk_folder, prefix="text_key_", suffix=".bin")
    try:
        with os.fdopen(descriptor, "wb") as output:
            output.write(key)
            output.flush()
            os.fsync(output.fileno())
        os.replace(temporary_path, final_path)
    except BaseException:
        delete_temporary_key(temporary_path)
        raise
    return final_path


def extract_key(package_path, apk_folder):
    try:
        library = read_library(package_path)
    except OSError as error:
        raise ExtractionError("could not read " + package_path + ": " + failure_reason(error)) from None
    keys = find_keys(library, read_check_message(apk_folder))
    if not keys:
        raise ExtractionError("no key found: " + MAY_BE_ANOTHER_BUILD)
    if len(keys) > 1:
        raise ExtractionError("several different keys found, so none was saved: " + MAY_BE_ANOTHER_BUILD)
    try:
        return save_key(apk_folder, keys[0])
    except OSError as error:
        raise ExtractionError("could not save the key in " + apk_folder + ": " + failure_reason(error)) from None


def exit_so_the_temporary_file_is_deleted(signal_number, _frame):
    global stop_signal_number
    for stop_signal in STOP_SIGNALS:
        signal.signal(stop_signal, signal.SIG_IGN)
    stop_signal_number = signal_number
    sys.exit(EXIT_CODE_BASE_FOR_SIGNALS + signal_number)


def extract_and_report(arguments):
    try:
        saved_path = extract_key(arguments.package, arguments.apk_folder)
    except ExtractionError as error:
        print("error: " + str(error), file=sys.stderr)
        return 1
    except Exception as error:
        print("error: stopped by " + type(error).__name__, file=sys.stderr)
        return 1
    print("saved the text key to " + saved_path)
    return 0


def main():
    for stop_signal in STOP_SIGNALS:
        signal.signal(stop_signal, exit_so_the_temporary_file_is_deleted)
    parser = argparse.ArgumentParser(description="Finds the text key in your own copy of the Android package and saves it beside the packs.")
    parser.add_argument("package", help="your copy of the Android package, or the library taken out of it")
    parser.add_argument("--apk-folder", default=DEFAULT_APK_FOLDER, help="folder holding the remaster's pack files")
    arguments = parser.parse_args()
    exit_code = extract_and_report(arguments)
    if stop_signal_number is not None:
        return EXIT_CODE_BASE_FOR_SIGNALS + stop_signal_number
    return exit_code


if __name__ == "__main__":
    sys.exit(main())
