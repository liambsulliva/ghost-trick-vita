int func_0206ff40(void)
{
    int bg;
    unsigned int dispcnt;

    bg = (*(volatile unsigned short *)0x04000008 & 0x3c) >> 2;
    dispcnt = *(volatile unsigned int *)0x04000000;
    return ((dispcnt & 0x7000000) >> 24) * 0x10000 + 0x6000000 + bg * 0x4000;
}
