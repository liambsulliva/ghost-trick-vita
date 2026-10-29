void func_0201ed90(int param_1)

{
    int iVar1;
    int iVar2;

    iVar1 = ((int)0x00000608u);
    iVar2 = *(int *)(param_1 + ((int)0x00000608u)) + -1;
    *(int *)(param_1 + ((int)0x00000608u)) = iVar2;
    *(int *)(param_1 + iVar1 + -4) = param_1 + 4 + iVar2 * 0xc;
    return;
}
