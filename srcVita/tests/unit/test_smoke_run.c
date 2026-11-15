#include <string.h>
#include "command_file.h"
#include "run_log.h"
#include "smoke_run.h"
#include "test_support.h"

#define LOG_KEPT 4096

static char log_kept[LOG_KEPT];
static int title_is_open;
static int cursor_row;

static void keep_line(const char *line)
{
    if (strlen(log_kept) + strlen(line) + 2 < sizeof(log_kept)) {
        strcat(log_kept, line);
        strcat(log_kept, "\n");
    }
    smoke_run_note_log_line(line);
}

static int probe(const char *word, const char *argument)
{
    if (strcmp(word, "title") == 0) {
        return title_is_open;
    }
    if (strcmp(word, "cursor") == 0) {
        return cursor_row == argument[0] - '0';
    }
    return SMOKE_PROBE_UNKNOWN;
}

static void begin(const char *text)
{
    log_kept[0] = '\0';
    title_is_open = 0;
    cursor_row = 0;
    run_log_watch(keep_line);
    smoke_run_use_probe(probe);
    smoke_run_begin("flow_under_test", text, (unsigned int)strlen(text));
}

static int frames_until_a_command(char *line, unsigned int line_size, int frames_most)
{
    int frames;

    for (frames = 0; frames < frames_most; frames++) {
        smoke_run_note_frame();
        if (smoke_run_next_command(line, line_size)) {
            return frames;
        }
        if (!smoke_run_is_active()) {
            return -1;
        }
    }
    return frames_most;
}

static void test_commands_pass_through_in_order_and_the_flow_ends_ok(void)
{
    char line[SMOKE_RUN_LINE_MAX];
    int over_before;
    int failed_before;

    over_before = smoke_run_flows_over();
    failed_before = smoke_run_flows_failed();
    begin("press 10\n\ncapture shot\n");
    CHECK(smoke_run_is_active());
    CHECK_TEXT(smoke_run_name(), "flow_under_test");
    CHECK_INT(frames_until_a_command(line, sizeof(line), 5), 0);
    CHECK_TEXT(line, "press 10");
    CHECK_INT(frames_until_a_command(line, sizeof(line), 5), 0);
    CHECK_TEXT(line, "capture shot");
    CHECK_INT(frames_until_a_command(line, sizeof(line), 5), -1);
    CHECK(!smoke_run_is_active());
    CHECK_INT(smoke_run_flows_over(), over_before + 1);
    CHECK_INT(smoke_run_flows_failed(), failed_before);
    CHECK_HAS_TEXT(log_kept, "smoke begin name=flow_under_test");
    CHECK_HAS_TEXT(log_kept, "smoke ok name=flow_under_test frames=3");
}

static void test_wait_holds_the_next_command_for_its_frames(void)
{
    char line[SMOKE_RUN_LINE_MAX];

    begin("wait 4\npress 20\n");
    CHECK_INT(frames_until_a_command(line, sizeof(line), 20), 4);
    CHECK_TEXT(line, "press 20");
}

static void test_until_waits_for_its_condition(void)
{
    char line[SMOKE_RUN_LINE_MAX];
    int frame;

    begin("until 50 title\npress 10\n");
    for (frame = 0; frame < 6; frame++) {
        smoke_run_note_frame();
        CHECK(!smoke_run_next_command(line, sizeof(line)));
    }
    title_is_open = 1;
    smoke_run_note_frame();
    CHECK(smoke_run_next_command(line, sizeof(line)));
    CHECK_TEXT(line, "press 10");
}

static void test_until_fails_once_its_frames_run_out(void)
{
    char line[SMOKE_RUN_LINE_MAX];
    int failed_before;

    failed_before = smoke_run_flows_failed();
    begin("press 1\nuntil 8 title\npress 10\n");
    CHECK_INT(frames_until_a_command(line, sizeof(line), 5), 0);
    CHECK_INT(frames_until_a_command(line, sizeof(line), 40), -1);
    CHECK_INT(smoke_run_flows_failed(), failed_before + 1);
    CHECK_HAS_TEXT(log_kept, "smoke fail name=flow_under_test line=2 reason=timeout");
    CHECK_HAS_TEXT(log_kept, "text=until 8 title");
    CHECK(strstr(log_kept, "smoke ok") == NULL);
}

