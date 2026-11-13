#include <string.h>
#include "script_file.h"
#include "script_machine.h"
#include "script_opcodes.h"
#include "test_support.h"

#define CRATE 0
#define BARREL 1
#define LAMP 2
#define SAME_SHAPE 3
#define OTHER_SHAPE 4
#define OUTLINE_TICKS 0x18
#define SWAP_TICKS (0x19 + 0x20)
#define FRAME_HOLD_SHIFT 10
#define MARK_MAX 16

static struct script_machine machine;
static struct script_outputs outputs;
static struct script_source source;
static unsigned char bytes[SCRIPT_FILE_HEADER_SIZE + 2 * 16];
static char marks[MARK_MAX][48];
static int mark_count;

static void keep_the_mark(const char *kind, const char *detail)
{
    (void)detail;
    if (mark_count < MARK_MAX) {
        strncpy(marks[mark_count], kind, sizeof(marks[mark_count]) - 1);
        mark_count = mark_count + 1;
    }
}

static int marks_named(const char *kind)
{
    int index;
    int count;

    count = 0;
    for (index = 0; index < mark_count; index++) {
        if (strcmp(marks[index], kind) == 0) {
            count = count + 1;
        }
    }
    return count;
}

static void start_with_missile_on_the_crate(void)
{
    memset(&outputs, 0, sizeof(outputs));
    memset(marks, 0, sizeof(marks));
    mark_count = 0;
    outputs.marked = keep_the_mark;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.object_count = 3;
    machine.objects[CRATE].name = "crate";
    machine.objects[BARREL].name = "barrel";
    machine.objects[LAMP].name = "lamp";
    machine.objects[CRATE].actor.exists = 1;
    machine.objects[BARREL].actor.exists = 1;
    machine.objects[LAMP].actor.exists = 1;
    machine.objects[CRATE].actor.shape = SAME_SHAPE;
    machine.objects[BARREL].actor.shape = SAME_SHAPE;
    machine.objects[LAMP].actor.shape = OTHER_SHAPE;
    script_soul_make_ghost_active(&machine, SCRIPT_GHOST_MISSILE);
    script_soul_set_first_core(&machine, CRATE, 0);
    machine.soul.is_on = 1;
}

static void step_for(int ticks)
{
    int index;

    for (index = 0; index < ticks; index++) {
        script_exchange_step(&machine);
    }
}

static void run_core_op(unsigned int op, int object_index)
{
    unsigned int words[4];
    struct script_vm vm;

    memset(&vm, 0, sizeof(vm));
    memset(words, 0, sizeof(words));
    vm.owner = &machine.objects[object_index];
    vm.selected_object = SCRIPT_NO_SELECTION;
    CHECK_INT(script_ops_actor_run(&machine, &vm, op, words), SCRIPT_STEP_NEXT);
}

static void test_only_missile_links_and_only_to_the_same_shape(void)
{
    start_with_missile_on_the_crate();
    CHECK_INT(script_exchange_link(&machine, LAMP, 0), 0);
    CHECK_INT(script_exchange_is_on(&machine), 0);
    machine.objects[CRATE].actor.shape = 0;
    machine.objects[BARREL].actor.shape = 0;
    CHECK_INT(script_exchange_link(&machine, BARREL, 0), 0);
    start_with_missile_on_the_crate();
    script_soul_make_ghost_active(&machine, SCRIPT_GHOST_CICEL);
    script_soul_set_first_core(&machine, CRATE, 0);
    CHECK_INT(script_exchange_link(&machine, BARREL, 0), 0);
    start_with_missile_on_the_crate();
    CHECK_INT(script_exchange_link(&machine, BARREL, 0), 1);
    CHECK_INT(script_exchange_is_on(&machine), 1);
    CHECK_INT(script_exchange_holds_the_chain_out(&machine), 1);
    CHECK_INT(marks_named("exchange_linked"), 1);
    CHECK_INT(script_exchange_link(&machine, BARREL, 0), 0);
}

