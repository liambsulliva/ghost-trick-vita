#include <string.h>
#include "animation_library.h"
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_renderer.h"
#include "fakes/fake_texture_files.h"
#include "scene_stage.h"
#include "scene_view.h"
#include "script_bridge.h"
#include "script_tables.h"
#include "texture_cache.h"
#include "test_support.h"

static const char lynne_still[] =
    "animation t_lynne\n"
    "loops 0\n"
    "frame lynne_00 4 -10 -40 20 40 0 0 1 1\n";
static const char crate_still[] =
    "animation t_crate\n"
    "loops 0\n"
    "frame crate_00 4 -10 -20 20 20 0 0 1 1\n";

static struct script_tables tables;

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

static int whole_screen_black_fills(void)
{
    int count;
    int index;

    count = 0;
    for (index = 0; index < fake_renderer_draw_count(); index++) {
        const struct fake_draw *draw;

        draw = fake_renderer_draw(index);
        if (draw->is_filled_rect && draw->color.red == 0.0f && draw->color.alpha == 1.0f && draw->screen_area.width >= (float)RENDERER_WIDTH && draw->screen_area.height >= (float)RENDERER_HEIGHT) {
            count = count + 1;
        }
    }
    return count;
}

static void draw_the_world(void)
{
    fake_renderer_clear_draws();
    scene_view_draw_world(scene_stage_view());
}

static const struct script_outputs *start_in_a_room_with_lynne(renderer_texture *room, renderer_texture *lynne)
{
    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add_rgba_texture("tex/room.gtex", 4, 4);
    fake_files_add_rgba_texture("tex/lynne_00.gtex", 2, 2);
    fake_files_add_rgba_texture("tex/crate_00.gtex", 2, 2);
    fake_files_add("anims/t_lynne.anim", (const unsigned char *)lynne_still, sizeof(lynne_still) - 1);
    fake_files_add("anims/t_crate.anim", (const unsigned char *)crate_still, sizeof(crate_still) - 1);
    texture_cache_init("tex", 1u << 20);
    animation_library_init("anims");
    memset(&tables, 0, sizeof(tables));
    script_bridge_start(&tables);
    scene_stage_reset();
    scene_view_forget_held_poses();
    scene_stage_add_plate("room", 0.0f, 0.0f, 512.0f, 256.0f);
    scene_stage_set_camera(128.0f, 96.0f);
    *room = loaded("room");
    *lynne = loaded("lynne_00");
    scene_stage_place_object("linne", "t_lynne", 120.0f, 120.0f, 1, 0);
    draw_the_world();
    CHECK_INT(draws_of(*room), 1);
    CHECK_INT(draws_of(*lynne), 1);
    return script_bridge_outputs();
}

static void test_the_swirl_hides_the_room_and_its_people(void)
{
    const struct script_outputs *outputs;
    renderer_texture room;
    renderer_texture lynne;

    outputs = start_in_a_room_with_lynne(&room, &lynne);
    outputs->swirl_changed(1);
    draw_the_world();
    CHECK_INT(draws_of(room), 0);
    CHECK_INT(draws_of(lynne), 0);
    CHECK_INT(whole_screen_black_fills(), 1);
    outputs->swirl_changed(0);
    draw_the_world();
    CHECK_INT(draws_of(room), 1);
    CHECK_INT(draws_of(lynne), 1);
    scene_stage_reset();
    animation_library_clear();
}

static void test_art_under_the_swirl_is_neither_waited_for_nor_read(void)
{
    const struct script_outputs *outputs;
    renderer_texture room;
    renderer_texture lynne;

    outputs = start_in_a_room_with_lynne(&room, &lynne);
    outputs->swirl_changed(1);
    scene_stage_place_object("box", "t_crate", 160.0f, 120.0f, 1, 0);
    CHECK(!scene_view_visible_art_is_loading(scene_stage_view()));
    draw_the_world();
    CHECK_INT(texture_cache_loading_count(), 0);
    outputs->swirl_changed(0);
    CHECK(scene_view_visible_art_is_loading(scene_stage_view()));
    scene_stage_reset();
    animation_library_clear();
}

static void test_a_cleared_stage_uncovers_the_world(void)
{
    const struct script_outputs *outputs;
    renderer_texture room;
    renderer_texture lynne;

    outputs = start_in_a_room_with_lynne(&room, &lynne);
    outputs->swirl_changed(1);
    outputs->stage_cleared();
    CHECK(!scene_stage_view()->world_is_covered);
    scene_stage_reset();
    animation_library_clear();
}

int main(void)
{
    test_the_swirl_hides_the_room_and_its_people();
    test_art_under_the_swirl_is_neither_waited_for_nor_read();
    test_a_cleared_stage_uncovers_the_world();
    fake_files_reset();
    return test_finish("unit/swirl_hides_the_world");
}
