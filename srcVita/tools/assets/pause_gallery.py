import re

import numpy as np

import bake_record
import pause_people

ART_PREFIX = "pause_art_"
ART_THUMB_SHEET_NAME = "pause_art_thumbs"
ART_SIZE = (960, 540)
ART_THUMB_SIZE = (104, 58)
ART_THUMB_PITCH = (108, 60)
ART_THUMB_COLUMNS = 18
IMAGE_IN_GALLERY = re.compile(r"GUI/ui040100/tex/(Image\d+)_IML4")


def gallery_order(gallery_bytes):
    order = []
    for name in IMAGE_IN_GALLERY.findall(gallery_bytes.decode("utf-16-le", "ignore")):
        if name not in order:
            order.append(name)
    return order


def art_sources(packs, gallery):
    sources = []
    for name in gallery_order(packs.read(gallery["table"])):
        path = gallery["image"] % name.lower()
        try:
            packs.read(path)
        except KeyError:
            continue
        sources.append((ART_PREFIX + "%03d" % len(sources), path))
    return sources


def thumb_places(count):
    return [((index % ART_THUMB_COLUMNS) * ART_THUMB_PITCH[0], (index // ART_THUMB_COLUMNS) * ART_THUMB_PITCH[1]) for index in range(count)]


def bake_one(packs, tool_path, work_folder, record, code, name, path):
    data = packs.read(path)
    key = record.key_for(code, {"art": ART_SIZE}, data)
    relative_path = "textures/" + name + ".gtex"
    gtex = record.read_if_current(relative_path, key)
    if gtex is None:
        art = pause_people.whole_art(packs, tool_path, work_folder, path)
        gtex = pause_people.encoded(work_folder, name, pause_people.resized(work_folder, art, ART_SIZE, (0, 0, 0)), "dxt1")
        record.store(relative_path, key, gtex)
    return bake_record.result_of_own_size(name, gtex)


def bake_thumbs(packs, tool_path, work_folder, record, code, sources):
    places = thumb_places(len(sources))
    key = record.key_for(code, {"thumb": ART_THUMB_SIZE, "places": places}, b"".join(packs.read(path) for _, path in sources))
    relative_path = "textures/" + ART_THUMB_SHEET_NAME + ".gtex"
    gtex = record.read_if_current(relative_path, key)
    if gtex is None:
        rows = (len(sources) + ART_THUMB_COLUMNS - 1) // ART_THUMB_COLUMNS
        sheet = np.zeros((rows * ART_THUMB_PITCH[1], ART_THUMB_COLUMNS * ART_THUMB_PITCH[0], 4), dtype=np.uint8)
        for (_, path), (x, y) in zip(sources, places):
            art = pause_people.whole_art(packs, tool_path, work_folder, path)
            sheet[y:y + ART_THUMB_SIZE[1], x:x + ART_THUMB_SIZE[0]] = pause_people.resized(work_folder, art, ART_THUMB_SIZE, (0, 0, 0))
        gtex = pause_people.encoded(work_folder, ART_THUMB_SHEET_NAME, sheet, "dxt1")
        record.store(relative_path, key, gtex)
    return bake_record.result_of_own_size(ART_THUMB_SHEET_NAME, gtex)


def table_lines(sources):
    lines = []
    for (name, _), (x, y) in zip(sources, thumb_places(len(sources))):
        lines.append("art %s %d %d %d %d" % (name, x, y, ART_THUMB_SIZE[0], ART_THUMB_SIZE[1]))
    return lines


def bake_gallery(packs, tool_path, work_folder, record, code, gallery):
    sources = art_sources(packs, gallery)
    if not sources:
        return [], []
    results = [bake_one(packs, tool_path, work_folder, record, code, name, path) for name, path in sources]
    results.append(bake_thumbs(packs, tool_path, work_folder, record, code, sources))
    return table_lines(sources), results
