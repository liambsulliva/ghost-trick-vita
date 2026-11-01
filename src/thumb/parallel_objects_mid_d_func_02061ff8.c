void func_02061ff8(int param_1, int param_2)
{
    unsigned int lo;
    unsigned int hi;
    lo = (unsigned short)((int)(param_1 << 8) >> 16);
    hi = (unsigned short)((int)(param_2 << 8) >> 16);
    *(volatile unsigned int *)0x04000488 = lo | (hi << 16);
}
