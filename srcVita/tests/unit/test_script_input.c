#include <string.h>
#include "scene_stage.h"
#include "scene_view.h"
#include "script_input.h"
#include "script_machine.h"
#include "test_support.h"

#define NORMAL_PLAY_MODE 3
#define OPEN_CHOICE_OP 0x3b
#define ROW_Y 100
#define BODY_X 100
#define GATE_X 140
#define TICKS_FOR_A_DRAG 60

static struct script_machine machine;
static const struct script_outputs no_outputs;
static struct script_input pad;
static const char *const events[2] = { "first", "second" };

static void start(void)
{
    script_machine_init(&machine, &no_outputs, SCRIPT_TIMING_REMASTER, NULL);
    script_input_reset(&pad);
    machine.mode_byte = NORMAL_PLAY_MODE;
    machine.pause_bit = 0;
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

static void send(unsigned int pressed, int touch_is_down, float touch_x, float touch_y)
{
    struct input_state frame;

    memset(&frame, 0, sizeof(frame));
    frame.pressed = pressed;
    frame.touch_is_down = touch_is_down;
    frame.touch_x = touch_x;
    frame.touch_y = touch_y;
    script_input_frame(&pad, &machine, &frame);
    script_soul_step_before_tasks(&machine);
}

static void open_a_choice_of_two(void)
{
    static const unsigned int no_words[8];
    int index;

    start();
    machine.object_count = 1;
    machine.objects[0].name = "linne";
    machine.objects[0].actor.exists = 1;
    for (index = 0; index < 2; index++) {
        machine.choice.panels[index].action = SCRIPT_CHOICE_SENDS_EVENT;
        machine.choice.panels[index].target = "linne";
        machine.choice.panels[index].kind = "select";
        machine.choice.panels[index].event = events[index];
    }
    machine.choice.panel_count = 2;
    machine.choice.panel_limit = 2;
    machine.choice.control_is_up = 1;
    machine.choice.cancel_is_offered = 1;
    script_choice_run_op(&machine, NULL, OPEN_CHOICE_OP, no_words);
}

static int linne_got(const char *event)
{
    int index;

    for (index = 0; index < machine.objects[0].message_count; index++) {
        if (strcmp(machine.objects[0].messages[index].arg, event) == 0) {
            return 1;
        }
    }
    return 0;
}

static void test_the_d_pad_and_cross_choose_a_panel(void)
{
    open_a_choice_of_two();
    send(INPUT_DOWN, 0, 0.0f, 0.0f);
    send(INPUT_CROSS, 0, 0.0f, 0.0f);
    CHECK(!machine.choice.is_open);
    CHECK(linne_got("second"));
}

static void test_circle_cancels_an_open_choice(void)
{
    open_a_choice_of_two();
    send(INPUT_CIRCLE, 0, 0.0f, 0.0f);
    CHECK(!machine.choice.is_open);
    CHECK(linne_got("cancel"));
}

static void test_a_touch_drag_on_the_screen_moves_the_soul(void)
{
    float screen_x;
    float screen_y;
    float gate_screen_x;
    int tick;

    start();
    machine.object_count = 2;
    place_core(0, "body", BODY_X);
    place_core(1, "gate", GATE_X);
    script_soul_set_first_core(&machine, 0, 0);
    script_soul_step_before_tasks(&machine);
    machine.soul.is_on = 1;
    script_soul_step_before_tasks(&machine);
    scene_view_world_to_screen(scene_stage_view(), (float)BODY_X, (float)ROW_Y, &screen_x, &screen_y);
    scene_view_world_to_screen(scene_stage_view(), (float)GATE_X, (float)ROW_Y, &gate_screen_x, &screen_y);
    send(0, 1, screen_x, screen_y);
    for (tick = 0; tick < TICKS_FOR_A_DRAG; tick++) {
        send(0, 1, gate_screen_x + 10.0f, screen_y);
    }
    send(0, 0, 0.0f, 0.0f);
    CHECK_INT(machine.soul.active_object, 1);
}

static void test_the_d_pad_scrolls_the_camera_tutorial_while_its_talk_is_open(void)
{
    struct input_state frame;
    int camera_before;
    int tick;

    start();
    machine.object_count = 1;
    place_core(0, "body", BODY_X);
    script_soul_set_first_core(&machine, 0, 0);
    script_soul_run_command(&machine, "FreeCamera", "TutorialStart");
    machine.pause_bit = 1;
    machine.talk_is_open = 1;
    camera_before = machine.camera_x;
    memset(&frame, 0, sizeof(frame));
    frame.held = INPUT_RIGHT;
    for (tick = 0; tick < 10; tick++) {
        script_input_frame(&pad, &machine, &frame);
        script_soul_step_before_tasks(&machine);
    }
    CHECK(machine.camera_x > camera_before);
}

int main(void)
{
    test_the_d_pad_and_cross_choose_a_panel();
    test_circle_cancels_an_open_choice();
    test_a_touch_drag_on_the_screen_moves_the_soul();
    test_the_d_pad_scrolls_the_camera_tutorial_while_its_talk_is_open();
    return test_finish("unit/script_input");
}
