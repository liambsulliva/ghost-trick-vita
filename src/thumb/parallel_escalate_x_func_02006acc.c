struct Work {
    unsigned char pad[0x4cb9];
    unsigned char ready;
    unsigned char flags;
};

extern struct {
    char pad[0x10];
    struct Work *work;
} data_020b18a0;

struct WorkC {
    unsigned char pad[0x4cba];
    const unsigned char flags;
};

void func_02006acc(void)
{
    if ((data_020b18a0.work->flags & 2) && (((struct WorkC *)data_020b18a0.work)->flags & 4)) {
        data_020b18a0.work->ready = 1;
    } else {
        data_020b18a0.work->ready = 0;
    }
    data_020b18a0.work->flags &= 0xfd;
    data_020b18a0.work->flags &= 0xfb;
}
