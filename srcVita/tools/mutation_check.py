import os
import shutil
import signal
import subprocess
import sys
import tempfile

SRC_VITA_FOLDER = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TEST_TIME_LIMIT_SECONDS = 60
BUILD_ARGUMENTS = ["-j8", "test-binaries"]
TEST_ARGUMENTS = ["-j8", "test-parallel"]
PYTHON_TOOL_BUILD_ARGUMENTS = ["-C", "tools/assets", "-j8", "../../build/tools/music_reference", "../../build/tools/music_log_writer"]
PYTHON_TEST_COMMAND = [sys.executable, "-I", "-B", "tests/tools/run_tests.py", "test_audio_*.py"]

MUTATIONS = [
    ("src/app/animation.c", "frame_index + 1 < animation->frame_count", "frame_index < animation->frame_count"),
    ("src/app/animation.c", "player->time_in_frame_us < duration_us", "player->time_in_frame_us <= duration_us"),
    ("src/app/animation.c", "frame->ticks == 0", "frame->ticks == 99"),
    ("src/app/animation.c", "frame_total > ANIMATION_FRAME_MAX", "frame_total > ANIMATION_FRAME_MAX + 1"),
    ("src/app/animation.c", "frame_total * sizeof(struct animation_frame)", "(frame_total + 1) * sizeof(struct animation_frame)"),
    ("src/app/animation.c", "if (result != ANIMATION_OK) {\n        animation_free_frames(animation);\n    }", ""),
    ("src/app/animation_library.c", "animation_free_frames(loaded_animations[index]);", ""),
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
    ("src/app/viewer_app.c", "if (viewer_app_show_animation(manifest->animation_names[next])) {\n            return;\n        }", "viewer_app_show_animation(manifest->animation_names[next]);\n        return;"),
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
    ("src/app/crc32.c", "crc = crc ^ 0xFFFFFFFFU;", ""),
    ("src/app/crc32.c", "return crc ^ 0xFFFFFFFFU;", "return crc;"),
    ("src/app/pcm_ramp.c", "ramp->start_gain + (int)(travelled / ramp->frames_total)", "ramp->target_gain"),
    ("src/app/pcm_ramp.c", "#define GAIN_SHIFT 15", "#define GAIN_SHIFT 14"),
    ("src/app/pcm_ramp.c", "ramp->gain = frames > 0 ? start_gain : target_gain;", "ramp->gain = start_gain;"),
    ("src/app/pcm_ramp.c", "ramp->target_gain == 0 && ", ""),
    ("src/app/pcm_ramp.c", "(long long)(ramp->target_gain - ramp->start_gain)", "(ramp->target_gain - ramp->start_gain)"),
    ("src/app/pcm_mix.c", "sum = SAMPLE_MAX;", "sum = SAMPLE_MIN;"),
    ("src/app/pcm_mix.c", "if (sum < SAMPLE_MIN) {", "if (0) {"),
    ("src/app/vorbis_stream.c", "(int)(frame_count * STEREO_CHANNELS)", "(int)frame_count"),
    ("src/app/vorbis_stream.c", "!ogg_pages_are_intact(data, size)", "0"),
    ("src/app/ogg_pages.c", "size < HEADER_BYTES || ", ""),
    ("src/app/ogg_pages.c", " || memcmp(data, CAPTURE_PATTERN, CAPTURE_PATTERN_BYTES) != 0", ""),
    ("src/app/ogg_pages.c", "if (size < HEADER_BYTES + segment_count) {", "if (0) {"),
    ("src/app/ogg_pages.c", "page_size = page_size + data[HEADER_BYTES + segment];", ""),
    ("src/app/ogg_pages.c", "return page_size <= size ? page_size : 0;", "return page_size;"),
    ("src/app/ogg_pages.c", "(crc << BITS_PER_BYTE) ^ ", ""),
    ("src/app/ogg_pages.c", "crc = add_to_crc(crc, zero_crc, OGG_PAGE_CRC_BYTES);", "crc = add_to_crc(crc, &page[OGG_PAGE_CRC_OFFSET], OGG_PAGE_CRC_BYTES);"),
    ("src/app/ogg_pages.c", "0x04c11db7U", "0x04c11db6U"),
    ("src/app/ogg_pages.c", "0x130476dcU", "0x130476ddU"),
    ("src/app/ogg_pages.c", "page_size == 0 || ", ""),
    ("src/app/ogg_pages.c", " || ogg_page_crc(&data[offset], page_size) != stored_crc(&data[offset])", ""),
    ("src/app/ogg_pages.c", "(unsigned int)bytes[3] << 24", "0"),
    ("src/app/ogg_pages.c", "return size > 0;", "return 1;"),
    ("src/app/music_source.c", "source->loop_last + 1 - source->next_frame", "source->loop_last - source->next_frame"),
    ("src/app/music_source.c", "target = source->has_loop_points ? source->loop_start : 0;", "target = 0;"),
    ("src/app/music_source.c", "if (!source->is_looping || !vorbis_stream_seek", "if (!vorbis_stream_seek"),
    ("src/app/music_source.c", "memset(&stereo_frames[filled * SAMPLES_PER_FRAME], 0, (frame_count - filled) * SAMPLES_PER_FRAME * sizeof(short));", ""),
    ("src/app/music_source.c", "source->loop_count = source->loop_count + 1;", ""),
    ("src/app/music_source.c", "if (source->next_frame != 0 && !vorbis_stream_seek(&source->stream, 0)) {\n        return 0;\n    }", ""),
    ("src/app/music_source.c", "} else if (just_went_back || !go_back_to_the_loop_start(source)) {", "} else if (!go_back_to_the_loop_start(source)) {"),
    ("src/app/music_engine.c", "rest == 0 && playback.grains_submitted > 0", "0"),
    ("src/app/music_engine.c", "rest == 0 && playback.grains_submitted > 0", "rest == 0"),
    ("src/app/music_engine.c", "gap_us > GAP_LIMIT_US || ", ""),
    ("src/app/music_engine.c", "gap_us > GAP_LIMIT_US", "gap_us >= GAP_LIMIT_US"),
    ("src/app/music_engine.c", " || playback.last_block_us > BLOCK_LIMIT_US", ""),
    ("src/app/music_engine.c", "playback.last_block_us > BLOCK_LIMIT_US", "playback.last_block_us >= BLOCK_LIMIT_US"),
    ("src/app/music_engine.c", "if (!is_stopping) {\n            begin_ramp(0", "if (1) {\n            begin_ramp(0"),
    ("src/app/music_engine.c", "window.grains == WINDOW_GRAINS", "0"),
    ("src/app/music_engine.c", "audio_port_drain(AUDIO_PORT_MUSIC);", ""),
    ("src/app/music_engine.c", "audio_port_close(AUDIO_PORT_MUSIC);\n    mark_playback_stopped(reason);", "mark_playback_stopped(reason);"),
    ("src/app/music_engine.c", "audio_port_close(AUDIO_PORT_MUSIC);\n    mark_playback_stopped(\"submit_failed\");", "mark_playback_stopped(\"submit_failed\");"),
    ("src/app/music_engine.c", "if (result < 0) {\n        run_log_write(\"music port_open_failed", "if (0) {\n        run_log_write(\"music port_open_failed"),
    ("src/app/music_engine.c", "memset(grain, 0, sizeof(grain));\n    note_window_start();\n    if (!submit_grain(0)) {\n        abandon_playback();\n        return;\n    }\n    log_window_if_any();", "note_window_start();\n    if (!submit_grain(0)) {\n        abandon_playback();\n        return;\n    }\n    log_window_if_any();"),
    ("src/app/music_engine.c", "memset(grain, 0, sizeof(grain));\n    note_window_start();\n    if (!submit_grain(0)) {\n        abandon_playback();\n        return;\n    }\n    begin_ramp", "note_window_start();\n    if (!submit_grain(0)) {\n        abandon_playback();\n        return;\n    }\n    begin_ramp"),
    ("src/app/music_engine.c", "begin_ramp(0, PCM_RAMP_FULL_GAIN, RAMP_IN_FRAMES);\n}", "pcm_ramp_begin(&ramp, PCM_RAMP_FULL_GAIN, PCM_RAMP_FULL_GAIN, 0);\n}"),
    ("src/app/music_engine.c", "(unsigned int)wish.fade_ticks * FRAMES_PER_TICK", "(unsigned int)wish.fade_ticks"),
    ("src/app/music_engine.c", "if (wish.generation != fade_generation) {", "if (0) {"),
    ("src/app/music_engine.c", "fade_generation = handled_generation;", ""),
    ("src/app/music_engine.c", "mailbox.starve_ms = 0;", ""),
    ("src/app/music_engine.c", "if (!worker_is_running) {\n        return 1;\n    }", ""),
    ("src/app/music_engine.c", "is_done = strcmp(prepared_name, name) == 0;", "is_done = 1;"),
    ("src/app/music_engine.c", "prepared_name[0] = '\\0';\n    audio_mailbox_unlock();", "audio_mailbox_unlock();"),
    ("src/app/music_engine.c", "publish_prepared_name(wish->track_name);", ""),
    ("src/app/music_engine.c", "source.stream.sample_rate != MUSIC_RATE", "0"),
    ("src/app/music_engine.c", " || loop_last >= source.stream.frame_count", ""),
    ("src/app/music_engine.c", "loop_last >= source.stream.frame_count", "loop_last > source.stream.frame_count"),
    ("src/app/music_engine.c", "loop_start > loop_last || ", ""),
    ("src/app/music_engine.c", "mem_free(track_data);", ""),
    ("src/app/music_engine.c", "if (track_is_loaded && strcmp(loaded_name, wish->track_name) != 0) {", "if (0) {"),
    ("src/app/music_engine.c", "if (!track_is_loaded && wish->track_name[0] != '\\0') {", "if (wish->track_name[0] != '\\0') {"),
    ("src/app/music_engine.c", "wish->should_play && track_is_loaded", "track_is_loaded"),
    ("src/app/music_engine.c", "window.decode_us_total / window.grains", "window.decode_us_total"),
    ("src/app/music_engine.c", "playback.crc_total = crc32_add(playback.crc_total,", "playback.crc_total = crc32_add(0,"),
    ("src/app/music_engine.c", "window.first_source_frame = source.next_frame;", ""),
    ("src/app/music_engine.c", "is_stopping = 1;", ""),
    ("src/app/music_engine.c", "is_stopping = 0;\n    fade_generation = handled_generation;", "fade_generation = handled_generation;"),
    ("src/app/music_engine.c", "begin_ramp(ramp.gain, 0,", "begin_ramp(PCM_RAMP_FULL_GAIN, 0,"),
    ("src/app/music_engine.c", "if (size >= sizeof(text)) {", "if (0) {"),
    ("src/app/music_engine.c", "size >= sizeof(text)", "size > sizeof(text)"),
    ("src/app/music_engine.c", "size = sizeof(text) - 1;", "size = sizeof(text);"),
    ("src/app/music_engine.c", "if (!music_source_begin(&source, is_looping)) {", "music_source_begin(&source, is_looping);\n    if (0) {"),
    ("src/app/sound_engine.c", "rest == 0 && totals.grains_submitted > 0", "0"),
    ("src/app/sound_engine.c", "window.grains == WINDOW_GRAINS", "0"),
    ("src/app/sound_engine.c", "if (voice < 0) {", "if (0) {"),
    ("src/app/sound_engine.c", "voice->clip = NULL;", ""),
    ("src/app/sound_engine.c", "pcm_mix_add(grain, &voice->clip->frames[voice->next_frame * SAMPLES_PER_FRAME], frames * SAMPLES_PER_FRAME);", ""),
    ("src/app/sound_engine.c", "pcm_ramp_apply(&fade, grain, SOUND_GRAIN_FRAMES);", ""),
    ("src/app/sound_engine.c", "audio_port_drain(AUDIO_PORT_SOUND);", ""),
    ("src/app/sound_engine.c", "audio_port_close(AUDIO_PORT_SOUND);\n    mark_port_closed(\"stop\");", "mark_port_closed(\"stop\");"),
    ("src/app/sound_engine.c", "stream.sample_rate != SOUND_RATE", "0"),
    ("src/app/sound_engine.c", "mailbox.port_is_wanted = 0;\n    mailbox.start_count = 0;", "mailbox.port_is_wanted = 0;"),
    ("src/app/sound_engine.c", "is_done = mailbox.clips_handled >= mailbox.clips_asked;", "is_done = 1;"),
    ("src/app/sound_engine.c", "note_clip_is_handled();", ""),
    ("src/app/sound_engine.c", "requests.port_is_wanted && !port_has_failed", "requests.port_is_wanted"),
    ("src/app/sound_engine.c", "if (requests.stop_was_asked) {\n        port_has_failed = 0;\n    }", ""),
    ("src/app/sound_engine.c", "if (strcmp(mailbox.clip_names[index], name) == 0) {", "if (0) {"),
    ("src/app/sound_engine.c", "mem_free(clips[index].frames);", ""),
    ("src/app/sound_engine.c", "mem_free(data);", ""),
    ("src/app/sound_engine.c", "requests.stop_was_asked || requests.should_shut_down || requests.has_clip_to_load", "requests.stop_was_asked || requests.should_shut_down"),
    ("src/app/sound_engine.c", "clock_now_us() - start->requested_us", "0ULL"),
    ("src/app/sound_engine.c", "memset(voices, 0, sizeof(voices));\n    port_is_open = 0;", "port_is_open = 0;"),
    ("src/app/sound_engine.c", "voices[voice].next_frame = 0;", ""),
    ("src/app/sound_engine.c", "pcm_ramp_begin(&fade, PCM_RAMP_FULL_GAIN, 0, SOUND_GRAIN_FRAMES);", "pcm_ramp_begin(&fade, PCM_RAMP_FULL_GAIN, 0, 1);"),
    ("src/app/sound_engine.c", "if (stream.frame_count == 0) {", "if (0) {"),
    ("src/app/sound_engine.c", "if (clip->frame_count == 0) {", "if (0) {"),
    ("src/app/sound_engine.c", "mem_free(clip->frames);", ""),
    ("src/app/scene.c", "(step->is_looping == 0 || step->is_looping == 1)", "1"),
    ("src/app/scene.c", "step->ticks >= 0", "1"),
    ("src/app/scene_player.c", "music_request_play(step->audio_name, step->is_looping);", ""),
    ("src/app/scene_player.c", "music_request_stop(step->ticks);", ""),
    ("src/app/scene_player.c", "sound_request_play(step->audio_name);", ""),
    ("src/app/scene_player.c", "music_request_prepare(preload_music_name);", ""),
    ("src/app/scene_player.c", "sound_request_prepare(step->audio_name);", ""),
    ("src/app/scene_player.c", "if (!scene_audio_is_ready()) {\n        return;\n    }", ""),
    ("src/app/scene_player.c", "music_request_stop(MUSIC_QUICK_FADE_TICKS);\n            run_log_write(\"scene finished", "run_log_write(\"scene finished"),
    ("src/app/scene_player.c", "stop_scene_audio();\n    request_scene_audio();", "request_scene_audio();"),
    ("src/app/scene_player.c", "stop_scene_audio();\n    status = SCENE_PLAYER_IDLE;", "status = SCENE_PLAYER_IDLE;"),
    ("src/app/scene_player.c", "clock_now_us() - preload_started_us > LONGEST_AUDIO_PRELOAD_US", "0"),
    ("src/app/scene_player.c", "step->kind == SCENE_STEP_MUSIC && preload_music_name == NULL", "step->kind == SCENE_STEP_MUSIC"),
    ("src/app/viewer_app.c", "music_shutdown();", ""),
    ("src/app/viewer_app.c", "sound_shutdown();", ""),
    ("src/app/viewer_app.c", "asset_subfolder_path(sound_folder, \"sounds\");", "asset_subfolder_path(sound_folder, \"music\");"),
    ("src/app/command_file.c", "command->number <= 0 || command->number > MUSIC_STARVE_MS_MAX", "0"),
    ("src/app/command_file.c", "\"music_starve %d%c\", &command->number, &extra", "\"music_starve %d\", &command->number"),
]

