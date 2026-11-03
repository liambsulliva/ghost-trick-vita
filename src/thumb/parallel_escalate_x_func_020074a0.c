typedef struct Cmd {
    int type;
    int a1;
    int a2;
    int a3;
    int a4;
    int a5;
    int a6;
    int a7;
} Cmd;


struct Queue {
    int head;
    int tail;
    int count;
    int full;
    int empty;
};

struct Ctl {
    char *buf;
    struct Queue *q;
};

extern struct Ctl data_020b18fc;

Cmd *func_020074a0(void)
{
    Cmd *rec;

    if (data_020b18fc.q->empty == 1) {
        return 0;
    }
    data_020b18fc.q->full = 0;
    rec = (Cmd *)(data_020b18fc.buf + data_020b18fc.q->head * 32);
    data_020b18fc.q->head = (data_020b18fc.q->head + 1) % 32;
    if (data_020b18fc.q->tail == data_020b18fc.q->head) {
        data_020b18fc.q->empty = 1;
    }
    data_020b18fc.q->count--;
    return rec;
}
