#include <stdio.h>
#include <string.h>
#include "script_file.h"
#include "script_flags.h"
#include "script_machine.h"
#include "script_opcodes.h"
#include "test_support.h"

#define OP_WAIT 0x2f
#define OP_END 0x0
#define A_LONG_WAIT 6000
#define NAME_OFFSET 2
#define EVERY_NAME_OFFSET 11
#define ROOM_WIDTH 1024
#define ROOM_HEIGHT 640
#define FIXED_ONE 0x1000u
#define NO_HOME 0xffffu
#define HOME_X 253
#define HOME_Y 160
#define HOME_PX_PER_TICK 16
#define ALARM_MODE 9
#define PALETTE_OF_THE_GUARD 135
#define HUMAN_CLASS_NAME "uHumNormal"
#define REACH_PX 144
#define LEFT_OF_THE_OWNER 128

static struct script_machine machine;
static unsigned char scene[SCRIPT_FILE_HEADER_SIZE + 96];
static const char strings[] = "\0\0fountain\0*\0";
static char name_told[32];
static float x_told;
static float y_told;
static int ticks_told;
static int marks_of_ops_not_ported;

static void remember(const char *name, float x, float y, int ticks)
{
    snprintf(name_told, sizeof(name_told), "%s", name);
    x_told = x;
    y_told = y;
    ticks_told = ticks;
}

static void effect_placed(const char *name, float x, float y) { remember(name, x, y, 0); }
static void effect_seeked(const char *name, int ticks) { remember(name, 0.0f, 0.0f, ticks); }
static void effect_scaled(const char *name, float x, float y) { remember(name, x, y, 0); }
static void effect_scale_ramp_started(const char *name, int ticks, float x, float y) { remember(name, x, y, ticks); }

static void marked(const char *what, const char *name)
{
    (void)name;
    if (strcmp(what, "op_not_ported") == 0) {
        marks_of_ops_not_ported = marks_of_ops_not_ported + 1;
    }
}

static const struct script_outputs outputs = {
    .effect_placed = effect_placed,
    .effect_seeked = effect_seeked,
    .effect_scaled = effect_scaled,
    .effect_scale_ramp_started = effect_scale_ramp_started,
    .marked = marked,
};

static void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static void run_a_scene_of(const unsigned int *op_words, unsigned int op_word_count)
{
    unsigned int words[24];
    unsigned int count;
    unsigned int index;
    unsigned int code_size;

    count = 0;
    for (index = 0; index < op_word_count; index++) {
        words[count++] = op_words[index];
    }
    words[count++] = OP_WAIT;
    words[count++] = A_LONG_WAIT;
    words[count++] = OP_END;
    memset(scene, 0, sizeof(scene));
    memcpy(scene, "1LMG", 4);
    code_size = count * 2;
    put_u16(&scene[0x08], code_size);
    put_u16(&scene[0x0c], sizeof(strings) - 1);
    for (index = 0; index < count; index++) {
        put_u16(&scene[SCRIPT_FILE_HEADER_SIZE + 2 * index], words[index]);
    }
    memcpy(&scene[SCRIPT_FILE_HEADER_SIZE + code_size], strings, sizeof(strings) - 1);
    name_told[0] = '\0';
    marks_of_ops_not_ported = 0;
    script_machine_init(&machine, &outputs, SCRIPT_TIMING_DS, NULL);
    CHECK(script_machine_add_source(&machine, "a", scene, SCRIPT_FILE_HEADER_SIZE + code_size + sizeof(strings) - 1, NULL));
    CHECK(script_machine_start(&machine, "a"));
    script_machine_tick(&machine);
}

static int the_script_reached_the_wait(void)
{
    return machine.runner.wait_ticks > 0;
}

