extern int func_02072ec4();

unsigned int *func_020687c0(unsigned int *param_1, int param_2, int param_3)
{
    unsigned int *puVar1;
    unsigned int size;

    puVar1 = (unsigned int *)(~(param_3 - 1U) & (param_1[1] - param_2));
    if (puVar1 < (unsigned int *)*param_1)
    {
        return (unsigned int *)0;
    }
    size = param_1[1] - (unsigned int)puVar1;
    if (((unsigned char)param_1[-1] & 1) != 0)
    {
        func_02072ec4(0, puVar1, size);
    }
    param_1[1] = (unsigned int)puVar1;
    return puVar1;
}
