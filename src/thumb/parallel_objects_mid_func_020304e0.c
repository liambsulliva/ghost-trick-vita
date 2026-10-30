void func_020304e0(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4)
{
    *(volatile unsigned int *)0x04000580u = (param_4 << 24) | (param_1 | (param_2 << 8) | (param_3 << 16));
}