static void set_up_a_room(void)
{
    static const struct script_outputs quiet;
    int index;

    script_machine_init(&machine, &quiet, SCRIPT_TIMING_DS, NULL);
    machine.room = 0;
    machine.room_bounds[0].width = ROOM_WIDTH;
    machine.room_bounds[0].height = ROOM_HEIGHT;
    for (index = 0; index < SCRIPT_CAMERA_BAND_MAX; index++) {
        machine.camera_band_keys[index] = 0xffff;
        machine.camera_band_values[index] = 0xffff;
    }
    machine.mode_byte = SCRIPT_MODE_NORMAL;
}

static int the_last_message_is(int object, const char *verb, const char *arg)
{
    const struct script_object *listener;

    listener = &machine.objects[object];
    return listener->message_count > 0 && strcmp(listener->messages[listener->message_count - 1].verb, verb) == 0 && strcmp(listener->messages[listener->message_count - 1].arg, arg) == 0;
}

static void add_an_actor(const char *name, int x, int y)
{
    struct script_object *object;

    object = &machine.objects[machine.object_count];
    machine.object_count = machine.object_count + 1;
    object->name = name;
    object->actor.exists = 1;
    object->actor.has_core = 1;
    object->actor.x = x;
    object->actor.y = y;
}

static void run_view_op(unsigned int op, unsigned int a, unsigned int b, unsigned int c, unsigned int d)
{
    struct script_vm vm;
    unsigned int words[4] = { a, b, c, d };

    memset(&vm, 0, sizeof(vm));
    vm.selected_object = SCRIPT_NO_SELECTION;
    CHECK_INT(script_opcode_family(op), SCRIPT_OP_VIEW);
    script_ops_view_run(&machine, &vm, op, words);
}

static void test_no_op_of_the_twenty_logs_as_not_ported(void)
{
    static const unsigned int ops[] = { 0x08, 0x09, 0x14, 0x48, 0x49, 0x9a, 0xaf, 0xb4, 0xb5, 0xb6, 0xb7, 0xc2, 0xc3, 0xc5, 0xd1, 0xd2, 0xd3, 0xd4, 0xfd, 0x119 };
    unsigned int index;

    for (index = 0; index < sizeof(ops) / sizeof(ops[0]); index++) {
        unsigned int words[8] = { ops[index], NAME_OFFSET, 0, 0, 0, 0, 0, 0 };

        CHECK(script_opcode_family(ops[index]) != SCRIPT_OP_ABSENT);
        run_a_scene_of(words, script_opcode_length(ops[index]));
        CHECK_INT(marks_of_ops_not_ported, 0);
        if (ops[index] != 0xc5) {
            CHECK(the_script_reached_the_wait());
        }
    }
}

static void test_the_saved_camera_comes_back_as_a_cut(void)
{
    set_up_a_room();
    run_view_op(0xc8, 512, 14, 0, 0);
    run_view_op(0xd1, 0, 0, 0, 0);
    run_view_op(0xc8, 0, 0, 0, 0);
    machine.pan_is_active = 1;
    run_view_op(0xd2, 0, 0, 0, 0);
    CHECK_INT(machine.camera_x, 512);
    CHECK_INT(machine.camera_y, 14);
    CHECK_INT(machine.pan_is_active, 0);
}

static void test_a_camera_home_takes_the_view_close_from_the_saved_camera(void)
{
    set_up_a_room();
    add_an_actor("game", 0, 0);
    run_view_op(0xd3, HOME_X, HOME_Y, 0, 0);
    CHECK(script_machine_pan_camera_home(&machine));
    CHECK(machine.pan_is_active);
    script_machine_update_camera(&machine);
    CHECK_INT(machine.camera_x, HOME_PX_PER_TICK);
    while (machine.pan_is_active) {
        script_machine_update_camera(&machine);
    }
    CHECK_INT(machine.camera_x, HOME_X);
    CHECK_INT(machine.camera_y, HOME_Y);
    CHECK(the_last_message_is(0, "camera", "custom cancel"));
    run_view_op(0xd3, NO_HOME, NO_HOME, 0, 0);
    CHECK(!script_machine_pan_camera_home(&machine));
}

