extern int func_0202fc60();

int func_0202fd5c(int param_1)

{
    unsigned char *puVar1;
    int iVar2;

    puVar1 = *(unsigned char **)(param_1 + 4);
    iVar2 = 0;
    for (; puVar1 != (unsigned char *)0x0;
            puVar1 = (unsigned char *)func_0202fc60(puVar1)) {
        iVar2 = iVar2 + 1;
    }
    return iVar2;
}
