extern int func_020523b0();

void func_020523f8(int param_1)

{
    int iVar1;

    iVar1 = func_020523b0();
    *(unsigned int *)(param_1 + 0x34) = (unsigned int)(iVar1 * 0x30) >> 10;
    return;
}
