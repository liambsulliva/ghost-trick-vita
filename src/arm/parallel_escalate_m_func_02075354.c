struct TickState {
    unsigned short use;
    unsigned short pad02;
    int need_reset;
    unsigned long long counter;
};

extern struct TickState data_020d1250;
extern void func_02073550(int timer, void (*callback)(void), void *arg);

void func_02075354(void)
{
    data_020d1250.counter++;
    if (data_020d1250.need_reset) {
        *(volatile unsigned short *)0x04000102 = 0;
        *(volatile unsigned short *)0x04000100 = 0;
        *(volatile unsigned short *)0x04000102 = 0xc1;
        data_020d1250.need_reset = 0;
    }
    func_02073550(0, func_02075354, 0);
}