PYTHON_MUTATIONS = [
    ("tools/audio_check.py", "allowed_rises = 1\n    for playback in playbacks:\n        previous_trouble = 0\n", "for playback in playbacks:\n        allowed_rises = 1\n        previous_trouble = 0\n"),
    ("tools/audio_check.py", "window_before and window_after and not stopped_before", "window_after and not stopped_before"),
    ("tools/audio_check.py", "window_before and window_after and not stopped_before", "window_before and not stopped_before"),
    ("tools/audio_check.py", "window_before and window_after and not stopped_before", "window_before and window_after"),
    ("tools/audio_check.py", 'if number(fields, "mode") != STEREO_PORT_MODE:\n            problems.append("music port', 'if False:\n            problems.append("music port'),
    ("tools/audio_check.py", 'if number(fields, "mode") != STEREO_PORT_MODE:\n            problems.append("sound port', 'if False:\n            problems.append("sound port'),
    ("tools/audio_check.py", 'if number(fields, "adopt") != 1:\n            problems.append("music port', 'if False:\n            problems.append("music port'),
    ("tools/audio_check.py", 'if number(fields, "adopt") != 1:\n            problems.append("sound port', 'if False:\n            problems.append("sound port'),
    ("tools/audio_check.py", 'previous_wall_us = number(window, "wall_us")\n', "pass\n"),
    ("tools/audio_check.py", '"at") + number(window, "frames") - number(window, "rest") for window', '"at") + number(window, "frames") for window'),
    ("tools/audio_check.py", '"music bad_loop_file", ', ""),
    ("tools/audio_check.py", '"scene audio_wait_gave_up", ', ""),
    ("tools/audio_check.py", 'if number(window, "rest") <= 0:', "if False:"),
    ("tools/audio_check.py", 'if window["source_frame"] != source_frame:', "if False:"),
    ("tools/audio_check.py", 'if number(fields, "waited_us") > LONGEST_SOUND_WAIT_US:', "if False:"),
    ("tools/audio_check.py", "if previous_trouble is not None and trouble != previous_trouble:", "if False:"),
    ("tools/audio_check.py", "            start = index\n", "            pass\n"),
    ("tools/audio_check.py", "if worst_share >= DECODE_SHARE_LIMIT:", "if False:"),
    ("tools/audio_check.py", 'if number(fields, "rate") != MUSIC_RATE or number(fields, "grain") != SOUND_GRAIN_FRAMES:', "if False:"),
    ("tools/audio_check.py", 'return ["the capture stall came after the starve test, so it proves nothing"]', "pass"),
    ("tools/audio_check.py", 'grain_time = number(window, "block_us_avg") + number(window, "decode_us_avg")', "grain_time = MUSIC_GRAIN_US"),
    ("tools/audio_check.py", 'elif len(playback.windows) > 0 and playback.stopped["crc_total"] != playback.windows[-1]["crc_total"]:', "elif False:"),
    ("tools/assets/audio_file.py", "if data[OFFSET_LOOP_FLAG] != 0:", "if True:"),
    ("tools/assets/audio_file.py", 'if stream["rate"] != PLAYBACK_RATE:', "if False:"),
    ("tools/assets/audio_file.py", 'if struct.unpack_from("<I", data, OFFSET_OGG_SIZE)[0] != len(data) - WRAPPER_SIZE:', "if False:"),
    ("tools/assets/audio_file.py", '"ogg": data[WRAPPER_SIZE:],', '"ogg": data,'),
]


