#include <string.h>
#include "script_classes.h"
#include "script_machine.h"
#include "test_support.h"

#define START_CORE 0
#define BODY 1
#define LAMP 2
#define NO_CORE_HELD (-1)
#define FADE_IN_TICKS 60
#define HOLD_TICKS 60
#define VANISH_TICKS 100

static struct script_machine machine;
static const struct script_outputs no_outputs;

static void place(int index, const char *name, unsigned int class_id)
{
    machine.objects[index].name = name;
    machine.objects[index].actor.exists = 1;
    machine.objects[index].actor.is_visible = 1;
    machine.objects[index].actor.class_id = class_id;
}

static void start_a_puzzle_with_the_soul_on_its_start_core(void)
{
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.object_count = 3;
    place(START_CORE, "souldead", SCRIPT_CLASS_GMK_SOUL_DEAD);
    place(BODY, "linnedead", SCRIPT_CLASS_HUM_PAST_DEAD);
    place(LAMP, "lamp", SCRIPT_CLASS_GMK_COMMON_DEAD);
    machine.soul.active_object = START_CORE;
}

static void run_ticks(int ticks)
{
    int tick;

    for (tick = 0; tick < ticks; tick++) {
        script_ghost_copies_update(&machine);
    }
}

static int shown(int index)
{
    return script_ghost_copy_shown_level(&machine.objects[index].actor.ghost_copy);
}

static void test_a_past_dead_body_shows_a_full_copy_and_never_draws_itself(void)
{
    start_a_puzzle_with_the_soul_on_its_start_core();
    run_ticks(1);
    CHECK_INT(shown(BODY), SCRIPT_GHOST_COPY_LEVEL_FULL);
    CHECK_INT(shown(LAMP), -1);
    CHECK(!script_class_draws_in_living_view(SCRIPT_CLASS_HUM_PAST_DEAD));
    CHECK(!script_class_draws_in_soul_view(SCRIPT_CLASS_HUM_PAST_DEAD_MISSILE));
}

static void test_a_body_shown_again_fades_its_copy_back_in(void)
{
    start_a_puzzle_with_the_soul_on_its_start_core();
    run_ticks(1);
    machine.objects[BODY].actor.is_visible = 0;
    run_ticks(1);
    CHECK_INT(shown(BODY), 0);
    machine.objects[BODY].actor.is_visible = 1;
    run_ticks(FADE_IN_TICKS / 2);
    CHECK(shown(BODY) > 0);
    CHECK(shown(BODY) < SCRIPT_GHOST_COPY_LEVEL_FULL);
    run_ticks(FADE_IN_TICKS);
    CHECK_INT(shown(BODY), SCRIPT_GHOST_COPY_LEVEL_FULL);
}

static void test_leaving_the_start_core_holds_then_fades_the_body_and_the_core(void)
{
    start_a_puzzle_with_the_soul_on_its_start_core();
    run_ticks(10);
    machine.soul.active_object = LAMP;
    run_ticks(HOLD_TICKS - 1);
    CHECK_INT(shown(BODY), SCRIPT_GHOST_COPY_LEVEL_FULL);
    CHECK_INT(shown(START_CORE), SCRIPT_GHOST_COPY_LEVEL_FULL);
    run_ticks(VANISH_TICKS / 2);
    CHECK(shown(BODY) > 0);
    CHECK(shown(BODY) < SCRIPT_GHOST_COPY_LEVEL_FULL);
    CHECK_INT(shown(START_CORE), shown(BODY));
    run_ticks(VANISH_TICKS);
    CHECK_INT(shown(BODY), 0);
    CHECK_INT(shown(START_CORE), 0);
    CHECK(machine.objects[BODY].actor.ghost_copy.is_gone);
    machine.soul.active_object = START_CORE;
    run_ticks(HOLD_TICKS);
    CHECK_INT(shown(BODY), 0);
}

static void test_a_start_core_the_soul_never_held_stays_lit(void)
{
    start_a_puzzle_with_the_soul_on_its_start_core();
    machine.soul.active_object = NO_CORE_HELD;
    run_ticks(HOLD_TICKS + VANISH_TICKS + 10);
    CHECK_INT(shown(START_CORE), SCRIPT_GHOST_COPY_LEVEL_FULL);
    CHECK_INT(shown(BODY), SCRIPT_GHOST_COPY_LEVEL_FULL);
}

int main(void)
{
    test_a_past_dead_body_shows_a_full_copy_and_never_draws_itself();
    test_a_body_shown_again_fades_its_copy_back_in();
    test_leaving_the_start_core_holds_then_fades_the_body_and_the_core();
    test_a_start_core_the_soul_never_held_stays_lit();
    return test_finish("unit/ghost_copy");
}
