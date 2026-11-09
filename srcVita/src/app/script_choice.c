#include <string.h>
#include "script_choice.h"
#include "script_machine.h"

#define DEFAULT_PANEL_LIMIT 4
#define INDEX_BYTE_MASK 0xffu

static void mark(struct script_machine *machine, const char *what, const char *detail)
{
    if (machine->outputs != NULL && machine->outputs->marked != NULL) {
        machine->outputs->marked(what, detail != NULL ? detail : "-");
    }
}

static const struct script_choice_panel *panel_at(const struct script_choice *choice, int index)
{
    if (index < 0 || index >= SCRIPT_CHOICE_PANEL_MAX) {
        return NULL;
    }
    if (choice->panels[index].action != SCRIPT_CHOICE_EMPTY) {
        return &choice->panels[index];
    }
    if (choice->turned_panels[index].action != SCRIPT_CHOICE_EMPTY) {
        return &choice->turned_panels[index];
    }
    return NULL;
}

static const char *name_of_panel(const struct script_choice_panel *panel)
{
    if (panel->action == SCRIPT_CHOICE_SENDS_EVENT && panel->event != NULL) {
        return panel->event;
    }
    if (panel->talk_label != NULL) {
        return panel->talk_label;
    }
    return panel->caption;
}

static void clear_panels(struct script_choice *choice)
{
    memset(choice->panels, 0, sizeof(choice->panels));
    choice->panel_count = 0;
    choice->panel_limit = DEFAULT_PANEL_LIMIT;
}

static void clear_turned_panels(struct script_choice *choice)
{
    memset(choice->turned_panels, 0, sizeof(choice->turned_panels));
}

static void add_panel(struct script_choice *choice, const struct script_choice_panel *panel)
{
    if (choice->panel_count < choice->panel_limit && choice->panel_count < SCRIPT_CHOICE_PANEL_MAX) {
        choice->panels[choice->panel_count] = *panel;
        choice->panel_count = choice->panel_count + 1;
    }
}

static void put_panel_at(struct script_choice *choice, int index, const struct script_choice_panel *panel)
{
    if (index >= choice->panel_limit || index >= SCRIPT_CHOICE_PANEL_MAX) {
        return;
    }
    if (choice->turned_panels[index].action == SCRIPT_CHOICE_EMPTY) {
        choice->turned_panels[index] = *panel;
        return;
    }
    if (choice->panels[index].action != SCRIPT_CHOICE_EMPTY) {
        choice->panels[index] = *panel;
        return;
    }
    add_panel(choice, panel);
}

static struct script_choice_panel talk_panel(struct script_machine *machine, const struct script_vm *vm, unsigned int caption, unsigned int talk_label, unsigned int flag)
{
    struct script_choice_panel panel;

    memset(&panel, 0, sizeof(panel));
    panel.action = SCRIPT_CHOICE_OPENS_TALK;
    panel.caption = script_machine_string(machine, vm, caption);
    panel.talk_label = script_machine_string(machine, vm, talk_label);
    panel.flag = flag;
    return panel;
}

static struct script_choice_panel event_panel(struct script_machine *machine, const struct script_vm *vm, unsigned int caption, unsigned int flag, const unsigned int *event_words)
{
    struct script_choice_panel panel;

    memset(&panel, 0, sizeof(panel));
    panel.action = SCRIPT_CHOICE_SENDS_EVENT;
    panel.caption = script_machine_string(machine, vm, caption);
    panel.flag = flag;
    panel.target = script_machine_string(machine, vm, event_words[0]);
    panel.kind = script_machine_string(machine, vm, event_words[1]);
    panel.event = script_machine_string(machine, vm, event_words[2]);
    return panel;
}

static void open_choice(struct script_machine *machine)
{
    struct script_choice *choice;
    char names[SCRIPT_CHOICE_PANEL_MAX * 32];
    int index;

    choice = &machine->choice;
    if (!choice->control_is_up) {
        return;
    }
    choice->is_open = 1;
    choice->opened_at = machine->tick;
    choice->cursor = 0;
    choice->cancel_is_pending = 0;
    names[0] = '\0';
    for (index = 0; index < script_choice_panels_shown(machine); index++) {
        const struct script_choice_panel *panel;

        panel = panel_at(choice, index);
        if (panel != NULL && name_of_panel(panel) != NULL) {
            strncat(names, index > 0 ? "," : "", sizeof(names) - strlen(names) - 1);
            strncat(names, name_of_panel(panel), sizeof(names) - strlen(names) - 1);
        }
    }
    mark(machine, "choice_opened", names);
}

enum script_step_result script_choice_run_op(struct script_machine *machine, struct script_vm *vm, unsigned int op, const unsigned int *words)
{
    struct script_choice *choice;
    struct script_choice_panel panel;

