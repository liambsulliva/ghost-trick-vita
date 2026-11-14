#include "script_machine.h"
#include "test_support.h"

#define NORMAL_PLAY_MODE 3
#define SWITCH_SOUND_KIND 31
#define ROOM_WIDTH 1024
#define ROOM_HEIGHT 192
#define SISSEL_CORE_X 100
#define MISSILE_CORE_X 600
#define CORE_Y 96
#define STEPS_FOR_A_PILL_TO_SLIDE_IN 40

static struct script_machine machine;
static int switch_sounds;
static int views_entered_with_effect;

static void on_sound_played(int kind)
{
    if (kind == SWITCH_SOUND_KIND) {
        switch_sounds = switch_sounds + 1;
    }
}

static void on_soul_vision_changed(int is_on, int with_effect)
{
    if (is_on && with_effect) {
        views_entered_with_effect = views_entered_with_effect + 1;
    }
}

static const struct script_outputs outputs = {
    .sound_played = on_sound_played,
    .soul_vision_changed = on_soul_vision_changed,
};

static void step(int ticks)
{
    int tick;

    for (tick = 0; tick < ticks; tick++) {
        script_soul_step_before_tasks(&machine);
    }
}

static void start_with_sissel_and_missile_far_apart(void)
{
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_REMASTER, NULL);
    machine.room = 0;
    machine.room_bounds[0].width = ROOM_WIDTH;
    machine.room_bounds[0].height = ROOM_HEIGHT;
    machine.object_count = 2;
    machine.objects[0].name = "lamp";
    machine.objects[1].name = "basketball";
    machine.objects[0].actor.exists = 1;
    machine.objects[1].actor.exists = 1;
    machine.objects[0].actor.has_core = 1;
    machine.objects[1].actor.has_core = 1;
    machine.objects[0].actor.x = SISSEL_CORE_X;
    machine.objects[1].actor.x = MISSILE_CORE_X;
    machine.objects[0].actor.y = CORE_Y;
    machine.objects[1].actor.y = CORE_Y;
    machine.mode_byte = NORMAL_PLAY_MODE;
    machine.pause_bit = 0;
    script_soul_make_ghost_active(&machine, SCRIPT_GHOST_MISSILE);
    script_soul_set_first_core(&machine, 1, 0);
    script_soul_make_ghost_active(&machine, SCRIPT_GHOST_CICEL);
    script_soul_set_first_core(&machine, 0, 0);
    script_soul_run_command(&machine, "Button", "Missile:On");
    script_soul_run_command(&machine, "Button", "Cicel:On");
    switch_sounds = 0;
    views_entered_with_effect = 0;
}

static void switch_to_missile(void)
{
    int tick;

    for (tick = 0; tick < STEPS_FOR_A_PILL_TO_SLIDE_IN && !script_pills_is_up(&machine, SCRIPT_PILL_MISSILE); tick++) {
        step(1);
    }
    CHECK(script_pills_is_up(&machine, SCRIPT_PILL_MISSILE));
    script_soul_note_character_button(&machine);
    step(1);
    CHECK_INT(machine.soul.active_ghost, SCRIPT_GHOST_MISSILE);
}

static void test_a_switch_in_the_ghost_view_swooshes_and_lights_a_new_flame_on_the_new_core(void)
{
    start_with_sissel_and_missile_far_apart();
    machine.soul.is_on = 1;
    machine.pause_bit = 1;
    step(2);
    CHECK(machine.soul.chain.trail_x[0] < SISSEL_CORE_X + 1.0f);
    switch_to_missile();
    CHECK_INT(switch_sounds, 1);
    CHECK_INT(views_entered_with_effect, 1);
    step(1);
    CHECK(machine.soul.chain.trail_x[0] > MISSILE_CORE_X - 1.0f);
    CHECK(machine.soul.chain.trail_x[0] < MISSILE_CORE_X + 1.0f);
}

int main(void)
{
    test_a_switch_in_the_ghost_view_swooshes_and_lights_a_new_flame_on_the_new_core();
    return test_finish("unit/character_switch");
}
