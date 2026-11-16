#include <string.h>
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_renderer.h"
#include "fakes/fake_texture_files.h"
#include "fakes/fake_work_queue.h"
#include "game_save.h"
#include "renderer.h"
#include "save_icon.h"
#include "script_machine.h"
#include "soul_chain.h"
#include "test_support.h"
#include "texture_cache.h"

#define FRAME_US 16667ull

static struct script_machine machine;
static struct game_save save;

static void run_for(unsigned long long us, int is_writing)
{
    unsigned long long done;

    for (done = 0; done < us; done += FRAME_US) {
        save_icon_update(FRAME_US, is_writing);
    }
}

static void test_the_icon_fades_in_holds_then_fades_out(void)
{
    save_icon_reset();
    CHECK(!save_icon_is_showing());
    CHECK(save_icon_alpha() == 0.0f);
    save_icon_show();
    CHECK(save_icon_is_showing());
    CHECK(save_icon_alpha() == 0.0f);
    run_for(125000ull, 0);
    CHECK(save_icon_alpha() > 0.3f && save_icon_alpha() < 0.7f);
    run_for(200000ull, 0);
    CHECK(save_icon_alpha() == 1.0f);
    run_for(1300000ull, 0);
    CHECK(save_icon_alpha() == 1.0f);
    run_for(400000ull, 0);
    CHECK(save_icon_alpha() > 0.0f && save_icon_alpha() < 1.0f);
    run_for(400000ull, 0);
    CHECK(!save_icon_is_showing());
    CHECK(save_icon_alpha() == 0.0f);
}

static void test_the_icon_stays_while_a_save_is_still_being_written(void)
{
    save_icon_reset();
    save_icon_show();
    run_for(6000000ull, 1);
    CHECK(save_icon_alpha() == 1.0f);
    run_for(600000ull, 0);
    CHECK(!save_icon_is_showing());
}

static void test_a_second_save_keeps_the_icon_up_without_fading_in_again(void)
{
    float alpha;

    save_icon_reset();
    save_icon_show();
    run_for(1000000ull, 0);
    save_icon_show();
    CHECK(save_icon_alpha() == 1.0f);
    run_for(1400000ull, 0);
    CHECK(save_icon_alpha() == 1.0f);
    run_for(400000ull, 0);
    alpha = save_icon_alpha();
    CHECK(alpha > 0.0f && alpha < 1.0f);
    save_icon_show();
    CHECK(save_icon_alpha() > alpha - 0.05f && save_icon_alpha() < alpha + 0.05f);
    run_for(300000ull, 0);
    CHECK(save_icon_alpha() == 1.0f);
}

static void test_every_kind_of_save_shows_the_icon(void)
{
    memset(&machine, 0, sizeof(machine));
    machine.chapter_mark = 3;
    fake_files_reset();
    fake_work_queue_reset();
    game_save_capture(&save, &machine, "st01");
    save_icon_reset();
    CHECK_INT(game_save_store_as_newest(&save, GAME_SAVE_AUTO), 1);
    CHECK(save_icon_is_showing());
    save_icon_reset();
    CHECK_INT(game_save_store_as_newest(&save, GAME_SAVE_MANUAL), 1);
    CHECK(save_icon_is_showing());
    save_icon_reset();
    CHECK_INT(game_save_store(&save, GAME_SAVE_MANUAL), 1);
    CHECK(save_icon_is_showing());
    save_icon_reset();
    CHECK_INT(game_save_store_chapter(&save, 4), 1);
    CHECK(save_icon_is_showing());
    save_icon_reset();
    CHECK_INT(game_save_load_newest(&save), 1);
    CHECK(!save_icon_is_showing());
}

static void test_a_held_write_reports_as_writing_until_it_runs(void)
{
    fake_files_reset();
    fake_work_queue_reset();
    fake_work_queue_hold(1);
    CHECK_INT(game_save_is_writing(), 0);
    CHECK_INT(game_save_store(&save, GAME_SAVE_AUTO), 1);
    CHECK_INT(game_save_is_writing(), 1);
    fake_work_queue_run_pending();
    CHECK_INT(game_save_is_writing(), 0);
    fake_work_queue_hold(0);
}

