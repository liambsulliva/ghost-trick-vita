void func_0206fa8c(int enable, int mode, int slope, int offset)
{
    if (enable) {
        *(volatile unsigned short *)0x0400035c = (unsigned short)offset;
        *(volatile unsigned short *)0x04000060 = (unsigned short)((*(volatile unsigned short *)0x04000060 & ~0x3f40) | ((slope << 8) | (mode << 6) | 0x80));
    } else {
        *(volatile unsigned short *)0x04000060 = (unsigned short)(*(volatile unsigned short *)0x04000060 & 0xcf7f);
    }
}
