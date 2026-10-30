typedef struct Common {
    int *cmd;
    unsigned int flag;
    int priority;
    int pad[7];
    int thread[0x130];
    void (*task)(struct Common *);
} Common;

extern Common data_020d02a0;
extern int func_02075c98(void);
extern int func_02075cac(int bak);
extern void func_020740d0(void *queue);
extern int func_02069eb4(void (*task)(Common *), int async);

void func_02069f28(void *arg)
{
    Common *p;
    int bak;

    p = &data_020d02a0;
    for (;;) {
        bak = func_02075c98();
        for (;;) {
            if ((p->flag & 8) != 0) {
                break;
            }
            func_020740d0((void *)0);
        }
        func_02075cac(bak);
        func_02069eb4(p->task, 0);
    }
}
