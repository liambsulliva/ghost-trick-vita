extern int func_02075c98(void);
extern void func_02075cac(int enabled);
extern void func_020740d0(void *queue);
extern char data_027e0000;
extern char data_027e0068;

static inline void func_020732e8_clear_check(unsigned int intr)
{
    *(volatile unsigned int *)((unsigned int)&data_027e0000 + 0x3ff8) &= ~intr;
}

static inline unsigned int func_020732e8_get_check(void)
{
    return *(volatile unsigned int *)((unsigned int)&data_027e0000 + 0x3ff8);
}

void func_020732e8(int clear, unsigned int irqFlags)
{
    int enabled = func_02075c98();
    if (clear != 0) {
        func_020732e8_clear_check(irqFlags);
    }
    func_02075cac(enabled);
    while (!(func_020732e8_get_check() & irqFlags)) {
        func_020740d0(&data_027e0068);
    }
}
