extern int func_0201ee6c();
extern int func_02050b34();
extern int func_020515a0();

int func_02050ffc(int param_1)
{
    int iVar1;
    unsigned int *puVar2;

    iVar1 = func_02050b34((unsigned int *)(*(int *)(param_1 + 4) + *(int *)(param_1 + 8) * 2 + 2));
    puVar2 = (unsigned int *)func_0201ee6c(iVar1, ((unsigned int)0x020bbbb4u));
    if ((iVar1 != 0) && (puVar2 != (unsigned int *)0x0)) {
        func_020515a0(*(unsigned char **)(param_1 + 0x30), puVar2);
    }
    *(int *)(param_1 + 8) = *(int *)(param_1 + 8) + 3;
    return 1;
}
