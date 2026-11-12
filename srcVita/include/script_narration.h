#ifndef SCRIPT_NARRATION_H
#define SCRIPT_NARRATION_H

#define SCRIPT_CUE_LABEL_MAX 32
#define SCRIPT_CUE_MESSAGE_MAX 32
#define SCRIPT_CUE_PAGE_MAX 64
#define SCRIPT_CUE_GROUP_MAX 8
#define SCRIPT_NARRATION_DIM_TOP_LEVEL 16
#define SCRIPT_NARRATION_DIM_TICKS 10
#define SCRIPT_NARRATION_GROUP_IN_TICKS 62
#define SCRIPT_NARRATION_TEXT_OUT_TICKS 61

enum script_timing {
    SCRIPT_TIMING_DS,
    SCRIPT_TIMING_REMASTER
};

enum script_narration_event {
    SCRIPT_NARRATION_DIM_IN,
    SCRIPT_NARRATION_DIM_FULL,
    SCRIPT_NARRATION_GROUP_IN,
    SCRIPT_NARRATION_TEXT_OUT,
    SCRIPT_NARRATION_CUT,
    SCRIPT_NARRATION_DIM_OUT,
    SCRIPT_NARRATION_TEXT_FULL
};

struct script_cue_page {
    int limit;
    int options;
    int group_count;
    int group_hold[SCRIPT_CUE_GROUP_MAX];
};

struct script_cue_message {
    char label[SCRIPT_CUE_LABEL_MAX];
    int first_page;
    int page_count;
};

struct script_cue_list {
    int message_count;
    int page_count;
    struct script_cue_message messages[SCRIPT_CUE_MESSAGE_MAX];
    struct script_cue_page pages[SCRIPT_CUE_PAGE_MAX];
};

struct script_narration {
    const struct script_cue_list *cues;
    const struct script_cue_message *message;
    enum script_timing timing;
    int is_running;
    int page;
    int group_count;
    int group_hold[SCRIPT_CUE_GROUP_MAX];
    int wait[SCRIPT_CUE_GROUP_MAX];
    int limit;
    int options;
    int state;
    int group;
    int timer;
    int level;
    int speed;
    int counter;
    int count_to_limit;
    int group_was_reported;
    int press_is_pending;
    int fade_left;
};

typedef void (*script_narration_report)(void *listener, const char *label, enum script_narration_event event, int group);

int script_cues_parse(const char *text, unsigned int text_size, struct script_cue_list *cues);
const struct script_cue_message *script_cues_find(const struct script_cue_list *cues, const char *label);
void script_narration_start(struct script_narration *narration, const struct script_cue_list *cues, const struct script_cue_message *message, enum script_timing timing);
int script_narration_wants_a_press(const struct script_narration *narration);
int script_narration_takes_a_skip(const struct script_narration *narration);
void script_narration_note_press(struct script_narration *narration);
void script_narration_finish_soon(struct script_narration *narration);
int script_narration_step(struct script_narration *narration, script_narration_report report, void *listener);

#endif
