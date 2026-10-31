struct TickState {
    unsigned short use;
    unsigned short pad02;
    int need_reset;
    volatile unsigned long long counter;
};

extern struct TickState data_020d1250;
extern unsigned int func_02075c98(void);
extern unsigned int func_02075cac(unsigned int bits);

unsigned long long func_020753bc(void)
{
    volatile unsigned short countL;
    volatile unsigned long long countH;
    unsigned int prev;

    prev = func_02075c98();
    countL = *(volatile unsigned short *)0x04000100;
    countH = data_020d1250.counter & 0x0000ffffffffffffULL;

    if ((*(volatile unsigned int *)0x04000214 & 8) && !(countL & 0x8000)) {
        countH++;
    }

    func_02075cac(prev);

    return (countH << 16) | countL;
}
