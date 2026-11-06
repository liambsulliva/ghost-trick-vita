import os
import shutil
import signal
import subprocess
import sys
import tempfile

SRC_VITA_FOLDER = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TEST_TIME_LIMIT_SECONDS = 60
BUILD_ARGUMENTS = ["-B", "-j8", "test-binaries"]
TEST_ARGUMENTS = ["test"]

MUTATIONS = [
    ("src/app/animation.c", "frame_index + 1 < animation->frame_count", "frame_index < animation->frame_count"),
    ("src/app/animation.c", "player->time_in_frame_us < duration_us", "player->time_in_frame_us <= duration_us"),
    ("src/app/animation.c", "frame->ticks == 0", "frame->ticks == 99"),
    ("src/app/texture_cache.c", "entries[index].last_used_frame == current_frame", "0"),
    ("src/app/texture_cache.c", "entries[index].last_used_frame < oldest->last_used_frame", "entries[index].last_used_frame > oldest->last_used_frame"),
    ("src/app/texture_cache.c", "totals.bytes_in_use + bytes_needed > budget", "totals.bytes_in_use + bytes_needed > budget * 100"),
    ("src/app/texture_cache.c", "entry->state = CACHED_TEXTURE_MISSING;", "entry->state = CACHED_TEXTURE_READY;"),
    ("src/app/texture_cache.c", "uploads < upload_limit && ", ""),
    ("src/app/texture_cache.c", "entries[index].state != CACHED_TEXTURE_READY", "0"),
    ("src/app/animation.c", "player->late_frame_count = player->late_frame_count + 1;", ""),
    ("src/app/animation.c", "frame_is_ready != NULL && !frame_is_ready(&animation->frames[next_index])", "0"),
    ("src/app/viewer_app.c", "texture_cache_upload_finished_reads(UPLOADS_PER_FRAME);", ""),
    ("src/app/texture_file.c", "header->pixel_data_size != expected_pixel_data_size(header)", "0"),
    ("src/app/texture_file.c", "header->width > TEXTURE_FILE_MAX_DIMENSION || header->height > TEXTURE_FILE_MAX_DIMENSION", "0"),
    ("src/app/ui_list.c", "moved = moved + list->item_count;", "moved = 0;"),
    ("src/app/ui_list.c", "list->first_visible = list->selected - list->visible_rows + 1;", "list->first_visible = list->selected;"),
    ("src/app/command_file.c", "!character_is_safe_in_name(name[index])", "0"),
    ("src/app/frame_stats.c", "frame_us > FRAME_STATS_SLOW_FRAME_US", "frame_us < FRAME_STATS_SLOW_FRAME_US"),
    ("src/app/bmp_image.c", "row[pixel * 3] = rgba_pixels[pixel * 4 + 2];", "row[pixel * 3] = rgba_pixels[pixel * 4];"),
    ("src/app/scene_view.c", "area.y = (y - view->top)", "area.y = (y + view->top)"),
    ("src/app/scene_view.c", "area.x = (x - view->left)", "area.x = (x + view->left)"),
    ("src/app/scene_view.c", "if (start < art_start) {", "if (0) {"),
    ("src/app/scene_view.c", "if (start + view_length > art_end) {", "if (0) {"),
    ("src/app/scene_view.c", "if (art_end - art_start <= view_length) {", "if (0) {"),
    ("src/app/scene_view.c", "start = camera_start - (view_length - camera_length) / 2.0f;", "start = camera_start;"),
    ("src/app/scene_view.c", "plate->y + plate->height > stage->camera_y", "1"),
    ("src/app/scene_view.c", "draw_black_around_art(&view);", ""),
    ("src/app/scene_view.c", "renderer_draw_filled_rect(whole_screen_area(), black);", ""),
    ("src/app/scene_view.c", "renderer_draw_filled_rect(whole_screen_area(), fade_color);", ""),
    ("src/app/scene_view.c", "left = object->x - frame->x - frame->width;", "left = object->x + frame->x;"),
    ("src/app/scene_view.c", "texture_area.width = -frame->u_size;", ""),
    ("src/app/scene_view.c", "stage->objects[index].layer <= layer", "stage->objects[index].layer == layer"),
    ("src/app/scene_view.c", "renderer_draw_filled_rect(screen_area, missing_texture_color);", ""),
    ("src/app/scene_player.c", "progress = 1.0f - (float)script.hold_remaining_us / (float)script.hold_total_us;", "progress = 1.0f;"),
    ("src/app/scene_player.c", "while (script.hold_remaining_us == 0 && script.next_step < current_scene->step_count)", "if (script.hold_remaining_us == 0 && script.next_step < current_scene->step_count)"),
    ("src/app/scene_player.c", "if (texture_cache_loading_count() > 0) {", "if (0) {"),
    ("src/app/scene_player.c", "if (is_paused) {", "if (0) {"),
    ("src/app/scene_player.c", "if (script.next_step == 0) {", "if (0) {"),
    ("src/app/scene.c", "step->ticks > 0;\n    }\n    if (text_line_starts_with(line, \"camera \"))", "1;\n    }\n    if (text_line_starts_with(line, \"camera \"))"),
    ("src/app/scene.c", "if (scene->step_count >= SCENE_STEP_MAX) {", "if (0) {"),
    ("src/app/viewer_app.c", "list_is_open = 0;\n    }\n    if (input->pressed & (INPUT_CIRCLE", "list_is_open = 1;\n    }\n    if (input->pressed & (INPUT_CIRCLE"),
    ("src/app/viewer_app.c", "hud_is_visible = 0;\n        debug_controls_are_on = 0;", "debug_controls_are_on = 0;"),
    ("src/app/viewer_app.c", "debug_controls_are_on = 0;\n        scene_player_play", "scene_player_play"),
    ("src/app/viewer_app.c", "} else if (debug_controls_are_on) {", "} else if (1) {"),
    ("src/app/viewer_app.c", "scene_player_skip_hold();", ""),
    ("src/app/viewer_app.c", "scene_player_set_paused(0);\n    if (scene_player_status() == SCENE_PLAYER_IDLE", "if (scene_player_status() == SCENE_PLAYER_IDLE"),
    ("src/app/viewer_app.c", "scene_player_status() == SCENE_PLAYER_FINISHED && manifest_has_a_scene()", "0"),
    ("src/app/viewer_app.c", "scene_player_status() == SCENE_PLAYER_FINISHED && !debug_controls_are_on", "0"),
    ("src/app/viewer_app.c", "hud_is_visible = should_be_on;\n    list_is_open = 0;", "hud_is_visible = should_be_on;"),
    ("src/app/viewer_app.c", "run_log_write(\"input pressed buttons=0x%04x\", input->pressed);", ""),
    ("src/app/scene_player.c", "script.hold_remaining_us = 0;\n    run_log_write(\"scene hold_skipped", "run_log_write(\"scene hold_skipped"),
    ("src/app/texture_file.c", "header->format != TEXTURE_FILE_FORMAT_RGBA8888 && !dimensions_are_multiples_of_block(header)", "0"),
    ("src/app/debug_font.c", "(column >> y) & 1", "(column >> x) & 1"),
    ("src/app/texture_file.c", "allocated.width = round_up_to_power_of_two(header->width);", "allocated.width = header->width;"),
    ("src/app/texture_file.c", "allocated.height = round_up_to_power_of_two(header->height);", "allocated.height = header->height;"),
    ("src/app/texture_cache.c", "make_room_for(memory_bytes);", "make_room_for(header.pixel_data_size);"),
    ("src/app/texture_cache.c", "entry->memory_bytes = memory_bytes;", "entry->memory_bytes = header.pixel_data_size;"),
]


