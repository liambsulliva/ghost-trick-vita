#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "test_support.h"

#define OP_DEMO_VIEW_ON 0xe2
#define OP_SCRIPT_CHANGE 0x24
#define OP_WAIT 0x2f
#define OP_END 0x0
#define A_LONG_WAIT 6000
#define NEXT_SCRIPT_STRING 2
#define TICKS_UNTIL_THE_NEXT_SCRIPT_RUNS 12
#define NOT_TOLD_YET (-1)

static struct script_machine machine;
static int styled_count;
static int active_count;
static int marked_count;
static int view_the_stage_was_told;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 64];
static unsigned char next_scene[SCRIPT_FILE_HEADER_SIZE + 64];

static const char strings[] = "\0\0b\0";

static void on_styled(const char *object_name, unsigned int class_id, int has_core, int is_active, int is_dead, int is_phone_view)
{
    (void)object_name;
    (void)class_id;
    (void)has_core;
    (void)is_dead;
    (void)is_phone_view;
    styled_count = styled_count + 1;
    active_count = active_count + is_active;
}

static void on_core_marked(int object_index, int slot, float x, float y, int is_dead, float opacity)
{
    (void)object_index;
    (void)slot;
    (void)x;
    (void)y;
    (void)is_dead;
    (void)opacity;
    marked_count = marked_count + 1;
}

static void on_soul_vision_changed(int is_on, int with_effect)
{
    (void)with_effect;
    view_the_stage_was_told = is_on;
}

static const struct script_outputs outputs = {
    .soul_object_styled = on_styled,
    .soul_core_marked = on_core_marked,
    .soul_vision_changed = on_soul_vision_changed,
};

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static unsigned int make_script(unsigned char *into, const unsigned int *words, unsigned int word_count)
{
    unsigned int index;
    unsigned int code_size;

    memset(into, 0, SCRIPT_FILE_HEADER_SIZE + word_count * 2 + sizeof(strings) - 1);
    memcpy(into, "1LMG", 4);
    code_size = word_count * 2;
    put_u16(&into[0x08], code_size);
    put_u16(&into[0x0c], sizeof(strings) - 1);
    for (index = 0; index < word_count; index++) {
        put_u16(&into[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    memcpy(&into[SCRIPT_FILE_HEADER_SIZE + code_size], strings, sizeof(strings) - 1);
    return SCRIPT_FILE_HEADER_SIZE + code_size + sizeof(strings) - 1;
}

static void start_with_two_objects(void)
{
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.object_count = 2;
    machine.objects[0].name = "cone";
    machine.objects[0].actor.exists = 1;
    machine.objects[0].actor.has_core = 1;
    machine.objects[1].name = "guard";
    machine.objects[1].actor.exists = 1;
    script_soul_set_first_core(&machine, 0, 0);
    styled_count = 0;
    active_count = 0;
    marked_count = 0;
}

static void test_the_world_is_not_styled_outside_the_view(void)
{
    start_with_two_objects();
    script_soul_step_before_tasks(&machine);
    CHECK_INT(styled_count, 0);
}

static void test_a_scripted_soul_view_styles_every_object_and_marks_no_core(void)
{
    start_with_two_objects();
    machine.demo_soul_is_on = 1;
    script_soul_step_before_tasks(&machine);
    CHECK_INT(styled_count, 2);
    CHECK_INT(active_count, 0);
    CHECK_INT(marked_count, 0);
}

static void test_the_player_s_view_styles_every_object_too(void)
{
    start_with_two_objects();
    machine.soul.is_on = 1;
    script_soul_step_before_tasks(&machine);
    CHECK_INT(styled_count, 2);
    CHECK_INT(active_count, 1);
}

static void test_a_script_change_closes_the_demo_view_the_earlier_script_opened(void)
{
    static const unsigned int leaving_words[] = { OP_DEMO_VIEW_ON, 0, OP_SCRIPT_CHANGE, NEXT_SCRIPT_STRING, OP_WAIT, A_LONG_WAIT, OP_END };
    static const unsigned int idle_words[] = { OP_WAIT, A_LONG_WAIT, OP_END };
    int tick;

    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    CHECK(script_machine_add_source(&machine, "a", scene, make_script(scene, leaving_words, sizeof(leaving_words) / sizeof(leaving_words[0])), NULL));
    CHECK(script_machine_add_source(&machine, "b", next_scene, make_script(next_scene, idle_words, sizeof(idle_words) / sizeof(idle_words[0])), NULL));
    CHECK(script_machine_start(&machine, "a"));
    view_the_stage_was_told = NOT_TOLD_YET;
    script_machine_tick(&machine);
    CHECK_INT(machine.demo_soul_is_on, 1);
    CHECK_INT(view_the_stage_was_told, 1);
    for (tick = 0; tick < TICKS_UNTIL_THE_NEXT_SCRIPT_RUNS; tick++) {
        script_machine_tick(&machine);
    }
    CHECK_INT(machine.demo_soul_is_on, 0);
    CHECK_INT(view_the_stage_was_told, 0);
}

static void test_a_script_change_keeps_the_buttons_but_not_the_character_ones(void)
{
    static const unsigned int leaving_words[] = { OP_SCRIPT_CHANGE, NEXT_SCRIPT_STRING, OP_WAIT, A_LONG_WAIT, OP_END };
    static const unsigned int idle_words[] = { OP_WAIT, A_LONG_WAIT, OP_END };
    int tick;

    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    CHECK(script_machine_add_source(&machine, "a", scene, make_script(scene, leaving_words, sizeof(leaving_words) / sizeof(leaving_words[0])), NULL));
    CHECK(script_machine_add_source(&machine, "b", next_scene, make_script(next_scene, idle_words, sizeof(idle_words) / sizeof(idle_words[0])), NULL));
    CHECK(script_machine_start(&machine, "a"));
    script_pills_register(&machine, SCRIPT_PILL_TRICK, 1);
    script_pills_register(&machine, SCRIPT_PILL_JOWD, 1);
    script_pills_register(&machine, SCRIPT_PILL_MISSILE, 1);
    script_pills_register(&machine, SCRIPT_PILL_CICEL, 1);
    for (tick = 0; tick <= TICKS_UNTIL_THE_NEXT_SCRIPT_RUNS; tick++) {
        script_machine_tick(&machine);
    }
    CHECK_INT((machine.soul.pills_registered >> SCRIPT_PILL_TRICK) & 1u, 1);
    CHECK_INT((machine.soul.pills_registered >> SCRIPT_PILL_JOWD) & 1u, 0);
    CHECK_INT((machine.soul.pills_registered >> SCRIPT_PILL_MISSILE) & 1u, 0);
    CHECK_INT((machine.soul.pills_registered >> SCRIPT_PILL_CICEL) & 1u, 0);
}

int main(void)
{
    test_the_world_is_not_styled_outside_the_view();
    test_a_scripted_soul_view_styles_every_object_and_marks_no_core();
    test_the_player_s_view_styles_every_object_too();
    test_a_script_change_closes_the_demo_view_the_earlier_script_opened();
    test_a_script_change_keeps_the_buttons_but_not_the_character_ones();
    return test_finish("unit/demo_soul_view_styles");
}
