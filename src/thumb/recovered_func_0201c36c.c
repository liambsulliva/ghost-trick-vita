void func_0201c36c(int param_1)

{
    unsigned int uVar1;
    int iVar2;
    unsigned int uVar3;

    uVar1 = ((unsigned int)0x00007fffu);
    uVar3 = 1;
    do {
        iVar2 = uVar3 * 2;
        uVar3 = uVar3 + 1;
        *(short *)(param_1 + iVar2 + 0x3c) = (short)uVar1;
    } while (uVar3 < 0x10);
    return;
}