def command_succeeds(folder, command):
    process = subprocess.Popen(command, cwd=folder, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, start_new_session=True)
    try:
        return process.wait(timeout=TEST_TIME_LIMIT_SECONDS) == 0
    except subprocess.TimeoutExpired:
        os.killpg(process.pid, signal.SIGKILL)
        process.wait()
        return False


def make_succeeds(folder, make_arguments):
    return command_succeeds(folder, ["make"] + make_arguments)


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


def python_mutation_outcome(folder, path):
    with open(path, encoding="utf-8") as source:
        try:
            compile(source.read(), path, "exec")
        except SyntaxError:
            return "invalid"
    if command_succeeds(folder, PYTHON_TEST_COMMAND):
        return "survived"
    return "caught"


def forget_test_object_of(folder, relative_path):
    object_name = os.path.splitext(os.path.basename(relative_path))[0] + ".o"
    object_path = os.path.join(folder, "build", "test_objects", "app", object_name)
    if os.path.exists(object_path):
        os.remove(object_path)


def run_mutations(folder, mutations, outcome_of, survivors, invalid, source_changed=lambda relative_path: None):
    for relative_path, old_text, new_text in mutations:
        path = os.path.join(folder, relative_path)
        with open(path, encoding="utf-8") as source:
            original = source.read()
        if old_text not in original:
            invalid.append((relative_path, old_text))
            continue
        with open(path, "w", encoding="utf-8") as output:
            output.write(original.replace(old_text, new_text, 1))
        source_changed(relative_path)
        outcome = outcome_of(path)
        print(outcome, relative_path, "|", old_text[:60].replace("\n", " "), flush=True)
        if outcome == "survived":
            survivors.append((relative_path, old_text))
        if outcome == "invalid":
            invalid.append((relative_path, old_text))
        with open(path, "w", encoding="utf-8") as output:
            output.write(original)
        source_changed(relative_path)


