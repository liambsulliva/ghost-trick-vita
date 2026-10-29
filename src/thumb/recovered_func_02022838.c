extern int func_02022854();

int func_02022838(int param_1)

{
    int iVar1;
    int iVar2;

    func_02022854();
    iVar2 = 0;
    do {
        iVar1 = iVar2 * 2;
        iVar2 = iVar2 + 1;
        *(unsigned short *)(param_1 + iVar1 + 0x1c) = 0;
    } while (iVar2 < 0x20);
    return param_1;
}
