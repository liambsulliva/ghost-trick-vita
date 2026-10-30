extern int func_02036f3c();

int func_020595cc(int *param_1)
{
    int iVar1;
    int iVar2;
    unsigned int uVar3;

    iVar2 = 0;
    for (uVar3 = 0; uVar3 < *(unsigned char *)((char *)param_1 + 0xbe); uVar3 = uVar3 + 1) {
        iVar1 = param_1[uVar3 + 0x31];
        if (iVar1 != 0 && (iVar1 = func_02036f3c(iVar1), iVar1 == 0)) {
            iVar2 = iVar2 + 1;
        }
    }
    return iVar2;
}
