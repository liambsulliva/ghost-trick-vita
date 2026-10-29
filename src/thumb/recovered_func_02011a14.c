void func_02011a14(int param_1,unsigned int param_2)

{
    *(unsigned int *)(param_1 + ((int)0x00000d94u)) = param_2;
    *(unsigned int *)(param_1 + 8) = *(unsigned int *)(param_1 + 8) | 0x80;
    return;
}
