extern int func_0201f934();

unsigned char func_0201fcac(int param_1,unsigned int param_2)

{
    unsigned int uVar1;
    int iVar2;

    uVar1 = func_0201f934(param_1,param_2);
    if (uVar1 == 0) {
        return 0;
    }
    iVar2 = *(int *)(*(int *)(param_1 + 0xc) + param_2 * 4);
    if (iVar2 != 0) {
        return *(unsigned char *)(iVar2 + 7);
    }
    return 0;
}
