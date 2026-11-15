import json
import os

import ds_text

TABLE_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), "ds_one_screen.json")
LINE_MARK = "|"
CONTROLS_MARK = "^"
LINE_BREAK = ("control", ds_text.NEW_LINE, ())


class DsOneScreenError(ValueError):
    pass


def load_table(path=TABLE_PATH):
    with open(path, encoding="utf-8") as source:
        return json.load(source)


def symbols_of(text):
    missing = sorted(set(character for character in text if character not in ds_text.CODE_OF_CHARACTER and character not in (LINE_MARK, CONTROLS_MARK)))
    if missing:
        raise DsOneScreenError("the DS font has no %s" % " ".join(repr(character) for character in missing))
    return [None if character == LINE_MARK else ds_text.CODE_OF_CHARACTER[character] for character in text if character != CONTROLS_MARK]


def symbol_of(item):
    if item[0] == "char":
        return item[1]
    if item[1] == ds_text.NEW_LINE:
        return None
    return False


def is_page_end(item):
    return item[0] == "control" and item[1] in ds_text.PAGE_ENDS


def token_runs(items):
    runs = [[]]
    for index, item in enumerate(items):
        if is_page_end(item):
            runs.append([])
        elif symbol_of(item) is not False:
            runs[-1].append(index)
    return runs


def find_span(items, wanted):
    for run in token_runs(items):
        symbols = [symbol_of(items[index]) for index in run]
        for start in range(len(run) - len(wanted) + 1):
            if symbols[start:start + len(wanted)] == wanted:
                return run[start], run[start + len(wanted) - 1]
    return None


def control_groups(span_items):
    groups = []
    joined = False
    for item in span_items:
        if symbol_of(item) is not False:
            joined = False
        elif joined:
            groups[-1].append(item)
        else:
            groups.append([item])
            joined = True
    return groups


def replacement_items(text, groups, source_text):
    if text.count(CONTROLS_MARK) != len(groups):
        raise DsOneScreenError("%r keeps %d control groups, but the cartridge's %r holds %d" % (text, text.count(CONTROLS_MARK), source_text, len(groups)))
    pending = list(groups)
    items = []
    for character in text:
        if character == CONTROLS_MARK:
            items.extend(pending.pop(0))
        elif character == LINE_MARK:
            items.append(LINE_BREAK)
        else:
            items.append(("char", ds_text.CODE_OF_CHARACTER[character]))
    return items


def rewritten_once(items, source_text, target_text):
    span = find_span(items, symbols_of(source_text))
    if span is None:
        return None
    first, last = span
    groups = control_groups(items[first:last + 1])
    symbols_of(target_text)
    return items[:first] + replacement_items(target_text, groups, source_text) + items[last + 1:]


def pages_with_ends(items):
    pages = [[]]
    for item in items:
        pages[-1].append(item)
        if is_page_end(item):
            pages.append([])
    return pages


def page_symbols(page):
    return [symbol_of(item) for item in page if symbol_of(item) is not False and not is_page_end(item)]


def without_pages(items, dropped):
    wanted = {tuple(symbols_of(text)): text for text in dropped}
    pages = pages_with_ends(items)
    kept = []
    carried = []
    used = set()
    for index, page in enumerate(pages):
        if tuple(page_symbols(page)) not in wanted:
            body = page if not carried else carried + page
            kept.append(body)
            carried = []
            continue
        used.add(wanted[tuple(page_symbols(page))])
        carried = carried + [item for item in page if symbol_of(item) is False and not is_page_end(item)]
        if index == len(pages) - 1 and kept:
            last = kept[-1]
            if last and is_page_end(last[-1]):
                kept[-1] = last[:-1]
            kept[-1] = kept[-1] + carried
            carried = []
    if carried:
        kept.append(carried)
    return [item for page in kept for item in page], used


def one_screen_items(items, entry):
    used = set()
    for source_text, target_text in entry.get("rewrites", []):
        if source_text in target_text.replace(CONTROLS_MARK, ""):
            raise DsOneScreenError("the rewrite of %r holds its own source, so it would never stop" % source_text)
        while True:
            rewritten = rewritten_once(items, source_text, target_text)
            if rewritten is None:
                break
            items = rewritten
            used.add(source_text)
    kept, dropped = without_pages(items, entry.get("dropped_pages", []))
    return kept, used | dropped


def plain_text(items):
    return "".join(" " if symbol_of(item) is None else ("\n" if is_page_end(item) else ds_text.CHARACTER_OF_CODE.get(item[1], ds_text.UNKNOWN_CHARACTER)) for item in items if symbol_of(item) is not False or is_page_end(item))


def refused_phrase(items, entry):
    text = plain_text(items).lower()
    return next((phrase for phrase in entry.get("refused", []) if phrase.lower() in text), None)


class OneScreen:
    def __init__(self, language, table=None):
        self.entry = (load_table() if table is None else table).get(language, {})
        self.used = set()

    def message(self, label, message):
        items, used = one_screen_items(message["items"], self.entry)
        self.used |= used
        phrase = refused_phrase(items, self.entry)
        if phrase is not None:
            raise DsOneScreenError("DS message %s still names the DS's second screen (%r): add its single-screen wording to %s" % (label, phrase, os.path.basename(TABLE_PATH)))
        if items == message["items"]:
            return message
        return dict(message, items=items, pages=ds_text.pages_of_items(items))

    def messages(self, by_label):
        return {label: self.message(label, message) for label, message in by_label.items()}

    def unused(self):
        listed = [source for source, _ in self.entry.get("rewrites", [])] + list(self.entry.get("dropped_pages", []))
        return [text for text in listed if text not in self.used]