static void test_the_buttons_come_after_the_outline(void)
{
    start_with_missile_on_the_crate();
    script_exchange_link(&machine, BARREL, 0);
    step_for(OUTLINE_TICKS - 1);
    CHECK_INT(script_exchange_offers_the_buttons(&machine), 0);
    step_for(1);
    CHECK_INT(script_exchange_offers_the_buttons(&machine), 1);
}

static void test_the_swap_waits_for_the_scripts_then_settles(void)
{
    start_with_missile_on_the_crate();
    script_exchange_link(&machine, BARREL, 0);
    step_for(OUTLINE_TICKS);
    script_exchange_confirm(&machine);
    CHECK_INT(machine.soul.exchange.phase, SCRIPT_EXCHANGE_PRESSED);
    CHECK_INT(script_exchange_offers_the_buttons(&machine), 0);
    CHECK_INT(marks_named("exchange"), 0);
    step_for(1);
    CHECK_INT(machine.soul.exchange.phase, SCRIPT_EXCHANGE_ASKED);
    CHECK_INT(marks_named("exchange"), 1);
    step_for(200);
    CHECK_INT(machine.soul.exchange.phase, SCRIPT_EXCHANGE_ASKED);
    run_core_op(0xa4, BARREL);
    run_core_op(0xa5, CRATE);
    CHECK_INT(machine.soul.active_object, BARREL);
    CHECK_INT(machine.soul.ghost_core_objects[SCRIPT_GHOST_MISSILE], BARREL);
    CHECK_INT(machine.soul.exchange.root_object, BARREL);
    CHECK_INT(machine.soul.exchange.target_object, CRATE);
    step_for(1);
    CHECK_INT(machine.soul.exchange.phase, SCRIPT_EXCHANGE_SETTLING);
    CHECK_INT(script_exchange_holds_the_chain_out(&machine), 0);
    step_for(SWAP_TICKS - 1);
    CHECK_INT(script_exchange_is_on(&machine), 1);
    CHECK_INT(machine.soul.exchange.swaps_done, 0);
    step_for(1);
    CHECK_INT(script_exchange_is_on(&machine), 0);
    CHECK_INT(machine.soul.exchange.swaps_done, 1);
}

static void test_unlocking_tells_the_scripts_and_ends_the_link(void)
{
    start_with_missile_on_the_crate();
    script_exchange_link(&machine, BARREL, 0);
    step_for(OUTLINE_TICKS);
    script_exchange_cancel(&machine);
    CHECK_INT(script_exchange_is_on(&machine), 0);
    CHECK_INT(marks_named("exchange_cancelled"), 1);
    CHECK_INT(machine.soul.exchange.swaps_done, 0);
    CHECK_INT(machine.soul.active_object, CRATE);
}

static void test_a_swap_under_way_cannot_be_unlocked(void)
{
    start_with_missile_on_the_crate();
    script_exchange_link(&machine, BARREL, 0);
    script_exchange_confirm(&machine);
    script_exchange_cancel(&machine);
    CHECK_INT(script_exchange_is_on(&machine), 1);
    CHECK_INT(marks_named("exchange_cancelled"), 0);
}

static void test_leaving_the_ghost_view_drops_the_link(void)
{
    start_with_missile_on_the_crate();
    script_exchange_link(&machine, BARREL, 0);
    machine.soul.is_on = 0;
    step_for(1);
    CHECK_INT(script_exchange_is_on(&machine), 0);
}

static void test_a_view_the_scene_closes_mid_swap_still_counts_the_swap(void)
{
    start_with_missile_on_the_crate();
    script_exchange_link(&machine, BARREL, 0);
    step_for(OUTLINE_TICKS);
    script_exchange_confirm(&machine);
    step_for(1);
    run_core_op(0xa4, BARREL);
    run_core_op(0xa5, CRATE);
    step_for(2);
    CHECK_INT(machine.soul.exchange.phase, SCRIPT_EXCHANGE_SETTLING);
    machine.soul.is_on = 0;
    step_for(1);
    CHECK_INT(script_exchange_is_on(&machine), 0);
    CHECK_INT(machine.soul.exchange.swaps_done, 1);
    CHECK_INT(machine.soul.active_object, BARREL);
}