static void test_the_ending_picture_slides_to_its_place_without_holding_the_script(void)
{
    struct script_vm vm;
    unsigned int show[4] = { 190, 0, 0xff40u, 0 };
    unsigned int slide[4] = { 0, 0, FIXED_ONE, 0 };
    int tick;

    set_up_a_room();
    memset(&vm, 0, sizeof(vm));
    CHECK_INT(script_ops_view_run(&machine, &vm, 0xc5, show), SCRIPT_STEP_NEXT_THEN_YIELD);
    CHECK_INT(machine.ending_picture.id, 190);
    CHECK_INT(machine.ending_picture.y_fixed, -192 * (int)FIXED_ONE);
    CHECK_INT(script_ops_view_run(&machine, &vm, 0xd4, slide), SCRIPT_STEP_NEXT);
    for (tick = 0; tick < 191; tick++) {
        script_machine_update_camera(&machine);
    }
    CHECK(machine.ending_picture.y_fixed != 0);
    script_machine_update_camera(&machine);
    CHECK_INT(machine.ending_picture.y_fixed, 0);
    CHECK_INT(machine.ending_picture.ticks_left, 0);
}

static void test_the_screen_swap_is_kept(void)
{
    static const unsigned int swap[] = { 0x08 };
    static const unsigned int back[] = { 0x09 };

    run_a_scene_of(swap, 1);
    CHECK_INT(machine.main_picture_is_on_the_top_screen, 1);
    run_a_scene_of(back, 1);
    CHECK_INT(machine.main_picture_is_on_the_top_screen, 0);
}

static void test_the_effect_ops_tell_the_view(void)
{
    static const unsigned int place[] = { 0xb4, NAME_OFFSET, 0xf000, 0x000a, 0xb000, 0xfffe };
    static const unsigned int seek[] = { 0xb5, NAME_OFFSET, 28 };
    static const unsigned int scale[] = { 0xb6, NAME_OFFSET, 0x0800, 0, 0x1800, 0 };
    static const unsigned int ramp[] = { 0xb7, NAME_OFFSET, 30, 0, 0, 0x0800, 0 };

    run_a_scene_of(place, 6);
    CHECK(strcmp(name_told, "fountain") == 0);
    CHECK(x_told == 175.0f && y_told == -21.0f);
    run_a_scene_of(seek, 3);
    CHECK_INT(ticks_told, 28);
    run_a_scene_of(scale, 6);
    CHECK(x_told == 0.5f && y_told == 1.5f);
    run_a_scene_of(ramp, 7);
    CHECK_INT(ticks_told, 30);
    CHECK(x_told == 0.0f && y_told == 0.5f);
    CHECK(the_script_reached_the_wait());
}

static void test_the_alarm_filter_reddens_every_actor_and_lifts(void)
{
    static const unsigned int on[] = { 0x14, EVERY_NAME_OFFSET, ALARM_MODE };
    static const unsigned int off[] = { 0x14, EVERY_NAME_OFFSET, 0 };

    run_a_scene_of(on, 3);
    add_an_actor("pipe", 0, 0);
    add_an_actor("valve", 0, 0);
    machine.objects[0].is_sleeping = 1;
    machine.objects[1].is_sleeping = 1;
    machine.runner.pc = 0;
    machine.runner.wait_ticks = 0;
    script_machine_tick(&machine);
    CHECK_INT(machine.objects[0].actor.color_filter, SCRIPT_COLOR_FILTER_ALARM);
    CHECK_INT(machine.objects[1].actor.color_filter, SCRIPT_COLOR_FILTER_ALARM);
    run_a_scene_of(off, 3);
    CHECK(the_script_reached_the_wait());
}

