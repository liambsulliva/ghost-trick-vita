#include <string.h>
#include "fakes/fake_platform.h"
#include "fakes/fake_renderer.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_texture_files.h"
#include "texture_cache.h"
#include "scene_stage.h"
#include "script_ui_view.h"
#include "test_support.h"

#define PILL_FULLY_SHOWN 8
#define PAUSE_ICON_CELL_Y 421.0f
#define PILL_ART_TEXELS 512.0f

static void show_pill(struct script_hud_view *view, enum script_pill_id id, int slide_step, int is_pressed)
{
    view->pills[id].is_on_screen = 1;
    view->pills[id].slide_step = slide_step;
    view->pills[id].is_pressed = is_pressed;
}

static void test_a_shown_pill_answers_a_tap_inside_it(void)
{
    struct script_hud_view view;

    memset(&view, 0, sizeof(view));
    show_pill(&view, SCRIPT_PILL_TRICK, PILL_FULLY_SHOWN, 0);
    script_ui_view_hud_changed(&view);
    CHECK_INT(script_ui_view_pill_at(850.0f, 510.0f), SCRIPT_PILL_TRICK);
    CHECK_INT(script_ui_view_pill_at(480.0f, 510.0f), -1);
}

static void test_a_sliding_or_pressed_pill_does_not_answer(void)
{
    struct script_hud_view view;

    memset(&view, 0, sizeof(view));
    show_pill(&view, SCRIPT_PILL_TRICK, 3, 0);
    script_ui_view_hud_changed(&view);
    CHECK_INT(script_ui_view_pill_at(850.0f, 510.0f), -1);
    show_pill(&view, SCRIPT_PILL_TRICK, PILL_FULLY_SHOWN, 1);
    script_ui_view_hud_changed(&view);
    CHECK_INT(script_ui_view_pill_at(850.0f, 510.0f), -1);
}

static void test_pills_stay_quiet_during_a_movie(void)
{
    struct script_hud_view view;

    memset(&view, 0, sizeof(view));
    show_pill(&view, SCRIPT_PILL_GHOST, PILL_FULLY_SHOWN, 0);
    view.is_hidden_for_movie = 1;
    script_ui_view_hud_changed(&view);
    CHECK_INT(script_ui_view_pill_at(100.0f, 510.0f), -1);
}

static int pill_draws_now(void)
{
    fake_renderer_clear_draws();
    script_ui_view_draw();
    return fake_renderer_draw_count();
}

static int the_pause_icon_is_drawn(void)
{
    int index;

    for (index = 0; index < fake_renderer_draw_count(); index++) {
        const struct fake_draw *draw;

        draw = fake_renderer_draw(index);
        if (!draw->is_filled_rect && draw->screen_area.x > 800.0f && draw->texture_area.x == 0.0f && draw->texture_area.y == PAUSE_ICON_CELL_Y / PILL_ART_TEXELS) {
            return 1;
        }
    }
    return 0;
}

static void test_pills_hide_under_a_card_or_a_full_fade(void)
{
    static struct script_tables tables;
    struct script_hud_view view;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add_rgba_texture("assets/textures/hud_button_art.gtex", 8, 8);
    fake_files_add_rgba_texture("assets/textures/hud_sheet.gtex", 8, 8);
    texture_cache_init("assets/textures", TEXTURE_CACHE_DEFAULT_BUDGET_BYTES);
    CHECK(texture_cache_load_now("hud_button_art")->state == CACHED_TEXTURE_READY);
    CHECK(texture_cache_load_now("hud_sheet")->state == CACHED_TEXTURE_READY);
    memset(&tables, 0, sizeof(tables));
    script_ui_view_start(&tables);
    memset(&view, 0, sizeof(view));
    show_pill(&view, SCRIPT_PILL_GHOST, PILL_FULLY_SHOWN, 0);
    script_ui_view_hud_changed(&view);
    scene_stage_hide_card();
    scene_stage_set_fade(0.0f);
    CHECK(pill_draws_now() > 0);
    CHECK(the_pause_icon_is_drawn());
    CHECK_INT(script_ui_view_pill_at(100.0f, 510.0f), SCRIPT_PILL_GHOST);
    scene_stage_set_fade(0.5f);
    CHECK(pill_draws_now() > 0);
    scene_stage_set_fade(1.0f);
    CHECK_INT(pill_draws_now(), 0);
    CHECK_INT(script_ui_view_pill_at(100.0f, 510.0f), -1);
    scene_stage_set_fade(0.0f);
    scene_stage_show_card("some_card");
    CHECK_INT(pill_draws_now(), 0);
    CHECK_INT(script_ui_view_pill_at(100.0f, 510.0f), -1);
    scene_stage_hide_card();
    CHECK(pill_draws_now() > 0);
}

static int a_tap_somewhere_finds(enum script_pill_id id)
{
    float x;
    float y;

    for (y = 10.0f; y < 544.0f; y = y + 10.0f) {
        for (x = 5.0f; x < 960.0f; x = x + 10.0f) {
            if (script_ui_view_pill_at(x, y) == (int)id) {
                return 1;
            }
        }
    }
    return 0;
}