static void test_the_pills_show_only_swap_and_unlock_while_linked(void)
{
    start_with_missile_on_the_crate();
    machine.objects[CRATE].actor.is_visible = 1;
    script_pills_register_all(&machine);
    script_exchange_link(&machine, BARREL, 0);
    step_for(OUTLINE_TICKS);
    for (int tick = 0; tick < 40; tick++) {
        script_pills_step(&machine);
    }
    CHECK_INT(script_pills_is_up(&machine, SCRIPT_PILL_EXCHANGE), 1);
    CHECK_INT(script_pills_is_up(&machine, SCRIPT_PILL_UNLOCK), 1);
    CHECK_INT(script_pills_is_up(&machine, SCRIPT_PILL_BACK), 0);
    script_pills_register(&machine, SCRIPT_PILL_EXCHANGE, 0);
    for (int tick = 0; tick < 40; tick++) {
        script_pills_step(&machine);
    }
    CHECK_INT(script_pills_is_up(&machine, SCRIPT_PILL_EXCHANGE), 0);
    CHECK_INT(script_pills_is_up(&machine, SCRIPT_PILL_UNLOCK), 1);
}

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void test_the_motion_table_names_the_shape(void)
{
    static const unsigned int table[] = { 0xfffd, 0, 0, 0xfffa, SAME_SHAPE, 1 | (4u << FRAME_HOLD_SHIFT), 0xffff };
    unsigned int word_count;
    unsigned int index;

    word_count = sizeof(table) / sizeof(table[0]);
    memset(bytes, 0, sizeof(bytes));
    memcpy(bytes, "1LMG", 4);
    bytes[0x08] = (unsigned char)(word_count * 2);
    for (index = 0; index < word_count; index++) {
        put_u16(&bytes[SCRIPT_FILE_HEADER_SIZE + 2 * index], table[index]);
    }
    memset(&source, 0, sizeof(source));
    CHECK_INT(script_file_open(&source.file, bytes, SCRIPT_FILE_HEADER_SIZE + 2 * word_count), SCRIPT_FILE_OK);
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.object_count = 1;
    machine.objects[0].name = "crate";
    machine.objects[0].actor.exists = 1;
    machine.objects[0].main_vm.source = &source;
    CHECK_INT(machine.objects[0].actor.shape, 0);
    machine.objects[0].actor.has_queued_table = 1;
    machine.objects[0].actor.queued_table_word = 0;
    script_motion_update_all(&machine);
    CHECK_INT(machine.objects[0].actor.shape, SAME_SHAPE);
    CHECK_INT(machine.objects[0].actor.frame_number, 1);
}

static void test_the_ops_are_routed_to_the_actor_family(void)
{
    CHECK_INT(script_opcode_family(0xa5), SCRIPT_OP_ACTOR);
    CHECK_INT(script_opcode_family(0xa6), SCRIPT_OP_ACTOR);
}

int main(void)
{
    test_only_missile_links_and_only_to_the_same_shape();
    test_the_buttons_come_after_the_outline();
    test_the_swap_waits_for_the_scripts_then_settles();
    test_unlocking_tells_the_scripts_and_ends_the_link();
    test_a_swap_under_way_cannot_be_unlocked();
    test_leaving_the_ghost_view_drops_the_link();
    test_a_view_the_scene_closes_mid_swap_still_counts_the_swap();
    test_the_pills_show_only_swap_and_unlock_while_linked();
    test_the_motion_table_names_the_shape();
    test_the_ops_are_routed_to_the_actor_family();
    return test_finish("unit/exchange");
}
