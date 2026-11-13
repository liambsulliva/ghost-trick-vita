#include <stdio.h>
#include <string.h>
#include "asset_name.h"
#include "read_ahead.h"
#include "text_lines.h"
#include "texture_cache.h"

#define READ_AHEAD_NAME_MAX 48
#define READ_AHEAD_LINE_MAX 192
#define READ_AHEAD_ANCHOR_PREFIX "game"
#define READ_AHEAD_LINE_SCAN "ahead %47s %u " ASSET_NAME_SCAN

struct read_ahead_entry {
    char anchor[READ_AHEAD_NAME_MAX];
    unsigned int offset_ticks;
    char texture_name[ASSET_NAME_MAX];
};

struct read_ahead_plan {
    char script_name[READ_AHEAD_NAME_MAX];
    const char *text;
    unsigned int text_size;
};

static struct read_ahead_entry entries[READ_AHEAD_ENTRY_MAX];
static int entry_count;
static struct read_ahead_plan plans[READ_AHEAD_PLAN_MAX];
static int plan_count;
static char current_script[READ_AHEAD_NAME_MAX];
static char current_anchor[READ_AHEAD_NAME_MAX];
static unsigned int anchor_tick;

void read_ahead_clear(void)
{
    entry_count = 0;
    plan_count = 0;
    current_script[0] = '\0';
    current_anchor[0] = '\0';
    anchor_tick = 0;
}

static int read_entries_of(const struct read_ahead_plan *plan, int is_kept)
{
    char line[READ_AHEAD_LINE_MAX];
    struct read_ahead_entry entry;
    unsigned int offset;
    int count;

    count = 0;
    offset = 0;
    while (offset < plan->text_size) {
        offset = text_lines_read_next(plan->text, plan->text_size, offset, line, sizeof(line));
        if (sscanf(line, READ_AHEAD_LINE_SCAN, entry.anchor, &entry.offset_ticks, entry.texture_name) != 3) {
            continue;
        }
        count = count + 1;
        if (is_kept && entry_count < READ_AHEAD_ENTRY_MAX) {
            entries[entry_count] = entry;
            entry_count = entry_count + 1;
        }
    }
    return count;
}

int read_ahead_add_text(const char *script_name, const char *text, unsigned int text_size)
{
    struct read_ahead_plan *plan;

    if (plan_count >= READ_AHEAD_PLAN_MAX) {
        return 0;
    }
    plan = &plans[plan_count];
    snprintf(plan->script_name, sizeof(plan->script_name), "%s", script_name);
    plan->text = text;
    plan->text_size = text_size;
    plan_count = plan_count + 1;
    return read_entries_of(plan, 0);
}

static void read_the_plan_of(const char *script_name)
{
    int index;

    entry_count = 0;
    for (index = 0; index < plan_count; index++) {
        if (strcmp(plans[index].script_name, script_name) == 0) {
            read_entries_of(&plans[index], 1);
            return;
        }
    }
}

int read_ahead_entry_count(void)
{
    return entry_count;
}

void read_ahead_note_mark(const char *what, const char *name, unsigned int tick)
{
    if (strcmp(what, "script_begun") == 0) {
        snprintf(current_script, sizeof(current_script), "%s", name);
        read_the_plan_of(name);
        snprintf(current_anchor, sizeof(current_anchor), "*");
        anchor_tick = tick;
        return;
    }
    if (strcmp(what, "act_begun") == 0 && strncmp(name, READ_AHEAD_ANCHOR_PREFIX, strlen(READ_AHEAD_ANCHOR_PREFIX)) == 0) {
        snprintf(current_anchor, sizeof(current_anchor), "%s", name);
        anchor_tick = tick;
    }
}

static unsigned int lookahead_ticks = READ_AHEAD_LOOKAHEAD_TICKS;

void read_ahead_set_lookahead_scale(int scale)
{
    lookahead_ticks = READ_AHEAD_LOOKAHEAD_TICKS * (unsigned int)(scale > 1 ? scale : 1);
}

int read_ahead_step(unsigned int tick)
{
    unsigned int elapsed;
    int index;
    int requested;

    if (current_script[0] == '\0' || tick < anchor_tick) {
        return 0;
    }
    elapsed = tick - anchor_tick;
    requested = 0;
    for (index = 0; index < entry_count; index++) {
        const struct read_ahead_entry *entry;

        entry = &entries[index];
        if (entry->offset_ticks > elapsed + lookahead_ticks || entry->offset_ticks + READ_AHEAD_KEEP_BEHIND_TICKS < elapsed) {
            continue;
        }
        if (strcmp(entry->anchor, current_anchor) != 0) {
            continue;
        }
        texture_cache_keep_or_start_reading_ahead(entry->texture_name);
        requested = requested + 1;
    }
    return requested;
}
