int func_02070038(void)
{
    volatile unsigned int *regs;
    int dispcnt;
    unsigned int bg;

    regs = (volatile unsigned int *)0x04001000;
    dispcnt = regs[0];
    bg = ((volatile unsigned short *)regs)[6];
    if (((dispcnt & 7) < 5) || ((bg & 0x80) == 0)) {
        return ((bg & 0x3c) >> 2) * 0x4000 + 0x6200000;
    }
    return 0;
}
