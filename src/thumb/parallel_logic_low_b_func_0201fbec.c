extern unsigned int func_0201f934();

void func_0201fbec(int *param_1,unsigned int param_2)
{
    unsigned int uVar1;
    int iVar2;
    int mask;

    mask = 1;
    uVar1 = func_0201f934(param_1,param_2);
    if (uVar1 != 0) {
        for (uVar1 = 0; uVar1 < (unsigned int)param_1[2]; uVar1 = uVar1 + 1) {
            iVar2 = ((int *)param_1[3])[uVar1];
            if (iVar2 != 0) {
                *(unsigned char *)(iVar2 + 6) = *(unsigned char *)(iVar2 + 6) & ~mask;
            }
        }
        iVar2 = ((int *)param_1[3])[param_2];
        if (iVar2 != 0) {
            *(unsigned char *)(iVar2 + 6) = (*(unsigned char *)(iVar2 + 6) & ~mask) | 1;
        }
    }
}
