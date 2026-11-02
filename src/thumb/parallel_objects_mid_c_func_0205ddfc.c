void func_0205ddfc(int s, int t)
{
    *(volatile unsigned int *)0x04000488 = (unsigned int)(unsigned short)(short)(s >> 8) | ((unsigned int)(unsigned short)(short)(t >> 8) << 16);
}
