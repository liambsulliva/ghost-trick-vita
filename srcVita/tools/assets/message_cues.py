import struct

CONTROL_CODE_BASE = 0xff00
END_OF_MESSAGE = 0xfffe
PARAMETER_COUNTS = (0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 2, 2, 1, 0, 4, 1, 0, 0, 1, 2, 1, 1, 1, 3, 1, 2, 1, 0, 1, 0, 1, 1, 0, 2, 2, 2, 0, 1, 1, 2, 1, 0, 0)
NEW_PAGE_CODE = 0x02
GROUP_HOLD_CODE = 0x0c
PAGE_LIMIT_CODE = 0x15
NEW_GROUP_CODE = 0x16
PAGE_OPTIONS_CODE = 0x22
OPTIONS_MASK = 0xff


class CueError(ValueError):
    pass


def new_page():
    return {"limit": 0, "options": 0, "groups": 1, "holds": {}}


def control_codes(data, offset):
    position = offset
    while True:
        try:
            word = struct.unpack_from("<H", data, position)[0]
        except struct.error:
            raise CueError("a message runs past the end of its file") from None
        if word == END_OF_MESSAGE:
            return
        if word < CONTROL_CODE_BASE:
            position = position + 2
            continue
        code = word - CONTROL_CODE_BASE
        count = PARAMETER_COUNTS[code] if code < len(PARAMETER_COUNTS) else 0
        parameters = struct.unpack_from("<%dH" % count, data, position + 2)
        yield code, parameters
        position = position + 2 + 2 * count


def pages_of_message(data, offset):
    pages = [new_page()]
    for code, parameters in control_codes(data, offset):
        page = pages[-1]
        if code == GROUP_HOLD_CODE:
            page["holds"][page["groups"]] = parameters[0]
        elif code == NEW_GROUP_CODE:
            page["groups"] = page["groups"] + 1
        elif code == PAGE_OPTIONS_CODE:
            page["options"] = parameters[0] & OPTIONS_MASK
        elif code == PAGE_LIMIT_CODE:
            page["limit"] = parameters[0]
            pages.append(new_page())
        elif code == NEW_PAGE_CODE:
            pages.append(new_page())
    last = pages[-1]
    if len(pages) > 1 and last["groups"] == 1 and not last["holds"] and last["limit"] == 0:
        pages.pop()
    return pages


def page_line(page):
    holds = [str(page["holds"].get(index + 1, 0)) for index in range(page["groups"])]
    return "page %d %d %d %s" % (page["limit"], page["options"], page["groups"], " ".join(holds))


def label_targets(labels):
    targets = {}
    for target, names in labels.items():
        for name in names:
            targets[name] = target
    return targets


def cue_text(language_data, labels, wanted_labels):
    targets = label_targets(labels)
    lines = []
    for label in wanted_labels:
        if label not in targets:
            raise CueError("the language file has no message named " + label)
        lines.append("message " + label)
        for page in pages_of_message(language_data, targets[label]):
            lines.append(page_line(page))
    return "\n".join(lines) + "\n"
