extern char data_020cefc4;
void func_020520b0(int param_1)

{
    int iVar1;
    int iVar2;

    iVar2 = *(int *)(param_1 + 0x30) * 4;
    iVar1 = *(int *)(((int)&data_020cefc4) + iVar2);
    if (iVar1 != 0) {
        *(int *)(((int)&data_020cefc4) + iVar2) = iVar1 + -1;
    }
    return;
}
