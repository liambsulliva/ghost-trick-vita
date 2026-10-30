int func_02070078(void)
{
    int reg;
    unsigned int reg2;
    unsigned int field;

    reg = *(volatile unsigned int *)0x04000000;
    reg2 = *(volatile unsigned short *)0x0400000e;
    if (((reg & 7) < 3) || (((reg & 7) < 6) && ((reg2 & 0x80) == 0)))
    {
        field = (*(volatile unsigned int *)0x04000000 & 0x7000000) >> 24;
        return field * 0x10000 + 0x6000000 + ((reg2 & 0x3c) >> 2) * 0x4000;
    }
    return 0;
}
