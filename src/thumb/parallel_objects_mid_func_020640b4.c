extern unsigned int * func_02056ac8();

unsigned int * func_020640b4(unsigned int *param_1)

{
    int iVar1;
    unsigned int uVar2;

    func_02056ac8(param_1);
    uVar2 = 0;
    *param_1 = ((unsigned int)0x020afd14u);
    do {
        iVar1 = uVar2 * 4;
        uVar2 = uVar2 + 1;
        *(unsigned int *)((int)param_1 + iVar1 + 0xc8) = 0;
    } while (uVar2 < 4);
    return param_1;
}
