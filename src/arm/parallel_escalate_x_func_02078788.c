typedef struct PMWork02078788 {
    char pad0[0xc];
    int sleepEnd;
    void *preList;
    char pad14[4];
    void *postList;
} PMWork02078788;

extern PMWork02078788 data_020d2ec0;

extern void func_02078d58(void *list);
extern unsigned int func_02075c98(void);
extern unsigned int func_02075cac(unsigned int bits);
extern unsigned int func_020735f0(unsigned int mask);
extern unsigned int func_02073594(unsigned int mask);
extern unsigned short func_02075344(void);
extern unsigned short func_02075d50(void);
extern int func_02079ba4(void);
extern unsigned int func_02078cb0(void);
extern int func_02078668(unsigned int *top, unsigned int *bottom);
extern void func_02075d14(unsigned int cycles);
extern int func_0207855c(unsigned int lcd, unsigned int sw);
extern void func_020780a4(void);
extern void func_020786f4(void);
extern unsigned int func_0207833c(unsigned int param_1, unsigned int param_2);
extern void func_02075fc4(void);
extern int func_02078a74(int a, int b, int c, int d);
extern int func_020784ac(int on);
extern int func_020785fc(void);

static inline unsigned short disable_irq(void)
{
    unsigned short prep = *(volatile unsigned short *)0x04000208;
    *(volatile unsigned short *)0x04000208 = 0;
    return prep;
}

static inline unsigned short enable_irq(void)
{
    unsigned short prep = *(volatile unsigned short *)0x04000208;
    *(volatile unsigned short *)0x04000208 = 1;
    return prep;
}

static inline unsigned short restore_irq(unsigned short enable)
{
    unsigned short prep = *(volatile unsigned short *)0x04000208;
    *(volatile unsigned short *)0x04000208 = enable;
    return prep;
}

void func_02078788(unsigned int trigger, unsigned int logic, unsigned int keys)
{
    unsigned short prepIrq;
    unsigned int intrMode;
    unsigned int prepMask;
    int powerOff = 0;
    unsigned int dispcnt;
    unsigned int subDispcnt;
    unsigned int lcdPower;
    unsigned int top;
    unsigned int bottom;
    unsigned int topFlag;
    unsigned int bottomFlag;
    unsigned int flags;

    func_02078d58(data_020d2ec0.preList);
    prepIrq = disable_irq();
    intrMode = func_02075c98();
    prepMask = func_020735f0(0x3fffff);
    func_02073594((func_02075344() ? 8 : 0) | 0x40000);
    func_02075cac(intrMode);
    enable_irq();

    if (trigger & 8) {
        if ((unsigned short)(func_02075d50() + 0xfffe) <= 1) {
            trigger &= ~8;
        }
    }
    if (trigger & 0x10) {
        if (func_02079ba4() == 0) {
            trigger &= ~0x10;
        }
    }

    dispcnt = *(volatile unsigned int *)0x04000000;
    subDispcnt = *(volatile unsigned int *)0x04001000;
    lcdPower = func_02078cb0();

    while (func_02078668(&top, &bottom) != 0) {
        func_02075d14(0x51d23);
    }
    while (func_0207855c(2, 0) != 0) {
        func_02075d14(0x51d23);
    }

    func_020780a4();
    *(volatile unsigned int *)0x04000000 &= ~0x30000;
    *(volatile unsigned int *)0x04001000 &= ~0x10000;
    func_020780a4();
    func_020780a4();
    func_020786f4();

    data_020d2ec0.sleepEnd = 0;
    bottomFlag = bottom ? 0x80 : 0;
    topFlag = top ? 0x40 : 0;
    func_02073594(0x40000);
    flags = trigger | topFlag;
    func_0207833c((unsigned short)(flags | bottomFlag), (unsigned short)(logic | keys));

    while (data_020d2ec0.sleepEnd == 0) {
        func_02075fc4();
    }

    func_02073594((func_02075344() ? 8 : 0) | 0x40000);

    if (trigger & 8) {
        if (*(volatile unsigned int *)0x04000214 & 0x100000) {
            powerOff = 1;
        }
    }

    if (!powerOff) {
        if (lcdPower == 1) {
            while (func_02078a74(1, 1, 1, 1) != 1) {
            }
        } else {
            while (func_020784ac(1) != 0) {
                func_02075d14(0x51d23);
            }
        }
        *(volatile unsigned int *)0x04000000 = dispcnt;
        *(volatile unsigned int *)0x04001000 = subDispcnt;
    }

    func_02075d14(0x360000);

    disable_irq();
    func_02073594(prepMask);
    func_02075cac(intrMode);
    restore_irq(prepIrq);

    if (powerOff) {
        func_020785fc();
    }

    func_02078d58(data_020d2ec0.postList);
}
