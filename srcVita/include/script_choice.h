#ifndef SCRIPT_CHOICE_H
#define SCRIPT_CHOICE_H

#define SCRIPT_CHOICE_PANEL_MAX 4
#define SCRIPT_CHOICE_NONE (-1)
#define SCRIPT_CHOICE_CANCEL (-2)
#define SCRIPT_CHOICE_CANCEL_NAME "cancel"

enum script_choice_action {
    SCRIPT_CHOICE_EMPTY,
    SCRIPT_CHOICE_OPENS_TALK,
    SCRIPT_CHOICE_SENDS_EVENT
};

struct script_choice_panel {
    enum script_choice_action action;
    const char *caption;
    unsigned int flag;
    const char *talk_label;
    const char *target;
    const char *kind;
    const char *event;
};

struct script_choice {
    struct script_choice_panel panels[SCRIPT_CHOICE_PANEL_MAX];
    struct script_choice_panel turned_panels[SCRIPT_CHOICE_PANEL_MAX];
    int panel_count;
    int panel_limit;
    int control_is_up;
    int delete_is_due;
    int is_open;
    int cancel_is_offered;
    int chosen;
    int cursor;
    int cancel_is_pending;
    int picks;
    unsigned int opened_at;
};

struct script_machine;

void script_choice_clear_all(struct script_machine *machine);
int script_choice_holds_the_script(const struct script_machine *machine);
int script_choice_panels_shown(const struct script_machine *machine);
int script_choice_find(const struct script_machine *machine, const char *name);
int script_choice_first_unseen(const struct script_machine *machine);
void script_choice_pick(struct script_machine *machine, int index);
void script_choice_move_cursor(struct script_machine *machine, int steps);
void script_choice_note_cancel(struct script_machine *machine);
void script_choice_take_cancel(struct script_machine *machine);
void script_choice_delete_picked_control(struct script_machine *machine);

#endif
