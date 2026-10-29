void func_02010d18(int param_1)

{
    *(unsigned int *)(param_1 + 0xc) = ((unsigned int)0xfffffdffu) & (*(unsigned int *)(param_1 + 0xc) & 0xffffff7f | 0x100);
    return;
}
