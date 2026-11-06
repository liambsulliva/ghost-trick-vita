import re
import struct

MESSAGE_MAGIC = b"GMSG"
MESSAGE_VERSION = 22
VERSION_OFFSET = 0
MAGIC_OFFSET = 4
ENTRY_COUNT_OFFSET = 16
LANGUAGE_COUNT_OFFSET = 24
STRING_AREA_OFFSET = 32
ENTRY_POINTERS_OFFSET = 72
ENTRY_NAME_POINTER_OFFSET = 24
ENTRY_TEXT_POINTERS_OFFSET = 40
ENGLISH_SLOT = 1
TEXT_KEY_FILE_NAME = "text_key.bin"
TEXT_KEY_SIZE = 16
ENTRY_NAME_PATTERN = re.compile(r"[A-Za-z0-9_]+")

TAG_PATTERN = re.compile(r"<[^>]*>")
TRCK_TAG_PATTERN = re.compile(r'<TRCK\s+(\w+)((?:\s+\w+="[^"]*")*)\s*/>$')
ATTRIBUTE_PATTERN = re.compile(r'(\w+)="([^"]*)"')
MONO_TYPE_PREFIX = "MONO_TYPE_"
PARAGRAPH_MARK = "\u2029"


class MessageFileError(ValueError):
    pass


class MissingEntryError(MessageFileError):
    pass


def decode_string_area(data, key):
    decoded = bytearray(len(data))
    previous = 0
    for index, value in enumerate(data):
        decoded[index] = value ^ previous ^ key[index & 15]
        previous = value
    return bytes(decoded)


def read_number(data, offset, number_format):
    if offset + struct.calcsize(number_format) > len(data):
        raise MessageFileError("pointer outside the file")
    return struct.unpack_from(number_format, data, offset)[0]


def read_u32(data, offset):
    return read_number(data, offset, "<I")


def read_u64(data, offset):
    return read_number(data, offset, "<Q")


def read_string(string_area, string_area_start, pointer):
    start = pointer - string_area_start
    if start < 0 or start >= len(string_area):
        raise MessageFileError("string pointer outside the string area")
    end = start
    while string_area[end:end + 2] != b"\0\0":
        if end + 2 > len(string_area):
            raise MessageFileError("string has no terminator")
        end = end + 2
    try:
        return string_area[start:end].decode("utf-16-le")
    except UnicodeDecodeError:
        raise MessageFileError("string is not UTF-16") from None


def read_entry_and_name(data, string_area, string_area_start, index):
    try:
        entry = read_u64(data, ENTRY_POINTERS_OFFSET + 8 * index)
        name_pointer = read_u64(data, entry + ENTRY_NAME_POINTER_OFFSET)
        return entry, read_string(string_area, string_area_start, name_pointer)
    except MessageFileError as error:
        raise MessageFileError("entry number %d: %s" % (index, error)) from None


def read_english_text(data, string_area, string_area_start, entry, name):
    try:
        text_pointer = read_u64(data, entry + ENTRY_TEXT_POINTERS_OFFSET + 8 * ENGLISH_SLOT)
        return read_string(string_area, string_area_start, text_pointer)
    except MessageFileError as error:
        raise MessageFileError("entry %s: %s" % (name, error)) from None


def read_header(data):
    if data[MAGIC_OFFSET:MAGIC_OFFSET + 4] != MESSAGE_MAGIC:
        raise MessageFileError("not a GMSG file")
    if read_u32(data, VERSION_OFFSET) != MESSAGE_VERSION:
        raise MessageFileError("not GMSG version " + str(MESSAGE_VERSION))
    entry_count = read_u32(data, ENTRY_COUNT_OFFSET)
    language_count = read_u32(data, LANGUAGE_COUNT_OFFSET)
    if ENGLISH_SLOT >= language_count:
        raise MessageFileError("file has no English slot")
    return entry_count, language_count, read_u64(data, STRING_AREA_OFFSET)


def read_english_entries(data, wanted_names, key):
    entry_count, _, string_area_start = read_header(data)
    string_area = decode_string_area(data[string_area_start:], key)
    wanted = set(wanted_names)
    found = {}
    every_name_is_plain = True
    for index in range(entry_count):
        entry, name = read_entry_and_name(data, string_area, string_area_start, index)
        every_name_is_plain = every_name_is_plain and ENTRY_NAME_PATTERN.fullmatch(name) is not None
        if name in wanted:
            found[name] = read_english_text(data, string_area, string_area_start, entry, name)
    for name in sorted(wanted):
        if name not in found:
            if not every_name_is_plain:
                raise MessageFileError("entry names are not plain text")
            raise MissingEntryError("no such entry: " + name)
    return found


def read_entry_names(data, key):
    entry_count, _, string_area_start = read_header(data)
    string_area = decode_string_area(data[string_area_start:], key)
    return [read_entry_and_name(data, string_area, string_area_start, index)[1] for index in range(entry_count)]


def read_tag(tag_text):
    match = TRCK_TAG_PATTERN.match(tag_text)
    if match is None:
        raise MessageFileError("tag this reader does not know")
    return match.group(1), dict(ATTRIBUTE_PATTERN.findall(match.group(2)))


def number_attribute(verb, attributes, attribute_name, prefix=""):
    value = attributes.get(attribute_name, "")
    try:
        if not value.startswith(prefix):
            raise ValueError
        return int(value[len(prefix):])
    except ValueError:
        raise MessageFileError("%s has no number in %s" % (verb, attribute_name)) from None


def apply_tag(message, verb, attributes):
    if verb == "mes_wait_set":
        message["waits"].append(number_attribute(verb, attributes, "time"))
    elif verb == "mono_pos_set":
        message["pos_y"] = number_attribute(verb, attributes, "posY")
    elif verb == "auto_pf":
        message["auto_pf"] = number_attribute(verb, attributes, "time")
    elif verb == "mono_type_set":
        message["mono_type"] = number_attribute(verb, attributes, "type", MONO_TYPE_PREFIX)
    else:
        raise MessageFileError("verb this reader does not know: " + verb)


def split_paragraphs(text_without_tags):
    paragraphs = re.split(PARAGRAPH_MARK + "\n?", text_without_tags.replace("\r", ""))
    return [paragraph.split("\n") for paragraph in paragraphs]


def parse_message(raw_text):
    message = {"pos_y": None, "auto_pf": None, "mono_type": None, "waits": []}
    plain_parts = []
    position = 0
    for match in TAG_PATTERN.finditer(raw_text):
        plain_parts.append(raw_text[position:match.start()])
        position = match.end()
        verb, attributes = read_tag(match.group(0))
        if verb == "paragraph_end":
            plain_parts.append(PARAGRAPH_MARK)
        else:
            apply_tag(message, verb, attributes)
    plain_parts.append(raw_text[position:])
    message["paragraphs"] = split_paragraphs("".join(plain_parts))
    return message
