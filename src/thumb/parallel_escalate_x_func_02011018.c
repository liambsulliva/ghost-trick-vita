int func_02011018(int param_1,unsigned int param_2)
{
    return (*(unsigned int *)(param_1 + 0x40) & (1u << param_2)) != 0;
}
