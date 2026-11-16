#include <string.h>
#include "fakes/fake_renderer.h"
#include "renderer.h"
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
static const struct script_outputs outputs = { .staff_roll_changed = script_view_staff_roll_changed };

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

static int full_height_black_bars_drawn(void)
{
    int bars;
    int index;

    bars = 0;
    for (index = 0; index < fake_renderer_draw_count(); index++) {
        const struct fake_draw *draw;

        draw = fake_renderer_draw(index);
        if (draw->is_filled_rect && draw->color.red == 0.0f && draw->color.green == 0.0f && draw->color.blue == 0.0f && draw->color.alpha == 1.0f && draw->screen_area.y == 0.0f && draw->screen_area.height == (float)RENDERER_HEIGHT && draw->screen_area.width < (float)RENDERER_WIDTH) {
            bars = bars + 1;
        }
    }
    return bars;
}

static void test_the_ending_credits_draw_no_bars_beside_the_pages_or_the_scenes_between_them(void)
{
    static const unsigned int page_words[] = { OP_ENDING_MODE, OP_STAFF_ROLL_START, PAGE, OP_STAFF_ROLL_WAIT, OP_STAFF_ROLL_END, OP_CHANGE_SCRIPT, FIRST_STRING, OP_WAIT, A_LONG_WAIT, OP_END };
    static const unsigned int scene_words[] = { OP_ENDING_MODE, OP_WAIT, SCENE_BETWEEN_PAGES_TICKS, OP_CHANGE_SCRIPT, FIRST_STRING, OP_WAIT, A_LONG_WAIT, OP_END };
    static const char page_strings[] = "\0\0scene\0";
    static const char scene_strings[] = "\0\0GOTO TITLE\0";
    static struct script_tables tables;
    unsigned int size;
    int ticks;
    int ticks_with_the_roll;
    int bars;

    fake_renderer_reset();
    script_tables_clear(&tables);
    script_view_start(&tables);
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_DS, NULL);
    size = make_script(page_script, page_words, sizeof(page_words) / sizeof(page_words[0]), page_strings, sizeof(page_strings) - 1);
    CHECK(script_machine_add_source(&machine, "page", page_script, size, NULL));
    size = make_script(scene_script, scene_words, sizeof(scene_words) / sizeof(scene_words[0]), scene_strings, sizeof(scene_strings) - 1);
    CHECK(script_machine_add_source(&machine, "scene", scene_script, size, NULL));
    CHECK(script_machine_start(&machine, "page"));
    ticks_with_the_roll = 0;
    bars = 0;
    for (ticks = 0; ticks < TICK_LIMIT && !machine.is_finished; ticks++) {
        script_machine_tick(&machine);
        ticks_with_the_roll = ticks_with_the_roll + machine.staff_roll.is_on;
        fake_renderer_clear_draws();
        script_view_draw_on_covers();
        bars = bars + full_height_black_bars_drawn();
    }
    CHECK(ticks_with_the_roll > 0);
    CHECK_INT(machine.title_is_wanted, 1);
    CHECK_INT(bars, 0);
}

int main(void)
{
    test_the_ending_credits_draw_no_bars_beside_the_pages_or_the_scenes_between_them();
    return test_finish("unit/ending_credits_have_no_side_bars");
}
