int func_02010fa8(int param_1,unsigned int param_2)
{
    return (*(unsigned int *)(param_1 + 0x38) & (1u << param_2)) != 0;
}