static void test_every_held_trick_is_let_go(void)
{
    static const unsigned int words[] = { 0xeb, EVERY_NAME_OFFSET };

    run_a_scene_of(words, 2);
    add_an_actor("fan", 0, 0);
    add_an_actor("motor", 0, 0);
    machine.objects[0].is_sleeping = 1;
    machine.objects[1].is_sleeping = 1;
    machine.objects[0].actor.busy_cores = 1;
    machine.objects[1].actor.busy_cores = 3;
    machine.runner.pc = 0;
    machine.runner.wait_ticks = 0;
    script_machine_tick(&machine);
    CHECK_INT(machine.objects[0].actor.busy_cores, 0);
    CHECK_INT(machine.objects[1].actor.busy_cores, 0);
}

static struct script_balloon *open_a_tap_balloon_on(int owner)
{
    struct script_balloon *balloon;

    balloon = &machine.balloons[0];
    memset(balloon, 0, sizeof(*balloon));
    balloon->in_use = 1;
    balloon->owner_object = owner;
    balloon->tap_group = SCRIPT_TAP_GROUP_LEFT;
    balloon->kind = SCRIPT_BALLOON_KIND_TAP_DOTS;
    balloon->tap_target = "game";
    balloon->tap_verb = "EVENT";
    balloon->tap_arg = "keijitalk";
    return balloon;
}

static void run_media_op(unsigned int op, unsigned int word)
{
    struct script_vm vm;
    unsigned int words[2] = { word, 0 };

    memset(&vm, 0, sizeof(vm));
    vm.selected_object = SCRIPT_NO_SELECTION;
    CHECK_INT(script_opcode_family(op), SCRIPT_OP_MEDIA);
    script_ops_media_run(&machine, &vm, op, words);
}

static void set_up_a_listener_and_a_talker(int talker_x)
{
    set_up_a_room();
    add_an_actor("game", 100, 100);
    add_an_actor("detective", talker_x, 100);
    script_soul_set_first_core(&machine, 0, 0);
    open_a_tap_balloon_on(1);
}

static void test_the_listening_reach_is_kept_on_the_balloon(void)
{
    set_up_a_listener_and_a_talker(100 + REACH_PX + 60);
    run_media_op(0x48, REACH_PX);
    CHECK_INT(machine.balloons[0].reach_px, REACH_PX);
    run_media_op(0x49, 0);
    script_balloons_tap(&machine, SCRIPT_TAP_GROUP_LEFT);
    CHECK(the_last_message_is(0, "EVENT", "keijitalk"));
}

static void test_a_tap_pans_to_the_owner_first_unless_the_script_keeps_the_camera(void)
{
    set_up_a_listener_and_a_talker(600);
    script_balloons_tap(&machine, SCRIPT_TAP_GROUP_LEFT);
    CHECK(machine.pan_is_active);
    CHECK_INT(machine.objects[0].message_count, 0);
    CHECK_INT(machine.balloons[0].in_use, 1);
    while (machine.pan_is_active) {
        script_machine_update_camera(&machine);
    }
    CHECK_INT(machine.camera_x, 600 - LEFT_OF_THE_OWNER);
    script_balloons_count_down(&machine);
    CHECK(the_last_message_is(0, "EVENT", "keijitalk"));
    CHECK_INT(machine.balloons[0].in_use, 0);

    set_up_a_listener_and_a_talker(600);
    run_media_op(0x49, 0);
    script_balloons_tap(&machine, SCRIPT_TAP_GROUP_LEFT);
    CHECK(!machine.pan_is_active);
    CHECK(the_last_message_is(0, "EVENT", "keijitalk"));
}

