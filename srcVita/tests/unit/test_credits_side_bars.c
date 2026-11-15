#include <string.h>
#include "ds_units.h"
#include "fakes/fake_renderer.h"
#include "renderer.h"
#include "scene_view.h"
#include "script_file.h"
#include "script_machine.h"
#include "script_tables.h"
#include "script_view.h"
#include "test_support.h"

#define OP_END 0x0
#define OP_CHANGE_SCRIPT 0x24
#define OP_WAIT 0x2f
#define OP_ENDING_MODE 0x6a
#define OP_STAFF_ROLL_START 0xf8
#define OP_STAFF_ROLL_WAIT 0xf9
#define OP_STAFF_ROLL_END 0xfa
#define FIRST_STRING 2
#define PAGE 3
#define SCENE_BETWEEN_PAGES_TICKS 40
#define A_LONG_WAIT 6000
#define TICK_LIMIT 5000
#define SCRIPT_BYTES_MAX (SCRIPT_FILE_HEADER_SIZE + 128)

static struct script_machine machine;
static unsigned char page_script[SCRIPT_BYTES_MAX];
static unsigned char scene_script[SCRIPT_BYTES_MAX];
static unsigned char final_script[SCRIPT_BYTES_MAX];
static int credits_reported;
static int credits_reports;

static void credits_changed(int is_on)
{
    credits_reported = is_on;
    credits_reports = credits_reports + 1;
}

