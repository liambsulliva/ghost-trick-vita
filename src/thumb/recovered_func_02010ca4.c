extern int func_02010ccc();

void func_02010ca4(int param_1)

{
    int iVar1;

    iVar1 = func_02010ccc();
    if (iVar1 == 0) {
        *(unsigned int *)(param_1 + 0xc) = ((unsigned int)0xfffffdffu) & (*(unsigned int *)(param_1 + 0xc) & ((unsigned int)0xfffffeffu) | 0x80);
    }
    return;
}
