int func_020700d0(void)
{
    volatile unsigned int *regs;
    int dispcnt;
    unsigned int bg;

    regs = (volatile unsigned int *)0x04001000;
    dispcnt = regs[0];
    bg = ((volatile unsigned short *)regs)[7];
    if ((dispcnt & 7) < 3 || ((dispcnt & 7) < 6 && (bg & 0x80) == 0)) {
        return ((bg & 0x3c) >> 2) * 0x4000 + 0x6200000;
    }
    return 0;
}
