struct Queue {
    int head;
    int tail;
    int count;
    int full;
    int f10;
};
struct Ctl {
    char *buf;
    struct Queue *q;
};
extern struct Ctl data_020b18fc;
extern void func_02007498();

int func_0200743c(void *param_1)
{
    if (data_020b18fc.q->full == 1) {
        return -1;
    }
    func_02007498(param_1, data_020b18fc.buf + data_020b18fc.q->tail * 32, 0x20);
    data_020b18fc.q->f10 = 0;
    data_020b18fc.q->tail = (data_020b18fc.q->tail + 1) % 32;
    if (data_020b18fc.q->head == data_020b18fc.q->tail) {
        data_020b18fc.q->full = 1;
    }
    data_020b18fc.q->count = data_020b18fc.q->count + 1;
    return data_020b18fc.q->count;
}
