extern int func_02031254();
extern unsigned int *func_02039fb0();

void func_02039f3c(unsigned int **param_1, int param_2)
{
    unsigned int *puVar1;
    signed char iVar2;
    int iVar3;
    int iVar4;
    unsigned int *piVar6;
    unsigned int uVar5;

    uVar5 = 0;
    while (*(int *)(param_2 + uVar5 * 4) == 0) {
        uVar5 = uVar5 + 1 & 0xff;
        if (uVar5 == 5) {
            return;
        }
    }
    puVar1 = func_02039fb0(param_1);
    *param_1 = puVar1;
    if (puVar1 == (unsigned int *)0x0) {
        return;
    }
    puVar1[2] = *(unsigned int *)(param_2 + uVar5 * 4);
    piVar6 = *param_1;
    for (iVar2 = uVar5; iVar2 < 4; iVar2 = iVar2 + 1) {
        if ((iVar2 == 2) && (*(int *)(param_2 + iVar2 * 4) != 0)) {
            param_1[1] = piVar6;
        }
        iVar3 = param_2 + iVar2 * 4;
        if (*(int *)(iVar3 + 4) == 0) {
            return;
        }
        iVar4 = func_02031254(0xc);
        *piVar6 = iVar4;
        if (iVar4 == 0) {
            return;
        }
        *(unsigned int **)(iVar4 + 4) = piVar6;
        piVar6 = (unsigned int *)*piVar6;
        *piVar6 = 0;
        piVar6[2] = *(int *)(iVar3 + 4);
    }
    return;
}
