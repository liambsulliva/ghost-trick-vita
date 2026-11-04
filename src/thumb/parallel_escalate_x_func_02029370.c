void func_02029370(int s, int t)
{
    *(unsigned int *)0x04000488 = (unsigned short)((s << 8) >> 16) | ((unsigned int)(unsigned short)((t << 8) >> 16) << 16);
}
