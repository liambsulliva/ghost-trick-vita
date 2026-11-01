void func_0205dc0c(int size, int mode, int src_a, int src_b, int dest, int eva, int evb)
{
    *(volatile unsigned int *)0x04000064 = (unsigned int)(0x80000000 | (mode << 29) | (src_b << 25) | (src_a << 24) | (size << 20) | (dest << 16) | (evb << 8) | eva);
}
