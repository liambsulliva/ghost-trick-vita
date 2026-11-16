#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "test_support.h"

#define NORMAL_PLAY_MODE 3
#define ROW_Y 100
#define BODY 0
#define CORPSE 1
#define LAMP 2
#define DEAD_TALK_GIMMICK_CLASS_ID 0x2981d97eu

static struct script_machine machine;
static struct script_outputs outputs;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 16];
static int core_marked_dead[3];
static int object_styled_dead[3];

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void note_core(int object_index, int slot, float x, float y, int is_dead, float opacity)
{
    (void)slot;
    (void)x;
    (void)y;
    (void)opacity;
    if (object_index >= 0 && object_index < 3) {
        core_marked_dead[object_index] = is_dead;
    }
}

static void note_style(const char *object_name, unsigned int class_id, int has_core, int is_active, int is_dead, int is_phone_view)
{
    int index;

    (void)class_id;
    (void)has_core;
    (void)is_active;
    (void)is_phone_view;
    for (index = 0; index < 3; index++) {
        if (strcmp(object_name, machine.objects[index].name) == 0) {
            object_styled_dead[index] = is_dead;
        }
    }
}

static void place_core(int index, const char *name, int x)
{
    machine.objects[index].name = name;
    machine.objects[index].actor.exists = 1;
    machine.objects[index].actor.is_visible = 1;
    machine.objects[index].actor.has_core = 1;
    machine.objects[index].actor.x = x;
    machine.objects[index].actor.y = ROW_Y;
}

static void start_in_the_ghost_view(unsigned int corpse_class, int corpse_is_marked)
{
    static const unsigned int words[] = { 0x2f, 60000, 0x0 };
    unsigned int index;

    memset(scene, 0, sizeof(scene));
    memcpy(scene, "1LMG", 4);
    put_u16(&scene[0x08], sizeof(words) / sizeof(words[0]) * 2);
    for (index = 0; index < sizeof(words) / sizeof(words[0]); index++) {
        put_u16(&scene[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    memset(&outputs, 0, sizeof(outputs));
    outputs.soul_core_marked = note_core;
    outputs.soul_object_styled = note_style;
    memset(core_marked_dead, -1, sizeof(core_marked_dead));
    memset(object_styled_dead, -1, sizeof(object_styled_dead));
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    CHECK(script_machine_add_source(&machine, "a", scene, SCRIPT_FILE_HEADER_SIZE + sizeof(words) / sizeof(words[0]) * 2, NULL));
    CHECK(script_machine_start(&machine, "a"));
    machine.object_count = 3;
    place_core(BODY, "body", 100);
    place_core(CORPSE, "corpse", 140);
    place_core(LAMP, "lamp", 180);
    machine.objects[CORPSE].actor.class_id = corpse_class;
    machine.objects[CORPSE].is_dead = corpse_is_marked;
    machine.mode_byte = NORMAL_PLAY_MODE;
    machine.pause_bit = 0;
    script_soul_set_first_core(&machine, BODY, 0);
    machine.soul.is_on = 1;
    script_soul_step_before_tasks(&machine);
}

static void test_a_dead_talk_body_shows_a_corpse_core_without_the_script_marking_it(void)
{
    start_in_the_ghost_view(DEAD_TALK_GIMMICK_CLASS_ID, 0);
    CHECK_INT(core_marked_dead[CORPSE], 1);
    CHECK_INT(object_styled_dead[CORPSE], 1);
    CHECK_INT(core_marked_dead[LAMP], 0);
    CHECK_INT(object_styled_dead[LAMP], 0);
}

static void test_a_body_the_script_marks_dead_still_shows_a_corpse_core(void)
{
    start_in_the_ghost_view(0, 1);
    CHECK_INT(core_marked_dead[CORPSE], 1);
    CHECK_INT(object_styled_dead[CORPSE], 1);
}

int main(void)
{
    test_a_dead_talk_body_shows_a_corpse_core_without_the_script_marking_it();
    test_a_body_the_script_marks_dead_still_shows_a_corpse_core();
    return test_finish("unit/dead_talk_core_looks_dead");
}
