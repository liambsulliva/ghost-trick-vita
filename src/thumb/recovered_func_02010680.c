void func_02010680(int param_1,unsigned int param_2)

{
    *(unsigned int *)(param_1 + 0x164) = param_2;
    *(unsigned int *)(param_1 + 0xd0) = 0;
    *(unsigned char *)(param_1 + 0x130) = *(unsigned char *)(param_1 + 0x130) | 4;
    return;
}
