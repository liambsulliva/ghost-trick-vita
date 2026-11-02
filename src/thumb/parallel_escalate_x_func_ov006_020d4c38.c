void func_ov006_020d4c38(short x, short y, short z)
{
    *(volatile unsigned int *)0x0400048c = (unsigned int)((unsigned short)x | ((unsigned short)y << 16));
    *(volatile unsigned int *)0x0400048c = (unsigned int)(unsigned short)z;
}
