extern int func_0202b56c();

int func_0202da44(int param_1)

{
    int iVar1;

    *(unsigned int *)(param_1 + 0x88) = 0;
    iVar1 = *(unsigned char *)(param_1 + 0x86);
    *(unsigned char *)(param_1 + 0x86) = iVar1 & ~1;
    return func_0202b56c(param_1);
}
