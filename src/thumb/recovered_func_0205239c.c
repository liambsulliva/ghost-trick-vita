extern int func_020523b0();

void func_0205239c(int param_1)

{
    int iVar1;

    iVar1 = func_020523b0();
    *(unsigned int *)(param_1 + 0x34) = (unsigned int)(iVar1 * -0x10) >> 8;
    return;
}