static void test_expect_fails_at_once_and_not_inverts(void)
{
    char line[SMOKE_RUN_LINE_MAX];

    begin("expect not title\nexpect cursor 0\nexpect title\npress 10\n");
    CHECK_INT(frames_until_a_command(line, sizeof(line), 5), -1);
    CHECK_HAS_TEXT(log_kept, "line=3 reason=expect");
    CHECK_HAS_TEXT(log_kept, "text=expect title");
}

static void test_a_word_the_app_does_not_know_fails_the_flow(void)
{
    char line[SMOKE_RUN_LINE_MAX];

    begin("expect wobble 3\n");
    CHECK_INT(frames_until_a_command(line, sizeof(line), 5), -1);
    CHECK_HAS_TEXT(log_kept, "reason=unknown_condition");
}

static void test_log_conditions_take_one_line_each(void)
{
    char line[SMOKE_RUN_LINE_MAX];

    begin("until 5 log save stored\nuntil 5 log save stored\npress 10\n");
    run_log_write("save stored slot=1");
    CHECK_INT(frames_until_a_command(line, sizeof(line), 30), -1);
    CHECK_HAS_TEXT(log_kept, "line=2 reason=timeout");
    begin("until 5 log save stored\nuntil 5 log save stored\npress 10\n");
    run_log_write("save stored slot=1");
    run_log_write("save stored slot=2");
    CHECK_INT(frames_until_a_command(line, sizeof(line), 30), 0);
    CHECK_TEXT(line, "press 10");
}

static void test_log_lines_before_the_flow_do_not_count(void)
{
    char line[SMOKE_RUN_LINE_MAX];

    run_log_watch(keep_line);
    run_log_write("title open");
    begin("expect log title open\n");
    CHECK_INT(frames_until_a_command(line, sizeof(line), 5), -1);
    CHECK_HAS_TEXT(log_kept, "reason=expect");
}

static void test_a_stall_fails_the_flow_long_before_its_until_runs_out(void)
{
    char line[SMOKE_RUN_LINE_MAX];
    int frame;

    begin("stall 10 route_step_done\nuntil 5000 title\npress 10\n");
    for (frame = 0; frame < 30; frame++) {
        smoke_run_note_frame();
        CHECK(!smoke_run_next_command(line, sizeof(line)));
        if (frame % 8 == 0) {
            run_log_write("route_step_done name=%d:move:x", frame);
        }
    }
    CHECK(smoke_run_is_active());
    CHECK_INT(frames_until_a_command(line, sizeof(line), 40), -1);
    CHECK_HAS_TEXT(log_kept, "reason=stalled");
    CHECK_HAS_TEXT(log_kept, "text=route_step_done");
    begin("stall 10 route_step_done\nstall 0\nuntil 50 title\npress 10\n");
    CHECK_INT(frames_until_a_command(line, sizeof(line), 200), -1);
    CHECK_HAS_TEXT(log_kept, "reason=timeout");
}

static void test_the_smoke_command_carries_the_flow_name(void)
{
    const char text[] = "smoke pause_and_resume\npress 10\nsmoke ../x\n";
    struct command command;
    unsigned int offset;

    offset = command_parse_next_line(text, sizeof(text) - 1, 0, &command);
    CHECK_INT(command.kind, COMMAND_SMOKE);
    CHECK_TEXT(command.name, "pause_and_resume");
    CHECK_TEXT(&text[offset], "press 10\nsmoke ../x\n");
    offset = command_parse_next_line(text, sizeof(text) - 1, offset, &command);
    offset = command_parse_next_line(text, sizeof(text) - 1, offset, &command);
    CHECK_INT(command.kind, COMMAND_UNKNOWN);
}

int main(void)
{
    test_commands_pass_through_in_order_and_the_flow_ends_ok();
    test_wait_holds_the_next_command_for_its_frames();
    test_until_waits_for_its_condition();
    test_until_fails_once_its_frames_run_out();
    test_expect_fails_at_once_and_not_inverts();
    test_a_word_the_app_does_not_know_fails_the_flow();
    test_log_conditions_take_one_line_each();
    test_log_lines_before_the_flow_do_not_count();
    test_a_stall_fails_the_flow_long_before_its_until_runs_out();
    test_the_smoke_command_carries_the_flow_name();
    return test_finish("unit/smoke_run");
}
