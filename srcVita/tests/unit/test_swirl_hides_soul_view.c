#include <string.h>
#include "fakes/fake_platform.h"
#include "fakes/fake_renderer.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_texture_files.h"
#include "script_tables.h"
#include "script_view.h"
#include "texture_cache.h"
#include "test_support.h"

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

    texture = texture_cache_use_or_start_loading(name);
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

static void start_in_the_ghost_view_on_a_dead_core(void)
{
    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add_rgba_texture("tex/soul_glow.gtex", 2, 2);
    fake_files_add_rgba_texture("tex/soul_orb.gtex", 2, 2);
    texture_cache_init("tex", 1u << 20);
    script_tables_clear(&tables);
    script_view_start(&tables);
    script_view_soul_changed(1, 0);
    script_view_cursor_moved(1, 100.0f, 100.0f, 100.0f, 100.0f, 0);
}

static void draw_with_a_core_marked(void)
{
    script_view_core_marked(0, 0, 100.0f, 100.0f, 1, 1.0f);
    script_view_core_marked(1, 0, 200.0f, 120.0f, 0, 1.0f);
    fake_renderer_clear_draws();
    script_view_draw_on_world();
}

static void test_the_ghost_view_draws_its_cores_without_the_swirl(void)
{
    renderer_texture glow;
    renderer_texture orb;

    start_in_the_ghost_view_on_a_dead_core();
    glow = loaded("soul_glow");
    orb = loaded("soul_orb");
    draw_with_a_core_marked();
    CHECK_INT(draws_of(glow), 2);
    CHECK(draws_of(orb) >= 3);
}

static void test_the_swirl_hides_every_part_of_the_ghost_view(void)
{
    renderer_texture glow;
    renderer_texture orb;

    start_in_the_ghost_view_on_a_dead_core();
    glow = loaded("soul_glow");
    orb = loaded("soul_orb");
    script_view_swirl_changed(1);
    draw_with_a_core_marked();
    CHECK_INT(draws_of(glow), 0);
    CHECK_INT(draws_of(orb), 0);
    script_view_swirl_changed(0);
    draw_with_a_core_marked();
    CHECK_INT(draws_of(glow), 2);
}

int main(void)
{
    test_the_ghost_view_draws_its_cores_without_the_swirl();
    test_the_swirl_hides_every_part_of_the_ghost_view();
    fake_files_reset();
    return test_finish("unit/swirl_hides_soul_view");
}
