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
    ("src/app/scene_view.c", "start = view_start_centred_on_camera(camera_start, camera_length, view_length);", "start = camera_start;"),
    ("src/app/scene_view.c", "return camera_start - (view_length - camera_length) / 2.0f;", "return camera_start;"),
    ("src/app/scene_view.c", "return effects->mask_is_on && !effects->iris_is_on;", "return 0;"),
    ("src/app/scene_view.c", "return effects->mask_is_on && !effects->iris_is_on;", "return effects->mask_is_on;"),
    ("src/app/scene_view.c", "return effects->mask_is_on && !effects->iris_is_on;", "return !effects->iris_is_on;"),
    ("src/app/scene_view.c", "view.left = view_start_centred_on_camera(stage->camera_x, SCENE_VIEW_DS_SCREEN_WIDTH, SCENE_VIEW_WIDTH_IN_DS_PIXELS);", "view.left = view_start_along_axis(stage->camera_x, SCENE_VIEW_DS_SCREEN_WIDTH, SCENE_VIEW_WIDTH_IN_DS_PIXELS, view.art.left, view.art.right);"),
    ("src/app/scene_view.c", "view.top = view_start_centred_on_camera(stage->camera_y, SCENE_VIEW_DS_SCREEN_HEIGHT, SCENE_VIEW_HEIGHT_IN_DS_PIXELS);", "view.top = view_start_along_axis(stage->camera_y, SCENE_VIEW_DS_SCREEN_HEIGHT, SCENE_VIEW_HEIGHT_IN_DS_PIXELS, view.art.top, view.art.bottom);"),
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
    ("src/app/screen_changes.c", "is_first_picture || hash != changes->last_hash", "is_first_picture"),
    ("src/app/screen_changes.c", "if (!is_first_picture) {", "if (1) {"),
    ("src/app/screen_changes.c", "changes->still_run = 0;", ""),
    ("src/app/screen_changes.c", "changes->longest_still = changes->still_run;\n}", "changes->longest_still = 0;\n}"),
    ("src/app/screen_changes.c", "x = (row_number % 2 == 0) ? 0 : SCREEN_CHANGES_GRID_STEP / 2;", "x = 0;"),
    ("src/app/screen_changes.c", "row = pixels + y * pixels_per_row;", "row = pixels + y * width;"),
    ("src/app/scene_stage.c", "(to + frame_count - from - 1) % frame_count", "0"),
    ("src/app/animation.c", "player->frame_index = (unsigned int)next_index;\n    }", "player->frame_index = (unsigned int)next_index;\n        player->frame_was_shown[player->frame_index] = 1;\n    }"),
    ("src/app/scene.c", "step->ticks > 0;\n    }\n    if (text_line_starts_with(line, \"camera \"))", "1;\n    }\n    if (text_line_starts_with(line, \"camera \"))"),
    ("src/app/scene.c", "if (scene->step_count >= SCENE_STEP_MAX) {", "if (0) {"),
    ("src/app/viewer_app.c", "list_is_open = 0;\n    }\n    if (input->pressed & (INPUT_CIRCLE", "list_is_open = 1;\n    }\n    if (input->pressed & (INPUT_CIRCLE"),
    ("src/app/viewer_app.c", "hud_is_visible = 0;\n        debug_controls_are_on = 0;", "debug_controls_are_on = 0;"),
    ("src/app/viewer_app.c", "debug_controls_are_on = 0;\n        scene_player_play", "scene_player_play"),
    ("src/app/viewer_app.c", "} else if (debug_controls_are_on) {", "} else if (1) {"),
    ("src/app/viewer_app.c", "x = x - (frame->x + frame->width / 2.0f);", ""),
    ("src/app/viewer_app.c", "y = y - (frame->y + frame->height / 2.0f);", ""),
    ("src/app/viewer_app.c", "if (!animation_frame_is_blank(&animation->frames[index])) {", "if (1) {"),
    ("src/app/viewer_app.c", "x = scene_stage_view()->camera_x + BROWSE_CENTRE_ACROSS_CAMERA;", "x = BROWSE_CENTRE_ACROSS_CAMERA;"),
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
    ("src/app/texture_cache.c", "reads_in_flight < TEXTURE_CACHE_READS_AT_ONCE", "reads_in_flight < TEXTURE_CACHE_READS_AT_ONCE * 2"),
    ("src/app/texture_cache.c", "entry->last_used_frame > other->last_used_frame", "entry->last_used_frame < other->last_used_frame"),
    ("src/app/texture_cache.c", "entry->request_id < other->request_id", "entry->request_id > other->request_id"),
    ("src/app/texture_cache.c", "|| entries[index].read_is_started) {", ") {"),
    ("src/app/texture_cache.c", "reads_in_flight = reads_in_flight - 1;", ""),
    ("src/app/texture_cache.c", "mem_free(finished->data);\n    start_waiting_reads();", "mem_free(finished->data);"),
    ("src/app/texture_cache.c", "entry->in_use = 1;\n    start_waiting_reads();", "entry->in_use = 1;"),
    ("src/app/texture_cache.c", "if (finished->result == FILE_ERR_MEMORY) {", "if (0) {"),
    ("src/app/texture_cache.c", "out_of_memory_was_logged = 1;", ""),
    ("src/app/texture_cache.c", "mark_missing(entry, \"read_refused\", 0);", "entry->read_is_started = 1;"),
    ("src/app/texture_cache.c", "mark_missing(entry, \"path_too_long\", 0);", "entry->read_is_started = 1;"),
    ("src/app/texture_cache.c", "&& !entries[index].read_is_started) {", ") {"),
    ("src/app/texture_cache.c", "texture_cache_drop_waiting_reads();\n    while (reads_in_flight > 0)", "while (reads_in_flight > 0)"),
    ("src/app/viewer_app.c", "texture_cache_drop_waiting_reads();", ""),
    ("src/app/scene_stage.c", "return texture != NULL && texture->state != CACHED_TEXTURE_LOADING;", "return texture == NULL || texture->state != CACHED_TEXTURE_LOADING;"),
    ("src/app/texture_cache.c", "entry->memory_bytes = memory_bytes;", "entry->memory_bytes = header.pixel_data_size;"),
    ("src/app/spotlight_mask.c", "return (int)(x + 0.5f);", "return (int)x;"),
    ("src/app/spotlight_mask.c", "if (x >= (float)RENDERER_WIDTH) {", "if (0) {"),
    ("src/app/spotlight_mask.c", "triangle->base_y == triangle->apex_y", "0"),
    ("src/app/spotlight_mask.c", "progress < 0.0f || progress > 1.0f", "progress < 0.0f"),
    ("src/app/spotlight_mask.c", "centre = triangle->apex_x + (triangle->base_x - triangle->apex_x) * progress;", "centre = triangle->base_x;"),
    ("src/app/spotlight_mask.c", "half_width = triangle->half_width * progress;", "half_width = triangle->half_width;"),
    ("src/app/spotlight_mask.c", "return span->left < span->right;", "return 1;"),
    ("src/app/spotlight_mask.c", "spans[index - 1].left > span.left", "0"),
    ("src/app/spotlight_mask.c", "if (lit_spans[index].left > covered_up_to) {", "if (1) {"),
    ("src/app/spotlight_mask.c", "if (lit_spans[index].right > covered_up_to) {", "if (1) {"),
    ("src/app/spotlight_mask.c", "if (covered_up_to < RENDERER_WIDTH) {", "if (0) {"),
    ("src/app/spotlight_mask.c", "(int)rect->x == left && (int)rect->width == right - left", "0"),
    ("src/app/spotlight_mask.c", "builder->open_count = builder->next_open_count;", "builder->open_count = 0;"),
    ("src/app/spotlight_mask.c", "left = keep_between(area.x, 0.0f, RENDERER_WIDTH);", "left = area.x;"),
    ("src/app/spotlight_mask.c", "bottom = keep_between(area.y + area.height, 0.0f, RENDERER_HEIGHT);", "bottom = area.y + area.height;"),
    ("src/app/spotlight_mask.c", "count = add_rect_if_visible(rects, count, right, top, RENDERER_WIDTH - right, bottom - top);", ""),
    ("src/app/scene_effects.c", "ramp->elapsed_us >= ramp->total_us", "0"),
    ("src/app/scene_effects.c", "ramp->start + (ramp->target - ramp->start) * ((float)ramp->elapsed_us / (float)ramp->total_us)", "ramp->target"),
    ("src/app/scene_effects.c", "ramp->start = ramp->value;", "ramp->start = 0.0f;"),
    ("src/app/scene_effects.c", "ramp->elapsed_us = 0;\n", ""),
    ("src/app/scene_effects.c", "if (ramp->total_us == 0) {\n        ramp->value = target;\n    }", ""),
    ("src/app/scene_effects.c", "ramp->value = value;\n    ramp->total_us = 0;", "ramp->value = value;"),
    ("src/app/scene_effects.c", "ramp_set(&effects->brightness, 1.0f);", ""),
    ("src/app/scene_effects.c", "ramp_advance(&effects->beams[index].base_x, elapsed_us);", ""),
    ("src/app/scene_effects.c", "ramp_advance(&effects->beams[index].base_y, elapsed_us);", ""),
    ("src/app/scene_effects.c", "ramp_advance(&effects->beams[index].half_width, elapsed_us);", ""),
    ("src/app/scene_effects.c", "ramp_advance(&effects->iris_radius, elapsed_us);", ""),
    ("src/app/scene_effects.c", "ramp_advance(&effects->brightness, elapsed_us);", ""),
    ("src/app/scene_effects.c", "ramp_advance(&effects->flash_level, elapsed_us);", ""),
    ("src/app/scene_effects.c", "beam = find_beam(effects, name);\n    if (beam == NULL) {\n        beam = find_free_beam(effects);\n    }", "beam = find_free_beam(effects);"),
    ("src/app/scene_effects.c", "beam->in_use = 0;", ""),
    ("src/app/scene_effects.c", "effects->beams[index].in_use && strcmp(effects->beams[index].name, name) == 0", "strcmp(effects->beams[index].name, name) == 0"),
    ("src/app/scene_effects.c", "effects->iris_is_on = 0;", ""),
    ("src/app/scene_effects.c", "if (!effects->iris_is_on) {\n        ramp_set(&effects->iris_radius, 0.0f);\n    }", ""),
    ("src/app/scene_effects.c", "ramp_set(&effects->flash_level, brightness);", ""),
    ("src/app/scene_effects.c", "ramp_begin(&effects->flash_level, 0.0f, ticks);", ""),
    ("src/app/scene_player.c", "scene_effects_advance(scene_stage_effects(), script.hold_remaining_us);", ""),
    ("src/app/scene_player.c", "script.hold_remaining_us = script.hold_remaining_us - elapsed_us;\n    scene_effects_advance(scene_stage_effects(), elapsed_us);", "script.hold_remaining_us = script.hold_remaining_us - elapsed_us;"),
    ("src/app/scene_player.c", "status = SCENE_PLAYER_FINISHED;\n            scene_effects_advance(scene_stage_effects(), elapsed_us);", "status = SCENE_PLAYER_FINISHED;"),
    ("src/app/scene_player.c", "} else {\n        scene_effects_advance(scene_stage_effects(), elapsed_us);\n    }", "}"),
    ("src/app/scene_player.c", "scene_effects_reset(scene_stage_effects());\n    reset_script_progress();\n    is_paused = 0;", "reset_script_progress();\n    is_paused = 0;"),
    ("src/app/scene_player.c", "scene_stage_hide_card();\n    scene_effects_reset(scene_stage_effects());", "scene_stage_hide_card();"),
    ("src/app/scene_player.c", "scene_effects_create_beam(scene_stage_effects(), step->beam_name, step->apex_x, step->apex_y, step->x, step->y, step->half_width);", ""),
    ("src/app/scene_player.c", "scene_effects_animate_beam(scene_stage_effects(), step->beam_name, step->x, step->y, step->half_width, step->ticks);", ""),
    ("src/app/scene_player.c", "scene_effects_delete_beam(scene_stage_effects(), step->beam_name);", ""),
    ("src/app/scene_player.c", "scene_effects_set_mask(scene_stage_effects(), 1);", ""),
    ("src/app/scene_player.c", "scene_effects_set_mask(scene_stage_effects(), 0);", ""),
    ("src/app/scene_player.c", "scene_effects_start_iris(scene_stage_effects(), step->x, step->y, step->radius, step->ticks);", ""),
    ("src/app/scene_player.c", "scene_effects_dim_to(scene_stage_effects(), step->brightness, step->ticks);", ""),
    ("src/app/scene_player.c", "scene_effects_flash(scene_stage_effects(), step->brightness, step->ticks);", ""),
    ("src/app/scene_stage.c", "scene_effects_reset(&stage.effects);", ""),
    ("src/app/scene.c", "return brightness >= 0.0f && brightness <= 1.0f;", "return 1;"),
    ("src/app/scene.c", "&step->half_width) == 6 && step->half_width >= 0.0f;", "&step->half_width) == 6;"),
    ("src/app/scene.c", "&step->ticks) == 5 && step->half_width >= 0.0f && step->ticks >= 0;", "&step->ticks) == 5;"),
    ("src/app/scene.c", "&step->ticks) == 4 && step->radius >= 0.0f && step->ticks >= 0;", "&step->ticks) == 4;"),
    ("src/app/scene.c", "return read_effect_step(line, step);", "return 0;"),
    ("src/app/scene_view.c", "draw_mask_or_iris(&stage->effects, &view);", ""),
    ("src/app/scene_view.c", "draw_dim(&stage->effects);", ""),
    ("src/app/scene_view.c", "draw_flash(&stage->effects);", ""),
    ("src/app/scene_view.c", "draw_black_around_art(&view);\n    draw_mask_or_iris(&stage->effects, &view);", "draw_mask_or_iris(&stage->effects, &view);\n    draw_black_around_art(&view);"),
    ("src/app/scene_view.c", "draw_dim(&stage->effects);\n    draw_flash(&stage->effects);", "draw_flash(&stage->effects);\n    draw_dim(&stage->effects);"),
    ("src/app/scene_view.c", "draw_flash(&stage->effects);\n    if (stage->card_texture[0] != '\\0') {\n        draw_card(stage->card_texture);\n    }", "if (stage->card_texture[0] != '\\0') {\n        draw_card(stage->card_texture);\n    }\n    draw_flash(&stage->effects);"),
    ("src/app/scene_view.c", "return (x - view->left) * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL;", "return x * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL;"),
    ("src/app/scene_view.c", "return (y - view->top) * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL;", "return y * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL;"),
    ("src/app/scene_view.c", "triangles[count].half_width = beam->half_width.value * SCENE_VIEW_SCREEN_PIXELS_PER_DS_PIXEL;", "triangles[count].half_width = beam->half_width.value;"),
    ("src/app/scene_view.c", "if (beam->in_use) {", "if (1) {"),
    ("src/app/scene_view.c", "} else if (effects->mask_is_on) {", "} else if (1) {"),
    ("src/app/scene_view.c", "if (effects->iris_is_on) {", "if (0) {"),
    ("src/app/scene_view.c", "clear_radius = radius * IRIS_PATTERN_CLEAR_SHARE_OF_RADIUS;", "clear_radius = radius;"),
    ("src/app/scene_view.c", "return point_is_within_clear_part_of_iris(0.0f, 0.0f, center_x, center_y, clear_radius)\n        && ", "return "),
    ("src/app/scene_view.c", "&& point_is_within_clear_part_of_iris(RENDERER_WIDTH, 0.0f, center_x, center_y, clear_radius)", ""),
    ("src/app/scene_view.c", "&& point_is_within_clear_part_of_iris(0.0f, RENDERER_HEIGHT, center_x, center_y, clear_radius)", ""),
    ("src/app/scene_view.c", "\n        && point_is_within_clear_part_of_iris(RENDERER_WIDTH, RENDERER_HEIGHT, center_x, center_y, clear_radius)", ""),
    ("src/app/scene_view.c", "if (iris_clears_whole_screen(center_x, center_y, radius)) {", "if (0) {"),
    ("src/app/scene_view.c", "circle.x = center_x - radius;", "circle.x = center_x;"),
    ("src/app/scene_view.c", "circle.y = center_y - radius;", "circle.y = center_y;"),
    ("src/app/scene_view.c", "circle.width = radius * 2.0f;", "circle.width = radius;"),
    ("src/app/scene_view.c", "circle.height = radius * 2.0f;", "circle.height = radius;"),
    ("src/app/scene_view.c", "renderer_draw_filled_rects(rects, spotlight_mask_rects_around_area(circle, rects), black);", "renderer_draw_filled_rects(rects, spotlight_mask_rects_around_area(circle, rects), white);"),
    ("src/app/scene_view.c", "mem_free(pixels);", ""),
    ("src/app/scene_view.c", "if (iris_texture != RENDERER_NO_TEXTURE) {\n        renderer_draw_sprite", "if (1) {\n        renderer_draw_sprite"),
    ("src/app/scene_view.c", "dim_color.alpha = 1.0f - effects->brightness.value;", "dim_color.alpha = effects->brightness.value;"),
    ("src/app/scene_view.c", "if (effects->brightness.value < 1.0f) {", "if (1) {"),
    ("src/app/scene_view.c", "if (effects->flash_level.value > 0.0f) {", "if (1) {"),
    ("src/app/scene_view.c", "flash_color.alpha = effects->flash_level.value;", ""),
    ("src/app/scene_view.c", "rect_count > MASK_RECT_COUNT_WORTH_LOGGING && ", ""),
    ("src/app/scene_view.c", " && rect_count > largest_logged_mask_rect_count", ""),
    ("src/app/viewer_app.c", "scene_view_start();", ""),
    ("src/app/viewer_app.c", "scene_view_stop();", ""),
    ("src/app/iris_pattern.c", "(distance_as_share_of_radius - IRIS_PATTERN_CLEAR_SHARE_OF_RADIUS) / (1.0f - IRIS_PATTERN_CLEAR_SHARE_OF_RADIUS)", "distance_as_share_of_radius"),
    ("src/app/iris_pattern.c", "solid_from = middle - 1.0f;", "solid_from = middle + 4.0f;"),
    ("src/app/iris_pattern.c", "from_middle_y = (float)y + 0.5f - middle;", "from_middle_y = 0.0f;"),
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
