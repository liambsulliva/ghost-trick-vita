import os
import shutil
import subprocess
import sys
import tempfile

SRC_VITA_FOLDER = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

MUTATIONS = [
    ("src/app/animation.c", "player->frame_index + 1 < animation->frame_count", "player->frame_index < animation->frame_count"),
    ("src/app/animation.c", "player->time_in_frame_us < duration_us", "player->time_in_frame_us <= duration_us"),
    ("src/app/animation.c", "player->is_paused", "0"),
    ("src/app/animation.c", "frame->ticks == 0", "frame->ticks == 99"),
    ("src/app/texture_cache.c", "entries[index].last_used_frame == current_frame", "0"),
    ("src/app/texture_cache.c", "entries[index].last_used_frame < oldest->last_used_frame", "entries[index].last_used_frame > oldest->last_used_frame"),
    ("src/app/texture_cache.c", "totals.bytes_in_use + bytes_needed > budget", "totals.bytes_in_use + bytes_needed > budget * 100"),
    ("src/app/texture_cache.c", "entry->is_missing = 1;", "entry->is_missing = 0;"),
    ("src/app/texture_file.c", "header->pixel_data_size != header->width * header->height * 4", "0"),
    ("src/app/texture_file.c", "header->width > TEXTURE_FILE_MAX_DIMENSION || header->height > TEXTURE_FILE_MAX_DIMENSION", "0"),
    ("src/app/ui_list.c", "moved = moved + list->item_count;", "moved = 0;"),
    ("src/app/ui_list.c", "list->first_visible = list->selected - list->visible_rows + 1;", "list->first_visible = list->selected;"),
    ("src/app/command_file.c", "!character_is_safe_in_name(name[index])", "0"),
    ("src/app/frame_stats.c", "frame_us > FRAME_STATS_SLOW_FRAME_US", "frame_us < FRAME_STATS_SLOW_FRAME_US"),
    ("src/app/bmp_image.c", "row[pixel * 3] = rgba_pixels[pixel * 4 + 2];", "row[pixel * 3] = rgba_pixels[pixel * 4];"),
    ("src/app/viewer_app.c", "area.y = SCENE_SCREEN_TOP + y * scale_y;", "area.y = y * scale_y;"),
    ("src/app/viewer_app.c", "renderer_draw_filled_rect(area, missing_texture_color);", "return;"),
    ("src/app/viewer_app.c", "list_is_open = 0;\n    }\n    if (input->pressed & (INPUT_CIRCLE", "list_is_open = 1;\n    }\n    if (input->pressed & (INPUT_CIRCLE"),
    ("src/app/debug_font.c", "(column >> y) & 1", "(column >> x) & 1"),
    ("src/app/run_log.c", "log_line_count = log_line_count + 1;", ""),
]


def tests_pass(folder):
    result = subprocess.run(["make", "-B", "-j8", "test"], cwd=folder, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    return result.returncode == 0


def main():
    survivors = []
    invalid = []
    with tempfile.TemporaryDirectory() as temporary_root:
        folder = os.path.join(temporary_root, "srcVita")
        shutil.copytree(SRC_VITA_FOLDER, folder, ignore=shutil.ignore_patterns("build"))
        if not tests_pass(folder):
            print("baseline tests fail; stopping")
            return 1
        for relative_path, old_text, new_text in MUTATIONS:
            path = os.path.join(folder, relative_path)
            with open(path, encoding="utf-8") as source:
                original = source.read()
            if old_text not in original:
                invalid.append((relative_path, old_text))
                continue
            with open(path, "w", encoding="utf-8") as output:
                output.write(original.replace(old_text, new_text, 1))
            caught = not tests_pass(folder)
            print("caught  " if caught else "SURVIVED", relative_path, "|", old_text[:60].replace("\n", " "))
            if not caught:
                survivors.append((relative_path, old_text))
            with open(path, "w", encoding="utf-8") as output:
                output.write(original)
    print("mutations:", len(MUTATIONS), "survivors:", len(survivors), "invalid:", len(invalid))
    for entry in invalid:
        print("invalid", entry[0], entry[1][:60])
    return 1 if survivors or invalid else 0


if __name__ == "__main__":
    sys.exit(main())
