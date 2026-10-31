struct TickState {
    unsigned short use;
    unsigned short pad02;
    int need_reset;
    unsigned long long counter;
};

extern struct TickState data_020d1250;
extern void func_020752b0(int timer);
extern void func_02073480(int mask, void (*handler)(void));
extern void func_020735c0(int mask);
extern void func_02075354(void);

void func_020752d4(void)
{
    if (data_020d1250.use == 0) {
        data_020d1250.use = 1;
        func_020752b0(0);
        data_020d1250.counter = 0;
        *(volatile unsigned short *)0x04000102 = 0;
        *(volatile unsigned short *)0x04000100 = 0;
        *(volatile unsigned short *)0x04000102 = 0xc1;
        func_02073480(8, func_02075354);
        func_020735c0(8);
        data_020d1250.need_reset = 0;
    }
}
