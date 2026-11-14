#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "record_reveal.h"
#include "run_log.h"
#include "script_machine.h"
#include "script_narration.h"

enum reveal_kind {
    REVEAL_ON_NOTE,
    REVEAL_ON_TALK
};

struct reveal_rule {
    int stage;
    enum reveal_kind kind;
    int list;
    int id;
    int level;
    char talk[SCRIPT_CUE_LABEL_MAX];
};

static struct reveal_rule rules[RECORD_REVEAL_RULES_MOST];
static int rule_count;
static int stage;

void record_reveal_forget_rules(void)
{
    rule_count = 0;
}

int record_reveal_read_rule(const char *text)
{
    struct reveal_rule rule;
    char kind[16];
    int used;

    memset(&rule, 0, sizeof(rule));
    if (rule_count >= RECORD_REVEAL_RULES_MOST || sscanf(text, "%d %15s %n", &rule.stage, kind, &used) < 2 || rule.stage <= 0) {
        return 0;
    }
    if (strcmp(kind, "note") == 0 && sscanf(text + used, "%d %d %d", &rule.list, &rule.id, &rule.level) == 3) {
        rule.kind = REVEAL_ON_NOTE;
    } else if (strcmp(kind, "talk") == 0 && sscanf(text + used, "%31s", rule.talk) == 1) {
        rule.kind = REVEAL_ON_TALK;
    } else {
        return 0;
    }
    if (rule.kind == REVEAL_ON_NOTE && (rule.list < 0 || rule.list >= SCRIPT_NOTE_LIST_COUNT || rule.id <= 0 || rule.id >= SCRIPT_NOTE_ID_COUNT)) {
        return 0;
    }
    rules[rule_count] = rule;
    rule_count = rule_count + 1;
    return 1;
}

int record_reveal_load(const char *path)
{
    unsigned char *bytes;
    unsigned int size;
    char text[32];

    stage = 0;
    if (file_read_whole(path, &bytes, &size) != FILE_OK) {
        return 0;
    }
    if (size >= sizeof(text)) {
        size = sizeof(text) - 1u;
    }
    memcpy(text, bytes, size);
    text[size] = '\0';
    mem_free(bytes);
    if (sscanf(text, "stage %d", &stage) != 1 || stage < 0) {
        stage = 0;
    }
    return stage;
}

int record_reveal_stage(void)
{
    return stage;
}

static int rule_is_met(const struct reveal_rule *rule, const char *talk_label, const unsigned char *note_levels)
{
    if (rule->kind == REVEAL_ON_TALK) {
        return talk_label != NULL && strcmp(talk_label, rule->talk) == 0;
    }
    return note_levels != NULL && note_levels[rule->list * SCRIPT_NOTE_ID_COUNT + rule->id] > rule->level;
}

int record_reveal_watch(const char *talk_label, const unsigned char *note_levels, const char *path)
{
    char text[32];
    int index;
    int reached;
    int length;

    reached = stage;
    for (index = 0; index < rule_count; index++) {
        if (rules[index].stage > reached && rule_is_met(&rules[index], talk_label, note_levels)) {
            reached = rules[index].stage;
        }
    }
    if (reached == stage) {
        return 0;
    }
    stage = reached;
    length = snprintf(text, sizeof(text), "stage %d\n", stage);
    (void)file_make_folder(FILE_DATA_FOLDER);
    run_log_write("record reveal stage=%d stored=%d", stage, file_write_whole(path, (const unsigned char *)text, (unsigned int)length) == FILE_OK);
    return 1;
}

void record_reveal_forget(const char *path)
{
    stage = 0;
    (void)file_delete(path);
}
