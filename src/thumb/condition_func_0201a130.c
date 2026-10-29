unsigned int func_0201a130(int param_1)

{
    unsigned int uVar1;

    if (*(int *)(param_1 + 0x8c) - 1U <= 1) {
        return 0x40;
    }
    uVar1 = 0x80;
    if (*(int *)(param_1 + 0x8c) != 3) {
        uVar1 = 0;
    }
    return uVar1;
}