def main():
    survivors = []
    invalid = []
    with tempfile.TemporaryDirectory() as temporary_root:
        folder = os.path.join(temporary_root, "srcVita")
        shutil.copytree(SRC_VITA_FOLDER, folder, ignore=shutil.ignore_patterns("build", "__pycache__"))
        remove_werror_from_makefile(folder)
        if not make_succeeds(folder, BUILD_ARGUMENTS) or not make_succeeds(folder, TEST_ARGUMENTS):
            print("baseline tests fail; stopping")
            return 1
        run_mutations(folder, MUTATIONS, lambda path: mutation_outcome(folder), survivors, invalid, lambda relative_path: forget_test_object_of(folder, relative_path))
        if not make_succeeds(folder, PYTHON_TOOL_BUILD_ARGUMENTS) or not command_succeeds(folder, PYTHON_TEST_COMMAND):
            print("baseline python tests fail; stopping")
            return 1
        run_mutations(folder, PYTHON_MUTATIONS, lambda path: python_mutation_outcome(folder, path), survivors, invalid)
    print("mutations:", len(MUTATIONS) + len(PYTHON_MUTATIONS), "survivors:", len(survivors), "invalid:", len(invalid))
    for entry in invalid:
        print("invalid", entry[0], entry[1][:60])
    return 1 if survivors or invalid else 0


if __name__ == "__main__":
    sys.exit(main())
