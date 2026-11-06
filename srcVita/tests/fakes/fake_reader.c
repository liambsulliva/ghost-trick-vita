#include <stdio.h>
#include <string.h>
#include "background_reader.h"
#include "fake_reader.h"
#include "file.h"

struct fake_read {
    unsigned int request_id;
    char path[FILE_PATH_MAX];
};

static struct fake_read pending[BACKGROUND_READER_QUEUE_MAX];
static int pending_count;
static int released_count;
static int reads_are_held;
static int submits_are_refused;
static int start_should_fail;

void fake_reader_reset(void)
{
    pending_count = 0;
    released_count = 0;
    reads_are_held = 0;
    submits_are_refused = 0;
    start_should_fail = 0;
}

void fake_reader_hold_reads(int should_hold)
{
    reads_are_held = should_hold;
}

void fake_reader_refuse_submits(int should_refuse)
{
    submits_are_refused = should_refuse;
}

int fake_reader_pending_count(void)
{
    return pending_count;
}

void fake_reader_release_reads(int count)
{
    released_count = released_count + count;
}

void fake_reader_fail_start(int should_fail)
{
    start_should_fail = should_fail;
}

int background_reader_start(void)
{
    return !start_should_fail;
}

int background_reader_submit(unsigned int request_id, const char *path)
{
    if (submits_are_refused || pending_count >= BACKGROUND_READER_QUEUE_MAX) {
        return 0;
    }
    pending[pending_count].request_id = request_id;
    snprintf(pending[pending_count].path, FILE_PATH_MAX, "%s", path);
    pending_count = pending_count + 1;
    return 1;
}

static void complete_first_pending(struct finished_read *finished)
{
    int index;

    finished->request_id = pending[0].request_id;
    finished->data = NULL;
    finished->size = 0;
    finished->result = file_read_whole(pending[0].path, &finished->data, &finished->size);
    for (index = 1; index < pending_count; index++) {
        pending[index - 1] = pending[index];
    }
    pending_count = pending_count - 1;
}

int background_reader_take_finished(struct finished_read *finished)
{
    if (pending_count == 0) {
        return 0;
    }
    if (reads_are_held && released_count == 0) {
        return 0;
    }
    if (released_count > 0) {
        released_count = released_count - 1;
    }
    complete_first_pending(finished);
    return 1;
}

void background_reader_wait_for_finished(struct finished_read *finished)
{
    complete_first_pending(finished);
}