static void test_a_checkpoint_is_kept_until_the_scene_changes(void)
{
    static const unsigned int words[] = { 0xfd, 0 };

    run_a_scene_of(words, 2);
    CHECK(strcmp(machine.checkpoint_scene, "a") == 0);
    CHECK(script_machine_checkpoint_is_offered(&machine));
    CHECK(!script_machine_go_back_to_the_checkpoint(&machine));
    script_flags_write(&machine.flags, SCRIPT_FLAG_CHECKPOINT_IS_NOT_OFFERED, 1);
    CHECK(!script_machine_checkpoint_is_offered(&machine));
    script_flags_write(&machine.flags, SCRIPT_FLAG_CHECKPOINT_IS_NOT_OFFERED, 0);
    CHECK(script_machine_start(&machine, "a"));
    CHECK(machine.checkpoint_scene[0] == '\0');
}

static void test_a_checkpoint_outlives_the_scene_when_its_flag_is_set(void)
{
    static const unsigned int words[] = { 0xfd, 0 };

    run_a_scene_of(words, 2);
    script_flags_write(&machine.flags, SCRIPT_FLAG_CHECKPOINT_OUTLIVES_THE_SCENE, 1);
    script_flags_write(&machine.flags, 0x1020, 1);
    memcpy(machine.checkpoint_game_flags, machine.flags.game, sizeof(machine.checkpoint_game_flags));
    CHECK(script_machine_add_source(&machine, "b", scene, SCRIPT_FILE_HEADER_SIZE + 10 + sizeof(strings) - 1, NULL));
    CHECK(script_machine_start(&machine, "b"));
    CHECK(strcmp(machine.checkpoint_scene, "a") == 0);
    script_flags_write(&machine.flags, 0x1021, 1);
    CHECK(script_machine_go_back_to_the_checkpoint(&machine));
    CHECK(script_flags_read(&machine.flags, 0x1020));
    CHECK(!script_flags_read(&machine.flags, 0x1021));
    CHECK(machine.pending_script != NULL && strcmp(machine.pending_script, "a") == 0);
}

static int movies_skipped;

static void movie_skipped(void)
{
    movies_skipped = movies_skipped + 1;
}

static void test_a_playing_movie_is_skipped_once_and_its_time_jump_comes_at_once(void)
{
    static const struct script_outputs movie_outputs = { .movie_skipped = movie_skipped };

    script_machine_init(&machine, &movie_outputs, SCRIPT_TIMING_DS, NULL);
    movies_skipped = 0;
    CHECK(!script_machine_skip_movie(&machine));
    CHECK_INT(movies_skipped, 0);
    machine.movie_ticks_left = 300;
    machine.soul.time_jump_event.target = "*";
    machine.soul.time_jump_event.verb = "timeleap";
    machine.soul.time_jump_event.arg = "jump";
    machine.soul.time_jump_event.at = (long long)machine.tick + 300;
    CHECK(script_machine_skip_movie(&machine));
    CHECK_INT(machine.movie_ticks_left, 0);
    CHECK_INT(movies_skipped, 1);
    CHECK(machine.soul.time_jump_event.at == (long long)machine.tick + 1);
    CHECK(!script_machine_skip_movie(&machine));
    CHECK_INT(movies_skipped, 1);
}

int main(void)
{
    test_no_op_of_the_twenty_logs_as_not_ported();
    test_the_saved_camera_comes_back_as_a_cut();
    test_a_camera_home_takes_the_view_close_from_the_saved_camera();
    test_the_ending_picture_slides_to_its_place_without_holding_the_script();
    test_the_screen_swap_is_kept();
    test_the_effect_ops_tell_the_view();
    test_the_alarm_filter_reddens_every_actor_and_lifts();
    test_every_held_trick_is_let_go();
    test_the_listening_reach_is_kept_on_the_balloon();
    test_a_tap_pans_to_the_owner_first_unless_the_script_keeps_the_camera();
    test_a_checkpoint_is_kept_until_the_scene_changes();
    test_a_checkpoint_outlives_the_scene_when_its_flag_is_set();
    test_a_playing_movie_is_skipped_once_and_its_time_jump_comes_at_once();
    return test_finish("unit/ops_ported_for_the_beta");
}
