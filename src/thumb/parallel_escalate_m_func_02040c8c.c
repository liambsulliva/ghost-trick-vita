extern int func_02079020();

unsigned int func_02040c8c(unsigned int param_1, unsigned int param_2)
{
    int iVar1;
    unsigned int uVar2;
    signed char cVar3;

    iVar1 = func_02079020(param_2);
    if (iVar1 != 4) {
        return 0;
    }
    for (uVar2 = 0; uVar2 < 4; uVar2 = (uVar2 + 1) & 0xff) {
        cVar3 = *(signed char *)(param_2 + uVar2);
        if ((cVar3 < '0') || ('9' < cVar3)) {
            return 0;
        }
    }
    return 1;
}
