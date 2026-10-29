extern int func_02008828();

int func_02058b8c(int param_1)

{
    int iVar1;

    if (*(unsigned char **)(param_1 + 4) != (unsigned char *)0x0) {
        iVar1 = func_02008828(*(unsigned char **)(param_1 + 4),((unsigned char *)0x020cf2e8u));
        return iVar1;
    }
    return 0;
}
