typedef struct Common {
    int *cmd;
    unsigned int flag;
    int priority;
    int f0c;
    int f10;
    int pad[5];
    int thread[0x30];
    int stack[0x100];
    void *task;
    int f4ec;
    int f4f0;
    int f4f4;
    int f4f8;
    int f4fc;
    int f500;
    int f504;
    int f508;
    int f50c;
} Common;

struct Ctx {
    int unk0;
    int unk4;
    int job;
};

extern Common data_020d02a0;
extern struct Ctx data_020d08e0;
extern int func_02075d50(void);
extern void func_0207303c(const void *src, void *dst, unsigned int size);
extern void func_02069dd0(void);
extern void func_02069f28(void *arg);
extern void func_02073ed4(int *thread, void (*func)(void *), void *arg, void *stack, unsigned int size, int priority);
extern void func_02074188(int *thread);
extern void func_02069df8(void);
extern void func_0206afd8(void);
extern void func_02069b84(unsigned int value);
extern void func_0206b05c(void);

void func_02069a68(void)
{
    Common *p = &data_020d02a0;

    if (p->flag != 0) {
        return;
    }
    p->flag = 1;
    if (func_02075d50() == 1) {
        func_0207303c((const void *)0x02fffe00, (void *)0x02fffa80, 0x160);
    }
    p->f4fc = 0;
    p->f500 = 0;
    p->f504 = 0;
    p->f508 = -1;
    p->f50c = 0;
    p->f0c = 0x400;
    p->f10 = 0x2400;
    data_020d08e0.unk0 = 0;
    p->priority = 4;
    func_02069dd0();
    p->f4ec = 0;
    p->f4f0 = 0;
    p->f4f8 = 0;
    p->f4f4 = 0;
    func_02073ed4(p->thread, func_02069f28, 0, p->stack + 0x100, sizeof(p->stack), p->priority);
    func_02074188(p->thread);
    func_02069df8();
    func_0206afd8();
    if (func_02075d50() == 1) {
        func_02069b84(1);
    }
    func_0206b05c();
}
