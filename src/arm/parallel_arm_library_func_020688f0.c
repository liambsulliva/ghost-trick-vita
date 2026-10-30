extern unsigned char *func_02068864(int param_1);

unsigned char *func_020688f0(int param_1, unsigned int param_2)
{
    if (param_2 > *(unsigned int *)(*(int *)(param_1 + 4) + 0x28))
    {
        return 0;
    }
    return func_02068864(*(int *)(param_1 + 4));
}