    choice = &machine->choice;
    switch (op) {
    case 0x3a:
        if (!choice->control_is_up) {
            choice->control_is_up = 1;
            choice->cancel_is_offered = 0;
        }
        break;
    case 0x3b:
        open_choice(machine);
        break;
    case 0x3c:
        panel = talk_panel(machine, vm, script_machine_u32(words, 0), script_machine_u32(words, 2), script_machine_u32(words, 4));
        add_panel(choice, &panel);
        break;
    case 0x3d:
        panel = talk_panel(machine, vm, script_machine_u32(words, 1), script_machine_u32(words, 3), script_machine_u32(words, 5));
        put_panel_at(choice, (int)(words[0] & INDEX_BYTE_MASK), &panel);
        break;
    case 0x3e:
        clear_panels(choice);
        break;
    case 0x3f:
        clear_panels(choice);
        clear_turned_panels(choice);
        break;
    case 0x40:
        panel = event_panel(machine, vm, script_machine_u32(words, 0), script_machine_u32(words, 2), &words[4]);
        add_panel(choice, &panel);
        break;
    case 0x41:
        panel = event_panel(machine, vm, script_machine_u32(words, 1), script_machine_u32(words, 3), &words[5]);
        put_panel_at(choice, (int)(words[0] & INDEX_BYTE_MASK), &panel);
        break;
    case 0x42:
        if (choice->control_is_up) {
            choice->cancel_is_offered = 1;
        }
        break;
    default:
        break;
    }
    return SCRIPT_STEP_NEXT;
}

void script_choice_clear_all(struct script_machine *machine)
{
    struct script_choice *choice;

    choice = &machine->choice;
    clear_panels(choice);
    clear_turned_panels(choice);
    choice->control_is_up = 0;
    choice->is_open = 0;
    choice->cancel_is_offered = 0;
    choice->chosen = SCRIPT_CHOICE_NONE;
}

int script_choice_holds_the_script(const struct script_machine *machine)
{
    return machine->choice.control_is_up;
}

int script_choice_panels_shown(const struct script_machine *machine)
{
    int index;
    int shown;

    shown = 0;
    for (index = 0; index < SCRIPT_CHOICE_PANEL_MAX; index++) {
        if (panel_at(&machine->choice, index) != NULL) {
            shown = index + 1;
        }
    }
    return shown;
}

int script_choice_find(const struct script_machine *machine, const char *name)
{
    int index;

    if (strcmp(name, SCRIPT_CHOICE_CANCEL_NAME) == 0) {
        return machine->choice.cancel_is_offered ? SCRIPT_CHOICE_CANCEL : SCRIPT_CHOICE_NONE;
    }

    for (index = 0; index < script_choice_panels_shown(machine); index++) {
        const struct script_choice_panel *panel;

        panel = panel_at(&machine->choice, index);
        if (panel == NULL) {
            continue;
        }
        if ((panel->event != NULL && strcmp(panel->event, name) == 0) || (panel->talk_label != NULL && strcmp(panel->talk_label, name) == 0) || (panel->caption != NULL && strcmp(panel->caption, name) == 0)) {
            return index;
        }
    }
    return SCRIPT_CHOICE_NONE;
}

int script_choice_first_unseen(const struct script_machine *machine)
{
    int index;
    int first;

    first = SCRIPT_CHOICE_NONE;
    for (index = 0; index < script_choice_panels_shown(machine); index++) {
        const struct script_choice_panel *panel;
        int was_chosen;

        panel = panel_at(&machine->choice, index);
        if (panel == NULL) {
            continue;
        }
        if (first == SCRIPT_CHOICE_NONE) {
            first = index;
        }
        if (!script_flags_read_known(&machine->flags, panel->flag, &was_chosen) || !was_chosen) {
            return index;
        }
    }
    return first;
}

void script_choice_move_cursor(struct script_machine *machine, int steps)
{
    struct script_choice *choice;
    int count;
    int tries;

    choice = &machine->choice;
    count = script_choice_panels_shown(machine);
    if (!choice->is_open || count <= 1 || steps == 0) {
        return;
    }
    for (tries = 0; tries < count; tries++) {
        choice->cursor = ((choice->cursor + (steps > 0 ? 1 : -1)) % count + count) % count;
        if (panel_at(choice, choice->cursor) != NULL) {
            break;
        }
    }
    mark(machine, "choice_cursor", name_of_panel(panel_at(choice, choice->cursor)));
}

void script_choice_note_cancel(struct script_machine *machine)
{
    if (machine->choice.is_open) {
        machine->choice.cancel_is_pending = 1;
    }
}

void script_choice_take_cancel(struct script_machine *machine)
{
    if (machine->choice.cancel_is_pending) {
        machine->choice.cancel_is_pending = 0;
        script_choice_pick(machine, SCRIPT_CHOICE_CANCEL);
    }
}

void script_choice_pick(struct script_machine *machine, int index)
{
    struct script_choice *choice;
    struct script_choice_panel panel;
    const struct script_choice_panel *shown;

    choice = &machine->choice;
    if (choice->is_open && index == SCRIPT_CHOICE_CANCEL && choice->cancel_is_offered) {
        choice->is_open = 0;
        choice->control_is_up = 0;
        mark(machine, "choice_picked", SCRIPT_CHOICE_CANCEL_NAME);
        script_machine_send(machine, "*", "select", SCRIPT_CHOICE_CANCEL_NAME);
        return;
    }
    shown = panel_at(choice, index);
    if (!choice->is_open || shown == NULL) {
        return;
    }
    panel = *shown;
    choice->chosen = index;
    choice->is_open = 0;
    choice->control_is_up = 0;
    script_flags_write_known(&machine->flags, panel.flag, 1);
    mark(machine, "choice_picked", name_of_panel(&panel));
    if (panel.action == SCRIPT_CHOICE_OPENS_TALK) {
        if (panel.talk_label != NULL) {
            script_talk_open(machine, panel.talk_label);
        }
    } else if (panel.target != NULL && panel.kind != NULL && panel.event != NULL) {
        script_machine_send(machine, panel.target, panel.kind, panel.event);
    }
}
