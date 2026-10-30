extern unsigned int func_0201f934();
int func_0201fcf4(int param_1,unsigned int param_2)
{
    unsigned int uVar1;
    int iVar2;

    uVar1 = func_0201f934(param_1,param_2);
    if (uVar1 == 0) {
        return 0;
    }
    iVar2 = *(int *)(*(int *)(param_1 + 0xc) + param_2 * 4);
    if (iVar2 != 0) {
        if (((((unsigned int)*(unsigned char *)(iVar2 + 6)) << 24) >> 31) != 0) {
            return 0;
        }
        do {
            if (((((unsigned int)*(unsigned char *)(iVar2 + 6)) << 27) >> 31) != 0) {
                return 1;
            }
            iVar2 = *(int *)(iVar2 + 8);
        } while (iVar2 != 0);
    }
    return 0;
}
