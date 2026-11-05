#ifndef BACKGROUND_READER_H
#define BACKGROUND_READER_H

#define BACKGROUND_READER_QUEUE_MAX 64

struct finished_read {
    unsigned int request_id;
    int result;
    unsigned char *data;
    unsigned int size;
};

void background_reader_start(void);
int background_reader_submit(unsigned int request_id, const char *path);
int background_reader_take_finished(struct finished_read *finished);
void background_reader_wait_for_finished(struct finished_read *finished);

#endif
