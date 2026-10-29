extern int func_020523cc();

void func_020523b8(int param_1)

{
    int iVar1;

    iVar1 = func_020523cc();
    *(unsigned int *)(param_1 + 0x34) = (unsigned int)(iVar1 * -0x10) >> 8;
    return;
}
