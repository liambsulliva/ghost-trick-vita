extern unsigned int func_02028400();
extern void func_020410d0();
extern void func_020410c4();

void func_020411e4(int param_1)
{
    unsigned int uVar1;
    unsigned int uVar2;
    int *piVar3;

    uVar1 = func_02028400(*(int *)(param_1 + 4));
    if (uVar1 < 2) {
        func_020410d0(*(int *)(param_1 + uVar1 * 4 + 8));
        uVar2 = 1;
        if (uVar1 != 0) {
            uVar2 = 0;
        }
        piVar3 = (int *)(param_1 + 8);
        func_020410c4(piVar3[uVar2], *(unsigned short *)(param_1 + 2));
        *(int *)(param_1 + 4) = piVar3[uVar2];
    }
    *(unsigned short *)param_1 = 1;
    return;
}
