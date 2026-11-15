#include <string.h>
#include "animation_library.h"
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_renderer.h"
#include "fakes/fake_texture_files.h"
#include "run_log.h"
#include "scene_stage.h"
#include "scene_view.h"
#include "texture_cache.h"
#include "test_support.h"

static const char running_lynne[] =
    "animation t_running\n"
    "loops 0\n"
    "frame running_00 4 -10 -40 20 40 0 0 1 1\n"
    "frame running_01 4 -10 -40 20 40 0 0 1 1\n";
static const char desk_lamp[] =
    "animation t_lamp\n"
    "loops 0\n"
    "frame lamp_00 4 -10 -20 20 20 0 0 1 1\n";

static void finish_every_read(void)
{
    int round;

    for (round = 0; round < 64; round++) {
        fake_reader_release_reads(64);
        texture_cache_upload_finished_reads(64);
        texture_cache_begin_frame();
    }
}

static renderer_texture loaded(const char *name)
{
    const struct cached_texture *texture;

    texture_cache_use_or_start_loading(name);
    finish_every_read();
    texture = texture_cache_use_or_start_loading(name);
    CHECK(texture != NULL && texture->state == CACHED_TEXTURE_READY);
    return texture != NULL ? texture->texture : 0;
}

static int draws_of(renderer_texture texture)
{
    int count;
    int index;

    count = 0;
    for (index = 0; index < fake_renderer_draw_count(); index++) {
        if (fake_renderer_draw(index)->texture == texture) {
            count = count + 1;
        }
    }
    return count;
}

static int run_log_says(const char *text)
{
    const char *log;
    unsigned int size;

    run_log_flush();
    log = (const char *)fake_files_contents("run.log", &size);
    return log != NULL && strstr(log, text) != NULL;
}

static void draw_the_world(void)
{
    fake_renderer_clear_draws();
    scene_view_draw_world(scene_stage_view());
}

static renderer_texture start_with_lynne_running(void)
{
    renderer_texture running;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add_rgba_texture("tex/room.gtex", 4, 4);
    fake_files_add_rgba_texture("tex/running_00.gtex", 2, 2);
    fake_files_add_rgba_texture("tex/running_01.gtex", 2, 2);
    fake_files_add_rgba_texture("tex/lamp_00.gtex", 2, 2);
    fake_files_add("anims/t_running.anim", (const unsigned char *)running_lynne, sizeof(running_lynne) - 1);
    fake_files_add("anims/t_lamp.anim", (const unsigned char *)desk_lamp, sizeof(desk_lamp) - 1);
    run_log_open("run.log");
    texture_cache_init("tex", 1u << 20);
    animation_library_init("anims");
    scene_stage_reset();
    scene_view_forget_held_poses();
    scene_stage_add_plate("room", 0.0f, 0.0f, 512.0f, 256.0f);
    scene_stage_set_camera(128.0f, 96.0f);
    loaded("room");
    running = loaded("running_00");
    scene_stage_place_object("demohum2", "t_running", 120.0f, 120.0f, 1, 0);
    draw_the_world();
    CHECK_INT(draws_of(running), 1);
    return running;
}

static void test_a_new_animation_never_shows_the_last_ones_pose(void)
{
    renderer_texture running;

    running = start_with_lynne_running();
    scene_stage_place_object("demohum2", "t_lamp", 120.0f, 120.0f, 1, 0);
    draw_the_world();
    CHECK_INT(draws_of(running), 0);
    CHECK(scene_view_visible_art_is_loading(scene_stage_view()));
    draw_the_world();
    CHECK_INT(draws_of(running), 0);
    finish_every_read();
    draw_the_world();
    CHECK_INT(draws_of(loaded("lamp_00")), 1);
    CHECK_INT(draws_of(running), 0);
    CHECK(run_log_says("pose absent object=demohum2 frames=2 waited_for=lamp_00"));
    animation_library_clear();
}

static void test_a_frame_still_loading_holds_the_same_animations_pose(void)
{
    renderer_texture running;

    running = start_with_lynne_running();
    scene_stage_place_object("demohum2", "t_running", 120.0f, 120.0f, 1, 1);
    draw_the_world();
    CHECK_INT(draws_of(running), 1);
    animation_library_clear();
}

static void test_a_pose_is_forgotten_once_another_animation_shows(void)
{
    renderer_texture running;

    running = start_with_lynne_running();
    scene_stage_place_object("demohum2", "t_lamp", 120.0f, 120.0f, 1, 0);
    draw_the_world();
    scene_stage_place_object("demohum2", "t_running", 120.0f, 120.0f, 1, 1);
    draw_the_world();
    CHECK_INT(draws_of(running), 0);
    animation_library_clear();
}

int main(void)
{
    test_a_new_animation_never_shows_the_last_ones_pose();
    test_a_frame_still_loading_holds_the_same_animations_pose();
    test_a_pose_is_forgotten_once_another_animation_shows();
    fake_files_reset();
    return test_finish("unit/held_pose_keeps_its_animation");
}
