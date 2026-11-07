#include <string.h>
#include "fakes/fake_platform.h"
#include "fakes/fake_renderer.h"
#include "script_ui_view.h"
#include "test_support.h"

#define PILL_FULLY_SHOWN 8

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

int main(void)
{
    test_a_shown_pill_answers_a_tap_inside_it();
    test_a_sliding_or_pressed_pill_does_not_answer();
    test_pills_stay_quiet_during_a_movie();
    test_the_rewind_button_answers_only_while_it_is_up();
    return test_finish("unit/script_ui_view");
}
