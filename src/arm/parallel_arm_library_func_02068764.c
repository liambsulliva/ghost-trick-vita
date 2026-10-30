extern int func_02072ec4();

unsigned int func_02068764(unsigned int *param_1, int param_2, int param_3)
{
    unsigned int *puVar1;
    unsigned int uVar2;
    unsigned int uVar3;
    unsigned int size;

    puVar1 = (unsigned int *)*param_1;
    uVar2 = ~(param_3 - 1U) & ((param_3 - 1U) + (unsigned int)puVar1);
    uVar3 = param_2 + uVar2;
    if (uVar3 > param_1[1])
    {
        return 0;
    }
    size = uVar3 - (unsigned int)puVar1;
    if (((unsigned char)param_1[-1] & 1) != 0)
    {
        func_02072ec4(0, puVar1, size);
    }
    *param_1 = uVar3;
    return uVar2;
}
