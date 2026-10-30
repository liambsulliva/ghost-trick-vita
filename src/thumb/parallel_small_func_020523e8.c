extern int func_020523cc(int param_1);

void func_020523e8(int param_1)
{
    unsigned int value;

    value = func_020523cc(param_1);
    *(unsigned int *)(param_1 + 0x34) = (value << 4) >> 8;
}
