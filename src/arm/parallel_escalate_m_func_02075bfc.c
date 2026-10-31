unsigned int func_02075bfc(int param_1, int param_2, int param_3)
{
    short sVar1;
    int iVar2;

    sVar1 = *(short *)(param_1 + 0x10);
    iVar2 = *(int *)(param_1 + 0xc);
    param_2 = param_2 - iVar2;
    param_3 = param_3 - sVar1;
    if ((param_2 < 0) || ((param_2 == 0 && (param_3 < 0)))) {
        return 0;
    }
    if (param_3 < 0) {
        param_3 = param_3 + 0x107;
    }
    if (param_3 <= *(short *)(param_1 + 0x12)) {
        return 1;
    }
    return 2;
}
