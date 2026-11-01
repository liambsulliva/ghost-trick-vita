unsigned int func_0206f004(void)
{
    while (*(volatile unsigned short *)0x040002b0 & 0x8000) {
    }
    return (*(volatile unsigned int *)0x040002b4 + 512) >> 10;
}
