void func_0207d804(unsigned int *param_1, int param_2, int param_3)
{
    unsigned int *iVar1;

    iVar1 = (unsigned int *)(param_1[6] + param_2);
    *(unsigned int *)(param_1[6] + param_2) = param_1[1];
    iVar1[1] = *param_1;
    iVar1[2] = param_1[2];
    if (*(unsigned char *)*param_1 == 42) {
        iVar1[3] = (unsigned int)(iVar1 + 4);
        iVar1[4] = *(unsigned int *)param_1[1] + param_3;
        return;
    }
    iVar1[3] = param_1[1] + param_3;
}
