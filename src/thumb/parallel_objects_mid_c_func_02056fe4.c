void func_02056fe4(short x, short y, short z)
{
    *(volatile unsigned int *)0x0400048c = (unsigned int)(unsigned short)x | ((unsigned int)(unsigned short)y << 16);
    *(volatile unsigned int *)0x0400048c = (unsigned int)(unsigned short)z;
}
