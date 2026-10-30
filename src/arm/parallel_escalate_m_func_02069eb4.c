typedef struct Common {
    int *cmd;
    unsigned int flag;
    int priority;
    int pad[7];
    int thread[0x130];
    void (*task)(struct Common *);
} Common;

extern Common data_020d02a0;
void func_020741f8(int *thread, int priority);
void func_02074188(int *thread);
void func_02069fec(Common *p);

int func_02069eb4(void (*task)(Common *), int async)
{
    Common *p;

    p = &data_020d02a0;
    if (async) {
        func_020741f8(p->thread, p->priority);
        p->task = task;
        p->flag |= 8;
        func_02074188(p->thread);
    } else {
        task(p);
        func_02069fec(p);
    }
    return async ? 1 : (p->cmd[0] == 0);
}
