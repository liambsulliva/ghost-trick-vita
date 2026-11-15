#include <math.h>
#include <string.h>
#include "fakes/fake_platform.h"
#include "fakes/fake_renderer.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_texture_files.h"
#include "script_tables.h"
#include "script_view.h"
#include "swirl_waves.h"
#include "texture_cache.h"
#include "test_support.h"

#define WAVES_LINE "flipbook swirl_waves 16 16 4 4 0.25 0.25 swirl_waves_00\n"
#define PAGE_SIDE 16
#define NATIVE_TICKS_PER_TURN 2048u

static struct script_tables tables;

static int near(float a, float b)
{
    return fabsf(a - b) < 0.01f;
}

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

    (void)texture_cache_use_or_start_loading(name);
    finish_every_read();
    texture = texture_cache_use_or_start_loading(name);
    CHECK(texture != NULL && texture->state == CACHED_TEXTURE_READY);
    return texture != NULL ? texture->texture : 0;
}

static const struct fake_draw *draw_of(renderer_texture texture)
{
    int index;

    for (index = 0; index < fake_renderer_draw_count(); index++) {
        if (fake_renderer_draw(index)->texture == texture) {
            return fake_renderer_draw(index);
        }
    }
    return NULL;
}

static int index_of_draw(renderer_texture texture)
{
    int index;

    for (index = 0; index < fake_renderer_draw_count(); index++) {
        if (fake_renderer_draw(index)->texture == texture) {
            return index;
        }
    }
    return -1;
}

static void start_with_the_swirl_art(int with_waves)
{
    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add_rgba_texture("tex/swirl_back.gtex", 4, 4);
    fake_files_add_rgba_texture("tex/swirl_particle.gtex", 2, 2);
    fake_files_add_rgba_texture("tex/swirl_waves_00.gtex", PAGE_SIDE, PAGE_SIDE);
    texture_cache_init("tex", 1u << 20);
    script_tables_clear(&tables);
    if (with_waves) {
        CHECK(script_tables_read_effects(&tables, WAVES_LINE, (unsigned int)strlen(WAVES_LINE)));
    }
    script_view_start(&tables);
}

static void test_the_waves_cycle_one_palette_frame_every_sixteen_ticks(void)
{
    const struct script_flipbook *book;
    struct renderer_rect cell;

    script_tables_clear(&tables);
    CHECK(script_tables_read_effects(&tables, WAVES_LINE, (unsigned int)strlen(WAVES_LINE)));
    book = script_tables_find_flipbook(&tables, SWIRL_WAVES_FLIPBOOK);
    CHECK(book != NULL);
    cell = swirl_waves_cell(book, 15);
    CHECK(near(cell.x, 0.0f) && near(cell.y, 0.0f) && near(cell.width, 0.25f));
    cell = swirl_waves_cell(book, 16);
    CHECK(near(cell.x, 0.25f) && near(cell.y, 0.0f));
    cell = swirl_waves_cell(book, 16 * 5);
    CHECK(near(cell.x, 0.25f) && near(cell.y, 0.25f));
    cell = swirl_waves_cell(book, 16 * 16);
    CHECK(near(cell.x, 0.0f) && near(cell.y, 0.0f));
}

static void test_the_waves_turn_a_whole_turn_in_2048_ticks(void)
{
    CHECK(near(swirl_waves_radians(0), 0.0f));
    CHECK(near(swirl_waves_radians(NATIVE_TICKS_PER_TURN / 4u), 1.5707963f));
    CHECK(near(swirl_waves_radians(NATIVE_TICKS_PER_TURN), 0.0f));
}

static void test_the_wrapped_tiles_cover_the_screen_at_every_angle(void)
{
    static const float screen_corners[4][2] = { { 0.0f, 0.0f }, { 960.0f, 0.0f }, { 0.0f, 544.0f }, { 960.0f, 544.0f } };
    struct renderer_vertex corners[SWIRL_WAVES_TRIANGLES * 3];
    struct renderer_rect cell = { 0.25f, 0.5f, 0.25f, 0.25f };
    float side;
    unsigned int ticks;
    int corner;

    side = 256.0f * 2.8125f;
    for (ticks = 0; ticks < NATIVE_TICKS_PER_TURN; ticks += 37u) {
        float radians;

        radians = swirl_waves_radians(ticks);
        swirl_waves_corners(480.0f, 272.0f, side, radians, cell, corners);
        for (corner = 0; corner < 4; corner++) {
            float dx;
            float dy;
            float along;
            float across;

            dx = screen_corners[corner][0] - 480.0f;
            dy = screen_corners[corner][1] - 272.0f;
            along = dx * cosf(radians) + dy * sinf(radians);
            across = -dx * sinf(radians) + dy * cosf(radians);
            CHECK(fabsf(along) < 1.5f * side && fabsf(across) < 1.5f * side);
        }
    }
    swirl_waves_corners(480.0f, 272.0f, side, 0.0f, cell, corners);
    CHECK(near(corners[4 * 6].x, 480.0f - side / 2.0f) && near(corners[4 * 6].y, 272.0f - side / 2.0f));
    CHECK(near(corners[4 * 6].u, 0.25f) && near(corners[4 * 6].v, 0.5f));
    CHECK(near(corners[4 * 6 + 4].u, 0.5f) && near(corners[4 * 6 + 4].v, 0.75f));
    swirl_waves_corners(480.0f, 272.0f, side, 1.5707963f, cell, corners);
    CHECK(near(corners[4 * 6].x, 480.0f + side / 2.0f) && near(corners[4 * 6].y, 272.0f - side / 2.0f));
}

static void test_a_set_with_cartridge_waves_draws_them_under_a_dimmer_swirl(void)
{
    renderer_texture back;
    renderer_texture waves;
    const struct fake_draw *glow;

    start_with_the_swirl_art(1);
    back = loaded("swirl_back");
    waves = loaded("swirl_waves_00");
    (void)loaded("swirl_particle");
    script_view_swirl_changed(1);
    fake_renderer_clear_draws();
    script_view_draw_on_world();
    CHECK(draw_of(waves) != NULL);
    CHECK(index_of_draw(waves) < index_of_draw(back));
    glow = draw_of(back);
    CHECK(glow != NULL && near(glow->color.alpha, 20.0f / 31.0f));
    script_view_swirl_changed(0);
    fake_renderer_clear_draws();
    script_view_draw_on_world();
    CHECK(draw_of(waves) == NULL);
}

static void test_a_set_without_waves_keeps_its_swirl_whole(void)
{
    renderer_texture back;
    const struct fake_draw *glow;

    start_with_the_swirl_art(0);
    back = loaded("swirl_back");
    (void)loaded("swirl_particle");
    script_view_swirl_changed(1);
    fake_renderer_clear_draws();
    script_view_draw_on_world();
    glow = draw_of(back);
    CHECK(glow != NULL && near(glow->color.alpha, 1.0f));
    script_view_swirl_changed(0);
}

int main(void)
{
    test_the_waves_cycle_one_palette_frame_every_sixteen_ticks();
    test_the_waves_turn_a_whole_turn_in_2048_ticks();
    test_the_wrapped_tiles_cover_the_screen_at_every_angle();
    test_a_set_with_cartridge_waves_draws_them_under_a_dimmer_swirl();
    test_a_set_without_waves_keeps_its_swirl_whole();
    fake_files_reset();
    return test_finish("unit/swirl_waves");
}