static void test_the_icon_is_a_third_of_a_control_soul_inside_the_corner_margin(void)
{
    struct soul_chain_blob control[SOUL_BODY_BLOBS];
    struct soul_chain_blob icon[SOUL_BODY_BLOBS];
    struct soul_chain_blob core[SAVE_ICON_CORE_BLOBS];
    unsigned int step;

    save_icon_reset();
    for (step = 0; step < 240u; step++) {
        float control_left;
        float control_right;
        float icon_left;
        float icon_right;
        int control_count;
        int icon_count;
        int index;

        control_left = 0.0f;
        control_right = 0.0f;
        icon_left = (float)RENDERER_WIDTH;
        icon_right = 0.0f;
        control_count = soul_body_blobs(0.0f, 0.0f, 1.0f, save_icon_step(), control);
        icon_count = save_icon_body(icon);
        CHECK_INT(icon_count, control_count);
        CHECK(icon_count <= SOUL_BODY_BLOBS);
        for (index = 0; index < control_count; index++) {
            control_left = control[index].x - control[index].radius < control_left ? control[index].x - control[index].radius : control_left;
            control_right = control[index].x + control[index].radius > control_right ? control[index].x + control[index].radius : control_right;
        }
        for (index = 0; index < icon_count; index++) {
            icon_left = icon[index].x - icon[index].radius < icon_left ? icon[index].x - icon[index].radius : icon_left;
            icon_right = icon[index].x + icon[index].radius > icon_right ? icon[index].x + icon[index].radius : icon_right;
            CHECK(icon[index].x + icon[index].radius <= (float)RENDERER_WIDTH - SAVE_ICON_MARGIN + 0.01f);
            CHECK(icon[index].y + icon[index].radius <= (float)RENDERER_HEIGHT - SAVE_ICON_MARGIN + 0.01f);
            CHECK(icon[index].x > (float)RENDERER_WIDTH * 0.9f);
            CHECK(icon[index].y > (float)RENDERER_HEIGHT * 0.8f);
        }
        CHECK(icon_right - icon_left < (control_right - control_left) * 0.4f);
        save_icon_core(core);
        for (index = 0; index < SAVE_ICON_CORE_BLOBS; index++) {
            CHECK(core[index].x - core[index].radius > icon_left);
            CHECK(core[index].x + core[index].radius < icon_right);
        }
        save_icon_update(SAVE_ICON_STEP_US, 0);
    }
}

static void test_the_flame_is_redrawn_eight_times_a_second(void)
{
    unsigned int first;
    unsigned long long done;

    save_icon_reset();
    first = save_icon_step();
    for (done = 0; done < 120000ull; done += 10000ull) {
        save_icon_update(10000ull, 0);
    }
    CHECK_INT(save_icon_step(), first);
    save_icon_update(10000ull, 0);
    CHECK_INT(save_icon_step(), first + 1u);
    for (done = 0; done < 875000ull; done += 12500ull) {
        save_icon_update(12500ull, 0);
    }
    CHECK_INT(save_icon_step(), first + 8u);
}

static void test_the_icon_draws_only_while_shown_and_once_the_art_is_loaded(void)
{
    int index;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add_rgba_texture("tex/soul_blob.gtex", 2, 2);
    texture_cache_init("tex", 1u << 20);
    save_icon_reset();
    save_icon_draw();
    CHECK_INT(fake_renderer_draw_count(), 0);
    save_icon_show();
    fake_reader_hold_reads(1);
    save_icon_draw();
    CHECK_INT(fake_renderer_draw_count(), 0);
    fake_reader_release_reads(1);
    texture_cache_upload_finished_reads(1);
    run_for(125000ull, 0);
    save_icon_draw();
    CHECK(fake_renderer_draw_count() > SAVE_ICON_CORE_BLOBS);
    for (index = 0; index < fake_renderer_draw_count(); index++) {
        const struct fake_draw *draw;

        draw = fake_renderer_draw(index);
        CHECK(draw->color.alpha > 0.3f && draw->color.alpha < 0.7f);
        CHECK(draw->screen_area.x + draw->screen_area.width <= (float)RENDERER_WIDTH);
        CHECK(draw->screen_area.y + draw->screen_area.height <= (float)RENDERER_HEIGHT);
        CHECK(draw->screen_area.x > (float)RENDERER_WIDTH * 0.9f);
    }
    fake_renderer_clear_draws();
    run_for(3000000ull, 0);
    save_icon_draw();
    CHECK_INT(fake_renderer_draw_count(), 0);
}

int main(void)
{
    test_the_icon_fades_in_holds_then_fades_out();
    test_the_icon_stays_while_a_save_is_still_being_written();
    test_a_second_save_keeps_the_icon_up_without_fading_in_again();
    test_every_kind_of_save_shows_the_icon();
    test_a_held_write_reports_as_writing_until_it_runs();
    test_the_icon_is_a_third_of_a_control_soul_inside_the_corner_margin();
    test_the_flame_is_redrawn_eight_times_a_second();
    test_the_icon_draws_only_while_shown_and_once_the_art_is_loaded();
    return test_finish("unit/save_icon");
}
