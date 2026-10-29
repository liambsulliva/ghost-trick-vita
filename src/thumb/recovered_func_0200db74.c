void func_0200db74(int param_1)

{
    int iVar1;
    unsigned int uVar2;

    uVar2 = 0;
    do {
        iVar1 = uVar2 * 4;
        *(unsigned int *)(param_1 + iVar1) = 0;
        uVar2 = uVar2 + 1;
        *(unsigned int *)(param_1 + iVar1 + 0xc) = 0;
    } while (uVar2 < 3);
    return;
}