static const struct script_outputs outputs = { .credits_changed = credits_changed };

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static unsigned int make_script(unsigned char *into, const unsigned int *words, unsigned int word_count, const char *strings, unsigned int strings_size)
{
    unsigned int index;
    unsigned int code_size;

    memset(into, 0, SCRIPT_BYTES_MAX);
    memcpy(into, "1LMG", 4);
    code_size = word_count * 2u;
    put_u16(&into[0x08], code_size);
    put_u16(&into[0x0c], strings_size);
    for (index = 0; index < word_count; index++) {
        put_u16(&into[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    memcpy(&into[SCRIPT_FILE_HEADER_SIZE + code_size], strings, strings_size);
    return SCRIPT_FILE_HEADER_SIZE + code_size + strings_size;
}

static void start_the_credits_chain(void)
{
    static const unsigned int page_words[] = { OP_ENDING_MODE, OP_STAFF_ROLL_START, PAGE, OP_STAFF_ROLL_WAIT, OP_STAFF_ROLL_END, OP_CHANGE_SCRIPT, FIRST_STRING, OP_WAIT, A_LONG_WAIT, OP_END };
    static const unsigned int scene_words[] = { OP_ENDING_MODE, OP_WAIT, SCENE_BETWEEN_PAGES_TICKS, OP_CHANGE_SCRIPT, FIRST_STRING, OP_WAIT, A_LONG_WAIT, OP_END };
    static const char page_strings[] = "\0\0scene\0";
    static const char scene_strings[] = "\0\0GOTO TITLE\0";
    unsigned int size;

    credits_reported = 0;
    credits_reports = 0;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_DS, NULL);
    size = make_script(page_script, page_words, sizeof(page_words) / sizeof(page_words[0]), page_strings, sizeof(page_strings) - 1);
    CHECK(script_machine_add_source(&machine, "page", page_script, size, NULL));
    size = make_script(scene_script, scene_words, sizeof(scene_words) / sizeof(scene_words[0]), scene_strings, sizeof(scene_strings) - 1);
    CHECK(script_machine_add_source(&machine, "scene", scene_script, size, NULL));
    CHECK(script_machine_start(&machine, "page"));
}

static void test_the_credits_stay_on_from_the_first_page_through_the_scenes_until_the_title(void)
{
    int ticks;
    int scene_ticks_with_credits;
    int scene_ticks;

    start_the_credits_chain();
    for (ticks = 0; ticks < TICK_LIMIT && !machine.staff_roll.is_on; ticks++) {
        script_machine_tick(&machine);
    }
    CHECK_INT(machine.staff_roll.is_on, 1);
    CHECK_INT(credits_reported, 1);
    for (ticks = 0; ticks < TICK_LIMIT && machine.staff_roll.is_on; ticks++) {
        script_machine_tick(&machine);
    }
    CHECK_INT(machine.staff_roll.is_on, 0);
    scene_ticks = 0;
    scene_ticks_with_credits = 0;
    for (ticks = 0; ticks < TICK_LIMIT && !machine.is_finished; ticks++) {
        script_machine_tick(&machine);
        if (!machine.is_finished) {
            scene_ticks = scene_ticks + 1;
            scene_ticks_with_credits = scene_ticks_with_credits + credits_reported;
        }
    }
    CHECK(scene_ticks >= SCENE_BETWEEN_PAGES_TICKS);
    CHECK_INT(scene_ticks_with_credits, scene_ticks);
    CHECK_INT(machine.title_is_wanted, 1);
    CHECK_INT(credits_reported, 0);
    CHECK_INT(credits_reports, 2);
}

static void test_the_ending_mode_alone_is_not_the_credits(void)
{
    static const unsigned int words[] = { OP_ENDING_MODE, OP_WAIT, A_LONG_WAIT, OP_END };
    static const char strings[] = "\0\0";
    unsigned int size;
    int ticks;

    credits_reported = 0;
    credits_reports = 0;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_DS, NULL);
    size = make_script(final_script, words, sizeof(words) / sizeof(words[0]), strings, sizeof(strings) - 1);
    CHECK(script_machine_add_source(&machine, "final", final_script, size, NULL));
    CHECK(script_machine_start(&machine, "final"));
    for (ticks = 0; ticks < SCENE_BETWEEN_PAGES_TICKS; ticks++) {
        script_machine_tick(&machine);
    }
    CHECK_INT(machine.ending_mode_is_on, 1);
    CHECK_INT(credits_reports, 0);
}

static int side_bars_drawn(struct renderer_rect *left, struct renderer_rect *right)
{
    int bars;
    int index;

    bars = 0;
    for (index = 0; index < fake_renderer_draw_count(); index++) {
        const struct fake_draw *draw;

        draw = fake_renderer_draw(index);
        if (!draw->is_filled_rect || draw->color.red != 0.0f || draw->color.green != 0.0f || draw->color.blue != 0.0f || draw->color.alpha != 1.0f) {
            continue;
        }
        if (draw->screen_area.y != 0.0f || draw->screen_area.height != (float)RENDERER_HEIGHT) {
            continue;
        }
        if (draw->screen_area.x == 0.0f) {
            *left = draw->screen_area;
            bars = bars + 1;
        } else if (draw->screen_area.x + draw->screen_area.width == (float)RENDERER_WIDTH) {
            *right = draw->screen_area;
            bars = bars + 1;
        }
    }
    return bars;
}

static void test_the_view_draws_bars_beside_the_four_by_three_frame_only_during_the_credits(void)
{
    static struct script_tables tables;
    struct renderer_rect left;
    struct renderer_rect right;
    float frame_width;
    float frame_height;

    memset(&left, 0, sizeof(left));
    memset(&right, 0, sizeof(right));
    fake_renderer_reset();
    script_tables_clear(&tables);
    script_view_start(&tables);
    fake_renderer_clear_draws();
    script_view_draw_on_covers();
    CHECK_INT(side_bars_drawn(&left, &right), 0);
    script_view_credits_changed(1);
    fake_renderer_clear_draws();
    script_view_draw_on_covers();
    CHECK_INT(side_bars_drawn(&left, &right), 2);
    frame_width = right.x - (left.x + left.width);
    frame_height = SCENE_VIEW_DS_SCREEN_HEIGHT * VITA_PIXELS_PER_DS_PIXEL;
    CHECK(frame_width == SCENE_VIEW_DS_SCREEN_WIDTH * VITA_PIXELS_PER_DS_PIXEL);
    CHECK(frame_width * 3.0f == frame_height * 4.0f);
    CHECK(left.width == right.width);
    CHECK(left.width > 0.0f);
    CHECK(fake_renderer_draw(fake_renderer_draw_count() - 1)->is_filled_rect);
    script_view_credits_changed(0);
    fake_renderer_clear_draws();
    script_view_draw_on_covers();
    CHECK_INT(side_bars_drawn(&left, &right), 0);
    script_view_credits_changed(1);
    script_view_start(&tables);
    fake_renderer_clear_draws();
    script_view_draw_on_covers();
    CHECK_INT(side_bars_drawn(&left, &right), 0);
}

int main(void)
{
    test_the_credits_stay_on_from_the_first_page_through_the_scenes_until_the_title();
    test_the_ending_mode_alone_is_not_the_credits();
    test_the_view_draws_bars_beside_the_four_by_three_frame_only_during_the_credits();
    return test_finish("unit/credits_side_bars");
}
