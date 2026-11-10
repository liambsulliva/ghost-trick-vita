#include <string.h>
#include "script_machine.h"
#include "test_support.h"

static struct script_machine machine;
static int styled_count;
static int active_count;
static int marked_count;

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

static const struct script_outputs outputs = {
    .soul_object_styled = on_styled,
    .soul_core_marked = on_core_marked,
};

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

int main(void)
{
    test_the_world_is_not_styled_outside_the_view();
    test_a_scripted_soul_view_styles_every_object_and_marks_no_core();
    test_the_player_s_view_styles_every_object_too();
    return test_finish("unit/demo_soul_view_styles");
}
