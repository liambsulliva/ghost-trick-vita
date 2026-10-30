extern int func_020523b0(int param_1);

void func_020523d8(int param_1)
{
    unsigned int value;

    value = func_020523b0(param_1);
    *(unsigned int *)(param_1 + 0x34) = (value << 4) >> 8;
}
