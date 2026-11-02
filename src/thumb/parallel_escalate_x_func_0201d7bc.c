typedef struct Entry0201d7bc {
    unsigned short *text;
    int time;
    unsigned short *pos;
    char pad[8];
} Entry0201d7bc;

typedef struct Log0201d7bc {
    unsigned int count;
    int index;
    unsigned int length;
    unsigned short *text;
    int time;
    int pad14;
    unsigned char kind;
    unsigned char busy;
    unsigned short buf[0xea7];
    Entry0201d7bc entries[30];
} Log0201d7bc;

extern void *func_0201003c(void);
extern int func_0201d8e8(void *object);
extern int func_0201da90(Log0201d7bc *log, unsigned short *text);
extern void func_0201da80(Log0201d7bc *log, int *index);
extern void func_0201d95c(Log0201d7bc *log, int index, unsigned short *text);
extern int func_0201d938(Log0201d7bc *log, unsigned short c);
extern unsigned int func_02015d04(int c);

void func_0201d7bc(Log0201d7bc *log, unsigned short *text, unsigned char kind)
{
    unsigned short *p;
    unsigned short c;

    if (text == 0 || log->busy != 0) {
        return;
    }
    if (log->text == text && log->time == func_0201d8e8(func_0201003c())) {
        return;
    }
    if (func_0201da90(log, text) == 0) {
        return;
    }
    log->kind = kind;
    if (log->count < 30) {
        log->count++;
    }
    if (log->count > 1) {
        func_0201da80(log, &log->index);
    }
    func_0201d95c(log, log->index, text);
    log->text = text;
    log->time = func_0201d8e8(func_0201003c());
    log->entries[log->index].text = text;
    log->entries[log->index].time = func_0201d8e8(func_0201003c());
    log->entries[log->index].pos = &log->buf[log->length];
    p = text;
    for (;;) {
        c = *p;
        if (c == 0xff15) {
            c = 0xff02;
        }
        if (c == 0xff23) {
            c = 0xfffe;
        }
        if (c == 0xff2d) {
            p += 3;
            continue;
        }
        if (func_0201d938(log, c) == 0) {
            log->entries[log->index].pos = &log->buf[log->length];
            p = text;
            continue;
        }
        if (func_02015d04(c) != 0) {
            break;
        }
        p++;
    }
}
