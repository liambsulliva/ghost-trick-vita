int func_0206fca8(void)
{
    int scr;

    scr = (int)(*((volatile unsigned short *)0x04000008u) & 0x1f00) >> 8;
    return ((*((volatile unsigned int *)0x04000000u) & 0x38000000) >> 27) * 0x10000 + 0x6000000 + scr * 0x800;
}
