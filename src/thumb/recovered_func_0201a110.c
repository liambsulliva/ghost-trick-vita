unsigned int func_0201a110(int param_1)

{
    int iVar1;

    iVar1 = *(int *)(param_1 + 0x8c);
    if ((iVar1 == 1) || (iVar1 == 3)) {
        return 0x80;
    }
    if (iVar1 == 2) {
        return 0x100;
    }
    return 0;
}