static void test_every_pill_has_a_place_and_draws(void)
{
    struct script_hud_view view;
    int id;

    scene_stage_hide_card();
    scene_stage_set_fade(0.0f);
    for (id = 0; id < SCRIPT_PILL_COUNT; id++) {
        memset(&view, 0, sizeof(view));
        show_pill(&view, (enum script_pill_id)id, PILL_FULLY_SHOWN, 0);
        script_ui_view_hud_changed(&view);
        CHECK(pill_draws_now() > 0);
        CHECK(a_tap_somewhere_finds((enum script_pill_id)id));
    }
    memset(&view, 0, sizeof(view));
    script_ui_view_hud_changed(&view);
}

static void test_the_rewind_button_answers_only_while_it_is_up(void)
{
    struct script_hud_view view;

    memset(&view, 0, sizeof(view));
    script_ui_view_hud_changed(&view);
    CHECK(!script_ui_view_rewind_button_is_at(480.0f, 272.0f));
    view.rewind_select_ticks = 30;
    script_ui_view_hud_changed(&view);
    CHECK(script_ui_view_rewind_button_is_at(480.0f, 272.0f));
    CHECK(!script_ui_view_rewind_button_is_at(100.0f, 100.0f));
}

static void test_the_side_label_and_hourglass_draw_under_the_talk(void)
{
    static struct script_tables tables;
    struct script_hud_view view;
    int main_draws;

    fake_files_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_files_add_rgba_texture("assets/textures/side_label.gtex", 8, 8);
    fake_files_add_rgba_texture("assets/textures/ui_hourglass.gtex", 8, 8);
    texture_cache_init("assets/textures", TEXTURE_CACHE_DEFAULT_BUDGET_BYTES);
    CHECK(texture_cache_load_now("side_label")->state == CACHED_TEXTURE_READY);
    CHECK(texture_cache_load_now("ui_hourglass")->state == CACHED_TEXTURE_READY);
    memset(&tables, 0, sizeof(tables));
    script_ui_view_start(&tables);
    memset(&view, 0, sizeof(view));
    view.side_is_on = 1;
    view.fate_level = 3;
    view.world_is_running = 1;
    script_ui_view_hud_changed(&view);
    fake_renderer_clear_draws();
    script_ui_view_draw();
    main_draws = fake_renderer_draw_count();
    fake_renderer_clear_draws();
    script_ui_view_draw_under_talk(0.0f);
    CHECK_INT(main_draws, 0);
    CHECK(fake_renderer_draw_count() > 0);
    fake_renderer_clear_draws();
    script_ui_view_draw_under_talk(1.0f);
    CHECK_INT(fake_renderer_draw_count(), 0);
    view.is_hidden_for_movie = 1;
    script_ui_view_hud_changed(&view);
    fake_renderer_clear_draws();
    script_ui_view_draw_under_talk(0.0f);
    CHECK_INT(fake_renderer_draw_count(), 0);
}

static void test_the_start_button_shows_top_right_with_the_ui_and_steps_aside_for_the_hourglass(void)
{
    static struct script_tables tables;
    struct script_hud_view view;

    memset(&tables, 0, sizeof(tables));
    script_ui_view_start(&tables);
    scene_stage_hide_card();
    scene_stage_set_fade(0.0f);
    memset(&view, 0, sizeof(view));
    script_ui_view_hud_changed(&view);
    CHECK(!script_ui_view_start_button_is_shown());
    CHECK(!script_ui_view_start_button_is_at(910.0f, 30.0f));
    show_pill(&view, SCRIPT_PILL_GHOST, PILL_FULLY_SHOWN, 0);
    script_ui_view_hud_changed(&view);
    CHECK(script_ui_view_start_button_is_shown());
    CHECK(script_ui_view_start_button_is_at(910.0f, 30.0f));
    CHECK(!script_ui_view_start_button_is_at(480.0f, 30.0f));
    show_pill(&view, SCRIPT_PILL_HOURGLASS, PILL_FULLY_SHOWN, 0);
    script_ui_view_hud_changed(&view);
    CHECK(!script_ui_view_start_button_is_at(910.0f, 30.0f));
    CHECK_INT(script_ui_view_pill_at(910.0f, 30.0f), SCRIPT_PILL_HOURGLASS);
    CHECK(script_ui_view_start_button_is_at(830.0f, 30.0f));
    view.picture_is_hidden = 1;
    script_ui_view_hud_changed(&view);
    CHECK(!script_ui_view_start_button_is_shown());
    view.picture_is_hidden = 0;
    view.is_hidden_for_movie = 1;
    script_ui_view_hud_changed(&view);
    CHECK(!script_ui_view_start_button_is_shown());
    memset(&view, 0, sizeof(view));
    script_ui_view_hud_changed(&view);
}

int main(void)
{
    test_a_shown_pill_answers_a_tap_inside_it();
    test_the_side_label_and_hourglass_draw_under_the_talk();
    test_a_sliding_or_pressed_pill_does_not_answer();
    test_pills_stay_quiet_during_a_movie();
    test_pills_hide_under_a_card_or_a_full_fade();
    test_every_pill_has_a_place_and_draws();
    test_the_rewind_button_answers_only_while_it_is_up();
    test_the_start_button_shows_top_right_with_the_ui_and_steps_aside_for_the_hourglass();
    return test_finish("unit/script_ui_view");
}
