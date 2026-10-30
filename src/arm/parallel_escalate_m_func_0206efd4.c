void func_0206efd4(int denom)
{
    *(volatile unsigned short *)0x04000280 = 1;
    *(unsigned long long *)0x04000290 = (unsigned long long)0x1000 << 32;
    *(unsigned long long *)0x04000298 = (unsigned int)denom;
}
