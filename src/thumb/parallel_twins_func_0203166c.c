void func_0203166c(int param_1)
{
    unsigned short *reg = (unsigned short *)0x04000304;
    unsigned int old = *reg;
    *reg = (0xffff7fffu & old) | (param_1 << 15);
}
