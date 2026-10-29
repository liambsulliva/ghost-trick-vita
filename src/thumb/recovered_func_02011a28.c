void func_02011a28(int param_1,unsigned int param_2)

{
    *(unsigned int *)(param_1 + ((int)0x00000d98u)) = param_2;
    *(unsigned int *)(param_1 + 8) = *(unsigned int *)(param_1 + 8) | 0x80;
    return;
}
