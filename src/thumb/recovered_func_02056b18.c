void func_02056b18(int param_1,unsigned short param_2)

{
    int iVar1;
    int iVar2;

    iVar2 = 0;
    do {
        iVar1 = iVar2 * 2;
        iVar2 = iVar2 + 1;
        *(unsigned short *)(param_1 + iVar1 + 0x9c) = param_2;
    } while (iVar2 < 4);
    return;
}