def make_succeeds(folder, make_arguments):
    process = subprocess.Popen(["make"] + make_arguments, cwd=folder, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, start_new_session=True)
    try:
        return process.wait(timeout=TEST_TIME_LIMIT_SECONDS) == 0
    except subprocess.TimeoutExpired:
        os.killpg(process.pid, signal.SIGKILL)
        process.wait()
        return False


def remove_werror_from_makefile(folder):
    path = os.path.join(folder, "Makefile")
    with open(path, encoding="utf-8") as source:
        text = source.read()
    with open(path, "w", encoding="utf-8") as output:
        output.write(text.replace(" -Werror", ""))


def mutation_outcome(folder):
    if not make_succeeds(folder, BUILD_ARGUMENTS):
        return "invalid"
    if make_succeeds(folder, TEST_ARGUMENTS):
        return "survived"
    return "caught"


def main():
    survivors = []
    invalid = []
    with tempfile.TemporaryDirectory() as temporary_root:
        folder = os.path.join(temporary_root, "srcVita")
        shutil.copytree(SRC_VITA_FOLDER, folder, ignore=shutil.ignore_patterns("build"))
        remove_werror_from_makefile(folder)
        if not make_succeeds(folder, BUILD_ARGUMENTS) or not make_succeeds(folder, TEST_ARGUMENTS):
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
            outcome = mutation_outcome(folder)
            print(outcome, relative_path, "|", old_text[:60].replace("\n", " "), flush=True)
            if outcome == "survived":
                survivors.append((relative_path, old_text))
            if outcome == "invalid":
                invalid.append((relative_path, old_text))
            with open(path, "w", encoding="utf-8") as output:
                output.write(original)
    print("mutations:", len(MUTATIONS), "survivors:", len(survivors), "invalid:", len(invalid))
    for entry in invalid:
        print("invalid", entry[0], entry[1][:60])
    return 1 if survivors or invalid else 0


if __name__ == "__main__":
    sys.exit(main())
